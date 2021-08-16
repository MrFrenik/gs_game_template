#ifndef APP_H
#define APP_H

/* 
 *  Main application interface
 */ 

typedef struct physics_component_t
{
    gs_vec3 velocity;
    gs_vec3 angular_velocity;
    float speed;
} physics_component_t;

// App struct declaration 
typedef struct app_t
{
    // Fields
	core_t* core; 

    gs_framebuffer      fbo;
    gs_renderpass       rp;
    gs_texture          rt;
    gs_gfxt_pipeline_t  bb_pip;     // Pipeline object for back buffer
    gs_gfxt_material_t  bb_mat;     // Material for back buffer
    gs_gfxt_mesh_t      fs_quad;    // Full screen unit quad
    gs_camera_t         camera;
    gs_texture          tex;
    gs_vbo              vbo;
    gs_ibo              ibo;
    uint32_t            mesh_count;
	gs_gfxt_mesh_raw_data_t* meshes;
    gs_asset_texture_t  ship_tex;

    physics_component_t physics;
    gs_vqs xform;
} app_t; 

// Main application interface functions
GS_API_DECL gs_app_desc_t app_main(int32_t argc, char** argv);
GS_API_DECL void app_init();
GS_API_DECL void app_update();
GS_API_DECL void app_shutdown(); 
GS_API_DECL void app_draw_mesh(const gs_vqs* vqs);
GS_API_DECL void app_update_transform(physics_component_t* physics, gs_vqs* xform);

#ifdef APP_IMPL 

#define RT_WIDTH  800 / 4
#define RT_HEIGHT 600 / 4

// Shaders
#if (defined GS_PLATFORM_WEB || defined GS_PLATFORM_ANDROID)
    #define GS_GL_VERSION_STR "#version 300 es\n"
#else
    #define GS_GL_VERSION_STR "#version 330 core\n"
#endif

const char* bb_v_src =
GS_GL_VERSION_STR
"precision mediump float;\n"
"layout(location = 0) in vec2 a_pos;\n"
"layout(location = 1) in vec2 a_uv;\n"
"uniform vec2 u_tex_size;\n"
"out vec2 uv;\n"
"out vec2 tex_size;\n" 
"void main() {\n"
"  gl_Position = vec4(a_pos, 0.0, 1.0);\n"
"  uv = a_uv * u_tex_size;\n"
"  tex_size = u_tex_size;\n"
"}\n";

const char* bb_f_src =
GS_GL_VERSION_STR
"precision mediump float;\n"
"in vec2 uv;\n"
"in vec2 tex_size;\n"
"uniform sampler2D u_tex;\n"
"uniform vec2 u_screen_size;\n"
"out vec4 frag_color;\n"
"void main() {\n"
"  float tex_per_pix = (tex_size.x) / u_screen_size.x;\n" 
"  vec2 loc_within_texel = fract(uv);\n" 
"  vec2 lerp_amount = clamp(loc_within_texel / tex_per_pix, 0.0, 0.5);\n" 
"  lerp_amount = lerp_amount + clamp((loc_within_texel - 1) / tex_per_pix + 0.5, 0.0, 0.5);\n" 
"  vec2 final_uv = (floor(uv) + lerp_amount) / tex_size;\n" 
"  frag_color = texture(u_tex, final_uv);\n"  // Will have to upsample this
"}\n";

// Need a few things: 
    // offscreen pipeline for rendering simple objects (could honestly just use GSI for rendering a cube...)
    // onscreen pipeline for rendering default quad texture to screen 

