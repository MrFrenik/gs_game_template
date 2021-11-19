/*==============================================================================================================
    * Copyright: 2020 John Jackson 
    * Gunslinger Game Template
    * File: core.h
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
	
	// Register core components	
	entities_register_component(&core->entities, component_transform_t);
	entities_register_component(&core->entities, component_physics_t);

	return core;
}

GS_API_DECL void core_delete(core_t* core)
{
    gs_immediate_draw_free(&core->gsi);
    gs_command_buffer_free(&core->cb);
    
}


#endif // CORE_IMPL
#endif // CORE_H

