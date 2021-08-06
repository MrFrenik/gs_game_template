#define GS_NO_HIJACK_MAIN
#define GS_IMPL
#include <gs/gs.h>

#define GS_META_IMPL
#include <gs/util/gs_meta.h>

#define LEXER_IMPL
#include "lexer.h"

#define META_STR_MAX 512

typedef struct meta_enum_val_t
{
	char name[META_STR_MAX];
} meta_enum_val_t;

typedef struct meta_enum_t
{
	char name[META_STR_MAX];
	gs_dyn_array(meta_enum_val_t) values;
} meta_enum_t;

typedef struct meta_prop_t 
{
	char name[META_STR_MAX];
	gs_meta_property_type type;
} meta_prop_t;

typedef struct meta_class_t
{
	char name[META_STR_MAX];
	char base[META_STR_MAX];
	gs_dyn_array(meta_prop_t) properties;
} meta_class_t;

typedef struct reflection_data_t
{
	gs_hash_table(uint64_t, meta_class_t) classes;
	gs_hash_table(uint64_t, meta_enum_t) enums;
} reflection_data_t;

void parse_struct(reflection_data_t* refl, gs_lexer_t* lex)
{
	// Store current token for lexer
	gs_token_t cur_t = lex->current_token;
}

void parse_enum(reflection_data_t* refl, gs_lexer_t* lex)
{
	// Store current token
	gs_token_t cur_t = lex->current_token;
	gs_token_t t = lex->current_token;
	meta_enum_t e = {0};

	// Find opening brace (if identifier found, store name)
	while (lex->can_lex(lex) && t.type != GS_TOKEN_LBRACE)
	{
		t = lex->next_token(lex);
		switch (t.type)
		{
			case GS_TOKEN_IDENTIFIER:
			{
				memcpy(e.name, t.text, t.len);
			} break;
			default: break;
		}
	}

	// While identifiers are available, print them (these are enum vals), stop at closing brace
	while (lex->can_lex(lex) && t.type != GS_TOKEN_RBRACE)
	{
		t = lex->next_token(lex);
		switch (t.type)
		{
			case GS_TOKEN_IDENTIFIER:
		  	{
				// Add enum val
				meta_enum_val_t v = {0};
				memcpy(v.name, t.text, t.len);
				gs_dyn_array_push(e.values, v);
		  	} break;
			case GS_TOKEN_EQUAL:
			{
				// Need to skip through values, like 0x00
				gs_lexer_find_next_token_type(lex, GS_TOKEN_IDENTIFIER);
			} break;
			default: break;
		}
	}

	// Find semi colon
	while (lex->can_lex(lex) && t.type != GS_TOKEN_SEMI_COLON)
	{
		t = lex->next_token(lex);
		switch (t.type)
		{
			case GS_TOKEN_IDENTIFIER:
			{
				memcpy(e.name, t.text, t.len);
			} break;
		}
	}

	// Add into reflection data
	gs_hash_table_insert(refl->enums, gs_hash_str64(e.name), e);
}

void parse_file(reflection_data_t* refl, const char* path)
{
	gs_println("generating reflection: %s", path);

	char* contents = gs_platform_read_file_contents(path, "r", NULL);
	if (contents)
	{
		gs_lexer_t lex = gs_lexer_c_ctor(contents);
		while (lex.can_lex(&lex))
		{
			gs_token_t token = lex.next_token(&lex);
			switch (token.type)
			{
				case GS_TOKEN_IDENTIFIER:
				{
					if (gs_token_compare_text(&token, "_introspect"))
					{
						// Move to ending parens for introspect tag (can parse tags here)
						gs_lexer_find_next_token_type(&lex, GS_TOKEN_RPAREN);

						// Now to determine if parsing enum or structure
						gs_lexer_find_next_token_type(&lex, GS_TOKEN_IDENTIFIER);

						// If typedefed
						if (gs_token_compare_text(&lex.current_token, "typedef"))
						{
							// Advance to next after typedef
							gs_lexer_find_next_token_type(&lex, GS_TOKEN_IDENTIFIER);
						}

						gs_token_debug_print(&lex.current_token);

						// Struct
						if (gs_token_compare_text(&lex.current_token, "struct"))
						{
							gs_token_t* t = &lex.current_token;
							parse_struct(refl, &lex);
						}
						// Enum
						else if (gs_token_compare_text(&lex.current_token, "enum"))
						{
							gs_token_t* t = &lex.current_token;
							parse_enum(refl, &lex);
						}
					}
				} break;
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
						parse_file(&refl, PATH);
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

	// Now we should have all of our reflection data from these files
	gs_println("Enums: %zu", gs_hash_table_size(refl.enums));
}
