#ifndef APP_H
#define APP_H

/* 
 *  Main application interface
 */ 

/*
typedef struct renderable_t : public object_t
{
	OBJ_DECL(renderable_t);

	// Methods
	void set_material_uniform(const char* name, void* data);

	// Fields
	gs_gfxt_renderable_t data;
	asset_hndl_t mesh_hndl;
	asset_hndl_t material_hndl;

} renderable_t;

// Need a way to add renderables to a scene
typedef struct graphics_scene_t 
{
	OBJ_DECL(graphics_scene_t);

	// Methods
	// Need a way to sort renderables by material/pipeline
	uint32_t add_renderable(const renderable_t& renderable) 
	{
		return renderables.insert(renderable);
	}

	// Fields
	slot_array_t<renderable_t> renderables;

} graphics_scene_t;
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
    gs_camera_t camera;
    entity_handle_t player; 
	// graphics_scene_t graphics;
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

    // Initilialize all application structures 
    app->cb = gs_command_buffer_new();
    app->gsi = gs_immediate_draw_new();

	// Initialize asset manager
    const char* assets_path = gs_platform_file_exists("./assets/test.txt") ? "./assets" : "../assets";
	assets_init(&app->assets, assets_path);

    // Add some assets to the database
	// Get importer based on file extension
    assets_import(&app->assets, "textures/contra_player_sprite.png", NULL);  

	// Register all components	
	entities_register_component(&app->entities, component_transform_t);
	entities_register_component(&app->entities, component_physics_t);

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
