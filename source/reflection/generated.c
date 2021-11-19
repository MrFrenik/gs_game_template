
//============[ Implementation ]==============//

/*
 * This file has been generated. All modifications will be lost.
*/

GS_API_DECL void meta_register_generated(gs_meta_registry_t* meta)
{
	// == [ enums ] == 

	// asset_state
	gs_meta_enum_register(meta, (&(gs_meta_enum_decl_t) {
		.values = (gs_meta_enum_value_t[]) {
			(gs_meta_enum_value_t){.name = gs_to_str(ASSET_STATE_UNLOADED)},
			(gs_meta_enum_value_t){.name = gs_to_str(ASSET_STATE_LOADED)}
		},
		.size = 2 * sizeof(gs_meta_enum_value_t),
		.name = gs_to_str(asset_state)
	}));

	// == [ classes ] == 

	// vtable
	gs_meta_vtable_t my_comp_t_vt = gs_default_val();

	// my_comp_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(my_comp_t, float, f_val, GS_META_PROPERTY_TYPE_INFO_F32)
		},
		.size = 1 * sizeof(gs_meta_property_t),
		.vtable = &my_comp_t_vt,
		.name = gs_to_str(my_comp_t),
		.base = gs_to_str(component_base_t),
		.cls_size = sizeof(my_comp_t)
	}));

	// vtable
	gs_meta_vtable_t texture_t_vt = gs_default_val();
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(serialize)), (void*)texture_serialize);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(deserialize)), (void*)texture_deserialize);

	// texture_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &texture_t_vt,
		.name = gs_to_str(texture_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(texture_t)
	}));

	// vtable
	gs_meta_vtable_t asset_t_vt = gs_default_val();

	// asset_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &asset_t_vt,
		.name = gs_to_str(asset_t),
		.base = gs_to_str(object_t),
		.cls_size = sizeof(asset_t)
	}));

	// vtable
	gs_meta_vtable_t pipeline_t_vt = gs_default_val();

	// pipeline_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &pipeline_t_vt,
		.name = gs_to_str(pipeline_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(pipeline_t)
	}));

	// vtable
	gs_meta_vtable_t mesh_t_vt = gs_default_val();

	// mesh_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &mesh_t_vt,
		.name = gs_to_str(mesh_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(mesh_t)
	}));

	// vtable
	gs_meta_vtable_t material_t_vt = gs_default_val();

	// material_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &material_t_vt,
		.name = gs_to_str(material_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(material_t)
	}));

	// vtable
	gs_meta_vtable_t component_transform_t_vt = gs_default_val();

	// component_transform_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(component_transform_t, gs_vqs, transform, GS_META_PROPERTY_TYPE_INFO_VQS)
		},
		.size = 1 * sizeof(gs_meta_property_t),
		.vtable = &component_transform_t_vt,
		.name = gs_to_str(component_transform_t),
		.base = gs_to_str(component_base_t),
		.cls_size = sizeof(component_transform_t)
	}));

	// vtable
	gs_meta_vtable_t font_t_vt = gs_default_val();

	// font_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &font_t_vt,
		.name = gs_to_str(font_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(font_t)
	}));

	// vtable
	gs_meta_vtable_t component_base_t_vt = gs_default_val();

	// component_base_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &component_base_t_vt,
		.name = gs_to_str(component_base_t),
		.base = gs_to_str(object_t),
		.cls_size = sizeof(component_base_t)
	}));

	// vtable
	gs_meta_vtable_t audio_source_t_vt = gs_default_val();

	// audio_source_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &audio_source_t_vt,
		.name = gs_to_str(audio_source_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(audio_source_t)
	}));

	// vtable
	gs_meta_vtable_t component_physics_t_vt = gs_default_val();

	// component_physics_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(component_physics_t, gs_vec3, velocity, GS_META_PROPERTY_TYPE_INFO_VEC3),
			gs_meta_property(component_physics_t, gs_vec3, acceleration, GS_META_PROPERTY_TYPE_INFO_VEC3)
		},
		.size = 2 * sizeof(gs_meta_property_t),
		.vtable = &component_physics_t_vt,
		.name = gs_to_str(component_physics_t),
		.base = gs_to_str(component_base_t),
		.cls_size = sizeof(component_physics_t)
	}));

	// vtable
	gs_meta_vtable_t entity_t_vt = gs_default_val();

	// entity_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &entity_t_vt,
		.name = gs_to_str(entity_t),
		.base = gs_to_str(object_t),
		.cls_size = sizeof(entity_t)
	}));

	// vtable
	gs_meta_vtable_t component_renderable_t_vt = gs_default_val();

	// component_renderable_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(component_renderable_t, uint32_t, renderable_id, GS_META_PROPERTY_TYPE_INFO_U32)
		},
		.size = 1 * sizeof(gs_meta_property_t),
		.vtable = &component_renderable_t_vt,
		.name = gs_to_str(component_renderable_t),
		.base = gs_to_str(component_base_t),
		.cls_size = sizeof(component_renderable_t)
	}));

}


// ==== Objects API === //

// == my_comp_t API == //

GS_API_DECL my_comp_t my_comp_t_ctor(void)
{
	my_comp_t _obj = gs_default_val();
	my_comp_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(my_comp_t);
	{
            this->f_val = 3.145f;
        }
	return _obj;
}

