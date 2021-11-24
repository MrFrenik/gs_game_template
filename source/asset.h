/*==============================================================================================================
    * Copyright: 2020 John Jackson 
    * Gunslinger Game Template
    * File: asset.h
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

introspect()
typedef enum asset_state
{
	ASSET_STATE_UNLOADED = 0x00,
	ASSET_STATE_LOADED
} asset_state;

// Functions
GS_API_DECL void asset_qualified_name(const char* src, char* dst, size_t sz);

introspect()
typedef struct asset_t
{
    base(object_t)

	// Fields
    uint32_t record_hndl;    // Handle to internal record for asset
	asset_state state;

} asset_t;

introspect()
typedef struct mesh_t 
{
	base(asset_t)
	
	// Fields
	gs_gfxt_mesh_t mesh;

} mesh_t;

introspect()
typedef struct texture_t
{
	base(asset_t)

    serialize({
        return texture_serialize(buffer, this);
    })

    deserialize({
        return texture_deserialize(buffer, this);
    })

	// Fields
    field()
	gs_asset_texture_t texture;

} texture_t; 

GS_API_DECL gs_result texture_serialize(gs_byte_buffer_t* buffer, object_t* in);
GS_API_DECL gs_result texture_deserialize(gs_byte_buffer_t* buffer, object_t* out); 
GS_API_DECL bool texture_t_load_resource_from_file(const char* path, asset_t* out, void* user_data);

introspect()
typedef struct font_t
{
	base(asset_t)

	// Fields
	gs_asset_font_t font;

} font_t;

introspect()
typedef struct audio_source_t
{
    base(asset_t)

	// Fields
	gs_asset_audio_t audio;

} audio_source_t;

GS_API_DECL bool audio_source_t_load_resource_from_file(const char* path, asset_t* out, void* user_data);

introspect()
typedef struct pipeline_t
{
    base(asset_t)

    // Fields
    gs_gfxt_pipeline_t pipeline;

} pipeline_t;

GS_API_DECL gs_gfxt_pipeline_t* get_pipeline_raw(GS_GFXT_HNDL hndl, void* user_data)
{
    pipeline_t* pip = (pipeline_t*)hndl;
    return &pip->pipeline;
}

GS_API_DECL bool pipeline_t_load_resource_from_file(const char* path, asset_t* out, void* user_data);

introspect()
typedef struct material_t
{
	base(asset_t)

    ctor({
        params(pipeline_t* pip),
        func({
            gs_assert(pip);
            this->material = gs_gfxt_material_create(&(gs_gfxt_material_desc_t){
                .pip_func = {
                    .func = get_pipeline_raw,
                    .hndl = pip
                }
            });
            gs_assert(this->material.desc.pip_func.hndl);
        })
    })

	// Fields
	gs_gfxt_material_t material;

} material_t;

// VTable functions
GS_API_DECL gs_result material_t_serialize(gs_byte_buffer_t* buffer, object_t* in);
GS_API_DECL gs_result material_t_deserialize(gs_byte_buffer_t* buffer, object_t* out); 

// Should these be assets as well?
/*
typedef struct material_instance_t
{
    base(asset_t);

    uint32_t parent;            // Asset handle to parent material/instance
    gs_byte_buffer_t overrides; // Override of uniform data (don't want to copy data...)
} material_instance_t;
*/ 

typedef struct asset_record_t
{
	base(asset_t)

	// Fields
	uint32_t hndl;		        // Handle to asset slot array in storage
    char path[ASSET_STR_MAX];   // Absolute path to asset on disk
    gs_uuid_t uuid;             // UUID for asset
    char name[ASSET_STR_MAX];   // Qualified name for asset
} asset_record_t;

#define ASSET_IMPORTER_FILE_EXTENSIONS_MAX 10
#define ASSETS_FILE_EXTENSION_MAX_LENGTH 32

