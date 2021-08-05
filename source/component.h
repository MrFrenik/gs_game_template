#ifndef COMPONENT_H
#define COMPONENT_H

typedef struct component_base_t
{
    BASE(object_t);

    // Fields
} component_base_t;

typedef struct component_transform_t
{ 
    BASE(component_base_t);

    // Fields
    gs_vqs transform;

} component_transform_t;

typedef struct component_physics_t 
{
	BASE(component_base_t);

	// Fields
	gs_vec3 velocity;
	gs_vec3 acceleration;
	
} component_physics_t;

typedef struct component_renderable_t 
{ 
    BASE(component_base_t); 

    // Fields 
    uint32_t renderable_id;
} component_renderable_t;

#endif // COMPONENT_H
