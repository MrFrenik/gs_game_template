#ifndef ENTITY_H
#define ENTITY_H

typedef struct component_data_t 
{
	gs_hash_table(uint32_t, uint32_t) lookup;	// Lookup table from entity slot handle id to component slot handle id
	void** data;
} component_data_t;

introspect()
typedef struct entity_t
{
	base(object_t)

	uint32_t id;
	ignore() gs_dyn_array(uint32_t) components;	// Component id handle array
} entity_t;

typedef uint32_t entity_handle_t;

typedef struct entity_manager_t
{
	base(object_t)

	gs_slot_array(entity_t) entities;
	gs_hash_table(uint64_t, component_data_t*) components;
	component_data_t* cd;
} entity_manager_t;

GS_API_DECL uint32_t entities_allocate(entity_manager_t* entities);
GS_API_DECL void entities_deallocate(entity_manager_t* entities, uint32_t hndl);
GS_API_DECL bool _entities_has_component_internal(entity_manager_t* entities, uint64_t cid, entity_handle_t ent);

#define entities_register_component(ENTITIES, T)\
	do {\
		uint64_t id = gs_hash_str64(gs_to_str(T));\
		if (!gs_hash_table_exists((ENTITIES)->components, id))\
		{\
			component_data_t* cd = (component_data_t*)gs_malloc_init(component_data_t);\
			cd->data = gs_malloc_init(gs_slot_array(T)*);\
			gs_hash_table_insert((ENTITIES)->components, id, cd);\
		}\
	} while (0)

#define entities_has_component(ENTITIES, T, HNDL)\
	(_entities_has_component_internal(ENTITIES, obj_id(T), HNDL))

#define entities_add_component(ENTITIES, T, HNDL, ...)\
	do {\
		entity_manager_t* ents = (ENTITIES);\
		if (!entities_has_component(ents, T, HNDL))\
		{\
			/* Get component data */\
			entity_t* ent = gs_slot_array_getp(ents->entities, HNDL);\
			uint64_t id = obj_id(T);\
			component_data_t* cd = gs_hash_table_get(ents->components, id);\
			/* Check if exists, return if already exists */\
			if (!gs_hash_table_exists(cd->lookup, HNDL))\
			{\
				/* Insert new component and register with lookup */\
				gs_slot_array(T)* sa = (gs_slot_array(T)*)cd->data;\
				T comp = __VA_ARGS__;\
				uint32_t comp_hndl = gs_slot_array_insert(*sa, comp);\
				gs_hash_table_insert(cd->lookup, HNDL, comp_hndl);\
			}\
		}\
	} while (0)

#define entities_get_component(ENTITIES, T, HNDL)\
	((ENTITIES)->cd = gs_hash_table_get((ENTITIES)->components, obj_id(T)),\
	 gs_slot_array_getp(*((gs_slot_array(T)*)((ENTITIES)->cd->data)), gs_hash_table_get(((ENTITIES)->cd->lookup), HNDL))) 

#ifdef ENTITY_IMPL

GS_API_DECL uint32_t entities_allocate(entity_manager_t* entities)
{
	entity_t e = gs_default_val();
	uint32_t hndl = gs_slot_array_insert(entities->entities, e);
	gs_slot_array_getp(entities->entities, hndl)->id = hndl;
	return hndl;
}

GS_API_DECL void entities_deallocate(entity_manager_t* entities, uint32_t hndl)
{
	// Clean up entity data
}

GS_API_DECL bool _entities_has_component_internal(entity_manager_t* entities, uint64_t cid, entity_handle_t hndl)
{
	component_data_t* cd = gs_hash_table_get(entities->components, cid);
	return gs_hash_table_exists(cd->lookup, hndl);
}

#endif // ENTITY_IMPL
#endif // ENTITY_H
