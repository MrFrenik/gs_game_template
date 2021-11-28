/*==============================================================================================================
    * Copyright: 2020 John Jackson 
    * Gunslinger Game Template
    * File: app.h
    * Github: https://github.com/MrFrenik/gs_game_template

    All Rights Reserved

    BSD 3-Clause License

    Copyright (c) 2020 John Jackson

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be used to 
    endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDi
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=================================================================================================================*/

#ifndef GS_APP_H
#define GS_APP_H

/*
    [ Main Application Interface ] 

    USAGE: (IMPORTANT)

    * Entry point to your application from Gunslinger. Here you fill out all related application data and return
        an application description object back to Gunslinger for initialization.
    
        GS_API_DECL gs_app_desc_t app_main(int32_t argc, char** argv)
        {
            ...
        }

    * Application sync points: 
        
        // Initialization for your application
        GS_API_DECL void app_init();

        // Updating your application each frame
        GS_API_DECL void app_update();

        // Shutting down your application
        GS_API_DECL void app_shutdown(); 


    =================================================================================================================

    * Contents: 

    [ Core ] 

    - gs_core_t is the main core structure that holds all relevant gameplay systems and content managers.
    - Found in core.h
    
        typedef struct gs_core_t
        {
            gs_command_buffer_t cb;         // Command buffer for rendering
            gs_immediate_draw_t gsi;        // Immediate draw utility
            gs_entity_manager_t entities;   // Entity data
            gs_asset_manager_t  assets;     // Asset data
            gs_meta_t           meta;       // Reflection data
            gs_mu_ctx           gmu;        // GUI utility context
        } gs_core_t;

    - The application creates a context for this core data and holds onto it for the length of the application.  

    [ Asset Manager ]

    - gs_asset_manager_t	 is the main collection of assets held for your application. It's a central storage and utility
        for importing and managing the lifetime of assets. 

    [ Entity Manager ]

    - gs_entity_manager_t holds all entity and component data for your application.  

        * Components:

            - Components can be any structure you define, but they must be marked up for reflection.
            - All components must derive from `gs_component_base_t`

            ex. 

            introspect()
            typedef struct my_comp_t
            {
                base(gs_component_base_t)

                field()
                uint32_t u_val;
            } my_comp_t;

            - On application initialization, all components should be registered. To do this, use the
                provided macro:  

                    entity_register_component(ENTITIES, T)

                where ENTITIES is a pointer to the core entity manager and T is your component type

                ex. 

                    entity_register_component(&app->core->entities, my_comp_t); 

            - Internally, this creates all necessary storage for your component data at runtime.

            * Entities: 

                - An entity is a structure that holds its runtime id as well as a dynamic array of component ids.

                - Entities must be allocated at runtime using the function: 
                        
                    uint32_t handle = gs_entities_allocate(gs_entity_manager_t*) 

                - This function returns a uint32_t handle to the user to reference the entity data during runtime

                - To add a registered component to an entity, use the following macro: 
                    
                    // Add component to entity of type T, passing in optional struct parameter for copying data
                    void gs_entities_add_component(gs_entity_manager_t*, uint32_t handle, T component_type, ...) 

                - To remove a component from an entity, use the following macro: 

                    void gs_entities_remove_component(gs_entity_manager_t*, uint32_t handle, T component_type)

                - To check if an entity has a component, use the following macro:
                    
                    bool gs_entities_has_component(gs_entity_manager_t*, uint32_t handle, T component_type)
                
                - Deallocate an entity: 
                    
                    void gs_entities_deallocate(gs_entity_manager_t* entities, uint32_t hndl); 

    [ Introspection ] 

    - This game template uses reflection for many of its automated utilities and features, such as object construct, 
        introspection, serialization/deserialization. 

    - Usage: 
        
        In order to participate in reflection, your structures and enums must be marked up for the reflection
            generation to work properly.

        All relfected objects derive from `gs_object_t	` (found in object.h), which is holds a unique meta class 
            identitifer (uint64_t). This identifier is used as a unique type id for all utilities in this template.

        Structs: 
            
            - All desired reflected structs must be marked up with the `introspect()` tag before defining the
                structure.

            introspect()
            typedef struct my_struct_t
            {
                ...
            } my_struct_t;

            - Each structure must derived from some base object. Due to how c aliases types, this must collapse down
                to `gs_object_t	` in the hierarchy. This means you can have a complicated chain of hiearchy, but the 
                base MUST be the first field in the structure. Use the `base()` macro for establishing this.

            introspect()
            typedef struct my_struct_t
            {
                // This structure is derived from `gs_object_t	`
                base(gs_object_t	)
            } my_struct_t;

            introspect()
            typedef struct my_derived_struct_t
            {
                // This structure derives from `my_struct_t`, which collapses into `gs_object_t	`
                base(my_struct_t)
            } my_derived_struct_t;

            Fields: 

                - Markup fields with `field()` tag

                Ex. 

                introspect()
                typedef struct my_struct_t
                {
                    // This structure is derived from `gs_object_t	`
                    base(gs_object_t	)

                    field() 
                    float f_val;

                } my_struct_t;


            - There are serveral optional methods that can be generated for your structures: 

                - Constructor: 

                    Used for constructing your object and assigning the object id upon creation.
                    Implicit `this` available in your function (casted to your object type)
                    
                    ctor( 

                        // Function signature/parameters (if none, enter 'void')
                        params(void),

                        // Function
                        func({
                            this->f_val = 3.145f;
                        })
                    )
                    

                - Destructor: 
                    
                    - Used for destruction of your object.
                    - Implicit `this` available, casted to your object type.

                    dtor({
                        // Your destructor here...
                    })

                - Serialize: 

                    - Used for serialization of your object into a runtime byte buffer.
                    - Parameters: 
                        * gs_byte_buffer_t* buffer
                        * const gs_object_t	* obj 

                    - Implicit `this` available, casted to your object type.

                    serialize({ 
                        // Serialize your object data into the available buffer
                        gs_byte_buffer_write(buffer, float, this->f_val);
                    )}

                - Deserialize: 

                    - Parameters: 
                        * gs_byte_buffer_t* buffer
                        * gs_object_t	* obj 

                    - Used for deserialization of your object.

                    deserialize({ 
                        // Deserialize your object data from the available buffer
                        gs_byte_buffer_read(buffer, float, &this->f_val);
                    })

        - Enums

            All enums can be reflected by simply adding the `introspect()` tag before declaring and defining the
                enum.

            Ex. 

            introspect()
            typedef enum my_enum
            {
                ENUM_VAL_0 = 0x00,
                ENUM_VAL_1
            } my_enum;

    [ Objects ]

    - All first class citizens in this template are "objects". They should be created, serialized, deserialized, 
        destroyed, and introspected at runtime using the following methods:

    // Object functions
    gs_obj_id(OBJ)                     // Get object class id from instance
    gs_obj_sid(T)                      // Get object class id from static type
    gs_obj_vtable_w_id(ID)             // Get vtable for class
    gs_obj_func_w_id(ID, NAME)         // Get function in vtable based on name
    obj_new(T)                      // Heap allocate object of type based on type
    gs_obj_newid(ID)                   // Heap allocate object based on cls id 
    gs_obj_ctor(T, ...)                // Construct object 
    gs_obj_dtor(T, OBJ)                // Destruct object 
    gs_obj_serialize(T, BUFFER, OBJ)   // Serialize object
    gs_obj_deserialize(T, BUFFER, OBJ) // Deserialize object 

    ================================================================================================================

*/ 

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

