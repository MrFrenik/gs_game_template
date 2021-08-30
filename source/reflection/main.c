#define GS_NO_HIJACK_MAIN
#define GS_IMPL
#include <gs/gs.h>

#define GS_META_IMPL
#include <gs/util/gs_meta.h>

#define META_STR_MAX 1024 

typedef struct vtable_func_t
{
    char name[META_STR_MAX];
    char func_ptr[META_STR_MAX];
} vtable_func_t;

typedef struct vtable_t
{
    gs_dyn_array(vtable_func_t) funcs;
} vtable_t;

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
    vtable_t vtable;
} meta_class_t;

typedef struct reflection_data_t
{
	gs_hash_table(uint64_t, meta_class_t) classes;
	gs_hash_table(uint64_t, meta_enum_t) enums;
	gs_hash_table(uint64_t, const char*) type2info;
} reflection_data_t;

void reflection_data_init(reflection_data_t* refl)
{
	gs_hash_table_insert(refl->type2info, gs_hash_str64("uint8_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_U8));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("u8"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_U8));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("int8_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_S8));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("s8"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_S8));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("uint16_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_U16));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("u16"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_U16));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("int16_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_S16));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("uint32_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_U32));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("u32"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_U32));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("int32_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_S32));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("s32"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_S32));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("uint64_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_U64));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("u64"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_U64));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("int64_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_S64));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("s64"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_S64));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("float"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_F32));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("f32"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_F32));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("double"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_F64));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("f64"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_F64));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("gs_vec2"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_VEC2));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("gs_vec3"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_VEC3));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("gs_vec4"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_VEC4));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("gs_mat4"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_MAT4));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("gs_vqs"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_VQS));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("gs_uuid_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_UUID));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("size_t"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_SIZE_T));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("char"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_STR));
	gs_hash_table_insert(refl->type2info, gs_hash_str64("void"), gs_to_str(GS_META_PROPERTY_TYPE_INFO_OBJ));
}

bool prop_is_enum(reflection_data_t* refl, const char* type)
{
	return (gs_hash_table_exists(refl->enums, gs_hash_str64(type)));
}

const char* prop_type_to_type_info(reflection_data_t* refl, const char* type)
{
	uint64_t hash = gs_hash_str64(type);
	if (gs_hash_table_exists(refl->type2info, hash))
	{
		return gs_hash_table_get(refl->type2info, hash);
	}

	return "GS_META_PROPERTY_TYPE_INFO_OBJ";
}

