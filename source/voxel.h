#ifndef VOXEL_H
#define VOXEL_H 

// Each voxel is 1"^3 

typedef uint32_t voxel_t;
typedef uint32_t svo_node_t; 

#define VOXEL_SPLIT_TOLERANCE_MIN 0.001f 
#define VOXEL_SPLIT_TOLERANCE_MAX 1.0f
#define SVO_NODE_DEPTH_MAX        7

typedef svo_node_block_t
{
    svo_node_t[8] nodes;
} svo_node_block_t;

/*
    svo_node_t: 
        - 32 bits total
        - 7 bits for depth (where 0 is parent)
        - 25 bits for child index (0x00 is BARREN - no children)
*/ 

introspect()
typedef enum voxel_op
{
    VOXEL_OP_UNION,
    VOXEL_OP_INTERSECTION,
    VOXEL_OP_SUBTRACTION 
} voxel_op;

#define voxel_idx(X, Y, Z, W, H, D)\
    (X + W * (Y + D * Z))

// Sparse Voxel Octree (single, indirect array)
introspect()
typedef struct svo_t
{
    base(object_t) 

    ctor(
        params(uint32_t width, uint32_t height, uint32_t depth), 
        func({
            this->width = width;
            this->height = height;
            this->depth = depth;

            // Call initialize for svo
            svo_init(this);
        })
    ) 

    dtor({
        gs_dyn_array_free(this->nodes);
        gs_dyn_array_free(this->voxels);
    }) 

    serialize({
        return svo_serialize(buffer, this);
    })

    deserialize({
        return svo_deserialize(buffer, this);
    })

    ignore() 
    gs_dyn_array(svo_node_t) nodes;     // Array of octree nodes     

    ignore()
    gs_dyn_array(uint32_t) free_list;   // Free list of child node blocks

    ignore() 
    gs_dyn_array(voxel_t) voxels;       // Actual voxel data

    ignore()
    gs_vbo vbo;

    ignore()
    gs_ibo ibo;

    uint32_t width; 
    uint32_t height; 
    uint32_t depth;       
    gs_vqs xform;

} svo_t; 

// SVO API
GS_API_DECL void svo_init(svo_t* svo);
GS_API_DECL gs_result svo_serialize(gs_byte_buffer_t* buffer, const svo_t* in);
GS_API_DECL gs_result svo_deserialize(gs_byte_buffer_t* buffer, svo_t* out); 
GS_API_DECL uint8_t svo_node_depth(const svo_node_t* node); 

// SDF API
GS_API_DECL float sdf_sphere(gs_vec3 p, float r); 
GS_API_DECL float sdf_cylinder(gs_vec3 p, float h, float r);

#ifdef VOXEL_IMPL 

typedef struct vertex_t
{
    gs_vec3 position;
    gs_vec3 normal;
    gs_vec3 color;
} vertex_t;

GS_API_DECL void svo_init(svo_t* svo)
{ 
    const uint32_t ct = svo->width * svo->height * svo->depth;
    const size_t sz = ct * sizeof(uint32_t);

    // Reserve all voxel space
    gs_dyn_array_reserve(svo->voxels, ct); 

    // Reserve all nodes
    gs_dyn_array_reserve(svo->nodes, ct); 

    // Memset all memory to filled
    memset(svo->voxels, 0x01, sz);
    memset(svo->nodes, 0x00, sz);

    // Need a vertex definition for everything
    gs_dyn_array(vertex_t) vertices = {0};
    gs_dyn_array(uint32_t) indices = {0};

    // Construct vbo 
    svo->vbo = gs_graphics_vertex_buffer_create(&(gs_graphics_vertex_buffer_desc_t){ 
        .data = NULL,
        .size = 0,
        .usage = GS_GRAPHICS_BUFFER_USAGE_DYNAMIC
    });

    // Construct ibo
    svo->ibo = gs_graphics_index_buffer_create(&(gs_graphics_index_buffer_desc_t){
        .data = NULL,
        .size = 0,
        .usage = GS_GRAPHICS_BUFFER_USAGE_DYNAMIC
    });

    svo->xform = gs_vqs_default();
} 

GS_API_DECL gs_result svo_serialize(gs_byte_buffer_t* buffer, const svo_t* in)
{
    return GS_RESULT_SUCCESS;
}

GS_API_DECL gs_result svo_deserialize(gs_byte_buffer_t* buffer, svo_t* out)
{
    return GS_RESULT_SUCCESS;
} 


/*
GS_API_DECL void svo_box(svo_t* svo, voxel_op op, gs_vec3 min, gs_vec3 max, const gs_vqs_t* vqs)
{ 
    // Figure out intersection points using octree to determine where sdf
    // affects voxels

    Boxes have sdfs, just apply this sdf with some kind of "op"  (union/intersection)
    to determine voxel occupancy
}
*/

GS_API_DECL sdf_union(float v0, float v1)
{
    return gs_min(v0, v1);
}

GS_API_DECL sdf_intersection(float v0, float v1)
{
    return gs_max(v0, v1);
}

