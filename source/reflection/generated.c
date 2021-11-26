
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
	// componentsble_free
	gs_meta_enum_register(meta, (&(gs_meta_enum_decl_t) {
		.values = (gs_meta_enum_value_t[]) {
			(gs_meta_enum_value_t){.name = gs_to_str(for)},
			(gs_meta_enum_value_t){.name = gs_to_str(gs_hash_table_iter)},
			(gs_meta_enum_value_t){.name = gs_to_str(it)},
			(gs_meta_enum_value_t){.name = gs_to_str(entities)},
			(gs_meta_enum_value_t){.name = gs_to_str(components)},
			(gs_meta_enum_value_t){.name = gs_to_str(gs_hash_table_iter_valid)},
			(gs_meta_enum_value_t){.name = gs_to_str(entities)},
			(gs_meta_enum_value_t){.name = gs_to_str(components)},
			(gs_meta_enum_value_t){.name = gs_to_str(it)},
			(gs_meta_enum_value_t){.name = gs_to_str(gs_hash_table_iter_advance)},
			(gs_meta_enum_value_t){.name = gs_to_str(entities)},
			(gs_meta_enum_value_t){.name = gs_to_str(components)},
			(gs_meta_enum_value_t){.name = gs_to_str(it)},
			(gs_meta_enum_value_t){.name = gs_to_str(gs_free)},
			(gs_meta_enum_value_t){.name = gs_to_str(gs_hash_table_iter_get)},
			(gs_meta_enum_value_t){.name = gs_to_str(entities)},
			(gs_meta_enum_value_t){.name = gs_to_str(components)},
			(gs_meta_enum_value_t){.name = gs_to_str(it)}
		},
		.size = 18 * sizeof(gs_meta_enum_value_t),
		.name = gs_to_str(componentsble_free)
	}));

	// == [ classes ] == 

	// vtable
	gs_meta_vtable_t component_rotation_t_vt = gs_default_val();
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_3141622024);
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_3141622024);
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_3141622024);
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_3141622024);
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_3141622024);
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_3141622024);
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_3141622024);
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_3141622024);
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_3141622024);
	gs_hash_table_insert(component_rotation_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_3141622024);

	// component_rotation_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(component_rotation_t, float, rotation_speed, GS_META_PROPERTY_TYPE_INFO_F32),
			gs_meta_property(component_rotation_t, gs_vec3, rotation_axis, GS_META_PROPERTY_TYPE_INFO_VEC3)
		},
		.size = 2 * sizeof(gs_meta_property_t),
		.vtable = &component_rotation_t_vt,
		.name = gs_to_str(component_rotation_t),
		.base = gs_to_str(component_base_t),
		.cls_size = sizeof(component_rotation_t)
	}));

	// vtable
	gs_meta_vtable_t texture_t_vt = gs_default_val();
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_1462486611);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_1462486611);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_1462486611);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_1462486611);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_1462486611);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_1462486611);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_1462486611);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_1462486611);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_1462486611);
	gs_hash_table_insert(texture_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_1462486611);

	// texture_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(texture_t, gs_asset_texture_t, texture, GS_META_PROPERTY_TYPE_INFO_OBJ)
		},
		.size = 1 * sizeof(gs_meta_property_t),
		.vtable = &texture_t_vt,
		.name = gs_to_str(texture_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(texture_t)
	}));

	// vtable
	gs_meta_vtable_t asset_t_vt = gs_default_val();
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_335738428);
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_335738428);
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_335738428);
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_335738428);
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_335738428);
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_335738428);
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_335738428);
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_335738428);
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_335738428);
	gs_hash_table_insert(asset_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_335738428);

	// asset_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &asset_t_vt,
		.name = gs_to_str(asset_t),
		.base = gs_to_str(object_t),
		.cls_size = sizeof(asset_t)
	}));

	// vtable
	gs_meta_vtable_t pipeline_t_vt = gs_default_val();
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_3640749902);
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_3640749902);
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_3640749902);
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_3640749902);
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_3640749902);
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_3640749902);
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_3640749902);
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_3640749902);
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_3640749902);
	gs_hash_table_insert(pipeline_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_3640749902);

	// pipeline_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &pipeline_t_vt,
		.name = gs_to_str(pipeline_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(pipeline_t)
	}));

	// vtable
	gs_meta_vtable_t mesh_t_vt = gs_default_val();
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_1910793471);
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_1910793471);
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_1910793471);
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_1910793471);
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_1910793471);
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_1910793471);
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_1910793471);
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_1910793471);
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_1910793471);
	gs_hash_table_insert(mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_1910793471);

	// mesh_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &mesh_t_vt,
		.name = gs_to_str(mesh_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(mesh_t)
	}));

	// vtable
	gs_meta_vtable_t material_t_vt = gs_default_val();
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_2657371751);
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_2657371751);
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_2657371751);
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_2657371751);
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_2657371751);
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_2657371751);
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_2657371751);
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_2657371751);
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_2657371751);
	gs_hash_table_insert(material_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_2657371751);

	// material_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(material_t, asset_handle_t, pipeline, GS_META_PROPERTY_TYPE_INFO_OBJ)
		},
		.size = 1 * sizeof(gs_meta_property_t),
		.vtable = &material_t_vt,
		.name = gs_to_str(material_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(material_t)
	}));

	// vtable
	gs_meta_vtable_t component_transform_t_vt = gs_default_val();
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_3446999376);
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_3446999376);
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_3446999376);
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_3446999376);
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_3446999376);
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_3446999376);
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_3446999376);
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_3446999376);
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_3446999376);
	gs_hash_table_insert(component_transform_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_3446999376);

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
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_477039743);
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_477039743);
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_477039743);
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_477039743);
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_477039743);
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_477039743);
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_477039743);
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_477039743);
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_477039743);
	gs_hash_table_insert(font_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_477039743);

	// font_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &font_t_vt,
		.name = gs_to_str(font_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(font_t)
	}));

	// vtable
	gs_meta_vtable_t component_base_t_vt = gs_default_val();
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_3118336809);
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_3118336809);
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_3118336809);
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_3118336809);
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_3118336809);
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_3118336809);
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_3118336809);
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_3118336809);
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_3118336809);
	gs_hash_table_insert(component_base_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_3118336809);

	// component_base_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(component_base_t, uint32_t, entity, GS_META_PROPERTY_TYPE_INFO_U32)
		},
		.size = 1 * sizeof(gs_meta_property_t),
		.vtable = &component_base_t_vt,
		.name = gs_to_str(component_base_t),
		.base = gs_to_str(object_t),
		.cls_size = sizeof(component_base_t)
	}));

	// vtable
	gs_meta_vtable_t audio_source_t_vt = gs_default_val();
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_1947392792);
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_1947392792);
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_1947392792);
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_1947392792);
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_1947392792);
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_1947392792);
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_1947392792);
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_1947392792);
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_1947392792);
	gs_hash_table_insert(audio_source_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_1947392792);

	// audio_source_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &audio_source_t_vt,
		.name = gs_to_str(audio_source_t),
		.base = gs_to_str(asset_t),
		.cls_size = sizeof(audio_source_t)
	}));

	// vtable
	gs_meta_vtable_t component_static_mesh_t_vt = gs_default_val();
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_2159547720);
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_2159547720);
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_2159547720);
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_2159547720);
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_2159547720);
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_2159547720);
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_2159547720);
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_2159547720);
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_2159547720);
	gs_hash_table_insert(component_static_mesh_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_2159547720);

	// component_static_mesh_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(component_static_mesh_t, uint32_t, renderable_id, GS_META_PROPERTY_TYPE_INFO_U32),
			gs_meta_property(component_static_mesh_t, graphics_scene_t, scene, GS_META_PROPERTY_TYPE_INFO_OBJ)
		},
		.size = 2 * sizeof(gs_meta_property_t),
		.vtable = &component_static_mesh_t_vt,
		.name = gs_to_str(component_static_mesh_t),
		.base = gs_to_str(component_base_t),
		.cls_size = sizeof(component_static_mesh_t)
	}));

	// vtable
	gs_meta_vtable_t component_physics_t_vt = gs_default_val();
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_3852747479);
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_3852747479);
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_3852747479);
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_3852747479);
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_3852747479);
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_3852747479);
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_3852747479);
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_3852747479);
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_3852747479);
	gs_hash_table_insert(component_physics_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_3852747479);

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
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_1210153879);
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_1210153879);
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_1210153879);
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_1210153879);
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_1210153879);
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_1210153879);
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_1210153879);
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_1210153879);
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_1210153879);
	gs_hash_table_insert(entity_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_1210153879);

	// entity_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.vtable = &entity_t_vt,
		.name = gs_to_str(entity_t),
		.base = gs_to_str(object_t),
		.cls_size = sizeof(entity_t)
	}));

	// vtable
	gs_meta_vtable_t component_renderable_t_vt = gs_default_val();
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_ctor)), (void*)obj_ctor_218119740);
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_new)), (void*)obj_new_218119740);
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_dtor)), (void*)obj_dtor_218119740);
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_serialize)), (void*)obj_serialize_218119740);
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_deserialize)), (void*)obj_deserialize_218119740);
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_create)), (void*)obj_on_create_218119740);
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_start)), (void*)obj_on_start_218119740);
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_stop)), (void*)obj_on_stop_218119740);
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_update)), (void*)obj_on_update_218119740);
	gs_hash_table_insert(component_renderable_t_vt.funcs, gs_hash_str64(gs_to_str(obj_on_destroy)), (void*)obj_on_destroy_218119740);

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