void parse_struct_field(reflection_data_t* refl, meta_class_t* c, gs_lexer_t* lex)
{
	gs_token_t t = lex->current_token;
	
	// Ignore this field
	if (gs_token_compare_text(&t, "ignore"))
	{
		// Move to the semicolon
		gs_lexer_find_next_token_type(lex, GS_TOKEN_SEMI_COLON);
	}
	// Parse 'BASE' tag
	else if (gs_token_compare_text(&t, "base"))
	{
		// Get opening paren
		if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_LPAREN)) gs_assert(false);
		if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_IDENTIFIER)) gs_assert(false);
		t = lex->current_token;
		
		// Now we have the base class
		memcpy(c->base, t.text, t.len);

		// Move to the semicolon
		if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_RPAREN)) gs_assert(false);
	}
    // VTable
    else if (gs_token_compare_text(&t, "vtable")) 
    {
		if (!gs_lexer_find_next_token_type(lex, GS_TOKEN_LPAREN)) gs_assert(false);

        // Just copy everything in vtable description for now (not individual functions)
        if (gs_lexer_peek(lex).type != GS_TOKEN_RPAREN) 
        {
            /*
            typedef struct vtable_func_t
            {
                char name[META_STR_MAX];
                char func_ptr[META_STR_MAX];
            } vtable_func_t;
            */
            vtable_func_t f = {0};
            gs_token_t cur = lex->next_token(lex);
            t = cur;
            bool expect_fp = false;
            while (t.type != GS_TOKEN_RPAREN)
            {
                t = lex->next_token(lex); 
                switch (t.type)
                {
                    case GS_TOKEN_IDENTIFIER: 
                    {
                        if (expect_fp)
                        { 
                            memcpy(f.func_ptr, t.text, t.len); 
                            f.func_ptr[t.len] = '\0';
                            gs_dyn_array_push(c->vtable.funcs, f);
                        }
                        else
                        {
                            memcpy(f.name, t.text, t.len); 
                            f.name[t.len] = '\0';
                        }
                        expect_fp = !expect_fp;
                    } break;

                    case GS_TOKEN_EQUAL: 
                    {
                    } break; 
                } 
            } 
        } 
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

    if (gs_string_compare_equal(path, "../source/app.h") || 
		gs_string_compare_equal(path, "../source/world.h"))
	{
		return;
	}

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
					if (gs_token_compare_text(&token, "introspect"))
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

void write_reflection_file(reflection_data_t* refl, const char* dir)
{
	// Path to write to
	gs_snprintfc(PATH, META_STR_MAX, "%sgenerated.h", dir);

	// Open file, then dump meta information
	FILE* fp = fopen(PATH, "w");
	if (!fp) 
	{
		return;
	}

	// Write out header information
	gs_fprintln(fp, "#ifndef GENERATED_H");	
	gs_fprintln(fp, "#define GENERATED_H\n");	

	// Write out warning information
	gs_fprintln(fp, "/*");	
	gs_fprintln(fp, " * This file has been generated. All modifications will be lost.");	
	gs_fprintln(fp, "*/");	

	// Formatting
	gs_fprintln(fp, "");	
	
	// Functions API
	gs_fprintln(fp, "GS_API_DECL void meta_register_generated(gs_meta_registry_t* meta);");	

	// Implementation
	gs_fprintln(fp, "\n//============[ Implementation ]==============//\n");	

	gs_fprintln(fp, "#ifdef GENERATED_IMPL\n");	

	// Register meta information function implementation
	gs_fprintln(fp, "GS_API_DECL void meta_register_generated(gs_meta_registry_t* meta)");	
	gs_fprintln(fp, "{");	
	{
		// Register enums
		gs_fprintln(fp, "\t// == [ enums ] == \n");	

		for
		(
			gs_hash_table_iter it = gs_hash_table_iter_new(refl->enums);
			gs_hash_table_iter_valid(refl->enums, it);
			gs_hash_table_iter_advance(refl->enums, it)
		)
		{
			meta_enum_t* e = gs_hash_table_iter_getp(refl->enums, it);

			const char* name = e->name;
			uint32_t val_cnt = gs_dyn_array_size(e->values);

			// Write out name as comment
			gs_fprintln(fp, "\t// %s", name);	
			gs_fprintln(fp, "\tgs_meta_enum_register(meta, (&(gs_meta_enum_decl_t) {");	
			{
				gs_fprintln(fp, "\t\t.values = (gs_meta_enum_value_t[]) {");	
				{
					gs_for_range_i(val_cnt)
					{ 
						const meta_enum_val_t* v = &e->values[i];
						gs_fprintf(fp, "\t\t\t(gs_meta_enum_value_t){.name = gs_to_str(%s)}", v->name);	
						if (i < val_cnt - 1)
						{
							gs_fprintf(fp, ",");	
						}
						gs_fprintf(fp, "\n");
					}
				}
				gs_fprintln(fp, "\t\t},");	
				gs_fprintln(fp, "\t\t.size = %zu * sizeof(gs_meta_enum_value_t),", val_cnt);	
				gs_fprintln(fp, "\t\t.name = gs_to_str(%s)", name);	
			}
			gs_fprintln(fp, "\t}));");	
		}

		// Formatting
		gs_fprintln(fp, "");	

		// Register classes
		gs_fprintln(fp, "\t// == [ classes ] == \n");	

		for 
		(
			gs_hash_table_iter it = gs_hash_table_iter_new(refl->classes);
			gs_hash_table_iter_valid(refl->classes, it);
			gs_hash_table_iter_advance(refl->classes, it)
		)
		{
			meta_class_t* c = gs_hash_table_iter_getp(refl->classes, it);

			const char* name = c->name;
			const char* base = c->base;
			uint32_t prop_cnt = gs_dyn_array_size(c->properties);

            // Write out vtable to pass in
			gs_fprintln(fp, "\t// vtable");	
            gs_fprintln(fp, "\tgs_meta_vtable_t %s_vt = gs_default_val();", name);	
            for (uint32_t i = 0; i < gs_dyn_array_size(c->vtable.funcs); ++i)
            {
                vtable_func_t* f = &c->vtable.funcs[i];
                gs_fprintln(fp, "\tgs_hash_table_insert(%s_vt.funcs, gs_hash_str64(gs_to_str(%s)), (void*)%s);", name, f->name, f->func_ptr);	
            }

            // Formatting
			gs_fprintln(fp, "");	

			// Write out name as comment
			gs_fprintln(fp, "\t// %s", name);	
			gs_fprintln(fp, "\tgs_meta_class_register(meta, (&(gs_meta_class_decl_t) {");	
			{
				if (prop_cnt)
				{
					// Write out properties
					gs_fprintln(fp, "\t\t.properties = (gs_meta_property_t[]) {");	
					{
						gs_for_range_i(prop_cnt)
						{
							// Write out property
							meta_prop_t* p = &c->properties[i];

							// Type of property
							const char* type = NULL;
							if (prop_is_enum(refl, p->type))
							{
								type = gs_to_str(GS_META_PROPERTY_TYPE_INFO_ENUM);
							}
							else
							{
								type = prop_type_to_type_info(refl, p->type);
							}

							gs_fprintf(fp, "\t\t\tgs_meta_property(%s, %s, %s, %s)", 
								name, 
								p->type, 
								p->name, 
								type
							);	

							if (i < prop_cnt - 1)
							{
								gs_fprintf(fp, ",");	
							}
							gs_fprintf(fp, "\n");	
						}
					}
					gs_fprintln(fp,  "\t\t},");	
					gs_fprintln(fp,  "\t\t.size = %zu * sizeof(gs_meta_property_t),", prop_cnt);	
				}

				gs_fprintln(fp,  "\t\t.vtable = &%s_vt,", name);	
				gs_fprintln(fp,  "\t\t.name = gs_to_str(%s),", name);	
				gs_fprintln(fp,  "\t\t.base = gs_to_str(%s),", base);	
                gs_fprintln(fp,  "\t\t.cls_size = sizeof(%s)", name);
			}
			gs_fprintln(fp, "\t}));");	

			// Formatting
			gs_fprintln(fp, "");	
		}
	}
	gs_fprintln(fp, "}");	

	// Formatting
	gs_fprintln(fp, "");

	// Footer
	gs_fprintln(fp, "#endif // GENERATED_IMPL");	
	gs_fprintln(fp, "#endif // GENERATED_H");	

	// Close the file
	fclose(fp);
}

int32_t main(int32_t argc, char** argv)
{
	// Init reflection data structure
	reflection_data_t refl = {0};
	reflection_data_init(&refl);

	if (!argc > 1)
	{
		return 0;
	}

	// Iterate through directory, collect files to open, generate data
	const char* path = argv[1];
	const char* output_dir = argv[2];
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
				
					// Skip directories for now...
					gs_snprintfc(DIRPATH, META_STR_MAX, "%s/%s", path, ent->d_name);
					gs_println("dir: %s", DIRPATH);
				} break;

				default:
				{
				} break;
			}
		}

		closedir(dir);
	}

	// Write reflection data to file
	write_reflection_file(&refl, output_dir);
}
