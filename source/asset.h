#ifndef ASSET_H
#define ASSET_H

#define ASSET_STR_MAX 	1024

// Forward decls.
struct asset_importer_t;
struct asset_record_t;
struct asset_storage_t;

/*
 * TODO(john):
 * 		Create default assets as fallbacks (in case of loading issues)
 * 		Try to load an animation file and animate for 3d shit
 * 		Use dirent.h for file iteration to get around windows garbage
*/

_introspect()
typedef enum asset_state
{
	ASSET_STATE_UNLOADED = 0x00,
	ASSET_STATE_LOADED
} asset_state;

// Functions
GS_API_DECL void asset_qualified_name(const char* src, char* dst, size_t sz);

typedef uint32_t asset_hndl_t;

_introspect()
typedef struct asset_t
{
	BASE(object_t);

	// Fields
    uint32_t record_hndl;    // Handle to internal record for asset
	asset_state state;

} asset_t;

typedef struct mesh_t 
{
	BASE(asset_t);
	
	// Fields
	gs_gfxt_mesh_t mesh;

} mesh_t;

_introspect()
typedef struct texture_t
{
	BASE(asset_t);

	// Fields
	gs_asset_texture_t texture;

} texture_t;

GS_API_DECL object_t* texture_t_new(size_t sz);
GS_API_DECL bool assets_texture_t_load_resource_from_file(const char* path, asset_t* out, void* user_data);

typedef struct font_t
{
	BASE(asset_t);

	// Fields
	gs_asset_font_t font;

} font_t;

typedef struct audio_source_t
{
	BASE(asset_t);

	// Fields
	gs_asset_audio_t audio;

} audio_source_t;

typedef struct material_t
{
	BASE(asset_t);

	// Fields
	gs_gfxt_material_t material;

} material_t;

typedef struct pipeline_t
{
	BASE(asset_t);

	// Fields
	gs_gfxt_pipeline_t pipeline;

} pipeline_t;

typedef struct asset_record_t
{
	BASE(asset_t);

	// Fields
	uint32_t hndl;		        // Handle to asset slot array in storage
    char path[ASSET_STR_MAX];   // Absolute path to asset on disk
    gs_uuid_t uuid;             // UUID for asset
    char name[ASSET_STR_MAX];   // Qualified name for asset
} asset_record_t;

#define ASSET_IMPORTER_FILE_EXTENSIONS_MAX 10

typedef struct asset_importer_desc_t
{
	bool (* load_resource_from_file)(const char* path, asset_t* out, void* user_data);
	char* file_extensions[ASSET_IMPORTER_FILE_EXTENSIONS_MAX];
	size_t file_extensions_size;
} asset_importer_desc_t;

typedef struct asset_importer_t
{
	BASE(object_t);

	gs_slot_array(asset_record_t) records;		// Slot array of asset records
	gs_hash_table(uint64_t, uint32_t) uuid2id;  // Lookup mapping from uuid to record slot id
	gs_hash_table(uint64_t, uint32_t) name2id;  // Lookup mapping from name to record slot id
	gs_slot_array(asset_t*) assets;			    // Slot array of raw asset data (could try to come up with a way to not malloc this)

	bool (* load_resource_from_file)(const char* path, asset_t* out, void* user_data);
	uint64_t asset_cls_id;
	size_t cls_sz;
} asset_importer_t;

typedef struct asset_manager_t
{
	BASE(object_t);

	// Fields
	char root_path[ASSET_STR_MAX];	
	gs_slot_array(asset_importer_t*) importers;	    // Slot array of asset data
	gs_hash_table(uint64_t, uint32_t) cid2importer; // Mapping from cls id to importer data
	gs_hash_table(uint64_t, uint32_t) fe2importer;  // Mapping from file extension to importer data

} asset_manager_t;

GS_API_DECL void assets_init(const char* root_path);
GS_API_DECL const char* assets_get_internal_file_extension(const char* ext);
GS_API_DECL void assets_import(const char* path, void* user_data);
GS_API_DECL gs_result assets_serialize_asset(const char* path, const asset_t* in);
GS_API_DECL gs_result assets_deserialize_asset(const char*path, asset_t* out);
GS_API_DECL void* assets_get_data_internal(uint64_t cls_id);
GS_API_DECL  const asset_t* _assets_get_w_name_internal(const asset_manager_t* am, uint64_t cid, const char* name);
GS_API_DECL void _assets_register_importer_internal(asset_manager_t* am, uint64_t cid, size_t cls_sz, asset_importer_desc_t* desc);

