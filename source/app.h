
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

static float bg[3] = {10, 10, 10};

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
    gs_vec3             palettes[3];
    gs_vec2             ranges;
    gs_rand             rand;
    physics_component_t physics;
    gs_asset_texture_t  pix_tex;
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

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define RT_WIDTH  WINDOW_WIDTH / 2
#define RT_HEIGHT WINDOW_HEIGHT / 2 

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
"uniform vec3 u_palette0;\n"
"uniform vec3 u_palette1;\n"
"uniform vec3 u_palette2;\n"
"uniform vec2 u_ranges;\n"
"out vec4 frag_color;\n"
"\n"
"float luminance(vec3 rgb)\n" 
"{\n"
"   vec3 w = vec3(0.2125, 0.7154, 0.0721);\n"
"   return dot(rgb, w);\n"
"}\n"
"\n"
"void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord)\n"
"{\n"
"   float w = 1.0 / tex_size.x;\n"
"   float h = 1.0 / tex_size.y;\n"
"   n[0] = texture(u_tex, coord + vec2( -w, -h));\n"
"   n[1] = texture(u_tex, coord + vec2(0.0, -h));\n"
"   n[2] = texture(u_tex, coord + vec2(  w, -h));\n"
"   n[3] = texture(u_tex, coord + vec2( -w, 0.0));\n"
"   n[4] = texture(u_tex, coord + vec2(0.0, 0.0));\n"
"   n[5] = texture(u_tex, coord + vec2(  w, 0.0));\n"
"   n[6] = texture(u_tex, coord + vec2( -w, h));\n"
"   n[7] = texture(u_tex, coord + vec2(0.0, h));\n"
"   n[8] = texture(u_tex, coord + vec2(  w, h));\n"
"}\n"
"\n"
"void main() {\n"
"  float tex_per_pix = (tex_size.x) / u_screen_size.x;\n" 
"  vec2 loc_within_texel = fract(uv);\n" 
"  vec2 lerp_amount = clamp(loc_within_texel / tex_per_pix, 0.0, 0.5);\n" 
"  lerp_amount = lerp_amount + clamp((loc_within_texel - 1) / tex_per_pix + 0.5, 0.0, 0.5);\n" 
"  vec2 final_uv = (floor(uv) + lerp_amount) / tex_size;\n" 
"  vec4 n[9];\n" 
"  make_kernel(n, u_tex, final_uv);\n" 
"  vec4 se_h = n[2] + (2.0 * n[5]) + n[8] - (n[0] + (2.0 * n[3]) + n[6]);\n" 
"  vec4 se_v = n[0] + (2.0 * n[1]) + n[2] - (n[6] + (2.0 * n[7]) + n[8]);\n" 
"  vec4 sobel = sqrt((se_h * se_h) + (se_v * se_v));\n" 
"  frag_color = texture(u_tex, final_uv);\n"  // Will have to upsample this 
"  float a = frag_color.a;\n" 
"  float lum = luminance(frag_color.rgb);\n" 
"  if (lum < u_ranges.x) frag_color.rgb = u_palette0;\n" 
"  else if (lum < u_ranges.y) frag_color.rgb = u_palette1;\n" 
"  else frag_color.rgb = u_palette2;\n" 
"  frag_color.a = a;\n" 
"  frag_color.rgb = frag_color.rgb * vec3(1.0 - sobel.rgb);\n" 
"}\n";

// Need a few things: 
    // offscreen pipeline for rendering simple objects (could honestly just use GSI for rendering a cube...)
    // onscreen pipeline for rendering default quad texture to screen 

