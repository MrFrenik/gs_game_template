/*==============================================================================================================
    * Copyright: 2020 John Jackson 
    * Gunslinger Game Template
    * File: graphics.h
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

#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef struct quad_vert_t
{ 
    gs_vec2 position;          
    gs_vec2 uv;
    gs_color_t color;
} quad_vert_t; 

typedef struct quad_t
{ 
    gs_vec2 position;
    gs_vec2 dimensions;
    gs_vec4 uvs;
    gs_color_t color;
    float depth;
} quad_t;

int32_t quad_compare(const void* a, const void* b)
{
    quad_t* q0 = (quad_t*)a;
    quad_t* q1 = (quad_t*)b;
    if (q0->depth < q1->depth) return -1;
    if (q0->depth > q1->depth) return 1;
    return 0;
}

typedef struct quad_batch_t
{ 
    gs_dyn_array(quad_t) quads;// Array of quads
    gs_vbo vbo;                // Vertex buffer handle
    gs_ibo ibo;                // Index buffer handle
    uint32_t material_hndl;    // Reference to a material asset
    uint32_t count;            // Total index buffer count for upload 
} quad_batch_t; 

GS_API_DECL void quad_batch_init(quad_batch_t* qb, uint32_t mat_hndl); 
GS_API_DECL void quad_batch_begin(quad_batch_t* qb);
GS_API_DECL void quad_batch_end(quad_batch_t* qb, gs_command_buffer_t* cb);
GS_API_DECL void quad_batch_add(quad_batch_t* qb, const gs_vec2* position, const gs_vec2* dimensions, const gs_vec4* uvs, const gs_color_t* color, float depth);
GS_API_PRIVATE void quad_batch_sort(quad_batch_t* qb);

typedef struct renderable_base_t
{ 
    uint32_t hndl;                          // Slot array renderable id handle
    gs_mat4 model_matrix;                   // Model matrix
    gs_dyn_array(asset_handle_t) materials; // Per primitive materials
} renderable_base_t;

GS_API_DECL renderable_base_set_material(renderable_base_t* rend, asset_handle_t mat, uint32_t idx); 

typedef struct renderable_static_mesh_t
{
    base(renderable_base_t)

    asset_handle_t mesh;                    // Asset handle for static mesh

} renderable_static_mesh_t;

GS_API_DECL renderable_static_mesh_set_mesh(renderable_static_mesh_t* rend, asset_handle_t mesh); 

typedef struct renderable_skeletal_mesh_t
{
    base(renderable_base_t)

    asset_handle_t mesh;                    // Asset handle for skeletal mesh

} renderable_skeletal_mesh_t; 

GS_API_DECL void renderable_set_material_uniform(renderable_base_t* rend, const char* name, void* data);

// Need a way to add renderables to a scene
typedef struct graphics_scene_t 
{ 
	gs_slot_array(renderable_static_mesh_t) static_meshes;

} graphics_scene_t;

GS_API_DECL uint32_t graphics_scene_add_renderable_static_mesh(graphics_scene_t* scene, renderable_static_mesh_t renderable); 
GS_API_DECL renderable_static_mesh_t* graphics_scene_get_renderable_static_mesh(graphics_scene_t* scene, uint32_t hndl);

#ifdef GRAPHICS_IMPL

GS_API_DECL void quad_batch_init(quad_batch_t* qb, uint32_t mat_hndl) 
{
    // Construct vbo
    gs_graphics_vertex_buffer_desc_t vdesc = gs_default_val();
    qb->vbo = gs_graphics_vertex_buffer_create(&vdesc);

    // Construct ibo
    gs_graphics_index_buffer_desc_t idesc = gs_default_val();
    qb->ibo = gs_graphics_index_buffer_create(&idesc);

    // Set mat handle
    qb->material_hndl = mat_hndl;
}

GS_API_DECL void quad_batch_begin(quad_batch_t* qb)
{
    // Could add these asserts to some debug define 
    gs_assert(qb->material_hndl != 0);
    gs_assert(qb->vbo.id != 0);
    gs_assert(qb->ibo.id != 0);
    gs_dyn_array_clear(qb->quads);
}

GS_API_DECL void quad_batch_end(quad_batch_t* qb, gs_command_buffer_t* cb)
{ 
    gs_dyn_array(quad_vert_t) verts = {0};
    gs_dyn_array(uint32_t) indices = {0};

    // Push back all verts, store indices to verts for quads 
    // NOTE(john): This is ~4ms
    for (uint32_t i = 0; i < gs_dyn_array_size(qb->quads); ++i)
    {
        const quad_t* q = &qb->quads[i]; 

        float l = q->position.x;
        float b = q->position.y;
        float r = q->position.x + q->dimensions.x;
        float t = q->position.y + q->dimensions.y; 

        gs_vec2 lb = gs_v2(l, b);
        gs_vec2 lt = gs_v2(l, t);
        gs_vec2 rb = gs_v2(r, b);
        gs_vec2 rt = gs_v2(r, t);

        float u0 = q->uvs.x; 
        float v0 = q->uvs.y;
        float u1 = q->uvs.z;
        float v1 = q->uvs.w; 

        gs_vec2 u0v0 = gs_v2(u0, v0);
        gs_vec2 u1v0 = gs_v2(u1, v0);
        gs_vec2 u0v1 = gs_v2(u0, v1);
        gs_vec2 u1v1 = gs_v2(u1, v1);

        // Capture vert size
        uint32_t vs = gs_dyn_array_size(verts);

        // Construct verts
        quad_vert_t qlb = {0}; qlb.position = lb; qlb.uv = u0v0; qlb.color = q->color;
        quad_vert_t qlt = {0}; qlt.position = lt; qlt.uv = u0v1; qlt.color = q->color;
        quad_vert_t qrb = {0}; qrb.position = rb; qrb.uv = u1v0; qrb.color = q->color;
        quad_vert_t qrt = {0}; qrt.position = rt; qrt.uv = u1v1; qrt.color = q->color; 

        // Push back verts
        gs_dyn_array_push(verts, qlb);    // 0
        gs_dyn_array_push(verts, qlt);    // 1
        gs_dyn_array_push(verts, qrb);    // 2
        gs_dyn_array_push(verts, qrt);    // 3

        // First triangle
        gs_dyn_array_push(indices, vs + 0);   // lb
        gs_dyn_array_push(indices, vs + 3);   // rb
        gs_dyn_array_push(indices, vs + 2);   // lt

        // Second triangle
        gs_dyn_array_push(indices, vs + 0);   // rb
        gs_dyn_array_push(indices, vs + 1);   // rt
        gs_dyn_array_push(indices, vs + 3);   // lt
    } 

    // Buffer descriptors
    gs_graphics_vertex_buffer_desc_t vdesc = {0};
    vdesc.data = verts;
    vdesc.size = gs_dyn_array_size(verts) * sizeof(quad_vert_t);

    gs_graphics_vertex_buffer_desc_t idesc = {0};
    idesc.data = indices;
    idesc.size = gs_dyn_array_size(indices) * sizeof(uint32_t); 

    // Upload all data to graphics backend
    gs_graphics_vertex_buffer_request_update(cb, qb->vbo, &vdesc);
    gs_graphics_index_buffer_request_update(cb, qb->ibo, &idesc);

    // Set count
    qb->count = gs_dyn_array_size(indices); 

    // Free vertex/indices data (might hold these around later on)
    gs_dyn_array_free(verts);
    gs_dyn_array_free(indices);
} 

GS_API_DECL void quad_batch_add(quad_batch_t* qb, const gs_vec2* position, const gs_vec2* dimensions, const gs_vec4* uvs, const gs_color_t* color, float depth)
{ 
    // Make a quad out of this
    quad_t q = {0};
    q.position = *position;
    q.dimensions = *dimensions;
    q.uvs = *uvs;
    q.color = *color;
    q.depth = depth; 
    gs_dyn_array_push(qb->quads, q); 
} 

GS_API_PRIVATE void quad_batch_sort(quad_batch_t* qb)
{
    // Sort all of our vertex data by y
    qsort(qb->quads, gs_dyn_array_size(qb->quads), sizeof(quad_t), quad_compare); 
}

// ==================== [ Renderable Base ] =============== //

GS_API_DECL renderable_base_set_material(renderable_base_t* rend, asset_handle_t mat, uint32_t idx)
{
    // Check for availability (this should be based on setting the mesh)
    if (idx >= gs_dyn_array_size(rend->materials))
    {
        gs_dyn_array_reserve(rend->materials, idx + 1);
    }

    // Set material
    rend->materials[idx] = mat;
}

// ==================== [ Renderable Static Mesh ] =============== //

GS_API_DECL renderable_static_mesh_set_mesh(renderable_static_mesh_t* rend, asset_handle_t mesh)
{
    // Get mesh primitive count
    mesh_t* mp = asset_handle_get(&mesh);
    uint32_t cnt = gs_dyn_array_size(mp->mesh.primitives); 

    // Reserve material space if necessary
    renderable_base_t* base = cast(rend, renderable_base_t);
    if (gs_dyn_array_size(base->materials) < cnt) {
        gs_dyn_array_reserve(base->materials, cnt);
    }

    // Set mesh
    rend->mesh = mesh;
}

// ==================== [ Graphics Scene ] =============== //

GS_API_DECL uint32_t graphics_scene_add_renderable_static_mesh(graphics_scene_t* scene, renderable_static_mesh_t rend)
{
    const uint32_t hndl = gs_slot_array_insert(scene->static_meshes, rend);
    gs_slot_array_getp(scene->static_meshes, hndl)->_base.hndl = hndl;
    return hndl;
} 

GS_API_DECL renderable_static_mesh_t* graphics_scene_get_renderable_static_mesh(graphics_scene_t* scene, uint32_t hndl)
{
    return gs_slot_array_getp(scene->static_meshes, hndl);
}








#endif // GRAPHICS_IMPL
#endif // GRAPHICS_H