typedef struct asset_importer_desc_t
{
	bool (* load_resource_from_file)(const char* path, asset_t* out, void* user_data);
	char* file_extensions[ASSET_IMPORTER_FILE_EXTENSIONS_MAX];
	size_t file_extensions_size;
    char file_extension[ASSETS_FILE_EXTENSION_MAX_LENGTH];
} asset_importer_desc_t;

typedef struct asset_importer_t
{
	base(object_t)

	gs_slot_array(asset_record_t) records;		// Slot array of asset records
	gs_hash_table(uint64_t, uint32_t) uuid2id;  // Lookup mapping from uuid to record slot id
	gs_hash_table(uint64_t, uint32_t) name2id;  // Lookup mapping from name to record slot id
	gs_slot_array(asset_t*) assets;			    // Slot array of raw asset data (could try to come up with a way to not malloc this)

	bool (* load_resource_from_file)(const char* path, asset_t* out, void* user_data);
	uint64_t asset_cls_id;
	size_t cls_sz;
    char file_extension[ASSETS_FILE_EXTENSION_MAX_LENGTH];
} asset_importer_t;

typedef struct asset_manager_t
{
	base(object_t)

	// Fields
	char root_path[ASSET_STR_MAX];
	gs_slot_array(asset_importer_t*) importers;	    // Slot array of asset data
	gs_hash_table(uint64_t, uint32_t) cid2importer; // Mapping from cls id to importer data
	gs_hash_table(uint64_t, uint32_t) fe2importer;  // Mapping from file extension to importer data

} asset_manager_t;

GS_API_DECL void assets_init(asset_manager_t* am, const char* root_path);
GS_API_DECL const char* assets_get_internal_file_extension(const char* ext);
GS_API_DECL void assets_import(asset_manager_t* am, const char* path, void* user_data, bool serialize_to_disk);
GS_API_DECL gs_result assets_serialize_asset(asset_manager_t* am, const char* path, const asset_t* in);
GS_API_DECL gs_result assets_deserialize_asset(asset_manager_t* am, const char*path, asset_t* out);
GS_API_DECL void* assets_get_data_internal(uint64_t cls_id);
GS_API_DECL  const asset_t* _assets_get_w_name_internal(const asset_manager_t* am, uint64_t cid, const char* name);
GS_API_DECL void _assets_register_importer_internal(asset_manager_t* am, uint64_t cid, size_t cls_sz, asset_importer_desc_t* desc);

#define assets_register_importer(ASSETS, T, DESC)\
	_assets_register_importer_internal(ASSETS, obj_sid(T), sizeof(T), DESC);

#define assets_getp(ASSETS, T, NAME)\
	_assets_get_w_name_internal(ASSETS, gs_hash_str64(gs_to_str(T)), NAME)

// Mechanism for getting asset manager instance
#define asset_manager_instance() _g_asset_manager

#ifdef ASSET_IMPL

// Global instance of asset manager
asset_manager_t* _g_asset_manager = NULL; 

// Maps resource file extension to internal engine file extension
GS_API_DECL const char* assets_get_internal_file_extension(asset_manager_t* am, const char* ext)
{
    // Find importer from file extension mapping
    uint64_t hash = gs_hash_str64(ext);
    bool exists = gs_hash_table_exists(am->fe2importer, hash);
    if (!exists)
    {
        gs_println("warning::asset_manager_t::assets_get_internal_file_extension::%s not registered", ext);
        return "ass"; 
    } 

    // Get importer
    asset_importer_t* importer = gs_slot_array_get(am->importers, gs_hash_table_get(am->fe2importer, hash)); 
    return importer->file_extension; 
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
    memcpy(importer->file_extension, desc->file_extension ? desc->file_extension : "ass", ASSETS_FILE_EXTENSION_MAX_LENGTH);
	uint32_t hndl = gs_slot_array_insert(am->importers, importer);
	gs_hash_table_insert(am->cid2importer, cid, hndl);
	uint32_t ct = desc->file_extensions_size ? desc->file_extensions_size / sizeof(char*) : 0;
	for (uint32_t i = 0; i < ct; ++i)
	{
		gs_hash_table_insert(am->fe2importer, gs_hash_str64(desc->file_extensions[i]), hndl);
	}
}

