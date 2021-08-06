#ifndef LEXER_H
#define LEXER_H

//==== [ Token ] ============================================================//

typedef enum gs_token_type
{
	GS_TOKEN_UNKNOWN = 0x00,
	GS_TOKEN_LPAREN,
	GS_TOKEN_RPAREN,
	GS_TOKEN_LTHAN, 
	GS_TOKEN_GTHAN, 
	GS_TOKEN_SEMI_COLON,
	GS_TOKEN_COLON,
	GS_TOKEN_COMMA, 
	GS_TOKEN_EQUAL,
	GS_TOKEN_NOT, 
	GS_TOKEN_HASH, 
	GS_TOKEN_PIPE, 
	GS_TOKEN_AMPERSAND, 
	GS_TOKEN_LBRACE, 
	GS_TOKEN_RBRACE, 
	GS_TOKEN_LBRACKET, 
	GS_TOKEN_RBRACKET, 
	GS_TOKEN_MINUS, 
	GS_TOKEN_PLUS, 
	GS_TOKEN_ASTERISK, 
	GS_TOKEN_BSLASH, 
	GS_TOKEN_FSLASH, 
	GS_TOKEN_QMARK, 
	GS_TOKEN_SPACE, 
	GS_TOKEN_NEWLINE, 
	GS_TOKEN_TAB, 
	GS_TOKEN_UNDERSCORE,
	GS_TOKEN_SINGLE_LINE_COMMENT, 
	GS_TOKEN_MULTI_LINE_COMMENT, 
	GS_TOKEN_IDENTIFIER, 
	GS_TOKEN_SINGLE_QUOTE,
	GS_TOKEN_DOUBLE_QUOTE,
	GS_TOKEN_STRING, 
	GS_TOKEN_NUMBER
} gs_token_type;

typedef struct gs_token_t 
{
	const char* text;
	gs_token_type type;
	uint32_t len;
} gs_token_t;

GS_API_DECL gs_token_t gs_token_invalid_token();
GS_API_DECL bool gs_token_compare_type(const gs_token_t* t, gs_token_type type);
GS_API_DECL bool gs_token_compare_text(const gs_token_t* t, const char* match);
GS_API_DECL void gs_token_print_text(const gs_token_t* t);
GS_API_DECL void gs_token_debug_print(const gs_token_t* t);
GS_API_DECL const char* gs_token_type_to_str(gs_token_type type);
GS_API_DECL bool gs_char_is_end_of_line(char c);
GS_API_DECL bool gs_char_is_white_space(char c);
GS_API_DECL bool gs_char_is_alpha(char c);
GS_API_DECL bool gs_char_is_numeric(char c);

//==== [ Lexer ] ============================================================//

typedef struct gs_lexer_t
{
	const char* at;
	const char* contents;
	gs_token_t current_token;
	bool (* can_lex)(struct gs_lexer_t* lex);
	void (* eat_white_space)(struct gs_lexer_t* lex);
	gs_token_t (* next_token)(struct gs_lexer_t*);
} gs_lexer_t;

GS_API_DECL void gs_lexer_set_contents(gs_lexer_t* lex, const char* contents);
GS_API_DECL gs_token_t gs_lexer_current_token(const gs_lexer_t* lex);
GS_API_DECL bool gs_lexer_require_token_text(gs_lexer_t* lex, const char* match);
GS_API_DECL bool gs_lexer_require_token_type(gs_lexer_t* lex, gs_token_type type);
GS_API_DECL bool gs_lexer_current_token_compare_type(const gs_lexer_t* lex, gs_token_type type);
GS_API_DECL gs_token_t gs_lexer_peek(gs_lexer_t* lex);
GS_API_DECL bool gs_lexer_find_next_token_type(gs_lexer_t* lex, gs_token_type type);
GS_API_DECL gs_token_t gs_lexer_advance_before_next_token_type(gs_lexer_t* lex, gs_token_type type);

// C specific functions for lexer
GS_API_DECL gs_lexer_t gs_lexer_c_ctor(const char* contents);
GS_API_DECL bool gs_lexer_c_can_lex(gs_lexer_t* lex);
GS_API_DECL void gs_lexer_c_eat_white_space(gs_lexer_t* lex);
GS_API_DECL gs_token_t gs_lexer_c_next_token(gs_lexer_t* lex);

#ifdef LEXER_IMPL

//==== [ Token ] ============================================================//

GS_API_DECL gs_token_t gs_token_invalid_token()
{
	gs_token_t t = gs_default_val();
	t.text = "";
	t.type = GS_TOKEN_UNKNOWN;
	t.len = 0;
	return t;
}

GS_API_DECL bool gs_token_compare_type(const gs_token_t* t, gs_token_type type)
{
	return (t->type == type);
}

