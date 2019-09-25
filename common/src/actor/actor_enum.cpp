#include "actor_enum.h"
void luaopen_actor_enum(lua_State* L) {
#define REG_ENUM(e) (lua_pushinteger(L, e), lua_setglobal(L, #e))
//ENUM_ACTOR_PROP
	REG_ENUM(PROP_ID);
	REG_ENUM(PROP_DIR);
	REG_ENUM(PROP_SHOW_BOUNDINGBOX);
	REG_ENUM(PROP_SHOW_AVATAR_INFO);
	REG_ENUM(PROP_IS_AUTORUN);
	REG_ENUM(PROP_IS_DEAD);
	REG_ENUM(PROP_FRAME_SPEED);
	REG_ENUM(PROP_ACTOR_TYPE);
	REG_ENUM(PROP_NAME);
	REG_ENUM(PROP_AVATAR_ID);
	REG_ENUM(PROP_WEAPON_AVATAR_ID);
	REG_ENUM(PROP_SCENE_ID);
	REG_ENUM(PROP_ROLE_ID);
	REG_ENUM(PROP_WEAPON_ID);
	REG_ENUM(PROP_WEAPON_TYPE);
	REG_ENUM(PROP_ACTION_ID);
	REG_ENUM(PROP_POS);
	REG_ENUM(PROP_MOVE_TO_POS);
	REG_ENUM(PROP_DIR_COUNT);
	REG_ENUM(PROP_IS_MOVE);
	REG_ENUM(PROP_MOVE_VELOCITY);
	REG_ENUM(PROP_CAN_MOVE);
	REG_ENUM(PROP_IS_COMBAT);
	REG_ENUM(PROP_TARGET_ID);
	REG_ENUM(PROP_CAST_ID);
	REG_ENUM(PROP_IS_CALC_MOVE_LIST);
	REG_ENUM(PROP_HP);
	REG_ENUM(PROP_MP);
	REG_ENUM(PROP_SAY_DURATION);
	REG_ENUM(PROP_IS_LOCAL);
	REG_ENUM(PROP_COMBAT_POS);
	REG_ENUM(PROP_COMBAT_DIR);
	REG_ENUM(PROP_COMBAT_BACKUP_POS);
	REG_ENUM(PROP_COMBAT_TARGET_POS);
	REG_ENUM(PROP_COMBAT_ACTING);
	REG_ENUM(PROP_TURN_READY);
	REG_ENUM(PROP_ASM_ATK_COMBO);
	REG_ENUM(PROP_ASM_BEHIT_ANIM);
	REG_ENUM(PROP_ASM_DAMAGE);
	REG_ENUM(PROP_ASM_HEAL);
	REG_ENUM(PROP_ASM_BUFF_ANIM);
	REG_ENUM(PROP_ASM_PLAY_BEHIT);
//ENUM_ACTOR_PROP_TYPE
	REG_ENUM(PROP_TYPE_BOOL);
	REG_ENUM(PROP_TYPE_FLOAT);
	REG_ENUM(PROP_TYPE_INT);
	REG_ENUM(PROP_TYPE_STR);
	REG_ENUM(PROP_TYPE_UINT64);
	REG_ENUM(PROP_TYPE_VEC2);
#undef REG_ENUM
};
