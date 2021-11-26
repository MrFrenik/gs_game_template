/*==============================================================================================================
    * Copyright: 2020 John Jackson 
    * Gunslinger Game Template
    * File: entity.h
    * Github: https://github.com/MrFrenik/gs_game_template

    All Rights Reserved

    BSD 3-Clause License

    Copyright (c) 2020 John Jackson

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be used to 
    endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDi
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=================================================================================================================*/ 

#ifndef ENTITY_H
#define ENTITY_H

typedef struct component_data_t 
{
	gs_hash_table(uint32_t, uint32_t) lookup;	// Lookup table from entity slot handle id to component slot handle id 
    gs_dyn_array(uint32_t) indices;             // Slot array indices
	void* data;                                 // Pointer to internal slot array of component data
    size_t data_size;                           // Individual unit of data
    uint64_t cid;                               // Component id
} component_data_t;

// Going to need to macro-fy the raw component data

gs_force_inline
void _gs_slot_array_erase_func(gs_dyn_array(uint32_t) indices, void* data, uint32_t hndl, size_t val_len)
{ 
    uint32_t _H0 = hndl; 

    if (gs_dyn_array_size(data) == 1) 
    {
        gs_dyn_array_clear(data);
        gs_dyn_array_clear(indices);
    } 
    else if (_H0 >= gs_dyn_array_size(indices) || indices[_H0] == GS_SLOT_ARRAY_INVALID_HANDLE)
    {
        gs_println("Warning: Attempting to erase invalid slot array handle (%zu)", _H0);
    } 
    else 
    {
        uint32_t _OG_DATA_IDX = indices[_H0];

        // Iterate through handles until last index of data found
        uint32_t _H = 0;
        for (uint32_t i = 0; i < gs_dyn_array_size(indices); ++i)
        {
            if (indices[i] == gs_dyn_array_size(data) - 1)
            {
                _H = i;
                break;
            }
        }
    
        // Swap and pop data
        uint32_t bidx = gs_dyn_array_size(data) - 1;
        memcpy(
            (uint8_t*)(data) + _OG_DATA_IDX * val_len, 
            (uint8_t*)(data) + bidx * val_len,
            val_len
        ); 
        if (gs_dyn_array_size(data)) 
        {
            gs_dyn_array_head(data)->size -= 1;
        }
    
        // Point new handle, Set og handle to invalid
        indices[_H] = _OG_DATA_IDX;
        indices[_H0] = GS_SLOT_ARRAY_INVALID_HANDLE;
    } 
}

gs_force_inline
void* _gs_slot_array_get_func(gs_dyn_array(uint32_t) indices, void** data, uint32_t hndl, size_t val_len)
{
    return (((uint8_t*)(*data) + indices[hndl] * val_len));
}

gs_force_inline
void _component_data_entity_add(component_data_t* cd, uint32_t ent_hndl, void* comp_data)
{
    uint32_t idx = gs_slot_array_insert_func(&cd->indices, &cd->data, comp_data, cd->data_size, NULL);
    gs_hash_table_insert(cd->lookup, ent_hndl, idx);
}

gs_force_inline
void* _component_data_entity_get(component_data_t* cd, uint32_t ent_hndl)
{
    if (!gs_hash_table_empty(cd->lookup) && gs_hash_table_key_exists(cd->lookup, ent_hndl))
    {
        uint32_t idx = gs_hash_table_get(cd->lookup, ent_hndl);
        if (cd->data && cd->indices && idx < gs_dyn_array_size(cd->indices) && idx != GS_SLOT_ARRAY_INVALID_HANDLE)
        {
            return (((uint8_t*)cd->data) + cd->indices[idx] * cd->data_size);
        }
    }
    return NULL;
} 

gs_force_inline
void _component_data_entity_remove(component_data_t* cd, uint32_t ent_hndl, obj_dtor_func dtor)
{
    uint32_t idx = gs_hash_table_get(cd->lookup, ent_hndl);
    component_base_t* cb = (component_base_t*)(((uint8_t*)cd->data) + cd->indices[idx] * cd->data_size);
    (*dtor)(cb);
    _gs_slot_array_erase_func(cd->indices, cd->data, idx, cd->data_size);
    gs_hash_table_erase(cd->lookup, ent_hndl);
} 

gs_force_inline
void* _component_data_entity_has(component_data_t* cd, uint32_t ent_hndl)
{
    return gs_hash_table_key_exists(cd->lookup, ent_hndl);
}