GS_API_DECL bool gs_token_compare_text(const gs_token_t* t, const char* match)
{
	return (gs_string_compare_equal_n(t->text, match, t->len));
}

GS_API_DECL void gs_token_print_text(const gs_token_t* t)
{
	gs_println("%.*s\n", t->len, t->text);
}

GS_API_DECL void gs_token_debug_print(const gs_token_t* t)
{
	gs_println("%s: %.*s", gs_token_type_to_str(t->type), t->len, t->text);
}

GS_API_DECL const char* gs_token_type_to_str(gs_token_type type)
{
	switch (type)
	{
		default:
		case GS_TOKEN_UNKNOWN: return gs_to_str(GS_TOKEN_UNKNOWN); break;
		case GS_TOKEN_LPAREN: return gs_to_str(GS_TOKEN_LPAREN); break;
		case GS_TOKEN_RPAREN: return gs_to_str(GS_TOKEN_RPAREN); break;
		case GS_TOKEN_LTHAN: return gs_to_str(GS_TOKEN_LTHAN); break; 
		case GS_TOKEN_GTHAN: return gs_to_str(GS_TOKEN_GTHAN); break; 
		case GS_TOKEN_SEMI_COLON: return gs_to_str(GS_TOKEN_SEMI_COLON); break;
		case GS_TOKEN_COLON: return gs_to_str(GS_TOKEN_COLON); break;
		case GS_TOKEN_COMMA: return gs_to_str(GS_TOKEN_COMMA); break; 
		case GS_TOKEN_EQUAL: return gs_to_str(GS_TOKEN_EQUAL); break;
		case GS_TOKEN_NOT: return gs_to_str(GS_TOKEN_NOT); break; 
		case GS_TOKEN_HASH:	return gs_to_str(GS_TOKEN_HASH); break; 
		case GS_TOKEN_PIPE: return gs_to_str(GS_TOKEN_PIPE); break; 
		case GS_TOKEN_AMPERSAND:return gs_to_str(GS_TOKEN_AMPERSAND); break; 
		case GS_TOKEN_LBRACE: return gs_to_str(GS_TOKEN_LBRACE); break; 
		case GS_TOKEN_RBRACE: return gs_to_str(GS_TOKEN_RBRACE); break; 
		case GS_TOKEN_LBRACKET: return gs_to_str(GS_TOKEN_LBRACKET); break; 
		case GS_TOKEN_RBRACKET: return gs_to_str(GS_TOKEN_RBRACKET); break; 
		case GS_TOKEN_MINUS: return gs_to_str(GS_TOKEN_MINUS); break; 
		case GS_TOKEN_PLUS: return gs_to_str(GS_TOKEN_PLUS); break; 
		case GS_TOKEN_ASTERISK: return gs_to_str(GS_TOKEN_ASTERISK); break; 
		case GS_TOKEN_BSLASH: return gs_to_str(GS_TOKEN_BLASH); break; 
		case GS_TOKEN_FSLASH: return gs_to_str(GS_TOKEN_FLASH); break; 
		case GS_TOKEN_QMARK: return gs_to_str(GS_TOKEN_QMARK); break;
		case GS_TOKEN_SPACE: return gs_to_str(GS_TOKEN_SPACE); break; 
		case GS_TOKEN_NEWLINE: return gs_to_str(GS_TOKEN_NEWLINE); break;
		case GS_TOKEN_TAB: return gs_to_str(GS_TOKEN_TAB); break;
		case GS_TOKEN_SINGLE_LINE_COMMENT: return gs_to_str(GS_TOKEN_SINGLE_LINE_COMMENT); break;
		case GS_TOKEN_MULTI_LINE_COMMENT: return gs_to_str(GS_TOKEN_MULTI_LINE_COMMENT); break;
		case GS_TOKEN_IDENTIFIER: return gs_to_str(GS_TOKEN_IDENTIFIER); break;
		case GS_TOKEN_NUMBER: return gs_to_str(GS_TOKEN_NUMBER); break;
	}
}

GS_API_DECL bool gs_char_is_null_term(char c)
{
	return (c == '\0');
}

GS_API_DECL bool gs_char_is_end_of_line(char c)
{
	return (c == '\n' || c == '\r');
}

GS_API_DECL bool gs_char_is_white_space(char c)
{
	return (c == '\t' || c == ' ' || gs_char_is_end_of_line(c));
}

GS_API_DECL bool gs_char_is_alpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));	
}

GS_API_DECL bool gs_char_is_numeric(char c)
{
	return (c >= '0' && c <= '9');
}

//==== [ Lexer ] ============================================================//

GS_API_DECL void gs_lexer_set_contents(gs_lexer_t* lex, const char* contents)
{
	lex->at = contents;
	lex->current_token = gs_token_invalid_token();
}

