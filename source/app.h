
#ifndef APP_H
#define APP_H

/* 
 *  Main application interface
 */ 

/* 
   // Each voxel is 1x1x1 units in grid space (in pixel space, these are isometric tiles of 32x16, in cartesian space they're 32x32 pixels); 

    #define WORLD_VOXEL_CARTESIAN_WIDTH   32
    #define WORLD_VOXEL_ISO_WIDTH         32
    #define WORLD_VOXEL_ISO_HEIGHT        16  
    #define WORLD_CHUNK_WIDTH             32
    #define WORLD_CHUNK_HEIGHT            32      // This will mean that each region will need a cubic series of chunks as well (or the region just corresponds to a point in space with currently generated chunks)
    #define WORLD_CHUNK_DEPTH             32

    // A region holds a group of chunks (I think I'll have a limit to height within a world chunk, very similar to minecraft in that regard)...unless infinite height can be a thing? Seems ridiculous, but might be ridiculous in a good way.
    // A chunk holds a group of voxels

    typedef enum voxel_type_t
    {
        VOXEL_TYPE_AIR = 0x00, 
        VOXEL_TYPE_GROUND = 0x01  
    } voxel_type_t;

    // Voxel
    typedef uint8_t world_voxel_t;
    
    // Chunk
    typedef struct world_chunk_t
    { 
        // To make this more efficient, memory wise, should implement an RLE interval tree for the chunk data
        world_voxel_t voxels[WORLD_CHUNK_WIDTH * WORLD_CHUNK_HEIGHT * WORLD_CHUNK_DEPTH]; // At 32 voxels per dimension, each chunk = 32^3 = 32.7k voxels      
    } world_chunk_t; 

    // Region
    typedef struct world_region_t
    { 
        // Hash id of world position to chunk (use fast, custom hash for this, probably bound the entire world to be positive positions only)
        gs_hash_table(uint64_t, world_chunk_t) chunks;  // Active world chunks (will be paged in)
    } world_region_t;

    // The world is comprised of regions
    // There could potentially be entire sections of a region that is empty space, so need to account for that somehow to save memory 
    typedef struct world_t
    {
        gs_hash_table(uint64_t, world_region_t) regions;    // Active world regions (will be paged in)
    } world_t;


Solomon: 
    - Overview: Diablo + Dwarf Fortress + Starcraft + Skyrim
    - Set in a post-apoc time, where advanced technology exists but is treated differently by various groups
        (think Warhammer or Dune), where the corrupted King Solomon rules the land and controls demons, beasts, 
        groups to do his bidding. His demonic ring gives him ultimate power.
    - Magic, technology, demons, spiritual forces, fantastical elements all exist in this world
    - Art: 2.5d, isometric, hand drawn. Gorey but stylistic.
    - Terrain: 
        - Procedurally generated worlds, consisting of biomes, caves, oceans, rivers, lakes, forests, mountains, 
            volcanoes, swamps
        - Player creates a world to live in and selects various traits for that world, including what entity types are allowed, 
            what factions can exists, the atmosphere for the world, the closeness to the sun, etc. 
    - Town-Building: 
        - Like DF, player will be responsible for managing a starting town. This town will have inhabitants with their own 
            traits, skills, desires, and abilities. The town will grow, the player will assign tasks and build the town from 
            scratch. 
        - As your town grows, it will attract new inhabitants. 
        - Towns-people will have relationships with each other, will build heirarchies (such as electing a mayor), and will 
            try to maintain whatever order they can. The player can influence this and give orders, but certain things 
            will be out of the player's control and left up to how well the town is managed.  
    - Roguelike: 
        - Adventure and exploration of the overworld is a large part of Solomon. The player must venture out into the world
            to secure new resources, upgrades, technology, skills, equipment for the town as well as partaking in randomly 
            generated side quests. All quests feed into the final story and goal of defeating Solomon. Defeating Solomon 
            will be a difficult almost impossible task. Most will fail. 
        - Players start with a 'hero', which has heightened attributes, traits, and abilities as well as better starting 
            equipment.
        - Players can take their hero (or multiple heroes) into overworld to explore caves, dungeons, raid other towns, 
            and recruit others into their party for play or join their town.
        - While the heroes are in the overworld, their towns are still being built.
    - Building: 
        - The entire world can be deconstructed and built to suit the player and other AI living in it. This means you 
            can build your town in a large, overgrown tree in a forest or in the side of a mountain. You could have a
            civilization build an underwater city that can only be accessed via a secret tunnel through an underground
            cave.
    - Meta game: 
        - Like other modern roguelikes, accessing new features/upgrades is a feature for each run to entice replayability, so   
            even complete failure reaps rewards.
    - Overworld: 
        - The overworld is dangerous. Full of monsters, raiders, dungeons, natural wonders and disasters. Players can 
            teleport back to their town at any time, assuming they have the correct equipment (either magic scroll
                or spell).
        - To save on processing, there are certain areas that will maintain some persistence, but not as much as 
            a designated town. Towns can be set up in dangerous areas (so evil towns can exist), but the overworld 
            with certain beasts will have a paired down AI to keep processing low. These beasts will also be 
            procedurally generated and placed, so they can be kept out of main memory as much as possible. (Only 
            proximity will keep them alive). 
    - Time: 
        - Like other colony-building games, time can be paused or sped up to control building. In Solomon, this is also 
            available, however time does not flow evenly across the entire world. Therefore the overworld will 
            behave differently from player controlled zones. In a player controlled world, the player is able to 
            alter time at will with no cost. However in the overworld, the player can only pause time for a certain amount 
            of time before this must cool down.
        - Time dilation isn't for free. Since time behaves differently between the overworld and your towns, there are 
            certain considerations to be made: 
                * Heroes can have relationships with towns people. If they age at different rates, this means those 
                    relationships could be affected and negatively affect the hero or town. 
    - Multiplayer: 
        - Players can play with others, either as a team in PvE managing the same colony or their own 
            or in PvP managing competing towns.  
        - Each player starts with a hero or can promote an existing town member to a hero. 
        - Each player is able to dilate time in his own town without affecting other towns. Players can pause time in the 
            overworld for a set amount of time before this feature needs to cool down.
        - If a PvP encounter occurs within a player's town boundary, then time acts similar to the overworld. This prevents
            either player from being able to pause time indefinitely.
    - Entities: 
        - All creatures, whether they be beasts, towns people, regardless of race, will be an AI "entity" in Solomon. These 
            entities all have various components that control how they behave: 
                * Traits/Attributes: 
                    - Attributes, such as whether or not an entity fears easily, or how much physical strength they have, 
                        which feeds into their well-being and how well they can perform certain actions. For instance, 
                        an entity with a resistance to poison might not get drunk easily.  
                * Skills: 
                * Physical Properties: 
                    - Describes the physical makeup of an entity, such as its skeletal system, what limbs it has, how many eyes, 
                        what its skin is made of (if applicable).
                * Equipment: 
                    - Descibes what equipment this entity has own its person at a time. This includes all physical items that 
                        can be removed from it. For instance, a live rabbit might not have any equipment, but a rabbit's corpse
                        will have fur, legs, arms, heart, liver, etc that can be taken.
                * Actions: 
                    - Actions are context specfic that generally take the form of entity -> entity, however they can also 
                        take the form of entity -> world, world -> entity, player -> entity as well. For instance, to 
                        "use" a firearm on an entity might have several subactions, such as striking with butt of the gun
                        or firing at it. These actions could also apply to the world as well, such as firing a bullet into 
                        the side of a mountain to cause a loud sound effect for a diversion.
    - Story Arc: 
        - To bind the stories together, the main objective will remain the same: Kill Solmon to bring peace to the world. 
            However, depending on the world generated and play style preferred, this could either be quick or slow. 
            Even as the player does things in the background to avoid the main story line, this will always be in action
            and will change as time progresses. 
        - The main quest line will have the typical elements that any would: Three acts, a completion arc, with 
            various required completed sub-quests before completing the final story. 
        - Due to the persistent nature of the entire world, sub-quests could be completed without the player's knowing. 
            For instance, if a sub-quest is to kill a particular entity and retrieve a key, if that entity is then 
            assassinated by a jaded lover, then the story will patch itself so that the quest is updated for the retrieval 
            of the key. The story can make certain items/entities necessary and indestructive so a viable quest path can always 
            exist.
    - Armies: 
        - Erect and command entire armies from your towns to raid other cities and towns to capture them.
    - Quests: 
        - Side quests are important to feel immersed in a world, and this will be no different in Solmon. As you progress
            through the game, venturing to other cities, towns, continents, to explore and gather resources or raid whole
            towns to take them over, you will encounter other entities that want you to do things for them. These 
            quests will be entirely dependent on the procedural generation for that world and will have sub-quests
            that each take part in their own three-act structure. These side quests could either be connected to the 
            larger story or could entirely be separate. 
    - Boss Fights: 
        - Certain entities will be designated as "bosses", which will be end of certain quest arcs, typically more challenging, 
            and are either entirely unique instances or special versions of entities with heightened abilities.
    - Events: 
        - All events are randomly generated but are controlled by various factors, such as how developed the town is, how 
            much time a town has been alive (not in terms of overworld but in terms of the town's time). These could include 
            natural disasters, raids by other AI or players, demonic possessions, corruption, political upheavals, etc.  


    What is the end game? I like the idea of having to kill Solomon, but what happens after that? Can you continue to play
    that world? Does it wrap difficulty like in Diablo? Does your hero now become the ring holder to be destroyed?
    Could have to beat the game at certain map sizes to unlock different kinds of available content for the game.  
    Killing Solomon unlocks the ability to travel to other areas, worlds? Possibly unlocks a gateway to hell itself
    to fight Satan? New bosses unlock? New monsters spawn? The difficulty of the world increases? 
    
    The town and your heroes/townspeople are analogous to Diablo's people. Is there a way to bring in your town or 
    heroes to another player's game? Perhaps a single hero but not the town? What happens if this hero dies in 
    another player's game? Is that persistent across games? 

    Should there be permanent town death? Or an option to continue your town from a previous save if you want? 
    This might cheapen the intensity of the game, but there could always be a "hardcore" version to disallow 
    saves and encourage challenge. There could even be specific unlocks that occur only from the hardcore game mode.  

    Progression: 

        Early Game: 
            Create world 
            Create starting townspeople 
            Create starting hero 
            Build town 
            Send hero to overworld 
            Gather resources, complete quests, kill bosses, raid towns to capture areas

        Mid Game: 
            Manage any towns under your control
            Continue to upgrade hero/s, create stronger parties of heros (up to 4? at a time) 
            Fight harder in game bosses/areas for better resources
            Stave off any outside/internal attacks and events that could destroy your civ
            Continue any side quests
            Continue main story line quests

        End Game: 
            Complete main story line quests needed to kill Solomon
            Kill Solomon to gain control over the ring (or not?)

        Meta Game: 
            After beating Solomon in your world, the meta game begins, opening up new challenges 
            Portal to a Hell opens in the world, new demons introduced
            Travel to Hell to challenge Satan himself
            Control demons to build, as Solomon did himself, now that you have control over the ring 
            Harder, more challenging quests open up, more unique loot


        How does permanent death play into this? How disheartening is it to lose all of your progress? 
        Can you continue to play in the world? Can you continue with your hero?  

    
 */


 /*
  *     Use an entity component system for all of this? 
  *
  *     Entity -> Traits, Abilities, Skills, Physical Characteristics, Equipment, Actions
  *
  *     Need a way to make large battles SCALE without destroying someone's computer.
  *
  *     How can I set this up to be scalable and moddable? How can I design new traits/abilities/actions 
  *
  *     Example: 
  *     Action:
  *         Fireball (a spell)
  *             - Spawn an entity at a given position with a given velocity (transform/physics/animation components) 
  *             - Entity has certain physical properties that determine it to be a "fireball"
  *
  *     Could add each trait/skill/characteristic/action as a component?
  *
  *     Fireball: 
  *         Transform Component
  *         Physics Kinematic Component
  *         2D Animation Component (this won't have a "renderable", since it will all be quad batched)
  *
  *         Not sure about these (All entities could have these): 
  *             Physical Characteristics Component      // List of physical characteristics
  *             Traits Component                        // List of traits
  *             Skills Component                        // List of skills
  *             Actions Component                       // List of possible actions
  *             Equipment Component                     // List of all equipment
  *             Memory Component  
  *
  *             Certain entities might have certain components and might not.
  *
  *    Action: ingest - what does it mean to actually DO an action? Ingesting some kind of item should do various things,
  *    depending on the material makeup of that item.
  *
  *     // Maybe make an entity component system for ingestion?
  *     void Ingest(entity ent, entity ent)
  *     {
  *         // Get physical properties of consuming entity, then do various checks.
  *     }
  *
  *     // Then action of "ingesting" something gives you an "ingesting" component to put you in that system?
  *     // Seems really over-complicated.
  *
  *     What can I work on in the meantime? 
  *
  *     - World generation: 
  *         Regions -> Chunks -> Voxels
  *         Biomes
  *         Rivers
  *         Volcanos
  *         Oceans
  *         Forests 
  *
  *         When zoomed out of a region, that region is updating at an LOD resolution scale: 
  *             - For instance, just like in reality, you don't know the specific events down to a granular level
  *                 and how it affects each and every person, what happens to their lives, etc.
  *                 You only know of large events and how they affect the region at a meta level.
  *             - When a player begins to actively invest in a region, they gain connection to it and 
  *                 therefore events will become more actively updated.
  *             - Regions the player has full control over will be actively updated at a full granular level.
  *         
  *         Need to generate a map of continents with biomes. Again, NO idea how to do this. Want seeds for various 
  *         things, such as biome generation, temperature, etc. 
  *
  *         Should I use 3D perlin noise to do terrain? Or 2D perlin then go through and add caves after the fact?
  *         Generate terrain with perlin/simplex noise.
  */

