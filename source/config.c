/*
 *  Main configurtion file for a single file build.
 */

#define GS_IMPL
#include <gs/gs.h>

#define GS_IMMEDIATE_DRAW_IMPL
#include <gs/util/gs_idraw.h> 

#define GS_GFXT_IMPL
#include <gs/util/gs_gfxt.h>

#define GS_META_IMPL
#include <gs/util/gs_meta.h>

#define GS_PHYSICS_IMPL
#include <gs/util/gs_physics.h>

#define LEXER_IMPL
#include "lexer.h"

#define CONTAINERS_IMPL
#include "containers.h"

#define OBJECT_IMPL
#include "object.h"

#define ASSET_IMPL
#include "asset.h"

#define GRAPHICS_IMPL
#include "graphics.h"

#define COMPONENT_IMPL
#include "component.h"

#define ENTITY_IMPL
#include "entity.h"

#define PLAYER_IMPL
#include "player.h"

#define APP_IMPL
#include "app.h"