GS_API_DECL void app_init()
{
    app_t* app = gs_engine_user_data(app_t); 

	// Initialize core
	app->core = core_new(); 

	// I want position, uv, color
	gs_gfxt_mesh_import_options_t options = {
		.layout = (gs_gfxt_mesh_layout_t[]) {
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_POSITION},
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_TEXCOORD},
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_COLOR},
		},
		.layout_size = 3 * sizeof(gs_gfxt_mesh_layout_t),
		.index_buffer_element_size = sizeof(uint32_t)
	};
	gs_gfxt_load_gltf_data_from_file("./assets/slave.gltf", &options, &app->meshes, &app->mesh_count); 

    // Init framebuffer, render target
    app->fbo = gs_graphics_framebuffer_create(NULL); 

    // Construct color render target
    app->rt = gs_graphics_texture_create(
        &(gs_graphics_texture_desc_t){
            .width = RT_WIDTH,  
            .height = RT_HEIGHT,
            .format = GS_GRAPHICS_TEXTURE_FORMAT_RGBA8,                        
            .wrap_s = GS_GRAPHICS_TEXTURE_WRAP_CLAMP_TO_EDGE,                        
            .wrap_t = GS_GRAPHICS_TEXTURE_WRAP_CLAMP_TO_EDGE,                       
            .min_filter = GS_GRAPHICS_TEXTURE_FILTER_LINEAR,                
            .mag_filter = GS_GRAPHICS_TEXTURE_FILTER_LINEAR,               
            .render_target = true
        }
    );

    // Construct render pass for offscreen render pass
    app->rp = gs_graphics_render_pass_create(
        &(gs_graphics_render_pass_desc_t){
            .fbo = app->fbo,                      
            .color = &app->rt,                        
            .color_size = sizeof(app->rt)        
        } 
    );

    app->fs_quad = gs_gfxt_mesh_unit_quad_generate(NULL);
    app->tex = gs_gfxt_texture_generate_default();

    // Create pipeline for back buffer
    app->bb_pip = gs_gfxt_pipeline_create(&(gs_gfxt_pipeline_desc_t){
        .pip_desc = (gs_graphics_pipeline_desc_t){
            .raster = {
                .shader = gs_graphics_shader_create(&(gs_graphics_shader_desc_t){ 
                    .sources = (gs_graphics_shader_source_desc_t[]){
                        {.type = GS_GRAPHICS_SHADER_STAGE_VERTEX, .source = bb_v_src},
                        {.type = GS_GRAPHICS_SHADER_STAGE_FRAGMENT, .source = bb_f_src} 
                    },
                    .size = 2 * sizeof(gs_graphics_shader_source_desc_t),
                    .name = "bb_shader"
                }),
                .index_buffer_element_size = sizeof(uint32_t)
            },
            .layout = {
                .attrs = (gs_graphics_vertex_attribute_desc_t[]){
                    {.format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2, .name = "a_pos"},
                    {.format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2, .name = "a_uv"}
                },
                .size = 2 * sizeof(gs_graphics_vertex_attribute_desc_t)
            },
        },
        .ublock_desc = (gs_gfxt_uniform_block_desc_t){
            .layout = (gs_gfxt_uniform_desc_t[]){ 
                {.name = "u_tex", .type = GS_GRAPHICS_UNIFORM_SAMPLER2D},
                {.name = "u_tex_size", .type = GS_GRAPHICS_UNIFORM_VEC2},
                {.name = "u_screen_size", .type = GS_GRAPHICS_UNIFORM_VEC2}
            },
            .layout_size = 3 * sizeof(gs_gfxt_uniform_desc_t)
        }
    }); 

    app->bb_mat = gs_gfxt_material_create(&(gs_gfxt_material_desc_t){
        .pip_func = (gs_gfxt_raw_data_func_desc_t){
            .hndl = &app->bb_pip
        }
    }); 

    // Vertex data for quad
    float v_data[] = {
        // Positions  UVs
        -1.0f, -1.0f,  0.0f, 0.0f,  // Top Left
         1.0f, -1.0f,  1.0f, 0.0f,  // Top Right 
        -1.0f,  1.0f,  0.0f, 1.0f,  // Bottom Left
         1.0f,  1.0f,  1.0f, 1.0f   // Bottom Right
    };

    // Index data for quad
    uint32_t i_data[] = {
        0, 3, 2,    // First Triangle
        0, 1, 3     // Second Triangle
    }; 

    // Construct vertex buffer
    app->vbo = gs_graphics_vertex_buffer_create(
        &(gs_graphics_vertex_buffer_desc_t) {
            .data = v_data,
            .size = sizeof(v_data)
        }
    );

    // Construct index buffer
    app->ibo = gs_graphics_index_buffer_create(
        &(gs_graphics_index_buffer_desc_t) {
            .data = i_data,
            .size = sizeof(i_data)
        }
    ); 

    /*
	app->camera = gs_camera_default();
	app->camera.ortho_scale = 10.f;
	app->camera.transform.position = gs_v3(6.4f, 8.9f, 6.6f);
	app->camera.transform.rotation = (gs_quat){.x = -0.19f, .y = 0.33f, .z = 0.09f, .w = 0.92f};
    */

	gs_asset_texture_load_from_file("./assets/slave_albedo.png", &app->ship_tex, NULL, false, false);

	app->camera = gs_camera_perspective();
	app->camera.transform.position = gs_v3(0.f, 6.f, 10.f);

    app->xform = gs_vqs_default();
    app->xform.position = gs_v3(0.f, 6.f, 0.f);
    app->xform.scale = gs_v3s(0.5f); 
    app->physics = (physics_component_t){.speed = 10.f};
}