GS_API_DECL void assets_import(asset_manager_t* am, const char* path, void* user_data, bool serialize_to_disk)	
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
	const char* file_ext = assets_get_internal_file_extension(am, FILE_EXT);
	gs_snprintf(record.path, ASSET_STR_MAX, "%s.%s", FINAL_PATH, file_ext);

       // Generate uuid for asset
	record.uuid = gs_platform_uuid_generate(); 

	// Need to construct asset type here using vtable
    asset_t* asset = obj_newid(id);
	gs_assert(asset); 

	// Construct raw asset (this will also place into storage and give asset the record's handle)
	bool loaded = importer->load_resource_from_file(PATH, asset, user_data); 
	if (loaded)
	{
        gs_println("asset_import::imported %s to %s", path, record.path);

		// Insert into data array
		uint32_t hndl = gs_slot_array_insert(importer->assets, asset);

		// Set up tables
		gs_transient_buffer(UUID_BUF, 34);
		gs_platform_uuid_to_string(UUID_BUF, &record.uuid);
		gs_hash_table_insert(importer->uuid2id, gs_hash_str64(UUID_BUF), hndl);
		gs_hash_table_insert(importer->name2id, gs_hash_str64(record.name), hndl); 

        // Assign asset handle to record
        record.hndl = hndl;

		// Store record in storage
		uint32_t rhndl = gs_slot_array_insert(importer->records, record);

		// Set asset record hndl
		asset->record_hndl = rhndl; 

		// Serialize asset to disk
        if (serialize_to_disk)
        {
		    assets_serialize_asset(am, record.path, asset);
        }
	}
}