GS_API_DECL void app_init()
{
    app_t* app = gs_engine_user_data(app_t); 

	// Initialize core
	app->core = core_new(); 

    // Seed rand
    app->rand = gs_rand_seed(gs_hash_str64("Gunslinger"));

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
                {.name = "u_screen_size", .type = GS_GRAPHICS_UNIFORM_VEC2},
                {.name = "u_palette0", .type = GS_GRAPHICS_UNIFORM_VEC3},
                {.name = "u_palette1", .type = GS_GRAPHICS_UNIFORM_VEC3},
                {.name = "u_palette2", .type = GS_GRAPHICS_UNIFORM_VEC3},
                {.name = "u_ranges", .type = GS_GRAPHICS_UNIFORM_VEC2}
            },
            .layout_size = 7 * sizeof(gs_gfxt_uniform_desc_t)
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

    app->palettes[0] = gs_v3s(0.1f);
    app->palettes[1] = gs_v3s(0.6f);
    app->palettes[2] = gs_v3s(0.9f);
    app->ranges = gs_v2(0.1f, 0.6f);

	app->camera = gs_camera_default();
	app->camera.ortho_scale = 1.f;
	// app->camera.transform.position = gs_v3(6.4f, 8.9f, 6.6f);
	app->camera.transform.position = gs_v3(-9.5f, 2.f, -.75f);
    app->camera.transform.rotation = gs_quat_from_euler(0.f, 45.f, -18.f);

	gs_asset_texture_load_from_file("./assets/slave_albedo.png", &app->ship_tex, NULL, false, false); 

    gs_graphics_texture_desc_t tdesc = {
        .format = GS_GRAPHICS_TEXTURE_FORMAT_RGBA8,
        .min_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST, 
        .mag_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST 
    };
    gs_asset_texture_load_from_file("./assets/water.png", &app->pix_tex, &tdesc, false, false);

    app->xform = gs_vqs_default();
    app->xform.position = gs_v3(0.f, 6.f, 0.f);
    app->xform.scale = gs_v3s(0.2f); 
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

void test_window(mu_Context* ctx); 
void camera_gui(mu_Context* ctx, gs_camera_t* cam);

