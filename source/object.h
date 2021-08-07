#ifndef OBJECT_H
#define OBJECT_H

// Forward decl
struct meta_t;

typedef struct object_t
{
	uint64_t cls_id;
} object_t; 

// Useful typedefs
typedef void (* obj_ctor_func)(object_t* obj); 
typedef void (* obj_dtor_func)(object_t* obj); 
typedef object_t* (* obj_malloc_func)(size_t); 
typedef gs_result (* obj_serialize_func)(gs_byte_buffer_t*, object_t*); 
typedef gs_result (* obj_deserialize_func)(gs_byte_buffer_t*, object_t*); 

typedef struct vtable_t
{
	void (* ctor)(object_t* obj);
	void (* dtor)(object_t* obj);
	object_t* (* malloc)(size_t sz);
	gs_result (* serialize)(gs_byte_buffer_t* buffer, object_t* obj);
	gs_result (* deserialize)(gs_byte_buffer_t* buffer, object_t* obj); 
} vtable_t; 

GS_API_DECL gs_result object_serialize_default(gs_byte_buffer_t* buffer, const object_t* in);
GS_API_DECL gs_result object_deserialize_default(gs_byte_buffer_t* buffer, object_t* out);
GS_API_DECL object_t* object_malloc_default(size_t sz);
GS_API_DECL void object_ctor_default(object_t* obj);
GS_API_DECL void object_dtor_default(object_t* obj);

GS_API_DECL object_t* _obj_new_internal(uint64_t id, size_t sz);

GS_API_DECL void obj_dump(struct meta_t* meta, void* obj, gs_meta_class_t* cls);

#define obj_ctor(T, ...)\
	obj_ctor_T(__VA_ARGS__); 

#define base(T) T _base;
#define vtable(...) /* __VA_ARGS__ */
#define ctor(...) //
#define ignore(...) gs_empty_instruction()
#define cast(A, B) ((B*)(A))

#define cls_id(OBJ) (CAST(OBJ, object_t)->cls_id)
#define obj_id(T) gs_hash_str64(gs_to_str(T))
#define obj_vtable_w_id(ID)\
	(gs_hash_table_getp(meta_get_instance()->vtables, ID))

#define obj_new(T)\
	_obj_new_internal(obj_id(T), sizeof(T))

#define introspect(...) gs_empty_instruction()

typedef struct meta_t
{
	gs_meta_registry_t registry;
	gs_hash_table(uint64_t, vtable_t) vtables;
} meta_t;

// Get global instance to meta information
GS_API_DECL meta_t meta_new();
GS_API_DECL meta_t* meta_get_instance();
GS_API_DECL void meta_set_instance(meta_t* meta);
GS_API_DECL void meta_register_gs(meta_t* meta);
GS_API_DECL void _meta_register_vtable_internal(meta_t* meta, uint64_t* id, vtable_t* table);

#define meta_register_vtable(META, T, TABLE)\
	_meta_register_vtable_internal(META, obj_id(T), TABLE)


#ifdef OBJECT_IMPL

GS_API_DECL object_t* _obj_new_internal(uint64_t id, size_t sz)
{
	void* obj = gs_malloc(sz);
	((object_t*)obj)->cls_id = id;
	return (object_t*)obj;
}

GS_API_DECL gs_result object_serialize_default(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_SUCCESS;
}

GS_API_DECL gs_result object_deserialize_default(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_SUCCESS;
}

GS_API_DECL object_t* object_malloc_default(size_t sz)
{
	void* data = gs_malloc(sz);
	return (object_t*)data;
}

GS_API_DECL void object_ctor_default(object_t* obj)
{
	// Nothing for now...
}

GS_API_DECL void object_dtor_default(object_t* obj)
{
	// Nothing for now...
}

meta_t* g_meta = NULL;

GS_API_DECL meta_t meta_new()
{
	meta_t meta = gs_default_val();
	return meta;
}

GS_API_DECL void meta_set_instance(meta_t* meta)
{
	g_meta = meta;
}

// Get global instance to meta information
GS_API_DECL meta_t* meta_get_instance()
{
	return g_meta;
}

GS_API_DECL void _meta_register_vtable_internal(meta_t* meta, uint64_t* id, vtable_t* itable)
{
	if (!itable)
	{
		return;
	}

	vtable_t table = *itable;
	table.ctor = table.ctor ? table.ctor : object_ctor_default;
	table.dtor = table.dtor ? table.dtor : object_dtor_default;
	table.malloc = table.malloc ? table.malloc : object_malloc_default; 
	table.serialize = table.serialize ? table.serialize : object_serialize_default;
	table.deserialize = table.deserialize ? table.deserialize : object_deserialize_default;
	gs_hash_table_insert(meta->vtables, id, table);
}

GS_API_DECL void meta_register_gs(meta_t* meta)
{
	//==== [ GS Meta ] ===== (these need to be manually registered for now)

	// gs_graphics_texture_desc_t
    gs_meta_class_register(&meta->registry, (&(gs_meta_class_decl_t){
        .properties = (gs_meta_property_t[]) {
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, width, GS_META_PROPERTY_TYPE_INFO_U32),
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, height, GS_META_PROPERTY_TYPE_INFO_U32),
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, format, GS_META_PROPERTY_TYPE_INFO_U32),
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, wrap_s, GS_META_PROPERTY_TYPE_INFO_U32),
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, wrap_t, GS_META_PROPERTY_TYPE_INFO_U32),
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, min_filter, GS_META_PROPERTY_TYPE_INFO_U32),
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, mag_filter, GS_META_PROPERTY_TYPE_INFO_U32),
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, mip_filter, GS_META_PROPERTY_TYPE_INFO_U32),
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, num_mips, GS_META_PROPERTY_TYPE_INFO_U32),
            gs_meta_property(gs_graphics_texture_desc_t, uint32_t, render_target, GS_META_PROPERTY_TYPE_INFO_U32)
        },
        .size = 10 * sizeof(gs_meta_property_t),
		.name = gs_to_str(gs_graphics_texture_desc_t)
    }));

	// gs_asset_texture_t
    gs_meta_class_register(&meta->registry, (&(gs_meta_class_decl_t){
        .properties = (gs_meta_property_t[]) {
            gs_meta_property(gs_asset_texture_t, gs_graphics_texture_desc_t, desc, GS_META_PROPERTY_TYPE_INFO_OBJ),
        },
        .size = 1 * sizeof(gs_meta_property_t),
		.name = gs_to_str(gs_asset_texture_t)
    }));
}


GS_API_DECL void obj_dump(meta_t* meta, void* obj, gs_meta_class_t* cls)
{
	gs_println("cls: %s, ct: %zu", cls->name, cls->property_count);

	for (uint32_t i = 0; i < cls->property_count; ++i)
	{
		const gs_meta_property_t* prop = &cls->properties[i];
		switch (prop->type.id)
		{
			case GS_META_PROPERTY_TYPE_U32:
			{
				uint32_t* v = gs_meta_getvp(obj, uint32_t, prop);
				gs_println("\t%s (%s): %zu", prop->name, prop->type.name, *v);
			} break;

			case GS_META_PROPERTY_TYPE_OBJ:
			{
				gs_println("\%s (%s): ", prop->name, prop->type.name);
				void* _obj = gs_meta_getvp(obj, void, prop);
				const gs_meta_class_t* clz = gs_meta_class_get_w_name(&meta->registry, prop->type_name); 
				if (_obj && clz)
				{
					obj_dump(meta, _obj, clz);
				}
			} break;
		}
	}
}

#endif // OBJECT_IMPL
#endif // OBJECT_H



