GS_API_DECL gs_result assets_serialize_asset(asset_manager_t* am, const char* path, const asset_t* in)
{ 
    // Get class id
    uint64_t id = obj_id(in);

    // Get asset storage based on type of asset
    uint32_t sid = gs_hash_table_get(am->cid2importer, id);
    asset_importer_t* importer = gs_slot_array_get(am->importers, sid);
    const asset_record_t* record = gs_slot_array_getp(importer->records, in->record_hndl);

	gs_byte_buffer_t bb = gs_byte_buffer_new();

	// === Object Header === //
	gs_byte_buffer_write(&bb, uint64_t, id);	// Class id (not sure about this) should write out class name instead?

	// === Asset Header === //
	gs_byte_buffer_write(&bb, gs_uuid_t, record->uuid);
	gs_byte_buffer_write_str(&bb, record->name);

	// Serialize asset data 
    obj_serialize_func serialize = obj_func_w_id(id, obj_serialize);
    gs_result res = (*serialize)(&bb, in);
		
	// Default serialization if no serializer provider
	if (res == GS_RESULT_INCOMPLETE)
	{
        res = object_serialize_default(&bb, in);
	}

	// Write to file
	gs_byte_buffer_write_to_file(&bb, path);

	// Free buffer
	gs_byte_buffer_free(&bb);

	return GS_RESULT_SUCCESS;
}

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
		.load_resource_from_file = texture_t_load_resource_from_file,
		.file_extensions = {"png", "jpg"},
		.file_extensions_size = 2 * sizeof(char*),
        .file_extension = "tex"
	}));

    // Register audio importer
    assets_register_importer(am, audio_source_t, (&(asset_importer_desc_t){
        .load_resource_from_file = audio_source_t_load_resource_from_file,
        .file_extensions = {"ogg", "wav", "mp3"}, 
        .file_extensions_size = 3 * sizeof(char*),
        .file_extension = "aud"
    })); 

    // Register pipeline importer
    assets_register_importer(am, pipeline_t, (&(asset_importer_desc_t){
        .load_resource_from_file = pipeline_t_load_resource_from_file,
        .file_extensions = {"sf"}, 
        .file_extensions_size = 1 * sizeof(char*),
        .file_extension = "pip"
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

GS_API_DECL bool texture_t_load_resource_from_file(const char* path, asset_t* out, void* user_data)
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

GS_API_DECL gs_result texture_serialize(gs_byte_buffer_t* buffer, const object_t* in)
{
    gs_println("SERIALIZE!");
    return GS_RESULT_INCOMPLETE;
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

GS_API_DECL gs_result texture_deserialize(gs_byte_buffer_t* buffer, object_t* out)
{
    gs_println("DESERIALIZE!");
    return GS_RESULT_INCOMPLETE;
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

//=======[ Audio ]====================================================================

GS_API_DECL bool audio_source_t_load_resource_from_file(const char* path, asset_t* out, void* user_data)
{ 
    audio_source_t* audio = (audio_source_t*)out;  
	return gs_asset_audio_load_from_file(path, &audio->audio);
}

//=======[ Font ]=====================================================================

GS_API_DECL bool font_t_load_resource_from_file(const char* path, asset_t* out, void* user_data)
{
	// Need to load up data, store in storage (slot array), then return pointer to asset for serialization.
	font_t* font = (font_t*)out;
	return gs_asset_font_load_from_file(path, &font->font, user_data ? *((uint32_t*)user_data) : 12);
}

//=======[ Mesh ]=====================================================================

GS_API_DECL bool mesh_t_load_resource_from_file(const char* path, asset_t* out, void* user_data)
{
	// Need to load up data, store in storage (slot array), then return pointer to asset for serialization.
	mesh_t* mesh = (mesh_t*)out;
	mesh->mesh = gs_gfxt_mesh_load_from_file(path, user_data);
	return true;
} 

//=======[ Pipeline ]================================================================= 

typedef struct tmp_buffer_t
{
    char txt[1024]; 
} tmp_buffer_t; 

typedef struct shader_out_data_t
{
    char type[64];
    char name[64];
} shader_out_data_t;

typedef struct pipeline_parse_data_t 
{ 
    gs_dyn_array(shader_out_data_t) out_list[2];
    char* code[2];
} ppd_t; 

const char* get_vertex_attribute_string(gs_graphics_vertex_attribute_type type)
{ 
    switch (type)
    {
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT:   return "float"; break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2:  return "vec2";  break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT3:  return "vec3";  break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT4:  return "vec4";  break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_UINT:    return "int";   break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_UINT2:   return "vec2";  break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_UINT3:   return "vec3";  break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_UINT4:   return "vec4";  break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_BYTE:    return "float"; break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_BYTE2:   return "vec2"; break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_BYTE3:   return "vec3"; break;
        case GS_GRAPHICS_VERTEX_ATTRIBUTE_BYTE4:   return "vec4"; break;
        default: return "UNKNOWN"; break;
    }
}

gs_graphics_vertex_attribute_type get_vertex_attribute_from_token(const gs_token_t* t)
{ 
    if (gs_token_compare_text(t, "float"))       return GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT;
    else if (gs_token_compare_text(t, "float2")) return GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2;
    else if (gs_token_compare_text(t, "float3")) return GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT3;
    else if (gs_token_compare_text(t, "float4")) return GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT4;
    else if (gs_token_compare_text(t, "uint4"))  return GS_GRAPHICS_VERTEX_ATTRIBUTE_UINT4;
    else if (gs_token_compare_text(t, "uint3"))  return GS_GRAPHICS_VERTEX_ATTRIBUTE_UINT3;
    else if (gs_token_compare_text(t, "uint2"))  return GS_GRAPHICS_VERTEX_ATTRIBUTE_UINT2;
    else if (gs_token_compare_text(t, "uint"))   return GS_GRAPHICS_VERTEX_ATTRIBUTE_UINT;
    else if (gs_token_compare_text(t, "byte4"))  return GS_GRAPHICS_VERTEX_ATTRIBUTE_BYTE4;
    else if (gs_token_compare_text(t, "byte3"))  return GS_GRAPHICS_VERTEX_ATTRIBUTE_BYTE3;
    else if (gs_token_compare_text(t, "byte2"))  return GS_GRAPHICS_VERTEX_ATTRIBUTE_BYTE2;
    else if (gs_token_compare_text(t, "byte"))   return GS_GRAPHICS_VERTEX_ATTRIBUTE_BYTE;
    return 0x00;
}

gs_graphics_uniform_type uniform_type_from_token(const gs_token_t* t)
{
    if (gs_token_compare_text(t, "float"))      return GS_GRAPHICS_UNIFORM_FLOAT; 
    else if (gs_token_compare_text(t, "int"))   return GS_GRAPHICS_UNIFORM_INT;
    else if (gs_token_compare_text(t, "vec2"))  return GS_GRAPHICS_UNIFORM_VEC2;
    else if (gs_token_compare_text(t, "vec3"))  return GS_GRAPHICS_UNIFORM_VEC3; 
    else if (gs_token_compare_text(t, "vec4"))  return GS_GRAPHICS_UNIFORM_VEC4; 
    else if (gs_token_compare_text(t, "mat4"))  return GS_GRAPHICS_UNIFORM_MAT4; 
    else if (gs_token_compare_text(t, "sampler2D"))  return GS_GRAPHICS_UNIFORM_SAMPLER2D; 
    return 0x00;
}

const char* uniform_string_from_type(gs_graphics_uniform_type type)
{
    switch (type)
    {
        case GS_GRAPHICS_UNIFORM_FLOAT:     return "float"; break; 
        case GS_GRAPHICS_UNIFORM_INT:       return "int"; break;
        case GS_GRAPHICS_UNIFORM_VEC2:      return "vec2"; break;
        case GS_GRAPHICS_UNIFORM_VEC3:      return "vec3"; break; 
        case GS_GRAPHICS_UNIFORM_VEC4:      return "vec4"; break; 
        case GS_GRAPHICS_UNIFORM_MAT4:      return  "mat4"; break;
        case GS_GRAPHICS_UNIFORM_SAMPLER2D: return "sampler2D"; break; 
        default: return "UNKNOWN"; break;
    }
    return 0x00;
}

void parse_uniforms(gs_lexer_t* lex, gs_gfxt_pipeline_desc_t* desc, ppd_t* ppd, gs_graphics_shader_stage_type stage)
{
	if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_LBRACE)) 
    { 
        gs_assert(false);
    } 

    uint32_t bc = 1; 
    while (bc)
    {
        gs_token_t token = lex->next_token(lex);
        switch (token.type)
        { 
            case GS_TOKEN_LBRACE: {bc++;} break; 
            case GS_TOKEN_RBRACE: {bc--;} break;
            
            case GS_TOKEN_IDENTIFIER: 
            {
                gs_gfxt_uniform_desc_t uniform = {0};
                uniform.type = uniform_type_from_token(&token); 
                uniform.stage = stage;

                if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_IDENTIFIER)) 
                { 
                    gs_assert(false);
                } 
                token = lex->current_token;

                memcpy(uniform.name, token.text, token.len);

                // Add uniform to ublock descriptor
                gs_dyn_array_push(desc->ublock_desc.layout, uniform); 
                
            } break;
        } 
    } 
}

void parse_out(gs_lexer_t* lex, gs_gfxt_pipeline_desc_t* desc, ppd_t* ppd, gs_graphics_shader_stage_type type)
{
	if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_LBRACE)) 
    { 
        gs_assert(false);
    } 

    uint32_t bc = 1; 
    while (bc)
    {
        gs_token_t token = lex->next_token(lex);
        switch (token.type)
        { 
            case GS_TOKEN_LBRACE: {bc++;} break; 
            case GS_TOKEN_RBRACE: {bc--;} break;
            
            case GS_TOKEN_IDENTIFIER: 
            {
                shader_out_data_t out = {0};
                memcpy(out.type, token.text, token.len);

                if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_IDENTIFIER)) 
                { 
                    gs_assert(false);
                } 

                token = lex->current_token;
                memcpy(out.name, token.text, token.len);

                switch (type)
                {
                    case GS_GRAPHICS_SHADER_STAGE_VERTEX:
                    {
                        gs_dyn_array_push(ppd->out_list[0], out);
                    } break;

                    case GS_GRAPHICS_SHADER_STAGE_FRAGMENT:
                    {
                        gs_dyn_array_push(ppd->out_list[1], out);
                    } break;
                }
            } break;
        }
    }
}