#define assets_register_importer(ASSETS, T, DESC)\
	_assets_register_importer_internal(ASSETS, obj_id(T), sizeof(T), DESC);

#define assets_getp(ASSETS, T, NAME)\
	_assets_get_w_name_internal(ASSETS, gs_hash_str64(gs_to_str(T)), NAME)

// Mechanism for getting asset manager instance
#define asset_manager_instance() _g_asset_manager

#ifdef ASSET_IMPL

// Global instance of asset manager
asset_manager_t* _g_asset_manager = NULL; 

// Maps resource file extension to internal engine file extension
GS_API_DECL const char* assets_get_internal_file_extension(const char* ext)
{
	// Textures
	if (gs_string_compare_equal(ext, "png") || gs_string_compare_equal(ext, "jpg"))
	{
		return "tex";
	}

	return "ass";
}

GS_API_DECL const asset_t* _assets_get_w_name_internal(const asset_manager_t* am, uint64_t cid, const char* name)
{
	if (!gs_hash_table_exists(am->cid2importer, cid)) 
	{
		gs_timed_action(60, {
			gs_println("error::asset_manager_t::getp:: asset type t doesn't exist: %s", name);
		});
		return NULL;
	}

	// Get the storage
	uint32_t shndl = gs_hash_table_get(am->cid2importer, cid);
	asset_importer_t* importer = gs_slot_array_get(am->importers, shndl);
	gs_assert(importer);

	// Get the asset from storage by name
	// TODO(john): Need to load asset if not loaded by default yet	
	uint64_t hash = gs_hash_str64(name);	
	if (!gs_hash_table_exists(importer->name2id, hash))
	{
		gs_timed_action(60, {
			gs_println("error::asset_manager_t::getp::asset doesn't exist: %s", name);
		});
		return NULL;
	}

	uint32_t rhndl = gs_hash_table_get(importer->name2id, hash);
	const asset_record_t* record = gs_slot_array_getp(importer->records, rhndl);
	gs_assert(record);

	const asset_t* asset = gs_slot_array_get(importer->assets, record->hndl);
	return asset;
}

GS_API_DECL void _assets_register_importer_internal(asset_manager_t* am, uint64_t cid, size_t cls_sz, asset_importer_desc_t* desc)
{
	asset_importer_t* importer = gs_malloc_init(asset_importer_t);	
	importer->load_resource_from_file = desc->load_resource_from_file;
	importer->asset_cls_id = cid;
	importer->cls_sz = cls_sz;
	uint32_t hndl = gs_slot_array_insert(am->importers, importer);
	gs_hash_table_insert(am->cid2importer, cid, hndl);
	uint32_t ct = desc->file_extensions_size ? desc->file_extensions_size / sizeof(char*) : 0;
	for (uint32_t i = 0; i < ct; ++i)
	{
		gs_hash_table_insert(am->fe2importer, gs_hash_str64(desc->file_extensions[i]), hndl);
	}
}