gs_force_inline
void fb_cb(void* win, int32_t width, int32_t height)
{
    gs_println("w: %d, h: %d", width, height);
}

const char* qb_v_src = 
"#version 330 core\n"
"precision mediump float;\n" 
"layout(location = 0) in vec2 a_position;\n"
"layout(location = 1) in vec2 a_uv;\n"
"layout(location = 2) in vec4 a_color;\n"
"uniform mat4 u_mvp;\n"
"out vec2 uv;\n"
"out vec4 color;\n"
"void main() {\n"
"  gl_Position = u_mvp * vec4(a_position, 0.0, 1.0);\n"
"  uv = a_uv;\n"
"  color = a_color;\n"
"}\n";

const char* qb_f_src =
"#version 330 core\n"
"precision mediump float;\n"
"in vec2 uv;\n"
"in vec4 color;\n"
"uniform sampler2D u_tex;\n"
"out vec4 frag_color;\n"
"void main() {\n"
"  frag_color = color * texture(u_tex, uv);\n"
"}\n";

pipeline_t g_pip;
material_t g_mat;
quad_batch_t g_qb;
quad_batch_t g_qb2;
quad_batch_t g_qb3;

typedef struct physics_component_t
{
    gs_vec3 velocity;
    gs_vec3 angular_velocity;
    float speed;
} physics_component_t;

static float bg[3] = {10, 10, 10};

// App struct declaration 
typedef struct app_t
{
    // Fields
	core_t* core; 

    gs_framebuffer      fbo;
    gs_renderpass       rp;
    gs_texture          rt;
    gs_gfxt_pipeline_t  bb_pip;     // Pipeline object for back buffer
    gs_gfxt_material_t  bb_mat;     // Material for back buffer
    gs_gfxt_mesh_t      fs_quad;    // Full screen unit quad
    gs_camera_t         camera;
    gs_texture          tex;
    gs_asset_texture_t  water_tex;
    gs_vbo              vbo;
    gs_ibo              ibo;
    uint32_t            mesh_count;
	gs_gfxt_mesh_raw_data_t* meshes; 
    gs_asset_texture_t  ship_tex;
    gs_vec3             palettes[3];
    gs_vec2             ranges;
    gs_rand             rand;
    physics_component_t physics;
    gs_asset_texture_t  pix_tex;
    float               amplitude;
    float               period;
    gs_vqs xform;
} app_t; 

// Main application interface functions
GS_API_DECL gs_app_desc_t app_main(int32_t argc, char** argv);
GS_API_DECL void app_init();
GS_API_DECL void app_update();
GS_API_DECL void app_shutdown(); 
GS_API_DECL void app_draw_mesh(const gs_vqs* vqs);
GS_API_DECL void app_update_transform(physics_component_t* physics, gs_vqs* xform);

#ifdef APP_IMPL 

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define RT_WIDTH  WINDOW_WIDTH  
#define RT_HEIGHT WINDOW_HEIGHT

// Shaders
#if (defined GS_PLATFORM_WEB || defined GS_PLATFORM_ANDROID)
    #define GS_GL_VERSION_STR "#version 300 es\n"
#else
    #define GS_GL_VERSION_STR "#version 330 core\n"
#endif