void parse_code(gs_lexer_t* lex, gs_gfxt_pipeline_desc_t* desc, ppd_t* ppd, gs_graphics_shader_stage_type stage)
{
	if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_LBRACE)) 
    { 
        gs_assert(false);
    } 

    uint32_t bc = 1; 
    gs_token_t cur = lex->next_token(lex);
    gs_token_t token = lex->current_token;
    while (bc)
    {
        switch (token.type)
        { 
            case GS_TOKEN_LBRACE: {bc++;} break; 
            case GS_TOKEN_RBRACE: {bc--;} break; 
        }
        token = lex->next_token(lex);
    }

    const size_t sz = (size_t)(token.text - cur.text - 1);
    char* code = gs_malloc(sz + 1);
    memset(code, 0, sz);
    memcpy(code, cur.text, sz - 1);

    switch (stage)
    {
        case GS_GRAPHICS_SHADER_STAGE_VERTEX: ppd->code[0]   = code; break; 
        case GS_GRAPHICS_SHADER_STAGE_FRAGMENT: ppd->code[1] = code; break;
    }
}

void parse_vertex_attributes(gs_lexer_t* lex, gs_gfxt_pipeline_desc_t* desc, ppd_t* ppd)
{
	if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_LBRACE)) 
    { 
        gs_assert(false);
    } 

    uint32_t bc = 1; 
    while (bc)
    {
        gs_token_t token = lex->next_token(lex);
        switch (token.type)
        { 
            case GS_TOKEN_LBRACE: {bc++;} break; 
            case GS_TOKEN_RBRACE: {bc--;} break;
            
            case GS_TOKEN_IDENTIFIER: {

                gs_graphics_vertex_attribute_desc_t attr = {0}; 
                attr.format = get_vertex_attribute_from_token(&token); 

                if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_IDENTIFIER))
                {
                    gs_assert(false);
                } 

                token = lex->current_token;
                memcpy(attr.name, token.text, token.len);

                // Push back into layout
                gs_dyn_array_push(desc->pip_desc.layout.attrs, attr);

            } break;
        }
    }
}