// App struct declaration 
typedef struct app_t
{
    // Core data
    gs_core_t* core; 

    // Your app data here...
    uint32_t ent;
} app_t; 

// Introspected struct
introspect()
typedef struct component_rotation_t
{
    // Parent 
    base(gs_component_base_t)

    // Functions

    /*
     *  Constructor
     *  @param: (optional) 
     *  @return: void
     */
    ctor
    ( 
        // Any parameters for this constructor
        params(float rotation_speed, gs_vec3 rotation_axis),

        // Function to be called
        func
        ({
            gs_println("CTOR");
            this->rotation_speed = rotation_speed;
            this->rotation_axis = rotation_axis;
        })
    )

    /*
     *  Destructor
     *  @param: component_rotation_t* this 
     *  @return: void
     */
    dtor
    ({
        gs_println("DTOR");
    })

    /*
     *  Serialize
     *  @param: gs_byte_buffer_t* buffer 
     *  @param: const component_rotation_t* this 
     *  @return: gs_result
     */
    serialize
    ({ 
        gs_println("SERIALIZE");
        gs_byte_buffer_write(buffer, float, this->rotation_speed);
        return GS_RESULT_SUCCESS;
    }) 

    /*
     *  Serialize
     *  @param: gs_byte_buffer_t* buffer 
     *  @param: component_rotation_t* this 
     *  @return: gs_result
     */
    deserialize
    ({
        gs_println("DESERIALIZE");
        gs_byte_buffer_read(buffer, float, &this->rotation_speed);
        return GS_RESULT_SUCCESS;
    }) 

    /*
     *  On Create
     *  @param: component_rotation_t* this 
     *  @return: void
     */
    on_create
    ({ 
        gs_println("ON CREATE");
    }) 

    /*
     *  On Update
     *  @param: component_rotation_t* this 
     *  @return: void
     */
    on_update
    ({ 
        gs_entity_manager_t* em = gs_entity_manager_instance(); 

        // Update rotation
        const float _t = gs_platform_elapsed_time();
        gs_quat rot = gs_quat_angle_axis(_t * 0.0001f * this->rotation_speed, this->rotation_axis);

        // Get entity for this component
        uint32_t ent = gs_component_get_entity(this);

        // Get transform component, if found 
        gs_component_transform_t* tc = gs_entities_get_component(em, ent, gs_component_transform_t);

        // Update rotation of transform 
        if (tc)
        { 
            tc->transform.rotation = rot;
        }
    })

    // Fields

    field(default = 100.f)
    float rotation_speed;

    field(default = GS_YAXIS)
    gs_vec3 rotation_axis; 

} component_rotation_t; 