const char* bb_v_src =
GS_GL_VERSION_STR
"precision mediump float;\n"
"layout(location = 0) in vec2 a_pos;\n"
"layout(location = 1) in vec2 a_uv;\n"
"uniform vec2 u_tex_size;\n"
"out vec2 uv;\n"
"out vec2 tex_size;\n" 
"void main() {\n"
"  gl_Position = vec4(a_pos, 0.0, 1.0);\n"
"  uv = a_uv * u_tex_size;\n"
"  tex_size = u_tex_size;\n"
"}\n";

const char* bb_f_src =
GS_GL_VERSION_STR
"precision mediump float;\n"
"in vec2 uv;\n"
"in vec2 tex_size;\n"
"uniform sampler2D u_tex;\n"
"uniform vec2 u_screen_size;\n"
"uniform vec3 u_palette0;\n"
"uniform vec3 u_palette1;\n"
"uniform vec3 u_palette2;\n"
"uniform vec2 u_ranges;\n"
"out vec4 frag_color;\n"
"\n"
"float luminance(vec3 rgb)\n" 
"{\n"
"   vec3 w = vec3(0.2125, 0.7154, 0.0721);\n"
"   return dot(rgb, w);\n"
"}\n"
"\n"
"void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord)\n"
"{\n"
"   float w = 1.0 / tex_size.x;\n"
"   float h = 1.0 / tex_size.y;\n"
"   n[0] = texture(u_tex, coord + vec2( -w, -h));\n"
"   n[1] = texture(u_tex, coord + vec2(0.0, -h));\n"
"   n[2] = texture(u_tex, coord + vec2(  w, -h));\n"
"   n[3] = texture(u_tex, coord + vec2( -w, 0.0));\n"
"   n[4] = texture(u_tex, coord + vec2(0.0, 0.0));\n"
"   n[5] = texture(u_tex, coord + vec2(  w, 0.0));\n"
"   n[6] = texture(u_tex, coord + vec2( -w, h));\n"
"   n[7] = texture(u_tex, coord + vec2(0.0, h));\n"
"   n[8] = texture(u_tex, coord + vec2(  w, h));\n"
"}\n"
"\n"
"void main() {\n"
"  float tex_per_pix = (tex_size.x) / u_screen_size.x;\n" 
"  vec2 loc_within_texel = fract(uv);\n" 
"  vec2 lerp_amount = clamp(loc_within_texel / tex_per_pix, 0.0, 0.5);\n" 
"  lerp_amount = lerp_amount + clamp((loc_within_texel - 1) / tex_per_pix + 0.5, 0.0, 0.5);\n" 
"  vec2 final_uv = (floor(uv) + lerp_amount) / tex_size;\n" 
"  vec4 n[9];\n" 
"  make_kernel(n, u_tex, final_uv);\n" 
"  vec4 se_h = n[2] + (2.0 * n[5]) + n[8] - (n[0] + (2.0 * n[3]) + n[6]);\n" 
"  vec4 se_v = n[0] + (2.0 * n[1]) + n[2] - (n[6] + (2.0 * n[7]) + n[8]);\n" 
"  vec4 sobel = sqrt((se_h * se_h) + (se_v * se_v));\n" 
"  frag_color = texture(u_tex, final_uv);\n"  // Will have to upsample this 
"  float a = frag_color.a;\n" 
"  float lum = luminance(frag_color.rgb);\n" 
"  if (lum < u_ranges.x) frag_color.rgb = u_palette0;\n" 
"  else if (lum < u_ranges.y) frag_color.rgb = u_palette1;\n" 
"  else frag_color.rgb = u_palette2;\n" 
"  frag_color.a = a;\n" 
"  frag_color.rgb = frag_color.rgb * vec3(1.0 - sobel.rgb);\n" 
"  frag_color = texture(u_tex, final_uv);\n"  // Will have to upsample this 
"}\n";

// Need a few things: 
    // offscreen pipeline for rendering simple objects (could honestly just use GSI for rendering a cube...)
    // onscreen pipeline for rendering default quad texture to screen 
    //

