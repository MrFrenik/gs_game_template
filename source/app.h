#ifndef APP_H
#define APP_H

/* 
 *  Main application interface
 */

// App struct declaration 
typedef struct app_t
{
    BASE(object_t);

    // Fields
    gs_command_buffer_t cb; 
    gs_immediate_draw_t gsi; 
	entity_manager_t entities;
	asset_manager_t assets;
	meta_t meta;
    gs_camera_t camera;
    entity_handle_t player; 
	graphics_scene_t graphics;
} app_t; 

// Main application interface functions
GS_API_DECL gs_app_desc_t app_main(int32_t argc, char** argv);
GS_API_DECL void app_init();
GS_API_DECL void app_update();
GS_API_DECL void app_shutdown();
GS_API_DECL void app_init_meta();

#ifdef APP_IMPL 

GS_API_DECL void app_init()
{
    app_t* app = gs_engine_user_data(app_t); 

    // Initilialize all application structures 
    app->cb = gs_command_buffer_new();
    app->gsi = gs_immediate_draw_new();

	// Initialize meta
	// This will all be done internally by the reflection system.
	app_init_meta();

	// Initialize asset manager with root path for assets to be imported
    const char* assets_path = gs_platform_file_exists("./assets/test.txt") ? "./assets" : "../assets";
	assets_init(&app->assets, assets_path);

    // Add some assets to the database (importer is based on file extension)
    assets_import(&app->assets, "textures/contra_player_sprite.png", NULL);  

	// Get asset after import
	texture_t* tex = assets_getp(&app->assets, texture_t, "textures.contra_player_sprite");
	obj_dump(&app->meta, tex, gs_meta_class_get(&app->meta.registry, texture_t));

	// Register all components	
	entities_register_component(&app->entities, component_transform_t);
	entities_register_component(&app->entities, component_physics_t);
	entities_register_component(&app->entities, component_renderable_t);

	gs_assert(gs_hash_table_exists(app->entities.components, obj_id(component_transform_t)));

	// Allocate new player entity
	app->player = entities_allocate(&app->entities);
	entities_add_component(&app->entities, component_transform_t, app->player, {.transform = gs_vqs_default()});
	entities_add_component(&app->entities, component_physics_t, app->player, {0});

	// Init camera
    app->camera = gs_camera_perspective(); 
	app->camera.transform.position = gs_v3(8.60, 5.66f, 8.34f);
	app->camera.transform.rotation = gs_quat_mul_list(3,
		gs_quat_angle_axis(gs_deg2rad(-12.6f), GS_XAXIS),
		gs_quat_angle_axis(gs_deg2rad(46.6f), GS_YAXIS),
		gs_quat_angle_axis(gs_deg2rad(8.7f), GS_ZAXIS)
	);
}

GS_API_DECL void app_update()
{
    app_t* app = gs_engine_user_data(app_t);
    gs_command_buffer_t* cb = &app->cb;
    gs_immediate_draw_t* gsi = &app->gsi;

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 

	// Move player	
	gs_vec3 vel = gs_v3s(0.f);
	component_transform_t* tc = entities_get_component(&app->entities, component_transform_t, app->player);
	component_physics_t* pc = entities_get_component(&app->entities, component_physics_t, app->player);
	gs_vec3* p = &tc->transform.position;
	gs_vec3* v = &pc->velocity;

	if (gs_platform_key_down(GS_KEYCODE_W)) vel.z -= 1.f;
	if (gs_platform_key_down(GS_KEYCODE_S)) vel.z += 1.f;
	if (gs_platform_key_down(GS_KEYCODE_A)) vel.x -= 1.f;
	if (gs_platform_key_down(GS_KEYCODE_D)) vel.x += 1.f;
	vel = gs_vec3_scale(gs_vec3_norm(vel), 0.1f);

	// Calculate final velocity
	*v = gs_v3(
		gs_interp_linear(v->x, vel.x, 0.2f),
		gs_interp_linear(v->y, vel.y, 0.2f),
		gs_interp_linear(v->z, vel.z, 0.2f)
	);

	*p = gs_vec3_add(*p, *v);

	// TODO(john): Need a capsule debug drawing method added into gsi
    // Render scene (debug for now)
    gsi_camera(gsi, &app->camera); 

	gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
	{
		gsi_mul_matrix(gsi, gs_vqs_to_mat4(&tc->transform));
		gsi_box(gsi, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 0, 255, 0, 255, GS_GRAPHICS_PRIMITIVE_LINES);
	}
	gsi_pop_matrix(gsi); 

	const texture_t* tex = assets_getp(&app->assets, texture_t, "textures.contra_player_sprite");

	if (tex)
	{
		gsi_camera2D(gsi);
		gsi_blend_enabled(gsi, true);
		gsi_texture(gsi, tex->texture.hndl);
		gsi_rectvd(gsi, gs_v2(100.f, 100.f), gs_v2s(500.f), gs_v2s(0.f), gs_v2s(1.f), GS_COLOR_WHITE, GS_GRAPHICS_PRIMITIVE_TRIANGLES);
	}

    // Get necessary data for drawing
    gsi_render_pass_submit(gsi, cb, gs_color(10, 10, 10, 255)); 
    gs_graphics_submit_command_buffer(cb);
}

GS_API_DECL void app_shutdown()
{
    app_t* app = gs_engine_user_data(app_t); 
}

GS_API_DECL void app_init_meta()
{
	app_t* app = gs_engine_user_data(app_t);

	app->meta = meta_new();
	meta_set_instance(&app->meta);

	// This will be generated...somehow
	// Register all necessary meta information (which includes reflection information)

	// gs_graphics_texture_desc_t
    gs_meta_class_register(&app->meta.registry, (&(gs_meta_class_decl_t){
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
    gs_meta_class_register(&app->meta.registry, (&(gs_meta_class_decl_t){
        .properties = (gs_meta_property_t[]) {
            gs_meta_property(gs_asset_texture_t, gs_graphics_texture_desc_t, desc, GS_META_PROPERTY_TYPE_INFO_OBJ),
        },
        .size = 1 * sizeof(gs_meta_property_t),
		.name = gs_to_str(gs_asset_texture_t)
    }));

	meta_register_vtable(&app->meta, texture_t, (&(vtable_t){0}));

	// Register from generated (doesn't generate information for internal stuff)
	meta_register_generated(&app->meta);
}
		
GS_API_DECL gs_app_desc_t app_main(int32_t argc, char** argv)
{
    return (gs_app_desc_t) 
	{
		.window_width = 800,
		.window_height = 600,
		.init = app_init,
		.update = app_update,
		.shutdown = app_shutdown,
		.user_data = gs_malloc_init(app_t)
	};
}

#endif  // APP_IMPL
#endif  // APP_H