GS_API_DECL void app_update()
{
    app_t* app = gs_engine_user_data(app_t);
	core_t* core = app->core;
    gs_command_buffer_t* cb = &core->cb;
    gs_immediate_draw_t* gsi = &core->gsi; 
    gs_mu_ctx* gmu = &core->gmu;

    const gs_vec2 fbs = gs_platform_framebuffer_sizev(gs_platform_main_window());
    const gs_vec2 ws = gs_platform_window_sizev(gs_platform_main_window()); 

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 

    app_update_transform(&app->physics, &app->xform);

    // === [ Render scene ] ===== //

    // Render pass action for clearing the screen
    const gs_graphics_clear_desc_t def_clear = {.actions = &(gs_graphics_clear_action_t)
        {.color = 0.1f, 0.1f, 0.1f, 1.f}
    };

    const gs_graphics_clear_desc_t fb_clear = {.actions = &(gs_graphics_clear_action_t)
        {0.f, 0.f, 0.f, 0.f}
    };

    const float t = gs_platform_elapsed_time() * 0.004f;

    // Offscreen pass
    gs_graphics_begin_render_pass(cb, app->rp);
        gs_graphics_set_viewport(cb, 0, 0, RT_WIDTH, RT_HEIGHT);
        gs_graphics_clear(cb, &fb_clear);
        gsi_camera(gsi, &app->camera);
        //gsi_camera2D(gsi);
        gsi_depth_enabled(gsi, true);
        gsi_face_cull_enabled(gsi, true);
        // gsi_texture(gsi, app->tex);
        gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
        {
            // Snap angle of rotation over time
            const float step = 5.f;
            float za = (float)((int32_t)(gs_rad2deg(gs_platform_elapsed_time() * 0.0003f)) % 360); 
            za = (float)floor((int32_t)(za / step)) * step; 

            gs_vqs xform = app->xform;
            xform.position.x = (float)((int32_t)xform.position.x);
            xform.position.y = (float)((int32_t)xform.position.y);
            xform.position.z = (float)((int32_t)xform.position.z); 
            float val = 3.f * sin(t); 
            float diff = val - round(val); 
            // No idea how to calculate what I'm thinking of calculating here...
            // Seems impossible
            xform.position.y = round(val) + (round(diff * 2.f) * 0.5f);

            // Not sure if this is possible, what I'm trying to do...
            // Maybe I should render it all out to a separate buffer at full resolution?

            static bool do_dis = false;

            if (gs_platform_key_pressed(GS_KEYCODE_P)) do_dis = !do_dis;

            app_draw_mesh(&xform); 

            // gsi_texture(gsi, app->tex);

            // Draw a cube
            gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
            {
                xform.position.x = -1.f;
                xform.position.z = 8.f;
                xform.scale = gs_v3s(1.f);
                xform.position.y = 0.f;
                xform.rotation = gs_quat_mul_list(3, 
                    gs_quat_angle_axis(gs_deg2rad(za), GS_XAXIS),
                    gs_quat_angle_axis(gs_deg2rad(za - step), GS_YAXIS),
                    gs_quat_angle_axis(gs_deg2rad(za + step), GS_ZAXIS) 
                ); 
                gsi_mul_matrix(gsi, gs_vqs_to_mat4(&xform));
                gsi_box(gsi, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 255, 255, 255, 255, GS_GRAPHICS_PRIMITIVE_LINES); 
            }
            gsi_pop_matrix(gsi);

            // Sphere
            gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
            {
                xform.position.x = -3.0f;
                xform.position.z = 8.f;
                xform.rotation = gs_quat_default();
                xform.scale = gs_v3s(1.f);
                gsi_mul_matrix(gsi, gs_vqs_to_mat4(&xform));
                gsi_sphere(gsi, 0.f, 0.f, 0.f, 0.5f, 255, 255, 255, 255, GS_GRAPHICS_PRIMITIVE_TRIANGLES); 
            }
            gsi_pop_matrix(gsi);

            static gs_vqs xforms[10 * 10];
            static bool updated = false; 
            if (!updated) 
            { 
                updated = true;
                gs_for_range_i(10)
                gs_for_range_j(10)
                {
                    uint32_t idx = i * 10 + j;
                    xforms[idx] = gs_vqs_default();
                    xforms[idx].position = gs_v3(j, 0.f, i);
                    xforms[idx].scale.y = gs_rand_gen_range(&app->rand, 0.1f, 3.f);
                } 
            }

            // Do a grid of randomly sized boxes
            gs_for_range_i(10)
            {
                gs_for_range_j(10)
                {
                    gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
                    {
                        uint32_t idx = i * 10 + j;
                        gsi_mul_matrix(gsi, gs_vqs_to_mat4(&xforms[idx]));
                        gsi_box(gsi, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 255, 255, 255, 255, GS_GRAPHICS_PRIMITIVE_TRIANGLES);
                    }
                    gsi_pop_matrix(gsi);
                }
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
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_palette0", &app->palettes[0]);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_palette1", &app->palettes[1]);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_palette2", &app->palettes[2]);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_ranges", &app->ranges);

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

        // UI
        gs_mu_new_frame(gmu); 
        mu_begin(&gmu->mu);
        // test_window(&gmu->mu);
        camera_gui(&gmu->mu, &app->camera);
        // log_window(&gmu->mu);
        mu_end(&gmu->mu);
        // Submits to cb
        gs_mu_render(gmu, cb); 
    gs_graphics_end_render_pass(cb);

    // Try to do a 3d scene, orthographic view, displaying quads (pixel art asset)
    gsi_camera(gsi, &app->camera);
   // gsi_camera2D(gsi); 

    // Not sure now what the size of this should be...
    gsi_texture(gsi, app->pix_tex.hndl);
    gs_quat cq = gs_quat_inverse(gs_quat_norm(app->camera.transform.rotation));
    gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
    {
        gsi_mul_matrix(gsi, gs_quat_to_mat4(cq));
        // Need to change the orientation of this crap...get the inverse of the camera's rotation?
        gsi_rectvd(gsi, gs_v2(0.f, 0.f), gs_vec2_scale(gs_v2(100.f, 64.f), 0.1f), gs_v2s(0.f), gs_v2s(1.f), GS_COLOR_WHITE, GS_GRAPHICS_PRIMITIVE_TRIANGLES); 
        gsi_draw(gsi, cb);
    }
    gsi_pop_matrix(gsi);

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
		.window_width = WINDOW_WIDTH,
		.window_height = WINDOW_HEIGHT,
		.init = app_init,
		.update = app_update,
		.shutdown = app_shutdown,
		.user_data = gs_malloc_init(app_t)
	};
} 