// == component_rotation_t API == //

GS_API_DECL component_rotation_t obj_ctor_3141622024(float rotation_speed, gs_vec3 rotation_axis)
{
	component_rotation_t _obj = gs_default_val();
	component_rotation_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_rotation_t);
	{
            gs_println("CTOR");
            this->rotation_speed = rotation_speed;
            this->rotation_axis = rotation_axis;
        }
	return _obj;
}
GS_API_DECL object_t* obj_new_3141622024(float rotation_speed, gs_vec3 rotation_axis)
{
	component_rotation_t* _obj = gs_malloc_init(component_rotation_t);
	component_rotation_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(component_rotation_t);
	{
            gs_println("CTOR");
            this->rotation_speed = rotation_speed;
            this->rotation_axis = rotation_axis;
        }
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_3141622024(object_t* obj)
{
	component_rotation_t* this = (component_rotation_t*)obj;
	{
        gs_println("DTOR");
    }
}

GS_API_DECL gs_result obj_serialize_3141622024(gs_byte_buffer_t* buffer, const object_t* in)
{
	const component_rotation_t* this = (const component_rotation_t*)in;
	{ 
        gs_println("SERIALIZE");
        gs_byte_buffer_write(buffer, float, this->rotation_speed);
        return GS_RESULT_SUCCESS;
    }
	return GS_RESULT_SUCCESS;
}

GS_API_DECL gs_result obj_deserialize_3141622024(gs_byte_buffer_t* buffer, object_t* out)
{
	component_rotation_t* this = (component_rotation_t*)out;
	{
        gs_println("DESERIALIZE");
        gs_byte_buffer_read(buffer, float, &this->rotation_speed);
        return GS_RESULT_SUCCESS;
    }
	return GS_RESULT_SUCCESS;
}

GS_API_DECL void obj_on_create_3141622024(object_t* obj)
{
	component_rotation_t* this = (component_rotation_t*)obj;
	{ 
        gs_println("ON CREATE");
    }
}

GS_API_DECL void obj_on_start_3141622024(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_3141622024(object_t* obj)
{
}

GS_API_DECL void obj_on_update_3141622024(object_t* obj)
{
	component_rotation_t* this = (component_rotation_t*)obj;
	{ 
        entity_manager_t* em = entity_manager_instance(); 

        // Update rotation
        const float _t = gs_platform_elapsed_time();
        gs_quat rot = gs_quat_angle_axis(_t * 0.0001f * this->rotation_speed, this->rotation_axis);

        // Get entity for this component
        uint32_t ent = component_get_entity(this);

        // Get transform component, if found 
        component_transform_t* tc = entities_get_component(em, ent, component_transform_t);

        // Update rotation of transform 
        if (tc)
        { 
            tc->transform.rotation = rot;
        }
    }
}

GS_API_DECL void obj_on_destroy_3141622024(object_t* obj)
{
}

// == texture_t API == //

GS_API_DECL texture_t obj_ctor_1462486611()
{
	texture_t _obj = gs_default_val();
	texture_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(texture_t);
	return _obj;
}
GS_API_DECL object_t* obj_new_1462486611()
{
	texture_t* _obj = gs_malloc_init(texture_t);
	texture_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(texture_t);
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_1462486611(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_1462486611(gs_byte_buffer_t* buffer, const object_t* in)
{
	const texture_t* this = (const texture_t*)in;
	{
        return texture_serialize(buffer, this);
    }
	return GS_RESULT_SUCCESS;
}

GS_API_DECL gs_result obj_deserialize_1462486611(gs_byte_buffer_t* buffer, object_t* out)
{
	texture_t* this = (texture_t*)out;
	{
        return texture_deserialize(buffer, this);
    }
	return GS_RESULT_SUCCESS;
}

GS_API_DECL void obj_on_create_1462486611(object_t* obj)
{
}

GS_API_DECL void obj_on_start_1462486611(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_1462486611(object_t* obj)
{
}

GS_API_DECL void obj_on_update_1462486611(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_1462486611(object_t* obj)
{
}

// == asset_t API == //

GS_API_DECL asset_t obj_ctor_335738428()
{
	asset_t _obj = gs_default_val();
	asset_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(asset_t);
	return _obj;
}
GS_API_DECL object_t* obj_new_335738428()
{
	asset_t* _obj = gs_malloc_init(asset_t);
	asset_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(asset_t);
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_335738428(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_335738428(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_335738428(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_335738428(object_t* obj)
{
}

GS_API_DECL void obj_on_start_335738428(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_335738428(object_t* obj)
{
}

GS_API_DECL void obj_on_update_335738428(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_335738428(object_t* obj)
{
}

// == pipeline_t API == //

GS_API_DECL pipeline_t obj_ctor_3640749902(void)
{
	pipeline_t _obj = gs_default_val();
	pipeline_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(pipeline_t);
	{
            gs_println("PIPELINE CTOR");
        }
	return _obj;
}
GS_API_DECL object_t* obj_new_3640749902(void)
{
	pipeline_t* _obj = gs_malloc_init(pipeline_t);
	pipeline_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(pipeline_t);
	{
            gs_println("PIPELINE CTOR");
        }
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_3640749902(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_3640749902(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_3640749902(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_3640749902(object_t* obj)
{
}

GS_API_DECL void obj_on_start_3640749902(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_3640749902(object_t* obj)
{
}

GS_API_DECL void obj_on_update_3640749902(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_3640749902(object_t* obj)
{
}

// == mesh_t API == //

GS_API_DECL mesh_t obj_ctor_1910793471()
{
	mesh_t _obj = gs_default_val();
	mesh_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(mesh_t);
	return _obj;
}
GS_API_DECL object_t* obj_new_1910793471()
{
	mesh_t* _obj = gs_malloc_init(mesh_t);
	mesh_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(mesh_t);
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_1910793471(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_1910793471(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_1910793471(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_1910793471(object_t* obj)
{
}

GS_API_DECL void obj_on_start_1910793471(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_1910793471(object_t* obj)
{
}

GS_API_DECL void obj_on_update_1910793471(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_1910793471(object_t* obj)
{
}

// == material_t API == //

GS_API_DECL material_t obj_ctor_2657371751(asset_handle_t pip)
{
	material_t _obj = gs_default_val();
	material_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(material_t);
	{
            pipeline_t* pipe = asset_handle_get(&pip);
            gs_assert(pipe);
            this->pipeline = pip;
            this->material = gs_gfxt_material_create(&(gs_gfxt_material_desc_t){
                .pip_func = {
                    .func = get_pipeline_raw,
                    .hndl = pipe
                }
            });
            gs_assert(this->material.desc.pip_func.hndl);
        }
	return _obj;
}
GS_API_DECL object_t* obj_new_2657371751(asset_handle_t pip)
{
	material_t* _obj = gs_malloc_init(material_t);
	material_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(material_t);
	{
            pipeline_t* pipe = asset_handle_get(&pip);
            gs_assert(pipe);
            this->pipeline = pip;
            this->material = gs_gfxt_material_create(&(gs_gfxt_material_desc_t){
                .pip_func = {
                    .func = get_pipeline_raw,
                    .hndl = pipe
                }
            });
            gs_assert(this->material.desc.pip_func.hndl);
        }
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_2657371751(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_2657371751(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_2657371751(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_2657371751(object_t* obj)
{
}

GS_API_DECL void obj_on_start_2657371751(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_2657371751(object_t* obj)
{
}

GS_API_DECL void obj_on_update_2657371751(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_2657371751(object_t* obj)
{
}

// == component_transform_t API == //

GS_API_DECL component_transform_t obj_ctor_3446999376(gs_vqs xform)
{
	component_transform_t _obj = gs_default_val();
	component_transform_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_transform_t);
	{
            this->transform = xform;
        }
	return _obj;
}
GS_API_DECL object_t* obj_new_3446999376(gs_vqs xform)
{
	component_transform_t* _obj = gs_malloc_init(component_transform_t);
	component_transform_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(component_transform_t);
	{
            this->transform = xform;
        }
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_3446999376(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_3446999376(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_3446999376(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_3446999376(object_t* obj)
{
}

GS_API_DECL void obj_on_start_3446999376(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_3446999376(object_t* obj)
{
}

GS_API_DECL void obj_on_update_3446999376(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_3446999376(object_t* obj)
{
}

// == font_t API == //

GS_API_DECL font_t obj_ctor_477039743()
{
	font_t _obj = gs_default_val();
	font_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(font_t);
	return _obj;
}
GS_API_DECL object_t* obj_new_477039743()
{
	font_t* _obj = gs_malloc_init(font_t);
	font_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(font_t);
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_477039743(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_477039743(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_477039743(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_477039743(object_t* obj)
{
}

GS_API_DECL void obj_on_start_477039743(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_477039743(object_t* obj)
{
}

GS_API_DECL void obj_on_update_477039743(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_477039743(object_t* obj)
{
}

// == component_base_t API == //

GS_API_DECL component_base_t obj_ctor_3118336809()
{
	component_base_t _obj = gs_default_val();
	component_base_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_base_t);
	return _obj;
}
GS_API_DECL object_t* obj_new_3118336809()
{
	component_base_t* _obj = gs_malloc_init(component_base_t);
	component_base_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(component_base_t);
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_3118336809(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_3118336809(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_3118336809(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_3118336809(object_t* obj)
{
}

GS_API_DECL void obj_on_start_3118336809(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_3118336809(object_t* obj)
{
}

GS_API_DECL void obj_on_update_3118336809(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_3118336809(object_t* obj)
{
}

// == audio_source_t API == //

GS_API_DECL audio_source_t obj_ctor_1947392792()
{
	audio_source_t _obj = gs_default_val();
	audio_source_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(audio_source_t);
	return _obj;
}
GS_API_DECL object_t* obj_new_1947392792()
{
	audio_source_t* _obj = gs_malloc_init(audio_source_t);
	audio_source_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(audio_source_t);
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_1947392792(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_1947392792(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_1947392792(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_1947392792(object_t* obj)
{
}

GS_API_DECL void obj_on_start_1947392792(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_1947392792(object_t* obj)
{
}

GS_API_DECL void obj_on_update_1947392792(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_1947392792(object_t* obj)
{
}

// == component_static_mesh_t API == //

GS_API_DECL component_static_mesh_t obj_ctor_2159547720(graphics_scene_t* scene)
{
	component_static_mesh_t _obj = gs_default_val();
	component_static_mesh_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_static_mesh_t);
	{
            renderable_static_mesh_t rend = {0};
            this->renderable_id = graphics_scene_add_renderable_static_mesh(scene, rend);
            this->scene = scene;
        }
	return _obj;
}
GS_API_DECL object_t* obj_new_2159547720(graphics_scene_t* scene)
{
	component_static_mesh_t* _obj = gs_malloc_init(component_static_mesh_t);
	component_static_mesh_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(component_static_mesh_t);
	{
            renderable_static_mesh_t rend = {0};
            this->renderable_id = graphics_scene_add_renderable_static_mesh(scene, rend);
            this->scene = scene;
        }
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_2159547720(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_2159547720(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_2159547720(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_2159547720(object_t* obj)
{
}

GS_API_DECL void obj_on_start_2159547720(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_2159547720(object_t* obj)
{
}

GS_API_DECL void obj_on_update_2159547720(object_t* obj)
{
	component_static_mesh_t* this = (component_static_mesh_t*)obj;
	{ 
        component_static_mesh_update(this);
    }
}

GS_API_DECL void obj_on_destroy_2159547720(object_t* obj)
{
}

// == component_physics_t API == //

GS_API_DECL component_physics_t obj_ctor_3852747479()
{
	component_physics_t _obj = gs_default_val();
	component_physics_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_physics_t);
	return _obj;
}
GS_API_DECL object_t* obj_new_3852747479()
{
	component_physics_t* _obj = gs_malloc_init(component_physics_t);
	component_physics_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(component_physics_t);
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_3852747479(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_3852747479(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_3852747479(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_3852747479(object_t* obj)
{
}

GS_API_DECL void obj_on_start_3852747479(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_3852747479(object_t* obj)
{
}

GS_API_DECL void obj_on_update_3852747479(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_3852747479(object_t* obj)
{
}

// == entity_t API == //

GS_API_DECL entity_t obj_ctor_1210153879()
{
	entity_t _obj = gs_default_val();
	entity_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(entity_t);
	return _obj;
}
GS_API_DECL object_t* obj_new_1210153879()
{
	entity_t* _obj = gs_malloc_init(entity_t);
	entity_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(entity_t);
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_1210153879(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_1210153879(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_1210153879(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_1210153879(object_t* obj)
{
}

GS_API_DECL void obj_on_start_1210153879(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_1210153879(object_t* obj)
{
}

GS_API_DECL void obj_on_update_1210153879(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_1210153879(object_t* obj)
{
}

// == component_renderable_t API == //

GS_API_DECL component_renderable_t obj_ctor_218119740()
{
	component_renderable_t _obj = gs_default_val();
	component_renderable_t* this = &_obj;
	cast(this, object_t)->cls_id = obj_sid(component_renderable_t);
	return _obj;
}
GS_API_DECL object_t* obj_new_218119740()
{
	component_renderable_t* _obj = gs_malloc_init(component_renderable_t);
	component_renderable_t* this = _obj;
	cast(this, object_t)->cls_id = obj_sid(component_renderable_t);
	return (object_t*)_obj;
}

GS_API_DECL void obj_dtor_218119740(object_t* obj)
{
}

GS_API_DECL gs_result obj_serialize_218119740(gs_byte_buffer_t* buffer, const object_t* in)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL gs_result obj_deserialize_218119740(gs_byte_buffer_t* buffer, object_t* out)
{
	return GS_RESULT_INCOMPLETE;
}

GS_API_DECL void obj_on_create_218119740(object_t* obj)
{
}

GS_API_DECL void obj_on_start_218119740(object_t* obj)
{
}

GS_API_DECL void obj_on_stop_218119740(object_t* obj)
{
}

GS_API_DECL void obj_on_update_218119740(object_t* obj)
{
}

GS_API_DECL void obj_on_destroy_218119740(object_t* obj)
{
}

