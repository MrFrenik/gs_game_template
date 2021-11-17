
#ifndef APP_H
#define APP_H

/* 
 *  Main application interface
 */ 

// App struct declaration 
typedef struct app_t
{
    // Fields
	core_t* core; 
} app_t; 

// Main application interface functions
GS_API_DECL gs_app_desc_t app_main(int32_t argc, char** argv);
GS_API_DECL void app_init();
GS_API_DECL void app_update();
GS_API_DECL void app_shutdown(); 

#ifdef APP_IMPL 

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

// Shaders
#if (defined GS_PLATFORM_WEB || defined GS_PLATFORM_ANDROID)
    #define GS_GL_VERSION_STR "#version 300 es\n"
#else
    #define GS_GL_VERSION_STR "#version 330 core\n"
#endif 

svo_t g_svo = gs_default_val();

GS_API_DECL void app_init()
{
	// Initialize core
    app_t* app = gs_engine_user_data(app_t); 
	app->core = core_new(); 

    gs_byte_buffer_t buffer = gs_default_val();
    g_svo = obj_ctor(svo_t, 32, 32, 32);

    svo_sphere(
        &g_svo, 
        &(gs_sphere_t){
            .c = gs_v3((float)g_svo.width / 2.f, (float)g_svo.height / 2.f, (float)g_svo.depth / 2.f),
            .r = 10.0f
        },
        NULL,
        VOXEL_OP_SUBTRACTION 
    );
} 

GS_API_DECL void app_update()
{
    app_t* app = gs_engine_user_data(app_t);
	core_t* core = app->core;
    gs_command_buffer_t* cb = &core->cb;
    gs_immediate_draw_t* gsi = &core->gsi; 
    gs_mu_ctx* gmu = &core->gmu;
    const float t = gs_platform_elapsed_time();
    const float frame = gs_engine_subsystem(platform)->time.frame;

    const gs_vec2 fb = gs_platform_framebuffer_sizev(gs_platform_main_window());
    const gs_vec2 ws = gs_platform_window_sizev(gs_platform_main_window()); 

    static gs_vec3 cp = {0.f, 0.f, -250.f};
    if (gs_platform_key_down(GS_KEYCODE_UP))    cp.y += 0.1f;
    if (gs_platform_key_down(GS_KEYCODE_DOWN))  cp.y -= 0.1f;
    if (gs_platform_key_down(GS_KEYCODE_LEFT))  cp.x -= 0.1f;
    if (gs_platform_key_down(GS_KEYCODE_RIGHT)) cp.x += 0.1f;
    if (gs_platform_key_down(GS_KEYCODE_Q))     cp.z -= 0.1f;
    if (gs_platform_key_down(GS_KEYCODE_E))     cp.z += 0.1f;

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 

    static gs_vec3 sp = gs_default_val();
    sp.x = sin(t * 0.0001f) * g_svo.width / 2.f + g_svo.width / 2.f; 
    sp.y = (float)g_svo.height / 2.f;
    sp.z = (float)g_svo.depth / 2.f;

    /*
    svo_sphere(
        &g_svo, 
        &(gs_sphere_t){
            .r = 10.f,
            .c = sp
        },
        NULL,
        VOXEL_OP_SUBTRACTION
    );
    */

    gsi_camera3D(gsi); 
    gsi_depth_enabled(gsi, true);
    gsi_blend_enabled(gsi, true);
    gsi_face_cull_enabled(gsi, true);
    gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
    gsi_transf(gsi, cp.x, cp.y, cp.z); 
    gs_quat rot = gs_quat_mul_list(3, 
        gs_quat_angle_axis(t * 0.0001f, GS_YAXIS), 
        gs_quat_angle_axis(t * 0.0002f, GS_XAXIS), 
        gs_quat_angle_axis(t * 0.0003f, GS_ZAXIS) 
    );
    gsi_mul_matrix(gsi, gs_quat_to_mat4(rot));

    const svo_t* svo = &g_svo;
    const gs_vec3 svo_wc  = svo->xform.position;    // SVO center position in world space
    const uint32_t xmin = 0, xmax = svo->width;
    const uint32_t ymin = 0, ymax = svo->height;
    const uint32_t zmin = 0, zmax = svo->depth;

    // Iterate over cells, determine intersections with voxels and all that shiz
    for (uint32_t x = xmin; x < xmax; ++x)
        for (uint32_t y = ymin; y < ymax; ++y)
            for (uint32_t z = zmin; z < zmax; ++z)
    { 
        const gs_vec3 vp = gs_vec3_add(svo_wc, gs_v3((float)x, (float)y, (float)z)); 

        const uint32_t vidx = voxel_idx(x, y, z, svo->width, svo->height, svo->depth); 
        if (svo->voxels[vidx])
        {
            gsi_box(gsi, vp.x, vp.y, vp.z, 0.5f, 0.5f, 0.5f, 255, 255, 255, 255, GS_GRAPHICS_PRIMITIVE_TRIANGLES);
        }
    } 

    gsi_pop_matrix(gsi);

    gsi_camera2D(gsi);
    gsi_defaults(gsi);
    gs_snprintfc(TMP_BUF, 256, "frame: %.2f", frame);
    gsi_text(gsi, 10.f, 30.f, TMP_BUF, NULL, false, 255, 255, 255, 255);

    gsi_render_pass_submit(gsi, cb, (gs_color_t){10, 10, 10, 255}); 
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
		.window_width = WINDOW_WIDTH,
		.window_height = WINDOW_HEIGHT,
		.init = app_init,
		.update = app_update,
		.shutdown = app_shutdown,
		.user_data = gs_malloc_init(app_t)
	};
} 

#endif  // APP_IMPL
#endif  // APP_H
