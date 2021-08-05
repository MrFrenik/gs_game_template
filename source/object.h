#ifndef OBJECT_H
#define OBJECT_H

// Forward declare
struct object_t;

typedef struct vtable_t
{
	void (* ctor)(struct object_t* obj);
	void (* dtor)(struct object_t* obj);
	gs_result (* serialize)(gs_byte_buffer_t* buffer, struct object_t* obj);
	gs_result (* deserialize)(gs_byte_buffer_t* buffer, struct object_t* obj);
} vtable_t;

typedef struct object_t
{
	vtable_t* table;
} object_t;

GS_API_DECL gs_result object_serialize_default(gs_byte_buffer_t* buffer, const object_t* in);
GS_API_DECL gs_result object_deserialize_default(gs_byte_buffer_t* buffer, object_t* out);

#define obj_ctor(T, ...)\
	obj_ctor_T(__VA_ARGS__);

#define BASE(T) T _base
#define obj_id(T) gs_hash_str64(gs_to_str(T))
#define _introspect(...) gs_empty_instruction()

#define obj_malloc(T)\
	_obj_malloc_internal(uint64_t id);

#endif
