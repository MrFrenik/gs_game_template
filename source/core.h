#ifndef CORE_H
#define CORE_H

// Core systems and data for app
typedef struct core_t
{
    gs_command_buffer_t cb; 
    gs_immediate_draw_t gsi; 
	entity_manager_t 	entities;
	asset_manager_t 	assets;
	meta_t 				meta;
    gs_mu_ctx           gmu;
} core_t;

GS_API_DECL core_t* core_new();
GS_API_DECL void core_delete(core_t* core);

#ifdef CORE_IMPL

GS_API_DECL core_t* core_new()
{
	core_t* core = gs_malloc_init(core_t);

    //=== [ Structures ] ===// 
	
    core->cb = gs_command_buffer_new();
    core->gsi = gs_immediate_draw_new();

    //=== [ GSMUI ] ===// 

    core->gmu = gs_mu_new(); 

	//=== [ Meta ] ====//
	
	core->meta = meta_new();
	meta_set_instance(&core->meta);

	// Register gunslinger meta information
	meta_register_gs(&core->meta); 

	// Register all generated meta information
	meta_register_generated(&core->meta);

	//=== [ Assets ] ====//
	
	// Initialize asset manager with root assets path
    const char* assets_path = gs_platform_dir_exists("./assets/") ? "./assets" : "../assets";
	assets_init(&core->assets, assets_path);

	//==== [ Entity ] ===//
	
	// Register components	
	entities_register_component(&core->entities, component_transform_t);
	entities_register_component(&core->entities, component_physics_t);

	return core;
}

GS_API_DECL void core_delete(core_t* core)
{
}


#endif // CORE_IMPL
#endif // CORE_H

