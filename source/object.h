/*==============================================================================================================
    * Copyright: 2020 John Jackson 
    * Gunslinger Game Template
    * File: object.h
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

#ifndef OBJECT_H
#define OBJECT_H

// Forward decl
struct meta_t;

typedef struct object_t
{
	uint64_t cls_id;
} object_t; 

// Useful typedefs
typedef void (* obj_ctor_func)(object_t* obj, void* args); 
typedef void (* obj_dtor_func)(object_t* obj); 
typedef gs_result (* obj_serialize_func)(gs_byte_buffer_t*, object_t*); 
typedef gs_result (* obj_deserialize_func)(gs_byte_buffer_t*, object_t*); 

// Default Functions
GS_API_DECL gs_result object_serialize_default(gs_byte_buffer_t* buffer, const object_t* in);
GS_API_DECL gs_result object_deserialize_default(gs_byte_buffer_t* buffer, object_t* out);
GS_API_DECL object_t* object_malloc_default(size_t sz);
GS_API_DECL void object_ctor_default(object_t* obj);
GS_API_DECL void object_dtor_default(object_t* obj);

// Utility
GS_API_DECL void obj_dump(struct meta_t* meta, void* obj, gs_meta_class_t* cls);

// Internal functions
GS_API_PRIVATE object_t* _obj_new_internal(uint64_t id);
GS_API_PRIVATE void _obj_ctor_internal(uint64_t id, object_t* obj, void* args);
GS_API_PRIVATE void _obj_dtor_internal(uint64_t id, object_t* obj);
GS_API_PRIVATE gs_result _obj_serialize_internal(uint64_t id, gs_byte_buffer_t* buffer, object_t* in);
GS_API_PRIVATE gs_result _obj_deserialize_internal(uint64_t id, gs_byte_buffer_t* buffer, object_t* out); 

// Useful defines
#define base(T) T _base;
#define ctor(...) gs_empty_instruction()
#define dtor(...) gs_empty_instruction() 
#define introspect(...) gs_empty_instruction()
#define field(...) gs_empty_instruction()
#define serialize(...) gs_empty_instruction()
#define deserialize(...) gs_empty_instruction()
#define cast(A, B) ((B*)(A))

// Reflection defines
#define vtable(...) /* __VA_ARGS__ */
#define ignore(...) gs_empty_instruction() 

// Object functions
#define obj_id(OBJ)                  (cast((OBJ), object_t)->cls_id)  // Get object class id from instance
#define obj_sid(T)                   gs_hash_str64(gs_to_str(T))    // Get object class id from static type
#define obj_vtable_w_id(ID)          (&(gs_hash_table_getp(meta_get_instance()->registry.classes, ID)->vtable)) // Get vtable for class
#define obj_func_w_id(ID, NAME)      gs_meta_func_getp_w_id(&meta_get_instance()->registry, ID, NAME) // Get function in vtable based on name
#define obj_new(T)                   _obj_new_internal(obj_sid(T)) // Heap allocate object of type based on type
#define obj_newid(ID)                _obj_new_internal(ID) // Heap allocate object based on cls id

#define obj_ctor(T, ...)                T##_ctor(__VA_ARGS__) 
#define obj_dtor(T, OBJ)                T##_dtor(OBJ) 
#define obj_serialize(T, BUFFER, OBJ)   T##_serialize(BUFFER, OBJ)
#define obj_deserialize(T, BUFFER, OBJ) T##_deserialize(BUFFER, OBJ)

typedef struct meta_t
{
	gs_meta_registry_t registry;
} meta_t;

// Get global instance to meta information
GS_API_DECL meta_t meta_new();
GS_API_DECL meta_t* meta_get_instance();
GS_API_DECL void meta_set_instance(meta_t* meta);
GS_API_DECL void meta_register_gs(meta_t* meta); 

#ifdef OBJECT_IMPL

//==== [ Meta ] ====

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

//===== [ Object functions ] ======

// Internal functions
GS_API_PRIVATE object_t* _obj_new_internal(uint64_t id)
{ 
    const gs_meta_class_t* cls = gs_meta_class_get_w_id(&meta_get_instance()->registry, id);
    object_t* obj = gs_malloc(cls->size);
    obj->cls_id = id;
    return obj;
} 

//===== [ Utils ] ======

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

//==== [ Object defaults ] ====

GS_API_DECL gs_result object_serialize_default(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_SUCCESS;
}

GS_API_DECL gs_result object_deserialize_default(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_SUCCESS;
} 


#endif // OBJECT_IMPL
#endif // OBJECT_H

