GS_API_DECL void assets_import(asset_manager_t* am, const char* path, void* user_data)	
{
	// Create record for asset, set path to asset using qualified name
	asset_record_t record = {0};

	// Get importer from importers
	gs_transient_buffer(FILE_EXT, 10);
	gs_platform_file_extension(FILE_EXT, 10, path);

	if (!gs_hash_table_exists(am->fe2importer, gs_hash_str64(FILE_EXT)))
	{
		return;
	}

	// Get asset storage
	uint32_t hndl = gs_hash_table_get(am->fe2importer, gs_hash_str64(FILE_EXT));
	asset_importer_t* importer = gs_slot_array_get(am->importers, hndl);

	// Get class id from storage
	uint64_t id = importer->asset_cls_id;

	// Get absolute path to asset
	gs_snprintfc(PATH, ASSET_STR_MAX, "%s/%s", am->root_path, path);

	// Get qualified name of asset
	gs_transient_buffer(QUAL_NAME, ASSET_STR_MAX);
	asset_qualified_name(path, QUAL_NAME, ASSET_STR_MAX);
	memcpy(record.name, QUAL_NAME, ASSET_STR_MAX);

	// Create final save path for asset
	gs_transient_buffer(FINAL_PATH_TMP, ASSET_STR_MAX);
	gs_transient_buffer(FINAL_PATH, ASSET_STR_MAX);
	gs_snprintf(FINAL_PATH_TMP, ASSET_STR_MAX, "%s/%s", am->root_path, QUAL_NAME);
	gs_util_string_replace((FINAL_PATH_TMP + 1), (FINAL_PATH + 1), ASSET_STR_MAX, '.', '/');
	FINAL_PATH[0] = '.';

	// Get file extension from registered mappings
	const char* file_ext = assets_get_internal_file_extension(FILE_EXT);
	gs_snprintf(record.path, ASSET_STR_MAX, "%s.%s", FINAL_PATH, file_ext);

       // Generate uuid for asset
	record.uuid = gs_platform_uuid_generate(); 

	// Need to construct asset type here using vtable
	vtable_t* vt = obj_vtable_w_id(id); 
	asset_t* asset = (asset_t*)vt->malloc(importer->cls_sz);
	gs_assert(asset);

	// Construct raw asset (this will also place into storage and give asset the record's handle)
	bool loaded = importer->load_resource_from_file(PATH, asset, user_data); 
	if (loaded)
	{
		// Insert into data array
		uint32_t hndl = gs_slot_array_insert(importer->assets, asset);

		// Set up tables
		gs_transient_buffer(UUID_BUF, 34);
		gs_platform_uuid_to_string(UUID_BUF, &record.uuid);
		gs_hash_table_insert(importer->uuid2id, gs_hash_str64(UUID_BUF), hndl);
		gs_hash_table_insert(importer->name2id, gs_hash_str64(record.name), hndl); 

		// Serialize asset to disk
		// serialize_asset(record.path, asset);

		// Store record in storage
		uint32_t rhndl = gs_slot_array_insert(importer->records, record);

		// Set asset record hndl
		asset->record_hndl = rhndl;
	}
}


/*
GS_API_DECL gs_result assets_serialize_asset(const char* path, const asset_t* in)
{
    // Get global instance of asset manager
    asset_manager_t* am = asset_manager_instance(); 

    // Get class id
    uint64_t id = in->cls_id();

    // Get asset storage based on type of asset
	uint32_t sid = am->cid2assets.get(id);
    asset_storage_base_t* storage = am->assets.get(sid);
    const asset_record_t* record = storage->records.getp(in->record_hndl);

	gs_byte_buffer_t bb = gs_byte_buffer_new();

	// === Object Header === //
	gs_byte_buffer_write(&bb, uint64_t, id);	// Class id (not sure about this)

	// === Asset Header === //
	gs_byte_buffer_write(&bb, gs_uuid_t, record->uuid);
	gs_byte_buffer_write_str(&bb, record->name);

	// Serialize asset data 
	gs_result res = in->serialize(&bb);	
		
	// Default serialization if no serializer provider
	if (res == GS_RESULT_INCOMPLETE)
	{
		object_t::serialize_default(&bb, in);
	}

	// Write to file
	gs_byte_buffer_write_to_file(&bb, path);

	// Free buffer
	gs_byte_buffer_free(&bb);

	return GS_RESULT_SUCCESS;
}
*/

/*
gs_result asset_manager_t::deserialize_asset(const char* path, asset_t* out)
{ 
    // NOTE(john): Object should already be instantiated.
    gs_assert(out);

    // Cache global asset manager instance
    asset_manager_t* am = asset_manager_instance();

	gs_byte_buffer_t bb = gs_byte_buffer_new();

	// Read buffer from file
	gs_byte_buffer_read_from_file(&bb, path);

	// === Object Header === //
	gs_byte_buffer_readc(&bb, uint64_t, id);	// Class id

    // Get storage based on id, need a record handle somehow. Don't have that yet.
    // Let's assume the asset has a record handle. It should, since we place that on load.  
	uint32_t sid = am->cid2assets.get(id);
    asset_storage_base_t* storage = am->assets.get(sid);
    const asset_record_t* record = storage->records.getp(out->record_hndl);

	// === Asset Header === //
	gs_byte_buffer_read(&bb, gs_uuid_t, &record->uuid);
	gs_byte_buffer_read_str(&bb, (char*)record->name);

	// Serialize asset data 
	gs_result res = out->deserialize(&bb);	
	
	// Default serialization if no serializer provider
	if (res == GS_RESULT_INCOMPLETE)
	{
		object_t::deserialize_default(&bb, out);
	}

	// Free buffer
	gs_byte_buffer_free(&bb);

	return GS_RESULT_SUCCESS;
} 
*/

