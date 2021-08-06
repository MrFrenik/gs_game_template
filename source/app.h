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
	core_t* core;

	void* user_data;

	// User defined app data
	gs_camera_t camera;	
	entity_handle_t player;
} app_t; 

// Main application interface functions
GS_API_DECL gs_app_desc_t app_main(int32_t argc, char** argv);
GS_API_DECL void app_init();
GS_API_DECL void app_update();
GS_API_DECL void app_shutdown();

#ifdef APP_IMPL 

GS_API_DECL void app_init()
{
    app_t* app = gs_engine_user_data(app_t); 

	// Initialize core
	app->core = core_new();

	// Import an asset
    assets_import(&core->assets, "textures/logo.png", NULL);  

	// Allocate new player entity
	app->player = entities_allocate(&app->entities);
	entities_add_component(&app->entities, component_transform_t, app->player, {.transform = gs_vqs_default()});

	// Init camera
    app->camera = gs_camera_perspective(); 
	app->camera.transform.position = gs_v3(0.f, 0.f, 2.f);
}

GS_API_DECL void app_update()
{
    app_t* app = gs_engine_user_data(app_t);
	core_t* core = &app->core;
    gs_command_buffer_t* cb = &core->cb;
    gs_immediate_draw_t* gsi = &core->gsi;

	// Get elapsed time
	const t = gs_platform_elapsed_time();

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 

	//=== [ Entity update ] ===	
	
	gs_vec3 vel = gs_v3s(0.f);
	component_transform_t* tc = entities_get_component(&core->entities, component_transform_t, app->player);
	component_physics_t* pc = entities_get_component(&core->entities, component_physics_t, app->player);
	tc->transform.rotation = gs_quat_mul_list(3, 
		gs_quat_angle_axis(gs_deg2rad(+t * 0.01f), GS_XAXIS), 
		gs_quat_angle_axis(gs_deg2rad(+t * 0.02f), GS_YAXIS), 
		gs_quat_angle_axis(gs_deg2rad(-t * 0.03f), GS_ZAXIS) 
	);

    //=== [ Render scene ] (debug for now, will add materials soon)
	
	// Grab texture from assets
	const texture_t* tex = assets_getp(&core->assets, texture_t, "textures.logo");

	// Set up GSI then render player as box
	gsi_blend_enabled(gsi, true);
	gsi_depth_enabled(gsi, true);
	gsi_face_cull_enabled(gsi, true);
	gsi_texture(gsi, tex->texture.hndl);
    gsi_camera(gsi, &app->camera); 
	gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
	{
		gsi_mul_matrix(gsi, gs_vqs_to_mat4(&tc->transform));
		gsi_box(gsi, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 255, 255, 255, 255, GS_GRAPHICS_PRIMITIVE_TRIANGLES);
	}
	gsi_pop_matrix(gsi); 

    // Submit
    gsi_render_pass_submit(gsi, cb, gs_color(10, 10, 10, 255)); 
    gs_graphics_submit_command_buffer(cb);
}

GS_API_DECL void app_shutdown()
{
    app_t* app = gs_engine_user_data(app_t); 
	core_delete(app->core);
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