introspect()
typedef struct entity_t
{
	base(object_t)

	uint32_t id;
	gs_dyn_array(uint64_t) components;	// Component id handle array
} entity_t;

typedef uint32_t entity_handle_t;

typedef struct entity_manager_t
{
	base(object_t)

	gs_slot_array(entity_t) entities;
	gs_hash_table(uint64_t, component_data_t*) components;
	component_data_t* cd; // Temporary pointer to component data for macro accessor
} entity_manager_t;

GS_API_DECL void entity_manager_init(entity_manager_t* entities);
GS_API_DECL uint32_t entities_allocate(entity_manager_t* entities);
GS_API_DECL void entities_deallocate(entity_manager_t* entities, uint32_t hndl);
GS_API_DECL bool _entities_has_component_internal(entity_manager_t* entities, uint64_t cid, entity_handle_t ent);
GS_API_DECL void entities_free(entity_manager_t* entities); 
GS_API_DECL bool entities_entity_is_valid(entity_manager_t* entities, uint32_t hndl); 
GS_API_DECL void entities_remove_component_w_id(entity_manager_t* entities, uint32_t hndl, uint64_t component_id);
GS_API_DECL void entities_update(entity_manager_t* entities);

// Mechanism for getting entity manager instance
#define entity_manager_instance() _g_entity_manager

#define entities_register_component(ENTITIES, T)\
	do {\
		uint64_t id = gs_hash_str64(gs_to_str(T));\
		if (!gs_hash_table_exists((ENTITIES)->components, id))\
		{\
			component_data_t* cd = (component_data_t*)gs_malloc_init(component_data_t);\
            cd->data_size = sizeof(T);\
            cd->cid = obj_sid(T);\
			gs_hash_table_insert((ENTITIES)->components, id, cd);\
		}\
	} while (0)

#define entities_has_component(ENTITIES, HNDL, T)\
	(_entities_has_component_internal(ENTITIES, obj_sid(T), HNDL))

#define entities_add_component(ENTITIES, HNDL, T, ...)\
    do {\
		entity_manager_t* _ENTS = (ENTITIES);\
        entity_t* _ENT = gs_slot_array_getp(_ENTS->entities, (HNDL));\
        uint64_t _ID = obj_sid(T);\
        component_data_t* _CD = gs_hash_table_get(_ENTS->components, _ID);\
        if (!gs_hash_table_exists(_CD->lookup, (HNDL)))\
        {\
            T _COMP = __VA_ARGS__;\
            obj_id(&_COMP) = _ID;\
            _COMP._base.entity = (HNDL);\
            T##_on_create(&_COMP);\
            _component_data_entity_add(_CD, (HNDL), &_COMP);\
            gs_dyn_array_push(_ENT->components, _ID);\
        }\
    } while (0) 

#define entities_remove_component(ENTITIES, HNDL, T)\
    do {\
		entity_manager_t* _ENTS = (ENTITIES);\
        entity_t* _ENT = gs_slot_array_getp(_ENTS->entities, HNDL);\
        uint64_t _ID = obj_sid(T);\
        component_data_t* _CD = gs_hash_table_get(_ENTS->components, _ID);\
        obj_dtor_func _DTOR = T##_dtor;\
        _component_data_entity_remove(_CD, HNDL, _DTOR);\
        int32_t _IDX = -1;\
        for (uint32_t _I = 0; _I < _ENT->components ; ++_I)\
        {\
            if (_ENT->components[_I] == _ID)\
            {\
                _IDX = _I;\
                break;\
            }\
        }\
        if (_IDX != -1)\
        {\
            _ENT->components[_IDX] = gs_dyn_array_back(_ENT->components);\
            gs_dyn_array_pop(_ENT->components);\
        }\
    } while (0)

#define entities_get_component(ENTITIES, HNDL, T)\
    _entities_get_component_internal((ENTITIES), (HNDL), obj_sid(T)) 

#ifdef ENTITY_IMPL

// Global instance of asset manager
entity_manager_t* _g_entity_manager = NULL; 

GS_API_DECL void entity_manager_init(entity_manager_t* entities)
{
    // Can only be ONE entity manager instance
    gs_assert(!_g_entity_manager); 
    _g_entity_manager = entities;
}

