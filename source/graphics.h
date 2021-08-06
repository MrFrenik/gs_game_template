#ifndef GRAPHICS_H
#define GRAPHICS_H

_introspect()
typedef struct renderable_t
{
	BASE(object_t);

	// Fields
	gs_gfxt_renderable_t data;
	asset_hndl_t mesh_hndl;
	asset_hndl_t material_hndl;

} renderable_t;

GS_API_DECL void renderable_set_material_uniform(renderable_t* rend, const char* name, void* data);

// Need a way to add renderables to a scene
typedef struct graphics_scene_t 
{
	BASE(object_t);

	// Fields
	gs_slot_array(renderable_t) renderables;

} graphics_scene_t;

// Need a way to sort renderables by material/pipeline
GS_API_DECL uint32_t graphics_scene_add_renderable(graphics_scene_t* scene, renderable_t renderable); 

#ifdef GRAPHICS_IMPL

#endif // GRAPHICS_IMPL
#endif // GRAPHICS_H