void parse_shader_stage(gs_lexer_t* lex, gs_gfxt_pipeline_desc_t* desc, ppd_t* ppd, gs_graphics_shader_stage_type stage)
{
	if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_LBRACE)) 
    { 
        gs_println("error::pipeline_t_load_resource_from_file::error parsing shader from .sf resource");
        gs_assert(false);
    } 

    uint32_t bc = 1; 
    while (bc)
    {
        gs_token_t token = lex->next_token(lex);
        switch (token.type)
        { 
            case GS_TOKEN_LBRACE: {bc++;} break; 
            case GS_TOKEN_RBRACE: {bc--;} break;

            case GS_TOKEN_IDENTIFIER:
            {
                if (stage == GS_GRAPHICS_SHADER_STAGE_VERTEX && 
                     gs_token_compare_text(&token, "attributes"))
                {
                    gs_println("parsing attributes...");
                    parse_vertex_attributes(lex, desc, ppd);
                }

                else if (gs_token_compare_text(&token, "uniforms"))
                {
                    parse_uniforms(lex, desc, ppd, stage);
                }

                else if (gs_token_compare_text(&token, "out"))
                {
                    parse_out(lex, desc, ppd, stage);
                }

                else if (gs_token_compare_text(&token, "code"))
                {
                    parse_code(lex, desc, ppd, stage);
                }

            } break;
        }
    }

}