GS_API_DECL void app_init()
{
    for (int32_t b = 0; b < 256; ++b)
    {
        gs_println(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(b));
    }

    app_t* app = gs_engine_user_data(app_t); 

	// Initialize core
	app->core = core_new(); 

    // Seed rand
    app->rand = gs_rand_seed(gs_hash_str64("Gunslinger"));

    gs_platform_set_framebuffer_resize_callback(gs_platform_main_window(), fb_cb);

	// I want position, uv, color
	gs_gfxt_mesh_import_options_t options = {
		.layout = (gs_gfxt_mesh_layout_t[]) {
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_POSITION},
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_TEXCOORD},
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_COLOR},
		},
		.layout_size = 3 * sizeof(gs_gfxt_mesh_layout_t),
		.index_buffer_element_size = sizeof(uint32_t)
	};
	gs_gfxt_load_gltf_data_from_file("./assets/iso_cube.gltf", &options, &app->meshes, &app->mesh_count); 

    // Init framebuffer, render target
    app->fbo = gs_graphics_framebuffer_create(NULL); 

    // Construct color render target
    app->rt = gs_graphics_texture_create(
        &(gs_graphics_texture_desc_t){
            .width = RT_WIDTH,  
            .height = RT_HEIGHT,
            .format = GS_GRAPHICS_TEXTURE_FORMAT_RGBA8,                        
            .wrap_s = GS_GRAPHICS_TEXTURE_WRAP_CLAMP_TO_EDGE,                        
            .wrap_t = GS_GRAPHICS_TEXTURE_WRAP_CLAMP_TO_EDGE,                       
            .min_filter = GS_GRAPHICS_TEXTURE_FILTER_LINEAR,                
            .mag_filter = GS_GRAPHICS_TEXTURE_FILTER_LINEAR,               
            .render_target = true
        }
    );

    // Construct render pass for offscreen render pass
    app->rp = gs_graphics_render_pass_create(
        &(gs_graphics_render_pass_desc_t){
            .fbo = app->fbo,                      
            .color = &app->rt,                        
            .color_size = sizeof(app->rt)        
        } 
    );

    app->fs_quad = gs_gfxt_mesh_unit_quad_generate(NULL);
    app->tex = gs_gfxt_texture_generate_default();

    // Create pipeline for back buffer
    app->bb_pip = gs_gfxt_pipeline_create(&(gs_gfxt_pipeline_desc_t){
        .pip_desc = (gs_graphics_pipeline_desc_t){
            .raster = {
                .shader = gs_graphics_shader_create(&(gs_graphics_shader_desc_t){ 
                    .sources = (gs_graphics_shader_source_desc_t[]){
                        {.type = GS_GRAPHICS_SHADER_STAGE_VERTEX, .source = bb_v_src},
                        {.type = GS_GRAPHICS_SHADER_STAGE_FRAGMENT, .source = bb_f_src} 
                    },
                    .size = 2 * sizeof(gs_graphics_shader_source_desc_t),
                    .name = "bb_shader"
                }),
                .index_buffer_element_size = sizeof(uint32_t)
            },
            .layout = {
                .attrs = (gs_graphics_vertex_attribute_desc_t[]){
                    {.format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2, .name = "a_pos"},
                    {.format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2, .name = "a_uv"}
                },
                .size = 2 * sizeof(gs_graphics_vertex_attribute_desc_t)
            },
        },
        .ublock_desc = (gs_gfxt_uniform_block_desc_t){
            .layout = (gs_gfxt_uniform_desc_t[]){ 
                {.name = "u_tex", .type = GS_GRAPHICS_UNIFORM_SAMPLER2D},
                {.name = "u_tex_size", .type = GS_GRAPHICS_UNIFORM_VEC2},
                {.name = "u_screen_size", .type = GS_GRAPHICS_UNIFORM_VEC2},
                {.name = "u_palette0", .type = GS_GRAPHICS_UNIFORM_VEC3},
                {.name = "u_palette1", .type = GS_GRAPHICS_UNIFORM_VEC3},
                {.name = "u_palette2", .type = GS_GRAPHICS_UNIFORM_VEC3},
                {.name = "u_ranges", .type = GS_GRAPHICS_UNIFORM_VEC2}
            },
            .layout_size = 7 * sizeof(gs_gfxt_uniform_desc_t)
        }
    }); 

    app->bb_mat = gs_gfxt_material_create(&(gs_gfxt_material_desc_t){
        .pip_func = (gs_gfxt_raw_data_func_desc_t){
            .hndl = &app->bb_pip
        }
    }); 

    // Vertex data for quad
    float v_data[] = {
        // Positions  UVs
        -1.0f, -1.0f,  0.0f, 0.0f,  // Top Left
         1.0f, -1.0f,  1.0f, 0.0f,  // Top Right 
        -1.0f,  1.0f,  0.0f, 1.0f,  // Bottom Left
         1.0f,  1.0f,  1.0f, 1.0f   // Bottom Right
    };

    // Index data for quad
    uint32_t i_data[] = {
        0, 3, 2,    // First Triangle
        0, 1, 3     // Second Triangle
    }; 

    // Construct vertex buffer
    app->vbo = gs_graphics_vertex_buffer_create(
        &(gs_graphics_vertex_buffer_desc_t) {
            .data = v_data,
            .size = sizeof(v_data)
        }
    );

    // Construct index buffer
    app->ibo = gs_graphics_index_buffer_create(
        &(gs_graphics_index_buffer_desc_t) {
            .data = i_data,
            .size = sizeof(i_data)
        }
    ); 

    app->palettes[0] = gs_v3s(0.1f);
    app->palettes[1] = gs_v3s(0.6f);
    app->palettes[2] = gs_v3s(0.9f);
    app->ranges = gs_v2(0.1f, 0.6f);

	app->camera = gs_camera_default();
	app->camera.ortho_scale = 1.f;
	// app->camera.transform.position = gs_v3(6.4f, 8.9f, 6.6f);
	app->camera.transform.position = gs_v3(-9.5f, 2.f, -.75f);
    app->camera.transform.rotation = gs_quat_from_euler(0.f, 45.f, -18.f);

    gs_graphics_texture_desc_t tdesc = {
        .format = GS_GRAPHICS_TEXTURE_FORMAT_RGBA8,
        .min_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST, 
        .mag_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST 
    };

	gs_asset_texture_load_from_file("./assets/iso_tiles3.png", &app->ship_tex, &tdesc, false, false); 
    gs_asset_texture_load_from_file("./assets/water.png", &app->pix_tex, &tdesc, false, false);

    app->xform = gs_vqs_default();
    app->xform.position = gs_v3(0.f, 6.f, 0.f);
    app->xform.scale = gs_v3s(1.f); 
    app->physics = (physics_component_t){.speed = 10.f};

    gs_graphics_shader_source_desc_t qb_sources[2] = {0};
    qb_sources[0].type = GS_GRAPHICS_SHADER_STAGE_VERTEX; qb_sources[0].source = qb_v_src;
    qb_sources[1].type = GS_GRAPHICS_SHADER_STAGE_FRAGMENT; qb_sources[1].source = qb_f_src; 

    // Construct shader for pipeline 
    gs_graphics_shader_desc_t sdesc = {0};
    sdesc.sources = qb_sources;
    sdesc.size = sizeof(qb_sources);
    sdesc.name = "quad_batch";
    gs_shader qshader = gs_graphics_shader_create(&sdesc);

    // Construct a pipeline for quadbatches
    gs_graphics_vertex_layout_desc_t layout = {0};
    gs_graphics_vertex_attribute_desc_t vattrs[3] = {0};
    vattrs[0].format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2; 
    vattrs[0].name = "a_position";
    vattrs[1].format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2; 
    vattrs[1].name = "a_uv";
    vattrs[2].format = GS_GRAPHICS_VERTEX_ATTRIBUTE_BYTE4; 
    vattrs[2].name = "a_color";
    layout.attrs = vattrs;
    layout.size = sizeof(vattrs);
    pipeline_t qpip = {0};
    gs_graphics_pipeline_desc_t pdesc = {0};
    gs_graphics_raster_state_desc_t rdesc = {0};
    gs_graphics_blend_state_desc_t bdesc = {0};
    bdesc.func = GS_GRAPHICS_BLEND_EQUATION_ADD;
    bdesc.src = GS_GRAPHICS_BLEND_MODE_SRC_ALPHA;
    bdesc.dst = GS_GRAPHICS_BLEND_MODE_ONE_MINUS_SRC_ALPHA;
    rdesc.shader = qshader;
    rdesc.index_buffer_element_size = sizeof(uint32_t);
    pdesc.raster = rdesc;
    pdesc.layout = layout; 
    pdesc.blend = bdesc;

    // Construct uniform block descriptor
    gs_gfxt_uniform_desc_t udesc[2] = {0}; 
    udesc[0].name = "u_mvp";
    udesc[0].type = GS_GRAPHICS_UNIFORM_MAT4;

    udesc[1].name = "u_tex";
    udesc[1].type = GS_GRAPHICS_UNIFORM_SAMPLER2D;

    gs_gfxt_uniform_block_desc_t ubdesc = {0}; 
    ubdesc.layout = udesc;
    ubdesc.layout_size = sizeof(udesc);

    // Pass in a gfxt pipeline desc, which needs a uniform block descriptor and a pipeline descriptor
    gs_gfxt_pipeline_desc_t gfxtpdesc = {0};
    gfxtpdesc.pip_desc = pdesc;
    gfxtpdesc.ublock_desc = ubdesc;
    g_pip.pipeline = gs_gfxt_pipeline_create(&gfxtpdesc);

    // Construct material, get handle, do da things with it
    gs_gfxt_material_desc_t mdesc = {0};
    gs_gfxt_raw_data_func_desc_t pip_func = {0};
    pip_func.hndl = &g_pip.pipeline;
    mdesc.pip_func = pip_func;
    g_mat.material = gs_gfxt_material_create(&mdesc);

    // Init quad batches
    quad_batch_init(&g_qb, 1);
    quad_batch_init(&g_qb2, 1); 
    quad_batch_init(&g_qb3, 1);
}

typedef struct mesh_vert_t {
	gs_vec3 position;
	gs_vec2 uv;
	gs_color_t color;
} mesh_vert_t;

GS_API_DECL void app_draw_mesh(const gs_vqs* vqs)
{
	app_t* app = gs_engine_user_data(app_t);
	gs_immediate_draw_t* gsi = &app->core->gsi;

	gs_gfxt_mesh_import_options_t options = {
		.layout = (gs_gfxt_mesh_layout_t[]) {
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_POSITION},		// Float_3
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_TEXCOORD},		// Float_2
			{.type = GS_GFXT_MESH_ATTRIBUTE_TYPE_COLOR},		// Uint8_4
		},
		.layout_size = 3 * sizeof(gs_gfxt_mesh_layout_t),
		.index_buffer_element_size = sizeof(uint32_t)
	};

	// Push texture
	gsi_texture(gsi, app->ship_tex.hndl);

	// Not sure how to do lines...but can do triangles, I think.
	gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
	{
		gsi_mul_matrix(gsi, gs_vqs_to_mat4(vqs));

		for (uint32_t m = 0; m < app->mesh_count; ++m)
		{
			// Get mesh data at m
			gs_gfxt_mesh_raw_data_t* mesh = &app->meshes[m];

			for (uint32_t p = 0; p < mesh->prim_count; ++p)
			{
				mesh_vert_t* vertices = (mesh_vert_t*)mesh->vertices[p];
				uint32_t* indices = (uint32_t*)mesh->indices[p];
				uint32_t index_count = (size_t)mesh->index_sizes[p] / sizeof(uint32_t);

				// Iterate through indices
				for (uint32_t i = 0; i < index_count; i += 3)
				{
					uint32_t i0 = indices[i];
					uint32_t i1 = indices[i + 1];
					uint32_t i2 = indices[i + 2];

					// Grab verts with mesh vert declaration
					mesh_vert_t* v0 = &vertices[i0];
					mesh_vert_t* v1 = &vertices[i1];
					mesh_vert_t* v2 = &vertices[i2]; 

					// Do triangle now
					gsi_trianglevxmc(
						gsi, 
						v0->position, v1->position, v2->position, 
						v0->uv, v1->uv, v2->uv, 
						v0->color, v1->color, v2->color, 
						GS_GRAPHICS_PRIMITIVE_TRIANGLES
					);
				}
			}
		}
	}
	gsi_pop_matrix(gsi);

	// Pop texture
	gsi_texture(gsi, (gs_handle(gs_graphics_texture_t)){.id = 0}); 
}

