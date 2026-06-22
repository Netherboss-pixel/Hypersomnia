#pragma once
#include "augs/math/vec2.h"
#include "game/cosmos/entity_handle.h"
#include "game/enums/faction_type.h"

struct arena_mode_ai_state;

class cosmos;
class physics_world_cache;

struct ai_character_context {
	const faction_type defusing_faction = faction_type::ATLANTIS;
	arena_mode_ai_state& ai_state;
	const vec2 character_pos;
	const physics_world_cache& physics;
	const cosmos& cosm;
	const entity_handle character_handle;
};