void parse_shader(gs_lexer_t* lex, gs_gfxt_pipeline_desc_t* desc, ppd_t* ppd)
{ 
	if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_LBRACE)) 
    { 
        gs_println("error::pipeline_t_load_resource_from_file::error parsing shader from .sf resource");
        gs_assert(false);
    } 

    // Braces
    uint32_t bc = 1; 
    while (bc)
    {
        gs_token_t token = lex->next_token(lex);
        switch (token.type)
        {
            case GS_TOKEN_LBRACE: {bc++;} break; 
            case GS_TOKEN_RBRACE: {bc--;} break;

            case GS_TOKEN_IDENTIFIER:
            {
                // Vertex shader
                if (gs_token_compare_text(&token, "vertex"))
                {
                    gs_println("parsing vertex shader");
                    parse_shader_stage(lex, desc, ppd, GS_GRAPHICS_SHADER_STAGE_VERTEX);
                }

                // Fragment shader
                else if (gs_token_compare_text(&token, "fragment"))
                {
                    gs_println("parsing fragment shader");
                    parse_shader_stage(lex, desc, ppd, GS_GRAPHICS_SHADER_STAGE_FRAGMENT);
                } 

            } break;
        }
    }
}

void parse_pipeline(gs_lexer_t* lex, gs_gfxt_pipeline_desc_t* desc, ppd_t* ppd)
{ 
    // Get next identifier
    while (lex->can_lex(lex))
    {
        gs_token_t token = lex->next_token(lex);
        switch (token.type)
        {
            case GS_TOKEN_IDENTIFIER:
            {
                if (gs_token_compare_text(&token, "shader"))
                {
                    gs_println("parsing shader");
                    parse_shader(lex, desc, ppd);
                }
            } break;
        }
    }
}

char* pipeline_generate_shader_code(gs_gfxt_pipeline_desc_t* pdesc, ppd_t* ppd, gs_graphics_shader_stage_type stage)
{
    // Shaders
    #ifdef GS_PLATFORM_WEB
        #define _GS_VERSION_STR "#version 300 es\n"
    #else
        #define _GS_VERSION_STR "#version 330 core\n"
    #endif

    // Source code 
    char* src = NULL; 
    uint32_t sidx = 0;

    // Set sidx
    switch (stage)
    {
        case GS_GRAPHICS_SHADER_STAGE_VERTEX:   sidx = 0; break;
        case GS_GRAPHICS_SHADER_STAGE_FRAGMENT: sidx = 1; break;
    }

    const char* shader_header = 
    _GS_VERSION_STR
    "precision mediump float;\n";

    // Generate shader code
    if (ppd->code[sidx])
    {
        const size_t header_sz = (size_t)gs_string_length(shader_header);
        size_t total_sz = gs_string_length(ppd->code[sidx]) + header_sz + 2048;
        src = gs_malloc(total_sz); 
        memset(src, 0, total_sz);
        strncat(src, shader_header, header_sz);
        
        // Attributes
        if (stage == GS_GRAPHICS_SHADER_STAGE_VERTEX)
        {
            for (uint32_t i = 0; i < gs_dyn_array_size(pdesc->pip_desc.layout.attrs); ++i)
            { 
                const char* aname = pdesc->pip_desc.layout.attrs[i].name;
                const char* atype = get_vertex_attribute_string(pdesc->pip_desc.layout.attrs[i].format); 

                gs_snprintfc(ATTR, 64, "layout(location = %zu) in %s %s;\n", i, atype, aname);
                const size_t sz = gs_string_length(ATTR);
                strncat(src, ATTR, sz);
            } 
        }

        // Uniforms
        for (uint32_t i = 0; i < gs_dyn_array_size(pdesc->ublock_desc.layout); ++i)
        { 
            gs_gfxt_uniform_desc_t* udesc = &pdesc->ublock_desc.layout[i]; 

            if (udesc->stage != stage) continue;

            // Need to go from uniform type to string
            const char* utype = uniform_string_from_type(udesc->type);
            const char* uname = udesc->name;
            gs_snprintfc(TMP, 64, "uniform %s %s;\n", utype, uname);
            const size_t sz = gs_string_length(TMP);
            strncat(src, TMP, sz);
        }

        // Out
        for (uint32_t i = 0; i < gs_dyn_array_size(ppd->out_list[sidx]); ++i)
        {
            shader_out_data_t* out = &ppd->out_list[sidx][i];
            const char* otype = out->type;
            const char* oname = out->name;
            gs_snprintfc(TMP, 64, "out %s %s;\n", otype, oname);
            const size_t sz = gs_string_length(TMP);
            strncat(src, TMP, sz); 
        }

        // In
        if (stage == GS_GRAPHICS_SHADER_STAGE_FRAGMENT)
        {
            for (uint32_t i = 0; i < gs_dyn_array_size(ppd->out_list[0]); ++i)
            {
                shader_out_data_t* out = &ppd->out_list[0][i];
                const char* otype = out->type;
                const char* oname = out->name;
                gs_snprintfc(TMP, 64, "in %s %s;\n", otype, oname);
                const size_t sz = gs_string_length(TMP);
                strncat(src, TMP, sz); 
            }
        }

        // Code
        { 
            strncat(src, "void main() {\n", 15); 
            const size_t sz = gs_string_length(ppd->code[sidx]);
            strncat(src, ppd->code[sidx], sz); 
            strncat(src, "}", 1); 
        } 
    } 

    return src;
}