GS_API_DECL void app_update_transform(physics_component_t* physics, gs_vqs* xform)
{ 
    const float dt = gs_platform_delta_time();
	gs_vec3* ps = &xform->position;

	const gs_vec2 xb = gs_v2(-12.f, 12.f);
	const gs_vec2 yb = gs_v2(-4.f, 11.f); 

	// Update player position based on input
	gs_vec3 v = gs_v3s(0.f);
	if (gs_platform_key_down(GS_KEYCODE_W) || gs_platform_key_down(GS_KEYCODE_UP))    v.y = ps->y > yb.x ? v.y - 1.f : 0.f;
	if (gs_platform_key_down(GS_KEYCODE_S) || gs_platform_key_down(GS_KEYCODE_DOWN))  v.y = ps->y < yb.y ? v.y + 1.f : 0.f;
	if (gs_platform_key_down(GS_KEYCODE_A) || gs_platform_key_down(GS_KEYCODE_LEFT))  v.x = ps->x > xb.x ? v.x - 1.f : 0.f;
	if (gs_platform_key_down(GS_KEYCODE_D) || gs_platform_key_down(GS_KEYCODE_RIGHT)) v.x = ps->x < xb.y ? v.x + 1.f : 0.f;

	// Normalize velocity then scale by player speed
	v = gs_vec3_scale(gs_vec3_norm(v), physics->speed * dt);

	// Add to velocity
	physics->velocity = gs_v3(
		gs_interp_linear(physics->velocity.x, v.x, 0.1f),
		gs_interp_linear(physics->velocity.y, v.y, 0.1f),
		gs_interp_linear(physics->velocity.z, v.z, 0.1f)
	);
	*ps = gs_vec3_add(*ps, physics->velocity); 

	// Move z back to origin
	float nz = -ps->z;
	ps->z = gs_interp_linear(ps->z, nz, 0.5f);

	gs_vec3 av = gs_v3s(0.f);
	bool br = false;
	if (gs_platform_key_down(GS_KEYCODE_W)) av.y -= 1.f;
	if (gs_platform_key_down(GS_KEYCODE_S)) av.y += 1.f;
	if (gs_platform_key_down(GS_KEYCODE_A)) av.x += 1.f;
	if (gs_platform_key_down(GS_KEYCODE_D)) av.x -= 1.f;
	if (gs_platform_key_down(GS_KEYCODE_Q)) av.z += 1.f;
	if (gs_platform_key_down(GS_KEYCODE_E)) av.z -= 1.f;
	if (gs_platform_key_down(GS_KEYCODE_B)) {br = true; av.z += 1.f;}
	av = gs_vec3_scale(gs_vec3_norm(av), 3.f);
	av.z = br ? av.z * 50.f : av.z * 10.f;
	// Add negated angular velocity to this
	av = gs_vec3_add(av, gs_vec3_scale(gs_vec3_neg(physics->angular_velocity), 1.5 * dt));
 
 	physics->angular_velocity = gs_vec3_add(physics->angular_velocity, av);
 	gs_vec3* pav = &physics->angular_velocity;
	const float max_rotx = 60.f;
	const float max_roty = 30.f;
	const float max_rotz = 90.f;
 	pav->x = gs_clamp(pav->x, -max_rotx, max_rotx);
 	pav->y = gs_clamp(pav->y, -max_roty, max_roty);
 	pav->z = br ? pav->z : gs_clamp(pav->z, -max_rotz, max_rotz);

	// Do rotation (need barrel roll rotation)
    /*
	xform->rotation = gs_quat_mul_list(3,
		gs_quat_angle_axis(gs_deg2rad(physics->angular_velocity.x), GS_YAXIS),
		gs_quat_angle_axis(gs_deg2rad(physics->angular_velocity.y), GS_XAXIS),
		gs_quat_angle_axis(gs_deg2rad(physics->angular_velocity.z), GS_ZAXIS)
	);
    */
}

void test_window(mu_Context* ctx); 
void camera_gui(mu_Context* ctx, gs_camera_t* cam);

/*
GS_API_DECL void app_update()
{
    app_t* app = gs_engine_user_data(app_t);
	core_t* core = app->core;
    gs_command_buffer_t* cb = &core->cb;
    gs_immediate_draw_t* gsi = &core->gsi; 
    gs_mu_ctx* gmu = &core->gmu;

    const gs_vec2 fbs = gs_platform_framebuffer_sizev(gs_platform_main_window());
    const gs_vec2 ws = gs_platform_window_sizev(gs_platform_main_window()); 

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 

    app_update_transform(&app->physics, &app->xform);

    // === [ Render scene ] ===== //

    // Render pass action for clearing the screen
    const gs_graphics_clear_desc_t def_clear = {.actions = &(gs_graphics_clear_action_t)
        {.color = {0.1f, 0.1f, 0.1f, 1.f}}
    };

    const gs_graphics_clear_desc_t fb_clear = {.actions = &(gs_graphics_clear_action_t)
        {.color = {0.1f, 0.1f, 0.1f, 1.f}}
    };

    const float t = gs_platform_elapsed_time() * 0.004f;

    // Offscreen pass
    gs_graphics_begin_render_pass(cb, app->rp);
        gs_graphics_set_viewport(cb, 0, 0, RT_WIDTH, RT_HEIGHT);
        gs_graphics_clear(cb, &fb_clear);
        gsi_camera(gsi, &app->camera);
        //gsi_camera2D(gsi);
        gsi_depth_enabled(gsi, true);
        gsi_face_cull_enabled(gsi, true);
        // gsi_texture(gsi, app->tex);
        gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
        {
            // Snap angle of rotation over time
            const float step = 5.f;
            float za = (float)((int32_t)(gs_rad2deg(gs_platform_elapsed_time() * 0.0003f)) % 360); 
            za = (float)floor((int32_t)(za / step)) * step; 

            // Still don't think this looks good.

            gs_vqs xform = app->xform;
            xform.position.x = (float)((int32_t)xform.position.x);
            xform.position.y = (float)((int32_t)xform.position.y);
            xform.position.z = (float)((int32_t)xform.position.z); 
            float val = 3.f * sin(t); 
            float diff = val - round(val); 
            // No idea how to calculate what I'm thinking of calculating here...
            // Seems impossible
            xform.position.y = round(val) + (round(diff * 2.f) * 0.5f);

            // Not sure if this is possible, what I'm trying to do...
            // Maybe I should render it all out to a separate buffer at full resolution?

            static bool do_dis = false;

            if (gs_platform_key_pressed(GS_KEYCODE_P)) do_dis = !do_dis;

            app_draw_mesh(&xform); 

            // gsi_texture(gsi, app->tex);

            // Draw a cube
            gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
            {
                xform.position.x = -1.f;
                xform.position.z = 8.f;
                xform.scale = gs_v3s(1.f);
                xform.position.y = 0.f;
                xform.rotation = gs_quat_mul_list(3, 
                    gs_quat_angle_axis(gs_deg2rad(za), GS_XAXIS),
                    gs_quat_angle_axis(gs_deg2rad(za - step), GS_YAXIS),
                    gs_quat_angle_axis(gs_deg2rad(za + step), GS_ZAXIS) 
                ); 
                gsi_mul_matrix(gsi, gs_vqs_to_mat4(&xform));
                gsi_box(gsi, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 255, 255, 255, 255, GS_GRAPHICS_PRIMITIVE_LINES); 
            }
            gsi_pop_matrix(gsi);

            // Sphere
            gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
            {
                xform.position.x = -3.0f;
                xform.position.z = 8.f;
                xform.rotation = gs_quat_default();
                xform.scale = gs_v3s(1.f);
                gsi_mul_matrix(gsi, gs_vqs_to_mat4(&xform));
                gsi_sphere(gsi, 0.f, 0.f, 0.f, 0.5f, 255, 255, 255, 255, GS_GRAPHICS_PRIMITIVE_TRIANGLES); 
            }
            gsi_pop_matrix(gsi);

            static gs_vqs xforms[10 * 10];
            static bool updated = false; 
            if (!updated) 
            { 
                updated = true;
                gs_for_range_i(10)
                gs_for_range_j(10)
                {
                    uint32_t idx = i * 10 + j;
                    xforms[idx] = gs_vqs_default();
                    xforms[idx].position = gs_v3(j, 0.f, i);
                    xforms[idx].scale.y = gs_rand_gen_range(&app->rand, 0.1f, 3.f);
                } 
            }

            // Do a grid of randomly sized boxes
            gs_for_range_i(10)
            {
                gs_for_range_j(10)
                {
                    gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
                    {
                        uint32_t idx = i * 10 + j;
                        gsi_mul_matrix(gsi, gs_vqs_to_mat4(&xforms[idx]));
                        gsi_box(gsi, 0.f, 0.f, 0.f, 0.5f, 0.5f, 0.5f, 255, 255, 255, 255, GS_GRAPHICS_PRIMITIVE_TRIANGLES);
                    }
                    gsi_pop_matrix(gsi);
                } 
            }
        }
        gsi_pop_matrix(gsi);
    gsi_draw(gsi, cb);
    gs_graphics_end_render_pass(cb); 
    
    // Backbuffer pass
    gs_graphics_begin_render_pass(cb, GS_GRAPHICS_RENDER_PASS_DEFAULT); 
        gs_graphics_set_viewport(cb, 0, 0, (int32_t)fbs.x, (int32_t)fbs.y); 
        gs_graphics_clear(cb, &def_clear);

        // Set uniforms for material
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_tex", &app->rt);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_tex_size", &(gs_vec2){RT_WIDTH, RT_HEIGHT});
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_screen_size", &fbs);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_palette0", &app->palettes[0]);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_palette1", &app->palettes[1]);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_palette2", &app->palettes[2]);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_ranges", &app->ranges);

        gs_gfxt_material_bind(cb, &app->bb_mat);
        gs_gfxt_material_bind_uniforms(cb, &app->bb_mat); 

        // Bind unit quad
        gs_graphics_bind_desc_t binds = {
            .vertex_buffers = {.desc = &(gs_graphics_bind_vertex_buffer_desc_t){.buffer = app->vbo}},
            .index_buffers = {.desc = &(gs_graphics_bind_index_buffer_desc_t){.buffer = app->ibo}},
        };
        gs_graphics_apply_bindings(cb, &binds); 

        // Draw
        gs_graphics_draw(cb, &(gs_graphics_draw_desc_t){.start = 0, .count = 6});

        // UI
        gs_mu_new_frame(gmu); 
        mu_begin(&gmu->mu);
        // test_window(&gmu->mu);
        camera_gui(&gmu->mu, &app->camera);
        // log_window(&gmu->mu);
        mu_end(&gmu->mu);
        // Submits to cb
        gs_mu_render(gmu, cb); 
    gs_graphics_end_render_pass(cb);

    // Try to do a 3d scene, orthographic view, displaying quads (pixel art asset)
    gsi_camera(gsi, &app->camera);
   // gsi_camera2D(gsi); 

    // Not sure now what the size of this should be...
    gsi_texture(gsi, app->pix_tex.hndl);
    gs_quat cq = gs_quat_inverse(gs_quat_norm(app->camera.transform.rotation));
    gsi_push_matrix(gsi, GSI_MATRIX_MODELVIEW);
    {
        gsi_mul_matrix(gsi, gs_quat_to_mat4(cq));
        // Need to change the orientation of this crap...get the inverse of the camera's rotation?
        gsi_rectvd(gsi, gs_v2(0.f, 0.f), gs_vec2_scale(gs_v2(100.f, 64.f), 0.1f), gs_v2s(0.f), gs_v2s(1.f), GS_COLOR_WHITE, GS_GRAPHICS_PRIMITIVE_TRIANGLES); 
        gsi_draw(gsi, cb);
    }
    gsi_pop_matrix(gsi);

    // Submit
    gs_graphics_submit_command_buffer(cb);
}
*/

