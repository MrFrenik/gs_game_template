#define GS_NO_HIJACK_MAIN
#define GS_IMPL
#include <gs/gs.h>

#define GS_META_IMPL
#include <gs/util/gs_meta.h>

#define LEXER_IMPL
#include "lexer.h"

#define META_STR_MAX 1024

typedef struct meta_enum_t
{
	char* name;
	gs_dyn_array(char*) values;
} meta_enum_t;

typedef struct meta_prop_t 
{
	char* name;
	gs_meta_property_type type;
} meta_prop_t;

typedef struct meta_class_t
{
	char* name;
	char* base;
	gs_dyn_array(meta_prop_t) properties;
} meta_class_t;

typedef struct reflection_data_t
{
	gs_hash_table(uint64_t, meta_class_t) classes;
	gs_hash_table(uint64_t, meta_enum_t) enums;
} reflection_data_t;

void parse_file(reflection_data_t* refl, const char* path)
{
	gs_println("reflecting file: %s", path);

	char* contents = gs_platform_read_file_contents(path, "r", NULL);
	if (contents)
	{
		gs_lexer_t lex = gs_lexer_c_ctor(contents);
		while (lex.can_lex(&lex))
		{
			gs_token_t token = lex.next_token(&lex);
			if (token.type == GS_TOKEN_IDENTIFIER)
			{
				gs_token_debug_print(&token); 
			}
		}
		gs_free(contents);
	}
}

int32_t main(int32_t argc, char** argv)
{
	if (!argc > 1)
	{
		return 0;
	}

	reflection_data_t refl = {0};

	const char* path = argv[1];
	gs_println("path: %s", path);

	parse_file(&refl, "../source/entity.h");

	// Iterate through directory, collect files to open, generate data
	DIR* dir = opendir(path);	
	if (dir)
	{
		struct dirent* ent = NULL;
		while ((ent = readdir(dir)) != NULL)
		{
			switch (ent->d_type)
			{
				case DT_REG: 
				{
					// Files
					gs_transient_buffer(FILE_EXT, 10);
					gs_platform_file_extension(FILE_EXT, 10, ent->d_name); 
					if (gs_string_compare_equal(FILE_EXT, "h"))
					{
						gs_snprintfc(PATH, META_STR_MAX, "%s%s", path, ent->d_name);
						// parse_file(&refl, PATH);
					}
				} break;

				case DT_DIR: 
				{
					// Skip these
					if (gs_string_compare_equal(ent->d_name, ".") || gs_string_compare_equal(ent->d_name, ".."))
					{
						continue;
					}
				
					gs_snprintfc(DIRPATH, META_STR_MAX, "%s/%s", path, ent->d_name);
					gs_println("dir: %s", DIRPATH);
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

		closedir(dir);
	}
}