typedef struct mesh_vert_t {
	gs_vec3 position;
	gs_vec2 uv;
	gs_color_t color;
} mesh_vert_t;

GS_API_DECL void app_draw_mesh(const gs_vqs* vqs)
{
	app_t* app = gs_engine_user_data(app_t);
	gs_immediate_draw_t* gsi = &app->core->gsi;

	gs_gfxt_mesh_import_options_t options = {
		.layout = (gs_gfxt_mesh_layout_t[]) {
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_POSITION},		// Float_3
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_TEXCOORD},		// Float_2
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_COLOR},		// Uint8_4
		},
		.layout_size = 3 * sizeof(gs_gfxt_mesh_layout_t),
		.index_buffer_element_size = sizeof(uint32_t)
	};

	// Push texture
	gsi_texture(gsi, app->ship_tex.hndl);

	// Not sure how to do lines...but can do triangles, I think.
	gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
	{
		gsi_mul_matrix(gsi, gs_vqs_to_mat4(vqs));

		for (uint32_t m = 0; m < app->mesh_count; ++m)
		{
			// Get mesh data at m
			gs_gfxt_mesh_raw_data_t* mesh = &app->meshes[m];

			for (uint32_t p = 0; p < mesh->prim_count; ++p)
			{
				mesh_vert_t* vertices = (mesh_vert_t*)mesh->vertices[p];
				uint32_t* indices = (uint32_t*)mesh->indices[p];
				uint32_t index_count = (size_t)mesh->index_sizes[p] / sizeof(uint32_t);

				// Iterate through indices
				for (uint32_t i = 0; i < index_count; i += 3)
				{
					uint32_t i0 = indices[i];
					uint32_t i1 = indices[i + 1];
					uint32_t i2 = indices[i + 2];

					// Grab verts with mesh vert declaration
					mesh_vert_t* v0 = &vertices[i0];
					mesh_vert_t* v1 = &vertices[i1];
					mesh_vert_t* v2 = &vertices[i2]; 

					// Do triangle now
					gsi_trianglevxmc(
						gsi, 
						v0->position, v1->position, v2->position, 
						v0->uv, v1->uv, v2->uv, 
						v0->color, v1->color, v2->color, 
						GS_GRAPHICS_PRIMITIVE_TRIANGLES
					);
				}
			}
		}
	}
	gsi_pop_matrix(gsi);

	// Pop texture
	gsi_texture(gsi, (gs_handle(gs_graphics_texture_t)){.id = 0}); 
}

