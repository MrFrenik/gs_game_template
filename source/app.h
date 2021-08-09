#ifndef APP_H
#define APP_H

/* 
 *  Main application interface
 */

// App struct declaration 
typedef struct app_t
{
    base(object_t)

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

typedef enum piano_key
{
    KEY_A,
    KEY_B, 
    KEY_C, 
    KEY_D,
    KEY_E, 
    KEY_F, 
    KEY_G,
    KEY_COUNT
} piano_key;

void audio_commit(int16_t* output, uint32_t num_channels, uint32_t sample_rate, uint32_t frame_count)
{ 
    // Try to do a simple sine wave output
    static float t = 0.f;   
    const float amplitude = 0.1f;
    const float phase = 0.f;
    const float dt = 1.f / (float)sample_rate; 
    static float pre_gain = 100.f;
    static float post_gain = 0.2f;
    bool play = false;
    bool stch = false;
    float freq = 440.f;
    if (t >= FLT_MAX) t = 0.f; 

    float freqs[KEY_COUNT] = {
       220.00f, // A
       246.94f, // B
       261.63f, // C
       293.66f, // D 
       329.63f, // E
       349.23f, // F
       391.99f  // G
    };

    static float thresh_higher = 0.7f;
    static float thresh_lower = 0.2f;
    static float thresh_noise = 0.1f;
    static float gain_lower = 2.f;
    static float gain_higher = 0.5f; 

    if (gs_platform_key_down(GS_KEYCODE_UP))    pre_gain += 1.f;
    if (gs_platform_key_down(GS_KEYCODE_DOWN))  pre_gain -= 1.f;
    if (gs_platform_key_down(GS_KEYCODE_LEFT))  post_gain -= 0.01f;
    if (gs_platform_key_down(GS_KEYCODE_RIGHT)) post_gain += 0.01f;
    if (gs_platform_key_down(GS_KEYCODE_Q))     thresh_higher -= 0.01f;
    if (gs_platform_key_down(GS_KEYCODE_E))     thresh_higher += 0.01f;
    if (gs_platform_key_down(GS_KEYCODE_N))     stch = true; 

    if (gs_platform_key_down(GS_KEYCODE_A)) {freq = freqs[KEY_A]; play = true;}
    if (gs_platform_key_down(GS_KEYCODE_B)) {freq = freqs[KEY_B]; play = true;}
    if (gs_platform_key_down(GS_KEYCODE_C)) {freq = freqs[KEY_C]; play = true;}
    if (gs_platform_key_down(GS_KEYCODE_D)) {freq = freqs[KEY_D]; play = true;}
    if (gs_platform_key_down(GS_KEYCODE_E)) {freq = freqs[KEY_E]; play = true;}
    if (gs_platform_key_down(GS_KEYCODE_F)) {freq = freqs[KEY_F]; play = true;}
    if (gs_platform_key_down(GS_KEYCODE_G)) {freq = freqs[KEY_G]; play = true;}

    //pre_gain = gs_clamp(pre_gain, 1.f, 100.f);
    //post_gain = gs_clamp(post_gain, 0.01f, 0.8f);
    gain_higher = gs_clamp(gain_higher, 0.5, 1.f); 
    gain_lower = gs_clamp(gain_lower, 0.01f, 0.5f); 

    // Generate wave, add samples
    if (play)
    {
        for (uint32_t i = 0; i < frame_count; ++i)
        {
            float v = sinf(2.f * GS_PI * freq * t + phase); 

            if (stch) 
            {
                v = post_gain * tanhf(pre_gain * sin(v)); 
            }
            else
            {
                // Has more overtones
                if (fabsf(v) < thresh_lower && fabsf(v) > thresh_noise) v = post_gain * (v * gain_lower); 
                if (fabsf(v) > thresh_higher) v = post_gain * (v * gain_higher); 
                v = post_gain * v;
            } 

            // Need to just pass back a float buffer
            int16_t s = gs_map_range(-1.f, 1.f, SHRT_MIN, SHRT_MAX, v); 
            *output++ += s;
            *output++ += s;
            t += dt;
        } 
    }
} 

#ifdef APP_IMPL 

GS_API_DECL void app_init()
{
    app_t* app = gs_engine_user_data(app_t); 

	// Initialize core
	app->core = core_new();
	core_t* core = app->core;

	// Import an asset
    assets_import(&core->assets, "textures/logo.png", NULL);
    assets_import(&core->assets, "audio/guitar_c3.wav", NULL);
    assets_import(&core->assets, "audio/music.ogg", NULL);

	// Allocate new player entity
	app->player = entities_allocate(&core->entities);
	entities_add_component(&core->entities, component_transform_t, app->player, {.transform = gs_vqs_default()});

	// Init camera
    app->camera = gs_camera_perspective(); 
	app->camera.transform.position = gs_v3(0.f, 0.f, 2.f);

    texture_t* t1 = obj_new(texture_t);
    texture_t* t2 = obj_newid(obj_sid(texture_t)); 

    gs_assert(obj_sid(texture_t) == obj_id(t1));

    // Call functions for objects
    obj_ctorc(texture_t, t3, NULL);
    obj_dtor(t1);
    obj_serialize(NULL, &t3);
    obj_deserialize(NULL, t1);

    gs_audio_register_commit(audio_commit);
}

GS_API_DECL void app_update()
{
    app_t* app = gs_engine_user_data(app_t);
	core_t* core = app->core;
    gs_command_buffer_t* cb = &core->cb;
    gs_immediate_draw_t* gsi = &core->gsi;

	// Get elapsed time
	const t = gs_platform_elapsed_time();

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 


    if (gs_platform_key_pressed(GS_KEYCODE_P))
    {
        const audio_source_t* as = assets_getp(&core->assets, audio_source_t, "audio.guitar_c3");
        if (as)
        {
            gs_audio_play_source(as->audio.hndl, 1.f); 
        }
    }

	//=== [ Entity update ] ===	
	
	gs_vec3 vel = gs_v3s(0.f);
	component_transform_t* tc = entities_get_component(&core->entities, component_transform_t, app->player);
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
