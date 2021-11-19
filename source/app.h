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

#ifndef APP_H
#define APP_H

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

    - core_t is the main core structure that holds all relevant gameplay systems and content managers.
    - Found in core.h
    
        typedef struct core_t
        {
            gs_command_buffer_t cb;         // Command buffer for rendering
            gs_immediate_draw_t gsi;        // Immediate draw utility
            entity_manager_t 	entities;   // Entity data
            asset_manager_t 	assets;     // Asset data
            meta_t 				meta;       // Reflection data
            gs_mu_ctx           gmu;        // GUI utility context
        } core_t;

    - The application creates a context for this core data and holds onto it for the length of the application.  

    [ Asset Manager ]

    - asset_manager_t is the main collection of assets held for your application. It's a central storage and utility
        for importing and managing the lifetime of assets. 

    [ Entity Manager ]

    - entity_manager_t holds all entity and component data for your application.  

        * Components:

            - Components can be any structure you define, but they must be marked up for reflection.
            - All components must derive from `component_base_t`

            ex. 

            introspect()
            typedef struct my_comp_t
            {
                base(component_base_t)

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
                        
                    uint32_t handle = entities_allocate(entity_manager_t*) 

                - This function returns a uint32_t handle to the user to reference the entity data during runtime

                - To add a registered component to an entity, use the following macro: 
                    
                    // Add component to entity of type T, passing in optional struct parameter for copying data
                    void entities_add_component(entity_manager_t*, uint32_t handle, T component_type, ...) 

                - To remove a component from an entity, use the following macro: 

                    void entities_remove_component(entity_manager_t*, uint32_t handle, T component_type)

                - To check if an entity has a component, use the following macro:
                    
                    bool entities_has_component(entity_manager_t*, uint32_t handle, T component_type)
                
                - Deallocate an entity: 
                    
                    void entities_deallocate(entity_manager_t* entities, uint32_t hndl); 

    [ Introspection ] 

    - This game template uses reflection for many of its automated utilities and features, such as object construct, 
        introspection, serialization/deserialization. 

    - Usage: 
        
        In order to participate in reflection, your structures and enums must be marked up for the reflection
            generation to work properly.

        All relfected objects derive from `object_t` (found in object.h), which is holds a unique meta class 
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
                to `object_t` in the hierarchy. This means you can have a complicated chain of hiearchy, but the 
                base MUST be the first field in the structure. Use the `base()` macro for establishing this.

            introspect()
            typedef struct my_struct_t
            {
                // This structure is derived from `object_t`
                base(object_t)
            } my_struct_t;

            introspect()
            typedef struct my_derived_struct_t
            {
                // This structure derives from `my_struct_t`, which collapses into `object_t`
                base(my_struct_t)
            } my_derived_struct_t;

            Fields: 

                - Markup fields with `field()` tag

                Ex. 

                introspect()
                typedef struct my_struct_t
                {
                    // This structure is derived from `object_t`
                    base(object_t)

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
                        * const object_t* obj 

                    - Implicit `this` available, casted to your object type.

                    serialize({ 
                        // Serialize your object data into the available buffer
                        gs_byte_buffer_write(buffer, float, this->f_val);
                    )}

                - Deserialize: 

                    - Parameters: 
                        * gs_byte_buffer_t* buffer
                        * object_t* obj 

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
    obj_id(OBJ)                     // Get object class id from instance
    obj_sid(T)                      // Get object class id from static type
    obj_vtable_w_id(ID)             // Get vtable for class
    obj_func_w_id(ID, NAME)         // Get function in vtable based on name
    obj_new(T)                      // Heap allocate object of type based on type
    obj_newid(ID)                   // Heap allocate object based on cls id 
    obj_ctor(T, ...)                // Construct object 
    obj_dtor(T, OBJ)                // Destruct object 
    obj_serialize(T, BUFFER, OBJ)   // Serialize object
    obj_deserialize(T, BUFFER, OBJ) // Deserialize object 

    ================================================================================================================

*/ 

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

// Shaders
#if (defined GS_PLATFORM_WEB || defined GS_PLATFORM_ANDROID)
    #define GS_GL_VERSION_STR "#version 300 es\n"
#else
    #define GS_GL_VERSION_STR "#version 330 core\n"
#endif 

// App struct declaration 
typedef struct app_t
{
    // Core data
	core_t* core; 

    // Entity handle
    uint32_t entity;
} app_t; 

// Introspected struct
introspect()
typedef struct my_comp_t
{
    base(component_base_t)

    // Constructor
    ctor( 
        // Any parameters for this constructor
        params(float v)

        // Function to be called
        func({
            this->f_val = v;
        })
    )

    // Destructor
    dtor({
        // Your function here...
    })

    // Serialize
    serialize({ 
        // Your function here...
    })

    // Deserialize
    deserialize({
        // Your function here...
    })

    // Introspected field data
    field()
    float f_val;

} my_comp_t;

// Main application interface functions
GS_API_DECL gs_app_desc_t app_main(int32_t argc, char** argv);
GS_API_DECL void app_init();
GS_API_DECL void app_update();
GS_API_DECL void app_shutdown(); 

// == [ Appplication Implementation ] == // 

#ifdef APP_IMPL 

GS_API_DECL void app_init()
{
	// Initialize core
    app_t* app = gs_engine_user_data(app_t); 
	app->core = core_new(); 

    // Register new component with entity manager
    entities_register_component(&app->core->entities, my_comp_t);

    // Allocate entity and attach component
    uint32_t ent = entities_allocate(&app->core->entities);
    entities_add_component(&app->core->entities, ent, my_comp_t, obj_ctor(my_comp_t)); 
} 

GS_API_DECL void app_update()
{
    // Cache app/core pointers
    app_t* app = gs_engine_user_data(app_t);
	core_t* core = app->core;
    gs_command_buffer_t* cb = &core->cb;
    gs_immediate_draw_t* gsi = &core->gsi; 
    entity_manager_t* em = &core->entities; 
    gs_mu_ctx* gmu = &core->gmu;

    // Get necessary platform metrics
    const gs_vec2 fb = gs_platform_framebuffer_sizev(gs_platform_main_window());
    const gs_vec2 ws = gs_platform_window_sizev(gs_platform_main_window()); 

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 

    // Get entity data
    my_comp_t* mc = entities_get_component(em, app->entity, my_comp_t);

    // Render entity info to screen using gsi
    gsi_camera2D(gsi);
    gs_snprintfc(TMP_BUF, 256, "f_val: %.2f", mc->f_val);
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