bool point_vs_aabb(gs_vec2 p, gs_vec4 aabb)
{
    return ((p.x >= aabb.x && p.x <= aabb.z) &&
            (p.y >= aabb.y && p.y <= aabb.w));
};

gs_vec4 calc_iso_rect(gs_vec2 cart_pos, gs_vec2 cart_dim)
{
    gs_vec2 min = cart_to_iso(cart_pos);
    gs_vec2 max = cart_to_iso(gs_vec2_add(cart_pos, cart_dim));
    return gs_v4(min.x, min.y, max.x, max.y);
}; 

GS_API_DECL void app_update()
{
    app_t* app = gs_engine_user_data(app_t);
	core_t* core = app->core;
    gs_command_buffer_t* cb = &core->cb;
    gs_immediate_draw_t* gsi = &core->gsi; 
    gs_mu_ctx* gmu = &core->gmu;

    const gs_vec2 fb = gs_platform_framebuffer_sizev(gs_platform_main_window());
    const gs_vec2 ws = gs_platform_window_sizev(gs_platform_main_window()); 

    // Process input (closing window) 
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit(); 

	gs_vec2 hws = gs_vec2_scale(ws, 0.5f);
	gs_camera_t c = gs_camera_default(); 
    
    static float os = 1.f;
    if (gs_platform_key_down(GS_KEYCODE_Q)) os += 0.05f;
    if (gs_platform_key_down(GS_KEYCODE_E)) os -= 0.05f;
    os = gs_clamp(os, -10000.f, 10000.f);
    c.ortho_scale = os;

    const gs_vec2 pixel_step = gs_v2(1.f / (float)RT_WIDTH, 1.f / (float)RT_HEIGHT);

    static float px = 0.f, py = 0.f, pz = 0.f; 
    if (gs_platform_key_down(GS_KEYCODE_W)) py -= (pixel_step.y * 3000.f * c.ortho_scale);
    if (gs_platform_key_down(GS_KEYCODE_S)) py += (pixel_step.y * 3000.f * c.ortho_scale);
    if (gs_platform_key_down(GS_KEYCODE_A)) px -= (pixel_step.x * 3000.f * c.ortho_scale);
    if (gs_platform_key_down(GS_KEYCODE_D)) px += (pixel_step.x * 3000.f * c.ortho_scale); 

	c.transform.position = gs_vec3_add(c.transform.position, gs_v3(px, py, -1.f)); 

    world_t w = world_generate(gs_hash_str64("Gunslinger"), &g_qb3, &app->core->cb, app->amplitude, gs_v2s(app->period)); 

	f32 l = -ws.x * 0.5f * c.ortho_scale; 
	f32 r = ws.x * 0.5f * c.ortho_scale;
	f32 b = ws.y * 0.5f * c.ortho_scale;
	f32 tp = -ws.y * 0.5f * c.ortho_scale;
	gs_mat4 ortho = gs_mat4_transpose(gs_mat4_ortho(l, r, b, tp, c.near_plane, c.far_plane)); 

    // We have our quad batch, should be able to pass things into it now.
	gs_mat4 mvp = gs_mat4_mul(ortho, gs_camera_get_view(&c)); 

    gsi_camera2D(gsi);

    // Detect whether or not mouse is over this tile by coordinates
    gs_vec2 mp = gs_platform_mouse_positionv();
    // Convert mouse position to screen position
    mp.x -= VOXEL_SIZE / c.ortho_scale;
    mp = gs_vec2_sub(mp, gs_v2(ws.x * 0.5f, ws.y * 0.5f));
    mp = gs_vec2_scale(mp, c.ortho_scale);
    mp = gs_vec2_add(mp, gs_v2(c.transform.position.x, c.transform.position.y));
    gs_vec2 cmp = iso_to_cart(mp);   

    static bool did = false;
    if (!did)
    {
        did = true;

        quad_batch_begin(&g_qb2); 
        {
            gs_vec2 d = gs_v2(100.f, 64.f);     // So this is actual dimension of the texture in screen space.
            const float hw = d.x * 0.5f;
            const float hh = d.y * 0.5f;
            const float bias = 15.f;
            static float t = 0.f;
            t += 0.001f;
            static float max = -FLT_MIN;
            static float min = FLT_MAX;
            static float vmin = FLT_MAX;
            static float vmax = -FLT_MAX;
            const int32_t td = 350;
            for (int32_t i = -td / 2 - td; i < td / 2 - td; ++i)
            {
                for (int32_t j = -td / 2; j < td / 2; ++j)
                { 
                    // Height (sin wav) 
                    float sx = (float)j / (float)50.f;
                    float sy = (float)i / (float)50.f; 
                    // const float z = 200 * sin(t * 20.f * sqrt(sx*sx + sy*sy));
                    // const float z = 10 * sin(j * t) * cos(i * t);
                    //const float per = (sin(t * app->period) * 0.5f + 1.5f) * 0.5f;
                    const float per = (t * app->period);
                    const float z = 800 * gs_perlin2(((float)j + per) / 16.f, ((float)i + per) / 16.f);
                    if (z < min) min = z; 
                    if (z > max) max = z;

                    // Cartesian coordinates
                    const float x = (float)i * VOXEL_SIZE;
                    const float y = (float)j * VOXEL_SIZE;

                    // Iso coordinates
                    const float cx = (x - y);
                    const float cy = (y + x) * 0.5f + z;

                    // Iso depth
                    const float depth = cy - z;

                    // Final isometric position
                    gs_vec2 p = gs_v2(cx, cy);

                    // Get cartesian coordinate back from iso (takes height into account)
                    gs_vec2 cp = iso_to_cart(p);

                    // Determine color based on height (palettes)
                    float v = gs_map_range(min, max, 0.f, 1.f, z);
                    if (v > vmax) vmax = v;
                    if (v < vmin) vmin = v;
                    gs_color_t hc = z < 0.8f ? GS_COLOR_GREEN : gs_color(10, 50, 100, 255);

                    // Check whether or not projected cartesian mouse is over a cartesian tile (this doesn't account for overlapping tiles)
                    gs_color_t c = (cmp.x >= cp.x && cmp.x <= cp.x + VOXEL_SIZE &&
                                    cmp.y >= cp.y && cmp.y <= cp.y + VOXEL_SIZE) ? GS_COLOR_RED : hc;

                    // gsi_rectvd(gsi, gs_v2(x, y), gs_v2(VOXEL_SIZE, VOXEL_SIZE), gs_v2s(0.f), gs_v2s(1.f), c, GS_GRAPHICS_PRIMITIVE_LINES); 

                    quad_batch_add(&g_qb2, &p, &d, &(gs_vec4){0.f, 0.f, 1.f, 1.f}, &c, depth);
                }
            } 
        }
        quad_batch_end(&g_qb2, cb);
    }

    quad_batch_begin(&g_qb); 
    {
        gs_vec2 d = gs_v2(100.f, 64.f);     // So this is actual dimension of the texture in screen space.
        const float hw = d.x * 0.5f;
        const float hh = d.y * 0.5f;
        const float bias = 15.f;
        static float t = 0.f;
        t += 0.001f;
        static float max = -FLT_MIN;
        static float min = FLT_MAX;
        static float vmin = FLT_MAX;
        static float vmax = -FLT_MAX;
        const int32_t td = 32; 

        // Try to generate a 3d voxel chunk. Need chunk/region/world tile definitions set up for this.
        for (int32_t i = -td / 2; i < td / 2; ++i)
        {
            for (int32_t j = -td / 2; j < td / 2; ++j)
            { 
                // Height (sin wav) 
                float sx = (float)j / (float)50.f;
                float sy = (float)i / (float)50.f; 
                // const float z = 200 * sin(t * 20.f * sqrt(sx*sx + sy*sy));
                // const float z = 10 * sin(j * t) * cos(i * t);
                //const float per = (sin(t * app->period) * 0.5f + 1.5f) * 0.5f;
                const float per = (t * app->period);
                const float z = app->amplitude * gs_perlin2(((float)j + per) / 16.f, ((float)i + per) / 16.f);
                if (z < min) min = z; 
                if (z > max) max = z;

                // Cartesian coordinates
                const float x = (float)i * VOXEL_SIZE;
                const float y = (float)j * VOXEL_SIZE;

                // Iso coordinates
                const float cx = (x - y);
                const float cy = (y + x) * 0.5f + z;

                // Iso depth
                const float depth = cy - z;

                // Final isometric position
                gs_vec2 p = gs_v2(cx, cy);

                // Get cartesian coordinate back from iso (takes height into account)
                gs_vec2 cp = iso_to_cart(p);

                // Determine color based on height (palettes)
                float v = gs_map_range(min, max, 0.f, 1.f, z);
                if (v > vmax) vmax = v;
                if (v < vmin) vmin = v;
                gs_color_t hc = z < 0.8f ? GS_COLOR_GREEN : gs_color(10, 50, 100, 255);

                // Check whether or not projected cartesian mouse is over a cartesian tile (this doesn't account for overlapping tiles)
                gs_color_t c = (cmp.x >= cp.x && cmp.x <= cp.x + VOXEL_SIZE &&
                                cmp.y >= cp.y && cmp.y <= cp.y + VOXEL_SIZE) ? GS_COLOR_RED : hc;

                // gsi_rectvd(gsi, gs_v2(x, y), gs_v2(VOXEL_SIZE, VOXEL_SIZE), gs_v2s(0.f), gs_v2s(1.f), c, GS_GRAPHICS_PRIMITIVE_LINES); 

                quad_batch_add(&g_qb, &p, &d, &(gs_vec4){0.f, 0.f, 1.f, 1.f}, &c, depth);
            }
        } 
    }
    quad_batch_end(&g_qb, cb);

    // gsi_rectvd(gsi, cmp, gs_v2(5.f, 5.f), gs_v2s(0.f), gs_v2s(1.f), GS_COLOR_GREEN, GS_GRAPHICS_PRIMITIVE_TRIANGLES);

	gs_graphics_clear_desc_t  fb_clear = (gs_graphics_clear_desc_t){
        .actions = (gs_graphics_clear_action_t[]) {
            {.color = {10.f / 255.f, 10.f / 255.f, 10.f / 255.f, 255.f}}
        } 
    };

    gs_graphics_begin_render_pass(cb, app->rp);
    {
        gs_graphics_set_viewport(cb, 0, 0, RT_WIDTH, RT_HEIGHT);
        gs_graphics_clear(cb, &fb_clear);

        // Set material uniform data, then bind
        gs_gfxt_material_set_uniform(&g_mat.material, "u_mvp", &mvp); 
        gs_gfxt_material_set_uniform(&g_mat.material, "u_tex", &app->ship_tex.hndl); 
        gs_gfxt_material_bind(cb, &g_mat.material);
        gs_gfxt_material_bind_uniforms(cb, &g_mat.material);

        gs_graphics_bind_vertex_buffer_desc_t vbuffer = {0};
        gs_graphics_bind_index_buffer_desc_t ibuffer = {0};
        vbuffer.buffer = g_qb.vbo;
        ibuffer.buffer = g_qb.ibo;

        /*
        // Draw qb2
        {
            gs_graphics_bind_desc_t binds = (gs_graphics_bind_desc_t){
                .vertex_buffers = {.desc = &(gs_graphics_bind_vertex_buffer_desc_t){.buffer = g_qb2.vbo}},
                .index_buffers = {.desc = &(gs_graphics_bind_index_buffer_desc_t){.buffer = g_qb2.ibo}}
            }; 
            gs_graphics_apply_bindings(cb, &binds);
            gs_graphics_draw(cb, &(gs_graphics_draw_desc_t){.start = 0, .count = g_qb2.count});
        }

        // Draw qb1
        {
            gs_graphics_bind_desc_t binds = (gs_graphics_bind_desc_t){
                .vertex_buffers = {.desc = &(gs_graphics_bind_vertex_buffer_desc_t){.buffer = g_qb.vbo}},
                .index_buffers = {.desc = &(gs_graphics_bind_index_buffer_desc_t){.buffer = g_qb.ibo}}
            }; 
            gs_graphics_apply_bindings(cb, &binds);
            gs_graphics_draw(cb, &(gs_graphics_draw_desc_t){.start = 0, .count = g_qb.count});
        }
        */

        // Draw qb3
        {
            gs_graphics_bind_desc_t binds = (gs_graphics_bind_desc_t){
                .vertex_buffers = {.desc = &(gs_graphics_bind_vertex_buffer_desc_t){.buffer = g_qb3.vbo}},
                .index_buffers = {.desc = &(gs_graphics_bind_index_buffer_desc_t){.buffer = g_qb3.ibo}}
            }; 
            gs_graphics_apply_bindings(cb, &binds);
            gs_graphics_draw(cb, &(gs_graphics_draw_desc_t){.start = 0, .count = g_qb3.count});
        }
    } 
    gs_graphics_end_render_pass(cb);

    // Backbuffer pass
    gs_graphics_begin_render_pass(cb, GS_GRAPHICS_RENDER_PASS_DEFAULT); 
    {
        gs_graphics_set_viewport(cb, 0, 0, (int32_t)fb.x, (int32_t)fb.y); 
        gs_graphics_clear(cb, &fb_clear);

        // Set uniforms for material
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_tex", &app->rt);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_tex_size", &(gs_vec2){RT_WIDTH, RT_HEIGHT});
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_screen_size", &fb);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_palette0", &app->palettes[0]);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_palette1", &app->palettes[1]);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_palette2", &app->palettes[2]);
        gs_gfxt_material_set_uniform(&app->bb_mat, "u_ranges", &app->ranges);

        gs_gfxt_material_bind(cb, &app->bb_mat);
        gs_gfxt_material_bind_uniforms(cb, &app->bb_mat); 

        // Bind unit quad
        gs_graphics_bind_desc_t binds = {
            .vertex_buffers = {.desc = &(gs_graphics_bind_vertex_buffer_desc_t){.buffer = app->vbo}},
            .index_buffers = {.desc = &(gs_graphics_bind_index_buffer_desc_t){.buffer = app->ibo}},
        };
        gs_graphics_apply_bindings(cb, &binds); 

        // Draw
        gs_graphics_draw(cb, &(gs_graphics_draw_desc_t){.start = 0, .count = 6});

        // UI
        gs_mu_new_frame(gmu); 
        mu_begin(&gmu->mu);
        camera_gui(&gmu->mu, &app->camera);
        mu_end(&gmu->mu);
        gs_mu_render(gmu, cb); 
    } 
    gs_graphics_end_render_pass(cb); 

    gsi_render_pass_submit_ex(gsi, cb, NULL); 
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

