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
	char type[META_STR_MAX];
	bool is_const_ptr;
	uint32_t pointer_count;	
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

void parse_struct_field(reflection_data_t* refl, meta_class_t* c, gs_lexer_t* lex)
{
	gs_token_t t = lex->current_token;
	
	// Ignore this field
	if (gs_token_compare_text(&t, "_ignore"))
	{
		// Move to the semicolon
		gs_lexer_find_next_token_type(lex, GS_TOKEN_SEMI_COLON);
	}
	// Parse 'BASE' tag
	else if (gs_token_compare_text(&t, "BASE"))
	{
		// Get opening paren
		if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_LPAREN)) gs_assert(false);
		if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_IDENTIFIER)) gs_assert(false);
		t = lex->current_token;
		
		// Now we have the base class
		memcpy(c->base, t.text, t.len);

		// Move to the semicolon
		if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_SEMI_COLON)) gs_assert(false);
	}
	// Otherwise, it's a field
	else
	{
		meta_prop_t p = {0};
		bool set_type = false;

		// Need to deal with any qualifier shit 
		if (gs_token_compare_text(&t, "const"))
		{
			p.is_const_ptr = true;
			if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_IDENTIFIER)) gs_assert(false);
		}

		// Get next field, require identifier for field type
		t = lex->current_token;
		memcpy(p.type, t.text, t.len);

		// Do remainder of field (account for asterisks)
		t = lex->next_token(lex);
		while (t.type != GS_TOKEN_IDENTIFIER)
		{
			t = lex->next_token(lex);
			switch (t.type)
			{
				case GS_TOKEN_ASTERISK:
				{
					p.pointer_count++;
				} break;
			}
		}

		// Require identifier for field name
		t = lex->current_token;
		memcpy(p.name, t.text, t.len);

		// Require semi colon
		if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_SEMI_COLON)) gs_assert(false);

		// Add to class properties
		gs_dyn_array_push(c->properties, p);
	}
}

void parse_struct(reflection_data_t* refl, gs_lexer_t* lex)
{
	// Store current token for lexer
	gs_token_t t = lex->current_token;
	meta_class_t cls = {0};

	// Find opening brace (if identifier found, store name)
	while (lex->can_lex(lex) && t.type != GS_TOKEN_LBRACE)
	{
		t = lex->next_token(lex);
		switch (t.type)
		{
			case GS_TOKEN_IDENTIFIER:
			{
				memcpy(cls.name, t.text, t.len);
			} break;
		}
	}

	// Parse to end of block
	while (lex->can_lex(lex) && t.type != GS_TOKEN_RBRACE)
	{
		t = lex->next_token(lex);
		switch (t.type)
		{
			// Parse all fields
			case GS_TOKEN_IDENTIFIER: 
			{
				parse_struct_field(refl, &cls, lex);
			} break;
		}
	}

	// Parse to semi colon (if identifier found, store name)
	while (lex->can_lex(lex) && t.type != GS_TOKEN_SEMI_COLON)
	{
		t = lex->next_token(lex);
		switch (t.type)
		{
			case GS_TOKEN_IDENTIFIER:
			{
				memcpy(cls.name, t.text, t.len);
			} break;
		}
	}

	// Add to reflection data
	gs_hash_table_insert(refl->classes, gs_hash_str64(cls.name), cls);
}

void parse_enum(reflection_data_t* refl, gs_lexer_t* lex)
{
	// Store current token
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

	// Print enums
	// Now we should have all of our reflection data from these files
	gs_println("Enums: %zu", gs_hash_table_size(refl.enums));

	for (
		gs_hash_table_iter it = gs_hash_table_iter_new(refl.enums);
		gs_hash_table_iter_valid(refl.enums, it);
		gs_hash_table_iter_advance(refl.enums, it)
	)
	{
		meta_enum_t* e = gs_hash_table_iter_getp(refl.enums, it);
		gs_println("e: %s", e->name);
		gs_for_range_i(gs_dyn_array_size(e->values))
		{
			gs_println("\tv: %s", e->values[i].name);
		}
	}

	// Print classes
	gs_println("Classes: %zu", gs_hash_table_size(refl.classes));
	for (
		gs_hash_table_iter it = gs_hash_table_iter_new(refl.classes);
		gs_hash_table_iter_valid(refl.classes, it);
		gs_hash_table_iter_advance(refl.classes, it)
	)
	{
		meta_class_t* c = gs_hash_table_iter_getp(refl.classes, it);
		gs_println("c: %s", c->name);
		gs_for_range_i(gs_dyn_array_size(c->properties))
		{
			meta_prop_t* p = &c->properties[i];
			gs_println("\tp: %s %s, cp: %zu, pc: %zu", p->type, p->name, p->is_const_ptr, p->pointer_count);
		}
	}

}
