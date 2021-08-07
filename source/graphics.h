#ifndef GRAPHICS_H
#define GRAPHICS_H

introspect()
typedef struct renderable_t
{
	base(object_t)

	// Fields
	gs_gfxt_renderable_t data;
	uint32_t mesh_hndl;
	uint32_t material_hndl;

} renderable_t;

GS_API_DECL void renderable_set_material_uniform(renderable_t* rend, const char* name, void* data);

// Need a way to add renderables to a scene
typedef struct graphics_scene_t 
{
	base(object_t)

	// Fields
	gs_slot_array(renderable_t) renderables;

} graphics_scene_t;

// Need a way to sort renderables by material/pipeline
GS_API_DECL uint32_t graphics_scene_add_renderable(graphics_scene_t* scene, renderable_t renderable); 

#ifdef GRAPHICS_IMPL

#endif // GRAPHICS_IMPL
#endif // GRAPHICS_H