GS_API_DECL uint32_t entities_allocate(entity_manager_t* entities)
{
	entity_t e = gs_default_val();
	uint32_t hndl = gs_slot_array_insert(entities->entities, e);
	gs_slot_array_getp(entities->entities, hndl)->id = hndl;
	return hndl;
}

GS_API_DECL void entities_deallocate(entity_manager_t* entities, uint32_t hndl)
{ 
    // Get raw entity
    entity_t* ent = gs_slot_array_getp(entities->entities, hndl);

    if (ent)
    {
        for (uint32_t i = 0; i < gs_dyn_array_size(ent->components); ++i) 
        {
            entities_remove_component_w_id(entities, hndl, ent->components[i], false);
        }
    } 

    // Erase entity
    gs_slot_array_erase(entities->entities, hndl);
}

GS_API_DECL bool entities_entity_is_valid(entity_manager_t* entities, uint32_t hndl)
{
    return gs_slot_array_handle_valid(entities->entities, hndl);
} 

GS_API_DECL bool _entities_has_component_internal(entity_manager_t* entities, uint64_t cid, uint32_t hndl)
{
	component_data_t* cd = gs_hash_table_get(entities->components, cid);
	return gs_hash_table_exists(cd->lookup, hndl);
}

GS_API_DECL component_base_t* _entities_get_component_internal(entity_manager_t* entities, uint32_t hndl, uint64_t cid)
{
    if (!entities_entity_is_valid(entities, hndl))
    {
        return NULL;
    }

    component_data_t* cd = gs_hash_table_get(entities->components, cid);
    return _component_data_entity_get(cd, hndl);
}


GS_API_DECL void entities_remove_component_w_id(entity_manager_t* entities, uint32_t hndl, uint64_t component_id) 
{
    entity_t* e = gs_slot_array_getp(entities->entities, hndl); 
    component_data_t* cd = gs_hash_table_get(entities->components, component_id); 
    obj_dtor_func dtor = obj_func_w_id(component_id, obj_dtor);
    _component_data_entity_remove(cd, hndl, dtor);
    int32_t idx = -1;
    for (uint32_t i = 0; i < e->components; ++i)
    {
        if (e->components[i] == component_id)
        {
            idx = i;
            break;\
        }
    }
    if (idx != -1)
    {
        e->components[idx] = gs_dyn_array_back(e->components);
        gs_dyn_array_pop(e->components);
    }
}

GS_API_DECL void entities_free(entity_manager_t* entities)
{
    // Free all component data 
    for (
        gs_hash_table_iter it = gs_hash_table_iter_new(entities->components);
        gs_hash_table_iter_valid(entities->components, it);
        gs_hash_table_iter_advance(entities->components, it)
    )
    {
        // Free component data
        gs_free(gs_hash_table_iter_get(entities->components, it));
    }
    gs_hash_table_free(entities->components);

    // Free all entity data 
    for (
        gs_slot_array_iter it = gs_slot_array_iter_new(entities->entities);
        gs_slot_array_iter_valid(entities->entities, it);
        gs_slot_array_iter_advance(entities->entities, it)
    )
    {
        // Free component list
        gs_dyn_array_free(gs_slot_array_iter_getp(entities->entities, it)->components);
    } 
    gs_slot_array_free(entities->entities);
}

GS_API_DECL void entities_update(entity_manager_t* entities)
{
    // Iterate through all component data, then update (this will be somewhat slow)
    for (
        gs_hash_table_iter it = gs_hash_table_iter_new(entities->components);
        gs_hash_table_iter_valid(entities->components, it);
        gs_hash_table_iter_advance(entities->components, it)
    )
    {
        component_data_t* cd = gs_hash_table_iter_get(entities->components, it); 

        // Get update function for component data 
        obj_on_update_func update = obj_func_w_id(cd->cid, obj_on_update);

        // Get slot array iterator
        for (
            gs_slot_array_iter sait = _gs_slot_array_iter_find_first_valid_index(cd->indices);
            (sait < gs_dyn_array_size(cd->indices) && cd->indices[sait] != GS_SLOT_ARRAY_INVALID_HANDLE); 
            _gs_slot_array_iter_advance_func(cd->indices, &sait)
        )
        { 
            // Grab pointer to component base 
            component_base_t* cb = (component_base_t*)(((uint8_t*)cd->data) + cd->indices[sait] * cd->data_size); 

            // Call update
            (*update)(cb);
        } 
    }
}

#endif // ENTITY_IMPL
#endif // ENTITY_H