void camera_gui(mu_Context* ctx, gs_camera_t* cam)
{
    app_t* app = gs_engine_user_data(app_t); 

    if (mu_begin_window(ctx, "Properties", mu_rect(40, 40, 300, 450)))
    { 
        if (mu_header_ex(ctx, "Ranges", MU_OPT_EXPANDED)) 
        {
            gs_vec2* ranges = &app->ranges;
            mu_label(ctx, "low:"); mu_slider(ctx, &ranges->x, 0.f, ranges->y); 
            mu_label(ctx, "high:"); mu_slider(ctx, &ranges->y, ranges->x, 1.f); 
        }

        if (mu_header_ex(ctx, "Palette", MU_OPT_EXPANDED)) 
        {
            for (uint32_t i = 0; i < 3; ++i)
            {
                gs_vec3* p = &app->palettes[i];
                gs_snprintfc(TMP, 32, "p%d: ", i);
                mu_label(ctx, TMP); 
                mu_label(ctx, "r:"); mu_slider(ctx, &p->x, 0.f, 1.f); 
                mu_label(ctx, "g:"); mu_slider(ctx, &p->y, 0.f, 1.f); 
                mu_label(ctx, "b:"); mu_slider(ctx, &p->z, 0.f, 1.f); 
            }
        }
        
        if (mu_header_ex(ctx, "Transform", MU_OPT_EXPANDED)) 
        { 
            if (mu_header_ex(ctx, "Position", MU_OPT_EXPANDED)) 
            {
                float range = 20.f;
                gs_vec3* p = &cam->transform.position;
                mu_label(ctx, "x:"); mu_slider(ctx, &p->x, -range, range); 
                mu_label(ctx, "y:"); mu_slider(ctx, &p->y, -range, range); 
                mu_label(ctx, "z:"); mu_slider(ctx, &p->z, -range, range); 
                p->x = round(p->x);
                p->y = round(p->y);
                p->z = round(p->z);
            }
            
            if (mu_header_ex(ctx, "Euler", MU_OPT_CLOSED)) 
            {
                static gs_vec3 ea = {0.f, 45.f, -18.f};
                mu_label(ctx, "x:"); 
                if (mu_slider(ctx, &ea.x, -360.f, 360.f)) goto set_rot;
                mu_label(ctx, "y:"); 
                if (mu_slider(ctx, &ea.y, -360.f, 360.f)) goto set_rot;
                mu_label(ctx, "z:"); 
                if (mu_slider(ctx, &ea.z, -360.f, 360.f)) goto set_rot;

                set_rot: 
                    cam->transform.rotation = gs_quat_from_euler(ea.x, ea.y, ea.z);
            } 

            if (mu_header_ex(ctx, "Quat", MU_OPT_CLOSED)) 
            {
                const gs_quat* q = &cam->transform.rotation;
                gs_transient_buffer(TMP, 32);
                gs_snprintf(TMP, 32, "<%.2f, %.2f, %.2f, %.2f>", q->x, q->y, q->z, q->w); 
                mu_label(ctx, TMP); 
            } 
        }

        if (mu_header_ex(ctx, "Ortho Scale", MU_OPT_EXPANDED)) 
        {
            mu_label(ctx, "Ortho:"); mu_slider(ctx, &cam->ortho_scale, 0.1f, 20.f); 
        } 

        mu_end_window(ctx);
    } 
}