GS_API_DECL void app_update_transform(physics_component_t* physics, gs_vqs* xform)
{ 
    const float dt = gs_platform_delta_time();
	gs_vec3* ps = &xform->position;

	const gs_vec2 xb = gs_v2(-12.f, 12.f);
	const gs_vec2 yb = gs_v2(-4.f, 11.f); 

	// Update player position based on input
	gs_vec3 v = gs_v3s(0.f);
	if (gs_platform_key_down(GS_KEYCODE_W) || gs_platform_key_down(GS_KEYCODE_UP))    v.y = ps->y > yb.x ? v.y - 1.f : 0.f;
	if (gs_platform_key_down(GS_KEYCODE_S) || gs_platform_key_down(GS_KEYCODE_DOWN))  v.y = ps->y < yb.y ? v.y + 1.f : 0.f;
	if (gs_platform_key_down(GS_KEYCODE_A) || gs_platform_key_down(GS_KEYCODE_LEFT))  v.x = ps->x > xb.x ? v.x - 1.f : 0.f;
	if (gs_platform_key_down(GS_KEYCODE_D) || gs_platform_key_down(GS_KEYCODE_RIGHT)) v.x = ps->x < xb.y ? v.x + 1.f : 0.f;

	// Normalize velocity then scale by player speed
	v = gs_vec3_scale(gs_vec3_norm(v), physics->speed * dt);

	// Add to velocity
	physics->velocity = gs_v3(
		gs_interp_linear(physics->velocity.x, v.x, 0.1f),
		gs_interp_linear(physics->velocity.y, v.y, 0.1f),
		gs_interp_linear(physics->velocity.z, v.z, 0.1f)
	);
	*ps = gs_vec3_add(*ps, physics->velocity); 

	// Move z back to origin
	float nz = -ps->z;
	ps->z = gs_interp_linear(ps->z, nz, 0.5f);

	gs_vec3 av = gs_v3s(0.f);
	bool br = false;
	if (gs_platform_key_down(GS_KEYCODE_W)) av.y -= 1.f;
	if (gs_platform_key_down(GS_KEYCODE_S)) av.y += 1.f;
	if (gs_platform_key_down(GS_KEYCODE_A)) av.x += 1.f;
	if (gs_platform_key_down(GS_KEYCODE_D)) av.x -= 1.f;
	if (gs_platform_key_down(GS_KEYCODE_Q)) av.z += 1.f;
	if (gs_platform_key_down(GS_KEYCODE_E)) av.z -= 1.f;
	if (gs_platform_key_down(GS_KEYCODE_B)) {br = true; av.z += 1.f;}
	av = gs_vec3_scale(gs_vec3_norm(av), 3.f);
	av.z = br ? av.z * 50.f : av.z * 10.f;
	// Add negated angular velocity to this
	av = gs_vec3_add(av, gs_vec3_scale(gs_vec3_neg(physics->angular_velocity), 1.5 * dt));
 
 	physics->angular_velocity = gs_vec3_add(physics->angular_velocity, av);
 	gs_vec3* pav = &physics->angular_velocity;
	const float max_rotx = 60.f;
	const float max_roty = 30.f;
	const float max_rotz = 90.f;
 	pav->x = gs_clamp(pav->x, -max_rotx, max_rotx);
 	pav->y = gs_clamp(pav->y, -max_roty, max_roty);
 	pav->z = br ? pav->z : gs_clamp(pav->z, -max_rotz, max_rotz);

	// Do rotation (need barrel roll rotation)
    /*
	xform->rotation = gs_quat_mul_list(3,
		gs_quat_angle_axis(gs_deg2rad(physics->angular_velocity.x), GS_YAXIS),
		gs_quat_angle_axis(gs_deg2rad(physics->angular_velocity.y), GS_XAXIS),
		gs_quat_angle_axis(gs_deg2rad(physics->angular_velocity.z), GS_ZAXIS)
	);
    */
}