GS_API_DECL bool pipeline_t_load_resource_from_file(const char* path, asset_t* out, void* user_data)
{
    // Cast to pip
    pipeline_t* pip = (pipeline_t*)out;

    // Load file, generate lexer off of file data, parse contents for pipeline information 
    size_t len = 0;
    char* file_data = gs_platform_read_file_contents(path, "r", &len);
    gs_assert(file_data); 

    ppd_t ppd = {0};
    gs_gfxt_pipeline_desc_t pdesc = {0};

	gs_lexer_t lex = gs_lexer_c_ctor(file_data);
    while (lex.can_lex(&lex))
    {
        gs_token_t token = lex.next_token(&lex);
        switch (token.type)
        {
            case GS_TOKEN_IDENTIFIER:
            {
                if (gs_token_compare_text(&token, "pipeline"))
                {
                    parse_pipeline(&lex, &pdesc, &ppd); 
                }
            } break;
        }
    }

    // Generate vertex shader code
    char* v_src = pipeline_generate_shader_code(&pdesc, &ppd, GS_GRAPHICS_SHADER_STAGE_VERTEX);

    // Generate fragment shader code
    char* f_src = pipeline_generate_shader_code(&pdesc, &ppd, GS_GRAPHICS_SHADER_STAGE_FRAGMENT);

    // Construct shader for pdesc 
    pdesc.pip_desc.raster.shader = gs_graphics_shader_create(&(gs_graphics_shader_desc_t){
        .sources = (gs_graphics_shader_source_desc_t[]){ 
            {.type = GS_GRAPHICS_SHADER_STAGE_VERTEX, .source = v_src},
            {.type = GS_GRAPHICS_SHADER_STAGE_FRAGMENT, .source = f_src}
        },
        .size = 2 * sizeof(gs_graphics_shader_source_desc_t),
        .name = path
    });

    // Set up raster
    pdesc.pip_desc.raster.index_buffer_element_size = sizeof(uint32_t); 

    // Set up layout
    pdesc.pip_desc.layout.size = gs_dyn_array_size(pdesc.pip_desc.layout.attrs) * sizeof(gs_graphics_vertex_attribute_desc_t);

    // Set up ublock
    pdesc.ublock_desc.size = gs_dyn_array_size(pdesc.ublock_desc.layout) * sizeof(gs_gfxt_uniform_desc_t); 

    // Free all malloc'd data 
    pip->pipeline = gs_gfxt_pipeline_create(&pdesc);

    gs_free(v_src);
    gs_free(f_src); 
    gs_free(file_data); 

    return true;
}

#endif // ASSET_IMPL
#endif // ASSET_H