void asset_recurse_dir(const char* path, DIR* dir)
{
	struct dirent* ent = NULL;
	while ((ent = readdir(dir)) != NULL)
	{
		switch (ent->d_type)
		{
			case DT_REG:
			{
				// Files
				gs_println("reg: %s", ent->d_name);
			} break;

			case DT_DIR:
			{
				// Skip these
				if (gs_string_compare_equal(ent->d_name, ".") || gs_string_compare_equal(ent->d_name, ".."))
				{
					continue;
				}
				
				gs_snprintfc(DIRPATH, ASSET_STR_MAX, "%s/%s", path, ent->d_name);
				gs_println("dir: %s", DIRPATH);
				DIR* cdir = opendir(DIRPATH);
				if (cdir)
				{
					asset_recurse_dir(DIRPATH, cdir);
				}
				closedir(cdir);
			} break;

			case DT_LNK:
			{
				gs_println("link: %s", ent->d_name);
			} break;

			default:
			{
				gs_println("def: %s", ent->d_name);
			} break;
		}
	}
}

// Set root path, begin importing all assets
GS_API_DECL void assets_init(asset_manager_t* am, const char* path)
{
	// Clear all previous records, if necessary
	memcpy(am->root_path, path, ASSET_STR_MAX);

	// Register texture importer
	assets_register_importer(am, texture_t, (&(asset_importer_desc_t){
		.load_resource_from_file = assets_texture_t_load_resource_from_file,
		.file_extensions = {"png", "jpg"},
		.file_extensions_size = 2 * sizeof(char*)
	}));

	// Open directory
	DIR* dir = opendir(path);
	if (!dir) {
		gs_println("error::asset_manager_t::init::could not open path: %s", path);
		return;
	}

    // Recursive through directory structure for assets
	asset_recurse_dir(path, dir);
	// Close dir
	closedir(dir);
}

typedef struct string_split_t 
{
	uint32_t start;
	uint32_t count;
} string_split_t;

GS_API_DECL void asset_qualified_name(const char* src, char* dst, size_t sz)
{
    // Need to remove extension
    uint32_t idx = 0;
    gs_transient_buffer(TMP, 1024);
    for (uint32_t i = gs_string_length(src) - 1; i >= 0; --i) {
        if (src[i] == '.') {
            idx = i;
            break;
        }
    }
    gs_util_string_substring(src, TMP, 1024, 0, idx);

    // Split string
    gs_dyn_array(string_split_t) splits = NULL;
    gs_dyn_array_reserve(splits, 1);

    // Take in delimiter character, then split based on that
    size_t len = gs_string_length(TMP);
    uint32_t start = 0;
	uint32_t rem = 0;
    for (uint32_t i = 0; i < len; ++i)
    {
        if (TMP[i] == '/') {
            string_split_t split = gs_default_val();
			split.start = start;
			split.count = i - start;
            gs_dyn_array_push(splits, split);
            start = i;
			rem = len - start;
        }
    }
	// Push last bit into split
	string_split_t split = {0};
	split.start = start;
	split.count = rem;
	gs_dyn_array_push(splits, split);

    // If no splits, then push back entire string into split
    if (gs_dyn_array_empty(splits)) {
        string_split_t split = gs_default_val();
		split.start = 0; split.count = len;
        gs_dyn_array_push(splits, split);
    }
	else
	{
		// For each split, print
		uint32_t c = 0;
		gs_for_range_i(gs_dyn_array_size(splits))
		{
			string_split_t* s = &splits[i];
			gs_transient_buffer(TMP2, ASSET_STR_MAX);
			gs_transient_buffer(TMP3, ASSET_STR_MAX);
			memcpy(TMP2, (TMP + s->start), s->count);
			gs_util_string_replace(TMP2, TMP3, ASSET_STR_MAX, '/', '.');
			memcpy((dst + c), TMP3, s->count);
			c += s->count;
		}

		// Need the last remainder of the string as well
		gs_dyn_array_free(splits);
	}
}

//=======[ Texture ]==================================================================

GS_API_DECL bool assets_texture_t_load_resource_from_file(const char* path, asset_t* out, void* user_data)
{
	// Need to load up texture data, store in storage (slot array), then return pointer to asset for serialization.
	gs_graphics_texture_desc_t* desc = (gs_graphics_texture_desc_t*)user_data;		

	// Load texture data from file, keep data	
	texture_t* t = (texture_t*)out;
	gs_asset_texture_t* tex = &t->texture;
	bool loaded = gs_asset_texture_load_from_file(path, tex, desc, false, true);

	if (!loaded)
	{
		// Error
		gs_println("error::assets_texture_t_load_resource_from_file:: texture: %s not loaded.", path);
		gs_free(t);
		return false;
	}

	return true;
}