GS_API_DECL void my_comp_t_dtor(my_comp_t* obj)
{
	my_comp_t* this = obj;
}
GS_API_DECL gs_result my_comp_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	my_comp_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result my_comp_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	my_comp_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == texture_t API == //

GS_API_DECL texture_t texture_t_ctor()
{
	texture_t _obj = gs_default_val();
	texture_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(texture_t);
	return _obj;
}

GS_API_DECL void texture_t_dtor(texture_t* obj)
{
	texture_t* this = obj;
}
GS_API_DECL gs_result texture_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	texture_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result texture_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	texture_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == asset_t API == //

GS_API_DECL asset_t asset_t_ctor()
{
	asset_t _obj = gs_default_val();
	asset_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(asset_t);
	return _obj;
}

GS_API_DECL void asset_t_dtor(asset_t* obj)
{
	asset_t* this = obj;
}
GS_API_DECL gs_result asset_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	asset_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result asset_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	asset_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == pipeline_t API == //

GS_API_DECL pipeline_t pipeline_t_ctor()
{
	pipeline_t _obj = gs_default_val();
	pipeline_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(pipeline_t);
	return _obj;
}

GS_API_DECL void pipeline_t_dtor(pipeline_t* obj)
{
	pipeline_t* this = obj;
}
GS_API_DECL gs_result pipeline_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	pipeline_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result pipeline_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	pipeline_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == mesh_t API == //

GS_API_DECL mesh_t mesh_t_ctor()
{
	mesh_t _obj = gs_default_val();
	mesh_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(mesh_t);
	return _obj;
}

GS_API_DECL void mesh_t_dtor(mesh_t* obj)
{
	mesh_t* this = obj;
}
GS_API_DECL gs_result mesh_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	mesh_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result mesh_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	mesh_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == material_t API == //

GS_API_DECL material_t material_t_ctor()
{
	material_t _obj = gs_default_val();
	material_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(material_t);
	return _obj;
}

GS_API_DECL void material_t_dtor(material_t* obj)
{
	material_t* this = obj;
}
GS_API_DECL gs_result material_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	material_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result material_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	material_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == component_transform_t API == //

GS_API_DECL component_transform_t component_transform_t_ctor(void)
{
	component_transform_t _obj = gs_default_val();
	component_transform_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_transform_t);
	{
            this->transform = gs_vqs_default();
        }
	return _obj;
}

GS_API_DECL void component_transform_t_dtor(component_transform_t* obj)
{
	component_transform_t* this = obj;
}
GS_API_DECL gs_result component_transform_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	component_transform_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result component_transform_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	component_transform_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == font_t API == //

GS_API_DECL font_t font_t_ctor()
{
	font_t _obj = gs_default_val();
	font_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(font_t);
	return _obj;
}

GS_API_DECL void font_t_dtor(font_t* obj)
{
	font_t* this = obj;
}
GS_API_DECL gs_result font_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	font_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result font_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	font_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == component_base_t API == //

GS_API_DECL component_base_t component_base_t_ctor()
{
	component_base_t _obj = gs_default_val();
	component_base_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_base_t);
	return _obj;
}

GS_API_DECL void component_base_t_dtor(component_base_t* obj)
{
	component_base_t* this = obj;
}
GS_API_DECL gs_result component_base_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	component_base_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result component_base_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	component_base_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == audio_source_t API == //

GS_API_DECL audio_source_t audio_source_t_ctor()
{
	audio_source_t _obj = gs_default_val();
	audio_source_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(audio_source_t);
	return _obj;
}

GS_API_DECL void audio_source_t_dtor(audio_source_t* obj)
{
	audio_source_t* this = obj;
}
GS_API_DECL gs_result audio_source_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	audio_source_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result audio_source_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	audio_source_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == component_physics_t API == //

GS_API_DECL component_physics_t component_physics_t_ctor()
{
	component_physics_t _obj = gs_default_val();
	component_physics_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_physics_t);
	return _obj;
}

GS_API_DECL void component_physics_t_dtor(component_physics_t* obj)
{
	component_physics_t* this = obj;
}
GS_API_DECL gs_result component_physics_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	component_physics_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result component_physics_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	component_physics_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == entity_t API == //

GS_API_DECL entity_t entity_t_ctor()
{
	entity_t _obj = gs_default_val();
	entity_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(entity_t);
	return _obj;
}

GS_API_DECL void entity_t_dtor(entity_t* obj)
{
	entity_t* this = obj;
}
GS_API_DECL gs_result entity_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	entity_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result entity_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	entity_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

// == component_renderable_t API == //

GS_API_DECL component_renderable_t component_renderable_t_ctor()
{
	component_renderable_t _obj = gs_default_val();
	component_renderable_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_renderable_t);
	return _obj;
}

GS_API_DECL void component_renderable_t_dtor(component_renderable_t* obj)
{
	component_renderable_t* this = obj;
}
GS_API_DECL gs_result component_renderable_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	component_renderable_t* this = in;
	return GS_RESULT_INCOMPLETE;
}
GS_API_DECL gs_result component_renderable_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	component_renderable_t* this = out;
	return GS_RESULT_INCOMPLETE;
}