GS_API_DECL void app_update()
{
    app_t* app = gs_engine_user_data(app_t);
	core_t* core = app->core;
    gs_command_buffer_t* cb = &core->cb;
    gs_immediate_draw_t* gsi = &core->gsi; 

    const gs_vec2 fbs = gs_platform_framebuffer_sizev(gs_platform_main_window());
    const gs_vec2 ws = gs_platform_window_sizev(gs_platform_main_window()); 

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 

    app_update_transform(&app->physics, &app->xform);

    // === [ Render scene ] ===== //

    // Render pass action for clearing the screen
    const gs_graphics_clear_desc_t def_clear = {.actions = &(gs_graphics_clear_action_t){.color = 0.0f, 0.0f, 0.0f, 1.f}};

    // Offscreen pass
    gs_graphics_begin_render_pass(cb, app->rp);
        gs_graphics_set_viewport(cb, 0, 0, RT_WIDTH, RT_HEIGHT);
        gs_graphics_clear(cb, &def_clear);
        gsi_camera(gsi, &app->camera);
        gsi_depth_enabled(gsi, true);
        gsi_face_cull_enabled(gsi, true);
        gsi_texture(gsi, app->tex);
        gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
        {
            // Snap angle of rotation over time
            const float step = 10.f;
            float za = (float)((int32_t)(gs_rad2deg(gs_platform_elapsed_time() * 0.0006f)) % 360); 
            za = (float)floor((int32_t)(za / step)) * step; 

            gs_vqs xform = app->xform;
            xform.position.x = (float)((int32_t)xform.position.x);
            xform.position.y = (float)((int32_t)xform.position.y);
            xform.position.z = (float)((int32_t)xform.position.z); 
            xform.rotation = gs_quat_angle_axis(gs_deg2rad(za), GS_YAXIS);

            static bool do_dis = false;

            if (gs_platform_key_pressed(GS_KEYCODE_P)) do_dis = !do_dis;

            if (do_dis) 
            {
                app_draw_mesh(&xform); 
            }
            else
            {
                // Draw a cube
                gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
                {
                    xform.position.z = 8.f;
                    xform.scale = gs_v3s(1.f);
                    gsi_mul_matrix(gsi, gs_vqs_to_mat4(&xform));
                    gsi_box(gsi, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 255, 255, 255, 255, GS_GRAPHICS_PRIMITIVE_TRIANGLES); 
                }
                gsi_pop_matrix(gsi);
            }

        }
        gsi_pop_matrix(gsi);
    gsi_draw(gsi, cb);
    gs_graphics_end_render_pass(cb); 
    
    // Backbuffer pass
    gs_graphics_begin_render_pass(cb, GS_GRAPHICS_RENDER_PASS_DEFAULT); 
        gs_graphics_set_viewport(cb, 0, 0, (int32_t)fbs.x, (int32_t)fbs.y); 
        gs_graphics_clear(cb, &def_clear);

        // Set uniforms for material
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_tex", &app->rt);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_tex_size", &(gs_vec2){RT_WIDTH, RT_HEIGHT});
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_screen_size", &fbs);

        gs_gfxt_material_bind(cb, &app->bb_mat);
        gs_gfxt_material_bind_uniforms(cb, &app->bb_mat); 

        // Bind unit quad
        gs_graphics_bind_desc_t binds = {
            .vertex_buffers = {.desc = &(gs_graphics_bind_vertex_buffer_desc_t){.buffer = app->vbo}},
            .index_buffers = {.desc = &(gs_graphics_bind_index_buffer_desc_t){.buffer = app->ibo}},
        };
        gs_graphics_apply_bindings(cb, &binds);

        // Draw
        gs_graphics_draw(cb, &(gs_graphics_draw_desc_t){.start = 0, .count = 6});

    gs_graphics_end_render_pass(cb);

    // Submit
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
