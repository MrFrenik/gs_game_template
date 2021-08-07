#ifndef COMPONENT_H
#define COMPONENT_H

introspect()
typedef struct component_base_t
{
    base(object_t)

    // Fields
} component_base_t;

introspect()
typedef struct component_transform_t
{ 
    base(component_base_t)

    // Fields
    gs_vqs transform;

} component_transform_t;

introspect()
typedef struct component_physics_t 
{
	base(component_base_t)

	// Fields
	gs_vec3 velocity;
	gs_vec3 acceleration;
	
} component_physics_t;

introspect()
typedef struct component_renderable_t 
{ 
    base(component_base_t)

    // Fields 
    uint32_t renderable_id;
} component_renderable_t;

#endif // COMPONENT_H
