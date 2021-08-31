#ifndef WORLD_H
#define WORLD_H


#define WORLD_VOXEL_CARTESIAN_WIDTH   32
#define WORLD_VOXEL_ISO_WIDTH         32
#define WORLD_VOXEL_ISO_HEIGHT        16  
#define WORLD_CHUNK_WIDTH             128
#define WORLD_CHUNK_HEIGHT            32    // Not sure about having infinite chunk height yet.
#define WORLD_CHUNK_DEPTH             128 
#define WORLD_REGION_WIDTH            1
#define WORLD_REGION_HEIGHT           1
#define WORLD_REGION_DEPTH            1

// Need quad trees for chunks

// This will change to above
#define VOXEL_SIZE 16.f         // Voxel size is half of the width of the isometric tile texture. I know. It's confusing.

// 16*16*1 chunks
// 32*32*32 voxels per chunk = 16*16*1*32*32*32 = ~8.4M voxels per region. 
// So there needs to be a faster way of handling the world generation. Can't reasonable loop through 8M voxels or store that data.
// Need to have data for the world about height values, which I can get at runtime.  

// Voxel
typedef uint8_t world_voxel_t; 

// Chunk
typedef struct world_chunk_t
{
    // Make this an RLE compressed interval tree that can be compressed/uncompressed for operations at runtime
    world_voxel_t voxels[WORLD_CHUNK_WIDTH * WORLD_CHUNK_HEIGHT * WORLD_CHUNK_DEPTH]; 
    gs_vec3 origin;
} world_chunk_t;

// Region 
typedef struct world_region_t
{
    gs_hash_table(uint64_t, world_chunk_t) chunks;  // Active world chunks (will be paged in)
    gs_vec3 origin;
} world_region_t;

// World
typedef struct world_t
{ 
    gs_hash_table(uint64_t, world_region_t) regions;    // Active world regions (will be paged in)
    uint32_t width;                                     // Number of regions in width
    uint32_t depth;                                     // Number of regions in depth
    uint32_t height;                                    // Number of regions in height
} world_t; 

gs_force_inline
gs_vec2 cart_to_iso(gs_vec2 cart)
{
    return gs_v2(cart.x - cart.y, (cart.x + cart.y) * 0.5f);
};

gs_force_inline
gs_vec2 iso_to_cart(gs_vec2 iso)
{
    return gs_v2((2.f * iso.y + iso.x) * 0.5f, (2.f * iso.y - iso.x) * 0.5f);
};

// Alright...now to actually generate something with a seed.
// Perfect is enemy of good. Get something going by end of day.

GS_API_DECL world_t world_generate(uint64_t seed, quad_batch_t* qb, gs_command_buffer_t* cb, float amplitude, float period);

/*
    Iterate through 2d mapping of world (which is ww * wh * rw * rd * cw * cd pixels) 
*/

#ifdef WORLD_IMPL 