void test_window(mu_Context* ctx) 
{
    #define write_log(TXT) gs_println("%s", TXT);

    if (mu_begin_window(ctx, "Demo Window", mu_rect(40, 40, 300, 450))) 
    {
        mu_Container *win = mu_get_current_container(ctx);
        win->rect.w = mu_max(win->rect.w, 240);
        win->rect.h = mu_max(win->rect.h, 300);

        // === window info === //
        if (mu_header(ctx, "Window Info")) {
            mu_Container *win = mu_get_current_container(ctx);
            char buf[64];
            mu_layout_row(ctx, 2, (int[]) { 54, -1 }, 0);
            mu_label(ctx,"Position:");
            sprintf(buf, "%d, %d", win->rect.x, win->rect.y); mu_label(ctx, buf);
            mu_label(ctx, "Size:");
            sprintf(buf, "%d, %d", win->rect.w, win->rect.h); mu_label(ctx, buf);
        }

        // === labels + buttons === //
        if (mu_header_ex(ctx, "Test Buttons", MU_OPT_EXPANDED)) 
        {
            mu_layout_row(ctx, 3, (int[]) { 86, -110, -1 }, 0);
            mu_label(ctx, "Test buttons 1:");
            if (mu_button(ctx, "Button 1")) {write_log("Pressed button 1");}
            if (mu_button(ctx, "Button 2")) {write_log("Pressed button 2");}
            mu_label(ctx, "Test buttons 2:");
            if (mu_button(ctx, "Button 3")) {write_log("Pressed button 3");}
            if (mu_button(ctx, "Popup")) {mu_open_popup(ctx, "Test Popup");}
            if (mu_begin_popup(ctx, "Test Popup")) 
            {
                mu_button(ctx, "Hello");
                mu_button(ctx, "World");
                mu_end_popup(ctx);
            }
        }

        // === tree === //

        if (mu_header_ex(ctx, "Tree and Text", MU_OPT_EXPANDED)) 
        {
            mu_layout_row(ctx, 2, (int[]) { 140, -1 }, 0);
            mu_layout_begin_column(ctx);
            if (mu_begin_treenode(ctx, "Test 1")) 
            {
                if (mu_begin_treenode(ctx, "Test 1a")) 
                {
                    mu_label(ctx, "Hello");
                    mu_label(ctx, "world");
                    mu_end_treenode(ctx);
                }

                if (mu_begin_treenode(ctx, "Test 1b")) 
                {
                    if (mu_button(ctx, "Button 1")) {write_log("Pressed button 1");}
                    if (mu_button(ctx, "Button 2")) {write_log("Pressed button 2");}
                    mu_end_treenode(ctx);
                }
                mu_end_treenode(ctx);
            }

            if (mu_begin_treenode(ctx, "Test 2")) 
            {
                mu_layout_row(ctx, 2, (int[]) { 54, 54 }, 0);
                if (mu_button(ctx, "Button 3")) {write_log("Pressed button 3");}
                if (mu_button(ctx, "Button 4")) {write_log("Pressed button 4");}
                if (mu_button(ctx, "Button 5")) {write_log("Pressed button 5");}
                if (mu_button(ctx, "Button 6")) {write_log("Pressed button 6");}
                mu_end_treenode(ctx);
            }

            if (mu_begin_treenode(ctx, "Test 3")) 
            {
                static int checks[3] = { 1, 0, 1 };
                mu_checkbox(ctx, "Checkbox 1", &checks[0]);
                mu_checkbox(ctx, "Checkbox 2", &checks[1]);
                mu_checkbox(ctx, "Checkbox 3", &checks[2]);
                mu_end_treenode(ctx);
            }

            mu_layout_end_column(ctx);

            mu_layout_begin_column(ctx);
            mu_layout_row(ctx, 1, (int[]) {-1}, 0);
            mu_text(ctx, "Lorem ipsum dolor sit amet, consectetur adipiscing "
                         "elit. Maecenas lacinia, sem eu lacinia molestie, mi risus faucibus "
                         "ipsum, eu varius magna felis a nulla.");
            mu_layout_end_column(ctx);
        }

        if (mu_header_ex(ctx, "Background Color", MU_OPT_EXPANDED)) 
        {
            mu_layout_row(ctx, 2, (int[]) {-78, -1}, 74);
            mu_layout_begin_column(ctx);
            mu_layout_row(ctx, 2, (int[]) {46, -1}, 0);
            mu_label(ctx, "Red:");   mu_slider(ctx, &bg[0], 0, 255);
            mu_label(ctx, "Green:"); mu_slider(ctx, &bg[1], 0, 255);
            mu_label(ctx, "Blue:");  mu_slider(ctx, &bg[2], 0, 255);
            mu_layout_end_column(ctx);
            mu_Rect r = mu_layout_next(ctx);
            mu_draw_rect(ctx, r, mu_color(bg[0], bg[1], bg[2], 255));
            char buf[32];
            sprintf(buf, "#%02X%02X%02X", (int) bg[0], (int) bg[1], (int) bg[2]);
            mu_draw_control_text(ctx, buf, r, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);
        }

        mu_end_window(ctx);
    }
}

#endif  // APP_IMPL
#endif  // APP_H