// Main application interface functions
GS_API_DECL gs_app_desc_t app_main(int32_t argc, char** argv);
GS_API_DECL void app_init();
GS_API_DECL void app_update();
GS_API_DECL void app_shutdown(); 

// == [ Appplication Implementation ] == // 

#ifdef GS_APP_IMPL 

gs_graphics_scene_t scene = {0};

GS_API_DECL void app_init()
{
    // Initialize core
    app_t* app = gs_engine_user_data(app_t); 
    app->core = gs_core_new(); 

    // Load asset texture
    gs_asset_handle_t tex = gs_assets_import(&app->core->assets, "textures/slave_albedo.png", NULL, true);

    // Load asset pipeline
    gs_asset_handle_t pip = gs_assets_import(&app->core->assets, "pipelines/simple.sf", NULL, true); 

    // Load asset mesh
    gs_gfxt_mesh_import_options_t mo = gs_pipeline_get_mesh_import_options(gs_asset_handle_get	(&pip));
    gs_asset_handle_t mesh = gs_assets_import(&app->core->assets, "meshes/slave.gltf", &mo, true);
    gs_gfxt_mesh_import_options_free(&mo); 

    // Add new material to asset database
    gs_asset_handle_t mat = gs_assets_add_to_database(&app->core->assets, gs_obj_newc(gs_material_t, pip), "materials", "simple_mat", true); 

    // Set texture uniform for material
    gs_material_set_uniform(gs_asset_handle_get	(&mat), "u_tex", &((gs_texture_t*)gs_asset_handle_get(&tex))->texture.hndl); 

    // Register new component with entity manager
    gs_entities_register_component(&app->core->entities, component_rotation_t);

    // Allocate entity and attach necessary components
    app->ent = gs_entities_allocate(&app->core->entities);
    gs_entities_add_component(&app->core->entities, app->ent, component_rotation_t, gs_obj_ctor(component_rotation_t, 3.145f, gs_v3(0.f, 1.f, 0.f))); 
    gs_entities_add_component(&app->core->entities, app->ent, gs_component_transform_t, gs_obj_ctor(gs_component_transform_t, gs_vqs_default())); 
    gs_entities_add_component(&app->core->entities, app->ent, gs_component_static_mesh_t, gs_obj_ctor(gs_component_static_mesh_t, &scene));

    // Get renderable component
    gs_component_static_mesh_t* sm = gs_entities_get_component(&app->core->entities, app->ent, gs_component_static_mesh_t);
    gs_renderable_static_mesh_t* rend = gs_graphics_scene_get_renderable_static_mesh(&scene, sm->renderable_id);

    // Assign mesh to renderable
    gs_renderable_static_mesh_set_mesh(rend, mesh);

    // Assign material to renderable at idx 0
    gs_renderable_base_set_material(rend, mat, 0); 
} 