GS_API_DECL world_t world_generate(uint64_t seed, quad_batch_t* qb, gs_command_buffer_t* cb, float amplitude, gs_vec2 period)
{
    // Generate new random seed
    gs_rand rand = gs_rand_seed(seed); 

    // Generate a single region for now 
    world_t world = {0};
    world.width  = 1;
    world.height = 1;
    world.depth  = 1; 

    // Want to alter a 2d map for height map information. No idea how to do this.
    // I don't need height information. That is implicit in the voxel itself, and there are no "in between" heights.  

    gs_vec2 iso_dims = cart_to_iso(gs_v2(VOXEL_SIZE, VOXEL_SIZE));

    // Heights for this
    const size_t sz = world.depth * world.width * (WORLD_CHUNK_WIDTH + 1) * (WORLD_CHUNK_DEPTH + 1) * (WORLD_CHUNK_HEIGHT + 1);
    int8_t* corners = gs_malloc(sz);
    memset(corners, 0, sz);

    // Perlin offset for map
    // gs_vec2 period  = gs_v2(gs_rand_gen_range(&rand, 1.f, 100.f), gs_rand_gen_range(&rand, 1.f, 100.f));
    // float amplitude = gs_rand_gen_range(&rand, 300.f, 1000.f);

    // Create height map for terrain height values (need to contour these values afterwards for shape, then can assign voxels)
    const gs_vec3 so = gs_v3((f32)WORLD_CHUNK_WIDTH / 2.f, (f32)WORLD_CHUNK_HEIGHT / 2.f, (f32)WORLD_CHUNK_DEPTH / 2.f);

	static float t = 0.f;
	static bool _paused = false;
	if (!_paused)
	{
		t += 0.001f;
	}
	if (gs_platform_key_pressed(GS_KEYCODE_P))
	{
		_paused = !_paused;
	}

    // Go through each corner for voxel values 
    float min = FLT_MAX;
    float max = FLT_MIN;
    for (uint32_t wz = 0; wz < world.depth; ++wz)
    {
        for (uint32_t wx = 0; wx < world.width; ++wx)
        { 
            for (int32_t cx = 0; cx < WORLD_CHUNK_WIDTH + 1; ++cx)
            {
                for (uint32_t cy = 0; cy < WORLD_CHUNK_HEIGHT + 1; ++cy)
                {
                    for (uint32_t cz = 0; cz < WORLD_CHUNK_DEPTH + 1; ++cz)
                    { 
                        // Chunk origin // (0, 0, 0) for now
                        gs_vec3 origin = gs_v3(0, 0, 0); 

                        // Voxels
                        uint32_t x = origin.x + cx;
                        uint32_t z = origin.z + cz; 
                        uint32_t y = origin.y + cy;

                        float vy = VOXEL_SIZE * y;

                        const uint32_t idx = x + y * (WORLD_CHUNK_WIDTH + 1) + z * (WORLD_CHUNK_DEPTH + 1) * (WORLD_CHUNK_HEIGHT + 1); 
                        float n = amplitude * ((gs_perlin2((x + period.x) / 16.f, (z + period.y) / 16.f))); 

                        // Get difference between n and vy
                        const float d = vy - n; 

                        const float r = 24 * (sin(t) * 0.5f + 0.8f);

                        // Try a sphere?
                        const float vx = x * VOXEL_SIZE;
                        const float vz = z * VOXEL_SIZE;
                        const float sd = gs_vec3_len(gs_vec3_sub(so, gs_v3(x, y, z))); 

                        // If valid, then we'll be on, otherwise off.
                        // Need to map valid corner cases to indices for UVs.
                        if (r - sd >= 0.f || d <= 0.5f)
                        //if (d <= 0.5f)
                        { 
                            corners[idx] = 1;
                        } 

                        if (y == 0 || y == 1) 
                        {
                            corners[idx] = 1;
                        }
                    }
                }
            }
        }
    } 

#define SW  224
#define SH  64

#define GET_IDX(X, Y, Z)\
    ((X) + (Y) * (WORLD_CHUNK_WIDTH + 1) + (Z) * (WORLD_CHUNK_DEPTH + 1) * (WORLD_CHUNK_HEIGHT + 1))

    static bool b = false;
    if (gs_platform_key_pressed(GS_KEYCODE_B)) b = !b;

    const uint32_t sw = 416;
    const uint32_t sh = 96;
    const uint32_t tw = 32;
    const uint32_t th = 32;

    float maxd = FLT_MIN;
    // Mesh generation
    quad_batch_begin(qb); 
    {
        // Eventually will need chunk origin
        gs_vec3 co = gs_v3(0 * WORLD_CHUNK_WIDTH, 0 * WORLD_CHUNK_HEIGHT, 0 * WORLD_CHUNK_DEPTH);

        static bool do_it = true;
        if (gs_platform_key_pressed(GS_KEYCODE_P)) do_it = !do_it;

        // Iterate through voxels for contouring (look at individual corners for each voxel)
        for (uint32_t cx = 0; cx < WORLD_CHUNK_WIDTH; ++cx)
        {
            for (uint32_t cy = 0; cy < WORLD_CHUNK_HEIGHT; ++cy)
            {
                for (uint32_t cz = 0; cz < WORLD_CHUNK_DEPTH; ++cz)
                { 
                    uint32_t x = co.x + cx;
                    uint32_t z = co.z + cz;
                    uint32_t y = co.y + cy; 

                    // Get 8 corners to get index for correct tile 
                    uint8_t cube = 0x00;
                    int8_t vc[8] = {
                        corners[GET_IDX(x, y, z)], 
                        corners[GET_IDX(x + 1, y, z)],
                        corners[GET_IDX(x + 1, y, z + 1)],
                        corners[GET_IDX(x, y, z + 1)],

                        corners[GET_IDX(x, y + 1, z)], 
                        corners[GET_IDX(x + 1, y + 1, z)],
                        corners[GET_IDX(x + 1, y + 1, z + 1)],
                        corners[GET_IDX(x, y + 1, z + 1)] 
                    }; 

                    if (vc[0]) cube |= 1; 
                    if (vc[1]) cube |= 2; 
                    if (vc[2]) cube |= 4; 
                    if (vc[3]) cube |= 8; 
                    if (vc[4]) cube |= 16;
                    if (vc[5]) cube |= 32;
                    if (vc[6]) cube |= 64;
                    if (vc[7]) cube |= 128;
                    if (!cube) continue; 

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c %c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#define get_uvs(UVS, C, R, TW, TH, W, H)\
    do {\
        (UVS) = (gs_vec4) {\
            ((f32)TW * (f32)C) / (f32)W,\
            ((f32)TH * (f32)R) / (f32)H,\
            ((f32)TW * (f32)(C + 1)) / (f32)W,\
            ((f32)TH * (f32)(R + 1)) / (f32)H\
        };\
    } while (0)
                    // Determine uvs based on case 
                    gs_vec4 uvs = gs_v4s(-1.f); 

                    // Check surrounding voxels to determine whether or not filled before adding quad to 
                    // prevent overdraw.  
                    if (y < WORLD_CHUNK_HEIGHT - 2 && z < WORLD_CHUNK_DEPTH - 1 && x < WORLD_CHUNK_WIDTH - 1)
                    {
                        if (
                            corners[GET_IDX(x, y + 2, z)]         && 
                            corners[GET_IDX(x, y + 2, z + 1)]     &&
                            corners[GET_IDX(x + 1, y + 2, z)]     && 
                            corners[GET_IDX(x + 1, y + 2, z + 1)] &&
                            corners[GET_IDX(x + 1, y + 2, z + 2)] && 
                            corners[GET_IDX(x + 1, y + 1, z + 2)] && 
                            corners[GET_IDX(x + 2, y + 1, z)]     &&      
                            corners[GET_IDX(x + 2, y + 1, z + 1)] &&     
                            corners[GET_IDX(x + 2, y + 1, z + 2)]    
                        )
                        {
                            continue;
                        }
                    }

                    switch (cube)
                    {
                        default: 
                        {
                            continue;
                        } break;

                        case 27:   get_uvs(uvs, 9, 0, tw, th, sw, sh);  break; // 0x00011011
                        case 31:   get_uvs(uvs, 7, 1, tw, th, sw, sh);  break; // 0x00011111
                        case 39:   get_uvs(uvs, 10, 0, tw, th, sw, sh); break; // 0x00100111
                        case 47:   get_uvs(uvs, 2, 1, tw, th, sw, sh);  break; // 0x00101111
                        case 55:   get_uvs(uvs, 3, 2, tw, th, sw, sh);  break; // 0x00110111
                        case 59:   get_uvs(uvs, 1, 2, tw, th, sw, sh);  break; // 0x00111011
                        case 63:   get_uvs(uvs, 2, 0, tw, th, sw, sh);  break; // 0x00111111
                        case 79:   get_uvs(uvs, 13, 0, tw, th, sw, sh); break; // 0x01001111
                        case 95:   get_uvs(uvs, 10, 1, tw, th, sw, sh); break; // 0x01011111
                        case 111:  get_uvs(uvs, 4, 0, tw, th, sw, sh);  break; // 0x01101111
                        case 119:  get_uvs(uvs, 4, 1, tw, th, sw, sh);  break; // 0x01110111
                        case 127:  get_uvs(uvs, 7, 0, tw, th, sw, sh);  break; // 0x01111111
                        case 141:  get_uvs(uvs, 11, 0, tw, th, sw, sh); break; // 0x10001101
                        case 143:  get_uvs(uvs, 9, 1, tw, th, sw, sh);  break; // 0x10001111
						case 155:  get_uvs(uvs, 1, 2, tw, th, sw, sh);  float tmp = uvs.z; uvs.z = uvs.x; uvs.x = tmp; break; // 0x10011011
						case 157:  get_uvs(uvs, 0, 2, tw, th, sw, sh);	break; // 0x10011101
                        case 159:  get_uvs(uvs, 1, 0, tw, th, sw, sh);  break; // 0x10011111
                        case 175:  get_uvs(uvs, 12, 1, tw, th, sw, sh); break; // 0x10101111
                        case 187:  get_uvs(uvs, 3, 1, tw, th, sw, sh);  break; // 0x10111011
                        case 191:  get_uvs(uvs, 6, 0, tw, th, sw, sh);  break; // 0x10111111
                        //case 102:  get_uvs(uvs, 4, 0, tw, th, sw, sh);  break; // 0x01101110
                        //case 103:  get_uvs(uvs, 4, 0, tw, th, sw, sh);  break; // 0x01101111
                        case 207:  get_uvs(uvs, 3, 0, tw, th, sw, sh);  break; // 0x11001111
                        case 221:  get_uvs(uvs, 5, 1, tw, th, sw, sh);  break; // 0x11011101
                        case 223:  get_uvs(uvs, 5, 0, tw, th, sw, sh);  break; // 0x11011111
                        case 238:  get_uvs(uvs, 8, 0, tw, th, sw, sh);  break; // 0x11101110 
                        case 239:  get_uvs(uvs, 8, 0, tw, th, sw, sh);  break; // 0x11101111 
                        case 255:  get_uvs(uvs, 0, 0, tw, th, sw, sh);  break; // 0x11111111
                    } 

                    // Cartesian coordinates
                    const float crt_x = (float)x * VOXEL_SIZE;
                    const float crt_z = (float)z * VOXEL_SIZE;
                    const float crt_y = (float)y * VOXEL_SIZE;

                    // Iso coordinates
                    const float ix = (crt_x - crt_z);
                    const float iz = (crt_z + crt_x) * 0.5f - crt_y;

                    // Isometric depth
                    const float id = iz + crt_y * 2.f; 

                    // Color
                    gs_color_t c = GS_COLOR_WHITE; 

                    // Want to darken their color by their depth 
                    uint8_t v = (uint8_t)gs_map_range(0.f, (f32)WORLD_CHUNK_HEIGHT, 200.f, 255.f, (f32)y); 
                    c.r = v;
                    c.g = v;
                    c.b = v; 

                    float f = ((f32)y / (f32)WORLD_CHUNK_HEIGHT);
                    if (f < 1.f / 6.f)      c = gs_color(212, 224, 155, 255);
                    else if (f < 2.f / 6.f) c = gs_color(246, 244, 210, 255);
                    else if (f < 3.f / 6.f) c = gs_color(203, 223, 189, 255);
                    else if (f < 4.f / 6.f) c = gs_color(241, 156, 121, 255);
                    else                    c = gs_color(164, 74, 63, 255);

                    gs_vec4* uv = &uvs; 

                    // Add a quad batch, bitches
                    quad_batch_add(qb, &(gs_vec2){ix, iz}, &(gs_vec2){32.f, 32.f}, uv, &c, id);
                }
            }
        } 

    }
    quad_batch_end(qb, cb); 

    gs_free(corners);

    return world;
}

#endif // WORLD_IMPL
#endif // WORLD_H













