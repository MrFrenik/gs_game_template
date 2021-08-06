#ifndef GENERATED_H
#define GENERATED_H

/*
 * This file has been generated. All modifications will be lost.
*/

GS_API_DECL void meta_register_generated(gs_meta_registry_t* meta);

//============[ Implementation ]==============//

#ifdef GENERATED_IMPL

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

	// asset_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(asset_t, uint32_t, record_hndl, GS_META_PROPERTY_TYPE_INFO_U32),
			gs_meta_property(asset_t, asset_state, state, GS_META_PROPERTY_TYPE_INFO_ENUM)
		},
		.size = 2 * sizeof(gs_meta_property_t),
		.name = gs_to_str(asset_t),
		.base = gs_to_str(object_t)
	}));

	// texture_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(texture_t, gs_asset_texture_t, texture, GS_META_PROPERTY_TYPE_INFO_OBJ),
			gs_meta_property(texture_t, char, blah, GS_META_PROPERTY_TYPE_INFO_STR)
		},
		.size = 2 * sizeof(gs_meta_property_t),
		.name = gs_to_str(texture_t),
		.base = gs_to_str(asset_t)
	}));

	// component_transform_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(component_transform_t, gs_vqs, transform, GS_META_PROPERTY_TYPE_INFO_VQS)
		},
		.size = 1 * sizeof(gs_meta_property_t),
		.name = gs_to_str(component_transform_t),
		.base = gs_to_str(component_base_t)
	}));

	// component_physics_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(component_physics_t, gs_vec3, velocity, GS_META_PROPERTY_TYPE_INFO_VEC3),
			gs_meta_property(component_physics_t, gs_vec3, acceleration, GS_META_PROPERTY_TYPE_INFO_VEC3)
		},
		.size = 2 * sizeof(gs_meta_property_t),
		.name = gs_to_str(component_physics_t),
		.base = gs_to_str(component_base_t)
	}));

	// component_base_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.name = gs_to_str(component_base_t),
		.base = gs_to_str(object_t)
	}));

	// renderable_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(renderable_t, gs_gfxt_renderable_t, data, GS_META_PROPERTY_TYPE_INFO_OBJ),
			gs_meta_property(renderable_t, uint32_t, mesh_hndl, GS_META_PROPERTY_TYPE_INFO_U32),
			gs_meta_property(renderable_t, uint32_t, material_hndl, GS_META_PROPERTY_TYPE_INFO_U32)
		},
		.size = 3 * sizeof(gs_meta_property_t),
		.name = gs_to_str(renderable_t),
		.base = gs_to_str(object_t)
	}));

	// component_renderable_t
	gs_meta_class_register(meta, (&(gs_meta_class_decl_t) {
		.properties = (gs_meta_property_t[]) {
			gs_meta_property(component_renderable_t, uint32_t, renderable_id, GS_META_PROPERTY_TYPE_INFO_U32)
		},
		.size = 1 * sizeof(gs_meta_property_t),
		.name = gs_to_str(component_renderable_t),
		.base = gs_to_str(component_base_t)
	}));

}

#endif // GENERATED_IMPL
#endif // GENERATED_H