GS_API_DECL bool gs_lexer_c_can_lex(gs_lexer_t* lex)
{
	return (lex->at && !gs_char_is_null_term(*(lex->at)));
}

GS_API_DECL void gs_lexer_c_eat_white_space(gs_lexer_t* lex)
{
	for (;;)
	{
		if (gs_char_is_white_space(*lex->at))
		{
			lex->at++;
		}

		// Single line comment
		else if ((lex->at[0] == '/') && (lex->at[1]) && (lex->at[1] == '/'))
		{
			lex->at += 2;
			while (*lex->at && !gs_char_is_end_of_line(*lex->at))
			{
				lex->at++;
			}
		}

		// Multi-line comment
		else if ((lex->at[0] == '/') && (lex->at[1]) && (lex->at[1] == '*'))
		{
			lex->at += 2;
			while (lex->at[0] && lex->at[1] && !(lex->at[0] == '*' && lex->at[1] == '/'))
			{
				lex->at++;
			}
			if (lex->at[0] == '*')
			{
				lex->at++;
			}
		}

		else
		{
			break;
		}
	}
}

GS_API_DECL gs_token_t gs_lexer_c_next_token(gs_lexer_t* lex)
{
	gs_token_t t = gs_token_invalid_token();
	t.text = lex->at;
	t.len = 1;

	if (lex->can_lex(lex))
	{
		char c = *lex->at;
		switch (c)
		{
			case '(': {t.type = GS_TOKEN_LPAREN; lex->at++;} break;
			case ')': {t.type = GS_TOKEN_RPAREN; lex->at++;} break;
			case '<': {t.type = GS_TOKEN_LTHAN; lex->at++;} break;
			case '>': {t.type = GS_TOKEN_GTHAN; lex->at++;} break;
			case ';': {t.type = GS_TOKEN_SEMI_COLON; lex->at++;} break;
			case ':': {t.type = GS_TOKEN_COLON; lex->at++;} break;
			case ',': {t.type = GS_TOKEN_COMMA; lex->at++;} break;
			case '=': {t.type = GS_TOKEN_EQUAL; lex->at++;} break;
			case '!': {t.type = GS_TOKEN_NOT; lex->at++;} break;
			case '#': {t.type = GS_TOKEN_HASH; lex->at++;} break;
			case '|': {t.type = GS_TOKEN_PIPE; lex->at++;} break;
			case '&': {t.type = GS_TOKEN_AMPERSAND; lex->at++;} break;
			case '{': {t.type = GS_TOKEN_LBRACE; lex->at++;} break;
			case '}': {t.type = GS_TOKEN_RBRACE; lex->at++;} break;
			case '[': {t.type = GS_TOKEN_LBRACKET; lex->at++;} break;
			case ']': {t.type = GS_TOKEN_RBRACKET; lex->at++;} break;
			case '-': {t.type = GS_TOKEN_MINUS; lex->at++;} break;
			case '+': {t.type = GS_TOKEN_PLUS; lex->at++;} break;
			case '*': {t.type = GS_TOKEN_ASTERISK; lex->at++;} break;
			case '\\': {t.type = GS_TOKEN_BSLASH; lex->at++;} break;
			case '?': {t.type = GS_TOKEN_QMARK; lex->at++;} break;
			case ' ': {t.type = GS_TOKEN_SPACE; lex->at++;} break;
			case '\n': {t.type = GS_TOKEN_NEWLINE; lex->at++;} break;
			case '\r': {t.type = GS_TOKEN_NEWLINE; lex->at++;} break;
			case '\t': {t.type = GS_TOKEN_TAB; lex->at++;} break;
			
			case '/':
			{
				// Single line comment
				if ((lex->at[0] == '/') && (lex->at[1]) && (lex->at[1] == '/'))
				{
					lex->at += 2;
					while (lex->at[0] && !gs_char_is_end_of_line(lex->at[0]))
					{
						lex->at++;
					}
					t.len = lex->at - t.text;
					t.type = GS_TOKEN_SINGLE_LINE_COMMENT;
				}

				// Multi line comment
				else if ((lex->at[0] == '/') && (lex->at[1]) && (lex->at[1] == '*'))
				{
					lex->at += 2;
					while (lex->can_lex(lex))
					{
						if (lex->at[0] == '*' && lex->at[1] == '/')
						{
							lex->at += 2;
							break;
						}
						lex->at++;
					}
					t.len = lex->at - t.text;
					t.type = GS_TOKEN_MULTI_LINE_COMMENT;
				}
				// it's just a forward slash
				else
				{
					t.type = GS_TOKEN_FSLASH;
					lex->at++;
				}
		    } break;

			case '"':
			{
				// Move forward after finding first quotation
				lex->at++;

				while (lex->at && *lex->at  != '"')
				{
					if (lex->at[0] == '\\' && lex->at[1])
					{
						lex->at++;
					}
					lex->at++;
				}

				//Move past quotation
				lex->at++;
				t.len = lex->at - t.text;
				t.type = GS_TOKEN_STRING;
			} break;

			// Alpha/Numeric/Identifier
			default: 
			{
				if ((gs_char_is_alpha(c) || c == '_') && c != '-')
				{
					while (
						gs_char_is_alpha(lex->at[0]) || 
						gs_char_is_numeric(lex->at[0]) || 
						lex->at[0] == '_'
					)
					{
						lex->at++;
					}

					t.len = lex->at - t.text;
					t.type = GS_TOKEN_IDENTIFIER;
				}
				else if (gs_char_is_numeric(c) || c == '-')
				{
					uint32_t num_decimals = 0;
					while (
						gs_char_is_numeric(lex->at[0]) || 
						(lex->at[0] == '.' && num_decimals == 0) || 
						lex->at[0] == 'f'
					)
					{
						// Grab decimal
						num_decimals = lex->at[0] == '.' ? num_decimals++ : num_decimals;

						//Increment
						lex->at++;
					}

					t.len = lex->at - t.text;
					t.type = GS_TOKEN_NUMBER;
				}
				else
				{
					t.type = GS_TOKEN_UNKNOWN;
					lex->at++;
				}

			} break;
		}
	}

	// Set current token for lex
	lex->current_token = t;

	return t;
}