void camera_gui(mu_Context* ctx, gs_camera_t* cam)
{
    app_t* app = gs_engine_user_data(app_t); 

    if (mu_begin_window(ctx, "Properties", mu_rect(40, 40, 300, 450)))
    { 
        if (mu_header_ex(ctx, "Terrain", MU_OPT_EXPANDED)) 
        {
            mu_label(ctx, "amplitude:"); mu_slider(ctx, &app->amplitude, 1.f, 5000.f); 
            mu_label(ctx, "period:"); mu_slider(ctx, &app->period, 1.f, 1000.f); 
        }

        if (mu_header_ex(ctx, "Ranges", MU_OPT_CLOSED)) 
        {
            gs_vec2* ranges = &app->ranges;
            mu_label(ctx, "low:"); mu_slider(ctx, &ranges->x, 0.f, ranges->y); 
            mu_label(ctx, "high:"); mu_slider(ctx, &ranges->y, ranges->x, 1.f); 
        }

        if (mu_header_ex(ctx, "Palette", MU_OPT_CLOSED)) 
        {
            for (uint32_t i = 0; i < 3; ++i)
            {
                gs_vec3* p = &app->palettes[i];
                gs_snprintfc(TMP, 32, "p%d: ", i);
                mu_label(ctx, TMP); 
                mu_label(ctx, "r:"); mu_slider(ctx, &p->x, 0.f, 1.f); 
                mu_label(ctx, "g:"); mu_slider(ctx, &p->y, 0.f, 1.f); 
                mu_label(ctx, "b:"); mu_slider(ctx, &p->z, 0.f, 1.f); 
            }
        }
        
        if (mu_header_ex(ctx, "Transform", MU_OPT_CLOSED)) 
        { 
            if (mu_header_ex(ctx, "Position", MU_OPT_CLOSED)) 
            {
                float range = 20.f;
                gs_vec3* p = &cam->transform.position;
                mu_label(ctx, "x:"); mu_slider(ctx, &p->x, -range, range); 
                mu_label(ctx, "y:"); mu_slider(ctx, &p->y, -range, range); 
                mu_label(ctx, "z:"); mu_slider(ctx, &p->z, -range, range); 
                p->x = round(p->x);
                p->y = round(p->y);
                p->z = round(p->z);
            }
            
            if (mu_header_ex(ctx, "Euler", MU_OPT_CLOSED)) 
            {
                static gs_vec3 ea = {0.f, 45.f, -18.f};
                mu_label(ctx, "x:"); 
                if (mu_slider(ctx, &ea.x, -360.f, 360.f)) goto set_rot;
                mu_label(ctx, "y:"); 
                if (mu_slider(ctx, &ea.y, -360.f, 360.f)) goto set_rot;
                mu_label(ctx, "z:"); 
                if (mu_slider(ctx, &ea.z, -360.f, 360.f)) goto set_rot;

                set_rot: 
                    cam->transform.rotation = gs_quat_from_euler(ea.x, ea.y, ea.z);
            } 

            if (mu_header_ex(ctx, "Quat", MU_OPT_CLOSED)) 
            {
                const gs_quat* q = &cam->transform.rotation;
                gs_transient_buffer(TMP, 32);
                gs_snprintf(TMP, 32, "<%.2f, %.2f, %.2f, %.2f>", q->x, q->y, q->z, q->w); 
                mu_label(ctx, TMP); 
            } 
        }

        if (mu_header_ex(ctx, "Ortho Scale", MU_OPT_CLOSED)) 
        {
            mu_label(ctx, "Ortho:"); mu_slider(ctx, &cam->ortho_scale, 0.1f, 20.f); 
        } 

        mu_end_window(ctx);
    } 
}