GS_API_DECL void app_update()
{
    // Cache app/core pointers
    app_t* app = gs_engine_user_data(app_t);
    gs_core_t* core = app->core;
    gs_command_buffer_t* cb = &core->cb;
    gs_immediate_draw_t* gsi = &core->gsi; 
    gs_entity_manager_t* em = &core->entities; 
    gs_mu_ctx* gmu = &core->gmu;

    // Get necessary platform metrics
    const gs_vec2 fb = gs_platform_framebuffer_sizev(gs_platform_main_window());
    const gs_vec2 ws = gs_platform_window_sizev(gs_platform_main_window()); 

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 

    // Update entity manager
    gs_entities_update(em); 

    // Get view projection
    gs_camera_t cam = gs_camera_perspective();
    cam.transform.position.z = 20.f; 
    gs_mat4 vp = gs_camera_get_view_projection(&cam, ws.x, ws.y);

    // Render pass action for clearing the screen
    gs_graphics_clear_desc_t clear = (gs_graphics_clear_desc_t){
        .actions = &(gs_graphics_clear_action_t){.color = {0.1f, 0.1f, 0.1f, 1.f}}
    }; 

    // Main render pass
    gs_graphics_begin_render_pass(cb, GS_GRAPHICS_RENDER_PASS_DEFAULT);           // Begin render pass (default render pass draws to back buffer)

        gs_graphics_clear(cb, &clear);                                            // Clear screen
        gs_graphics_set_viewport(cb, 0, 0, (uint32_t)ws.x, (uint32_t)ws.y);       // Set render viewport

        // Iterate through static meshes in scene
        for (
                gs_slot_array_iter it = gs_slot_array_iter_new(scene.static_meshes); 
                gs_slot_array_iter_valid(scene.static_meshes, it);
                gs_slot_array_iter_advance(scene.static_meshes, it)
        )
        {
            // Get renderable pointer from scene
            const gs_renderable_static_mesh_t* rend = gs_slot_array_iter_getp(scene.static_meshes, it); 
            gs_material_t* mat = gs_asset_handle_get(&cast(rend, gs_renderable_base_t)->materials[0]);
            gs_mesh_t* mesh = gs_asset_handle_get(&rend->mesh);

            // Get model matrix for renderable
            const gs_mat4 model = cast(rend, gs_renderable_base_t)->model_matrix;

            // Final MVP matrix
            const gs_mat4 mvp = gs_mat4_mul(vp, model);

            // Set material uniforms
            gs_material_set_uniform(mat, "u_mvp",  &mvp);

            // Bind material pipeline and uniforms
            gs_material_bind(cb, mat);                                                   // Bind material pipeline
            gs_material_bind_uniforms(cb, mat);                                          // Bind material uniforms

            // Draw mesh
            gs_mesh_draw(cb, mesh);                                                      // Draw mesh 
        }

    // End main pass
    gs_graphics_end_render_pass(cb);      

    // Submit command buffer for rendering
    gs_graphics_submit_command_buffer(cb); 
}

GS_API_DECL void app_shutdown()
{
    app_t* app = gs_engine_user_data(app_t); 
    gs_core_delete(app->core); 
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

#endif  // GS_APP_IMPL
#endif  // GS_APP_H