GS_API_DECL gs_result assets_texture_t_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
	const texture_t* t = (texture_t*)in;
	const gs_asset_texture_t* tex = &t->texture;

    // Verify that data is available first in desc
    if (!tex->desc.data) 
	{
        gs_println("error:assets_texture_t::serialize:: texture desc data is NULL");
        return GS_RESULT_FAILURE;
    }

    // Write out desc information
    gs_byte_buffer_write(buffer, uint32_t, tex->desc.width);                                 // Width
    gs_byte_buffer_write(buffer, uint32_t, tex->desc.height);                                // Height
    gs_byte_buffer_write(buffer, gs_graphics_texture_format_type, tex->desc.format);         // Texture Format
    gs_byte_buffer_write(buffer, gs_graphics_texture_wrapping_type, tex->desc.wrap_s);       // S Wrap
    gs_byte_buffer_write(buffer, gs_graphics_texture_wrapping_type, tex->desc.wrap_t);       // T Wrap
    gs_byte_buffer_write(buffer, gs_graphics_texture_filtering_type, tex->desc.min_filter);  // Min Filter Format
    gs_byte_buffer_write(buffer, gs_graphics_texture_filtering_type, tex->desc.mag_filter);  // Mag Filter Format
    gs_byte_buffer_write(buffer, uint32_t, tex->desc.num_mips);                              // Num Mips

    // TODO(): Need to switch on format type to get this information
    uint32_t num_comps = 4;
    size_t sz = tex->desc.width * tex->desc.height * 4;

    // Write out data
    gs_byte_buffer_write_bulk(buffer, tex->desc.data, sz);

	// Serialize texture data
	return GS_RESULT_SUCCESS;
}

GS_API_DECL gs_result assets_texture_t_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
	// Deserialize texture data
	texture_t* t = (texture_t*)out;
	gs_asset_texture_t* tex = &t->texture;

    // Read in desc information
    gs_byte_buffer_read(buffer, uint32_t, &tex->desc.width);                                 // Width
    gs_byte_buffer_read(buffer, uint32_t, &tex->desc.height);                                // Height
    gs_byte_buffer_read(buffer, gs_graphics_texture_format_type, &tex->desc.format);         // Texture Format
    gs_byte_buffer_read(buffer, gs_graphics_texture_wrapping_type, &tex->desc.wrap_s);       // S Wrap
    gs_byte_buffer_read(buffer, gs_graphics_texture_wrapping_type, &tex->desc.wrap_t);       // T Wrap
    gs_byte_buffer_read(buffer, gs_graphics_texture_filtering_type, &tex->desc.min_filter);  // Min Filter Format
    gs_byte_buffer_read(buffer, gs_graphics_texture_filtering_type, &tex->desc.mag_filter);  // Mag Filter Format
    gs_byte_buffer_read(buffer, uint32_t, &tex->desc.num_mips);                              // Num Mips

    // TODO(): Need to switch on format type to get this information
    uint32_t num_comps = 4;
    size_t sz = tex->desc.width * tex->desc.height * 4;

    // Allocate texture data
    tex->desc.data = gs_malloc(sz);

    // Read in texture data
    gs_byte_buffer_read_bulk(buffer, &tex->desc.data, sz);

    // Construct graphics texture handle
    tex->hndl = gs_graphics_texture_create(&tex->desc);

    // Free texture data
    gs_free(tex->desc.data);
	
	return GS_RESULT_SUCCESS;
}

//=======[ Material ]=================================================================

//=======[ Pipeline ]=================================================================

//=======[ Audio ]====================================================================

asset_t* assets_audio_source_t_load_resource_from_file(const char* path, const asset_record_t* record, void* user_data)
{
	// Need to load up data, store in storage (slot array), then return pointer to asset for serialization.
	return NULL;
}

//=======[ Font ]=====================================================================

asset_t* assets_font_t_load_resource_from_file(const char* path, const asset_record_t* record, void* user_data)
{
	// Need to load up data, store in storage (slot array), then return pointer to asset for serialization.
	return NULL;
}

//=======[ Mesh ]=====================================================================

asset_t* assets_mesh_t_load_resource_from_file(const char* path, const asset_record_t* record, void* user_data)
{
	// Need to load up data, store in storage (slot array), then return pointer to asset for serialization.
	return NULL;
}

#endif // ASSET_IMPL
#endif // ASSET_H