GS_API_DECL sdf_subtraction(float v0, float v1)
{ 
    return gs_max(-v0, v1);
} 

GS_API_DECL sdf_sphere(gs_vec3 p, float r)
{ 
    return gs_vec3_len(p) - r;
}

GS_API_DECL float sdf_cylinder(gs_vec3 p, float h, float r)
{
  gs_vec2 d = gs_vec2_sub(gs_v2(gs_vec2_len(gs_v2(p.x, p.z)), p.y), gs_v2(h,r));
  d = gs_v2(fabsf(d.x), fabsf(d.y));
  return gs_min(gs_max(d.x,d.y), 0.0f) + gs_max(gs_vec2_len(d), 0.0f);
}

GS_API_DECL sdf_union_smooth(float v0, float v1, float k)
{
    float h = gs_clamp(0.5f + 0.5f * (v1 - v0) / k, 0.0f, 1.0f);
    return (gs_interp_linear(v1, -v0, h) + k * h * (1.0f - h));
}

GS_API_DECL sdf_intersection_smooth(float v0, float v1, float k)
{
    float h = gs_clamp(0.5f - 0.5f * (v1 - v0) / k, 0.0f, 1.0f);
    return (gs_interp_linear(v1, v0, h) + k * h * (1.0f - h));
}

GS_API_DECL sdf_subtraction_smooth(float v0, float v1, float k)
{
    float h = gs_clamp(0.5f - 0.5f * (v1 + v0) / k, 0.0f, 1.0f);
    return (gs_interp_linear(v1, -v0, h) + k * h * (1.0f - h));
}

GS_API_DECL uint8_t svo_node_depth(svo_node_t node)
{
    return ((uint32_t)node >> 25) & 1;
} 

GS_API_DECL void svo_sphere(svo_t* svo, const gs_sphere_t* s, const gs_vqs* xform, voxel_op op)
{
    // Sphere would be easiest to figure out at first, since the 
    // transform is of no consequence to the result

    // Need a world space aabb for svo

    const uint32_t w = svo->width;
    const uint32_t h = svo->height;
    const uint32_t d = svo->depth;

    gs_aabb_t aabb = (gs_aabb_t)
    {
        .min = gs_v3(-(float)w/2.f, -(float)h/2.f, -(float)d/2.f), 
        .max = gs_v3( (float)w/2.f, (float)h/2.f, (float)d/2.f)
    }; 

    // Figure out intersection against AABB of svo
    gs_contact_info_t res = {0};
    gs_aabb_vs_sphere(&aabb, &svo->xform, s, xform, &res);
    if (res.hit)
    { 
        // Compute occupancy based on sphere sdf op'd with current occupancy
        // (Currently doing this for all of the voxels, could find region instead) 

        const gs_vec3 sphere_wc = xform ? gs_vec3_add(xform->position, s->c) : s->c;
        const gs_vec3 svo_wc  = svo->xform.position;    // SVO center position in world space
        const uint32_t xmin = 0, xmax = w;
        const uint32_t ymin = 0, ymax = h;
        const uint32_t zmin = 0, zmax = d;

        // Iterate over cells, determine intersections with voxels and all that shiz
        // We're looking at cell centers here, not corners

        uint32_t sc = 0;
        for (uint32_t x = xmin; x < xmax; ++x)
            for (uint32_t y = ymin; y < ymax; ++y)
                for (uint32_t z = zmin; z < zmax; ++z)
        {
            // Check sdf against voxel
            gs_vec3 vp = gs_vec3_add(svo_wc, gs_v3((float)x, (float)y, (float)z));

            // Need to split if this voxel is between some distance and not passed 
            //  last depth value 
            // Each voxel unit is 1"^3, the minimum will be 1/128", 2^7 , 7 levels deep

            float v = sdf_sphere(gs_vec3_sub(vp, sphere_wc), s->r); 

            uint32_t vidx = voxel_idx(x, y, z, svo->width, svo->height, svo->depth); 
            const svo_node_t* node = &svo->nodes[vidx]; 
            const uint32_t node_depth = 0;  
            const float d = fabsf(s->r - v); 

            // If inside the volume or on surface
            if (v <= s->r)
            { 
                // Split
                if (
                    v >= VOXEL_SPLIT_TOLERANCE_MIN &&
                    v <= VOXEL_SPLIT_TOLERANCE_MAX &&
                    node_depth < SVO_NODE_DEPTH_MAX
                )
                { 
                }

                // Determine value that you need in order to split
                svo->voxels[vidx] = 0x00;
            } 
        } 

        // Regenerate mesh
    } 
}

// Split parent node at index
/*
GS_API_DECL void svo_split_node(svo_t* svo, uint32_t idx) 
{
}

GS_API_DECL void svo_cylinder(svo_t* svo, float r, const gs_vqs_t* vqs);

GS_API_DECL void svo_cone(svo_t* svo, float r, const gs_vqs_t* vqs); 
*/

#endif // VOXEL_IMPL 
#endif // VOXEL_H
