GS_API_DECL gs_token_t gs_lexer_current_token(const gs_lexer_t* lex)
{
	return lex->current_token;
}

GS_API_DECL bool gs_lexer_current_token_compare_type(const gs_lexer_t* lex, gs_token_type type)
{
	return (lex->current_token.type == type);
}

GS_API_DECL gs_token_t gs_lexer_peek(gs_lexer_t* lex)
{
	// Store current at and current token
	const char* at = lex->at;
	gs_token_t cur_t = gs_lexer_current_token(lex);

	// Get next token
	gs_token_t next_t = lex->next_token(lex);

	// Reset
	lex->current_token = cur_t;
	lex->at = at;

	// Return
	return next_t;
}

// Check to see if token type of next valid token matches 'match'. Restores place in lex if not.
GS_API_DECL bool gs_lexer_require_token_text(gs_lexer_t* lex, const char* match)
{
	// Store current position and token
	const char* at = lex->at;
	gs_token_t cur_t = lex->current_token;

	// Get next token
	gs_token_t next_t = lex->next_token(lex);

	// Compare token text
	if (gs_token_compare_text(&next_t, match))
	{
		return true;
	}

	// Error
	gs_println("error::gs_lexer_require_token_text::%.*s, expected: %s", cur_t.len, cur_t.text, match);

	// Reset
	lex->at = at;
	lex->current_token = cur_t;

	return false;
}

GS_API_DECL bool gs_lexer_require_token_type(gs_lexer_t* lex, gs_token_type type)
{
	// Store current position and token
	const char* at = lex->at;
	gs_token_t cur_t = lex->current_token;

	// Get next token
	gs_token_t next_t = lex->next_token(lex);

	// Compare token type
	if (gs_token_compare_type(&next_t, type))
	{
		return;
	}

	// Error
	gs_println("error::gs_lexer_require_token_type::%s, expected: %s", gs_token_type_to_str(next_t.type), gs_token_type_to_str(type));

	// Reset
	lex->at = at;
	lex->current_token = cur_t;

	return false;
}

// Advances until next token of given type is found
GS_API_DECL bool gs_lexer_find_next_token_type(gs_lexer_t* lex, gs_token_type type)
{
	gs_token_t t = lex->next_token(lex);
	while (lex->can_lex(lex))
	{
		if (gs_token_compare_type(&t, type))
		{
			return true;
		}
		t = lex->next_token(lex);
	}
	return false;
}

GS_API_DECL gs_token_t gs_lexer_advance_before_next_token_type(gs_lexer_t* lex, gs_token_type type)
{
	gs_token_t t = lex->current_token;
	gs_token_t peek_t = gs_lexer_peek(lex);

	// Continue right up until required token type
	while (!gs_token_compare_type(&peek_t, type))
	{
		t = lex->next_token(lex);
		peek_t = gs_lexer_peek(lex);
	}

	return t;
}

GS_API_DECL gs_lexer_t gs_lexer_c_ctor(const char* contents)
{
	gs_lexer_t lex = gs_default_val();
	lex.contents = contents;
	lex.at = contents;
	lex.can_lex = gs_lexer_c_can_lex;
	lex.eat_white_space = gs_lexer_c_eat_white_space;
	lex.next_token = gs_lexer_c_next_token;
	return lex;
}

#endif // LEXER_IMPL
#endif // LEXER_H