void test_window(mu_Context* ctx) 
{
    #define write_log(TXT) gs_println("%s", TXT);

    if (mu_begin_window(ctx, "Demo Window", mu_rect(40, 40, 300, 450))) 
    {
        mu_Container *win = mu_get_current_container(ctx);
        win->rect.w = mu_max(win->rect.w, 240);
        win->rect.h = mu_max(win->rect.h, 300);

        // === window info === //
        if (mu_header(ctx, "Window Info")) {
            mu_Container *win = mu_get_current_container(ctx);
            char buf[64];
            mu_layout_row(ctx, 2, (int[]) { 54, -1 }, 0);
            mu_label(ctx,"Position:");
            sprintf(buf, "%d, %d", win->rect.x, win->rect.y); mu_label(ctx, buf);
            mu_label(ctx, "Size:");
            sprintf(buf, "%d, %d", win->rect.w, win->rect.h); mu_label(ctx, buf);
        }

        // === labels + buttons === //
        if (mu_header_ex(ctx, "Test Buttons", MU_OPT_EXPANDED)) 
        {
            mu_layout_row(ctx, 3, (int[]) { 86, -110, -1 }, 0);
            mu_label(ctx, "Test buttons 1:");
            if (mu_button(ctx, "Button 1")) {write_log("Pressed button 1");}
            if (mu_button(ctx, "Button 2")) {write_log("Pressed button 2");}
            mu_label(ctx, "Test buttons 2:");
            if (mu_button(ctx, "Button 3")) {write_log("Pressed button 3");}
            if (mu_button(ctx, "Popup")) {mu_open_popup(ctx, "Test Popup");}
            if (mu_begin_popup(ctx, "Test Popup")) 
            {
                mu_button(ctx, "Hello");
                mu_button(ctx, "World");
                mu_end_popup(ctx);
            }
        }

        // === tree === //

        if (mu_header_ex(ctx, "Tree and Text", MU_OPT_EXPANDED)) 
        {
            mu_layout_row(ctx, 2, (int[]) { 140, -1 }, 0);
            mu_layout_begin_column(ctx);
            if (mu_begin_treenode(ctx, "Test 1")) 
            {
                if (mu_begin_treenode(ctx, "Test 1a")) 
                {
                    mu_label(ctx, "Hello");
                    mu_label(ctx, "world");
                    mu_end_treenode(ctx);
                }

                if (mu_begin_treenode(ctx, "Test 1b")) 
                {
                    if (mu_button(ctx, "Button 1")) {write_log("Pressed button 1");}
                    if (mu_button(ctx, "Button 2")) {write_log("Pressed button 2");}
                    mu_end_treenode(ctx);
                }
                mu_end_treenode(ctx);
            }

            if (mu_begin_treenode(ctx, "Test 2")) 
            {
                mu_layout_row(ctx, 2, (int[]) { 54, 54 }, 0);
                if (mu_button(ctx, "Button 3")) {write_log("Pressed button 3");}
                if (mu_button(ctx, "Button 4")) {write_log("Pressed button 4");}
                if (mu_button(ctx, "Button 5")) {write_log("Pressed button 5");}
                if (mu_button(ctx, "Button 6")) {write_log("Pressed button 6");}
                mu_end_treenode(ctx);
            }

            if (mu_begin_treenode(ctx, "Test 3")) 
            {
                static int checks[3] = { 1, 0, 1 };
                mu_checkbox(ctx, "Checkbox 1", &checks[0]);
                mu_checkbox(ctx, "Checkbox 2", &checks[1]);
                mu_checkbox(ctx, "Checkbox 3", &checks[2]);
                mu_end_treenode(ctx);
            }

            mu_layout_end_column(ctx);

            mu_layout_begin_column(ctx);
            mu_layout_row(ctx, 1, (int[]) {-1}, 0);
            mu_text(ctx, "Lorem ipsum dolor sit amet, consectetur adipiscing "
                         "elit. Maecenas lacinia, sem eu lacinia molestie, mi risus faucibus "
                         "ipsum, eu varius magna felis a nulla.");
            mu_layout_end_column(ctx);
        }

        if (mu_header_ex(ctx, "Background Color", MU_OPT_EXPANDED)) 
        {
            mu_layout_row(ctx, 2, (int[]) {-78, -1}, 74);
            mu_layout_begin_column(ctx);
            mu_layout_row(ctx, 2, (int[]) {46, -1}, 0);
            mu_label(ctx, "Red:");   mu_slider(ctx, &bg[0], 0, 255);
            mu_label(ctx, "Green:"); mu_slider(ctx, &bg[1], 0, 255);
            mu_label(ctx, "Blue:");  mu_slider(ctx, &bg[2], 0, 255);
            mu_layout_end_column(ctx);
            mu_Rect r = mu_layout_next(ctx);
            mu_draw_rect(ctx, r, mu_color(bg[0], bg[1], bg[2], 255));
            char buf[32];
            sprintf(buf, "#%02X%02X%02X", (int) bg[0], (int) bg[1], (int) bg[2]);
            mu_draw_control_text(ctx, buf, r, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);
        }

        mu_end_window(ctx);
    }
}

#endif  // APP_IMPL
#endif  // APP_H
