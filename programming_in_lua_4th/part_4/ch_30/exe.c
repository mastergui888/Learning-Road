#include <stdio.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static int l_filter(lua_State *L)
{
	/* 1st argument must be a table */
	luaL_checktype(L, 1, LUA_TTABLE);	
	/* 2nd argument must be a function */
	luaL_checktype(L, 2, LUA_TFUNCTION);

	lua_newtable(L);

	int ct = 1;
	int len = luaL_len(L, 1);
	for (int i = 1; i <= len; i++) {
		lua_rawgeti(L, 1, i);
		lua_pushvalue(L, 2);
		lua_pushvalue(L, -2);
		lua_call(L, 1, 1);
		if (lua_toboolean(L, -1)) {
			lua_pop(L, 1);
			lua_rawseti(L, 3, ct++);
		} else {
			lua_pop(L, 2);
		}
	}
	
	return 1;	
}

static int l_split(lua_State *L)
{
	size_t len;
	const char *s = luaL_checklstring(L, 1, &len);
	const char *sep = luaL_checkstring(L, 2);
	const char *e;
	const char *d = s + len;
	int i = 1;	

	lua_newtable(L);

	while ((e = memchr(s, sep[0], d - s)) != NULL) {
		lua_pushlstring(L, s, e - s);
		lua_rawseti(L, -2, i++);
		s = e + 1;
	}
	lua_pushlstring(L, s, d - s);
	lua_rawseti(L, -2, i);

	return 1;
}

static int l_transliterate(lua_State *L)
{
	size_t len;
	/* 1st argument must be a string */
	const char *s = luaL_checklstring(L, 1, &len);
	/* 2nd argument must be a table */
	luaL_checktype(L, 2, LUA_TTABLE);

	luaL_Buffer b;
	char *p = luaL_buffinitsize(L, &b, len);
	int j = 0;
	for (int i = 0; i < len; i++) {
		lua_pushlstring(L, s + i, 1);
		int type = lua_gettable(L, 2);
		if (type == LUA_TSTRING) {
			p[j++] = lua_tostring(L, -1)[0];
		} else if (type == LUA_TNIL) {
			p[j++] = s[i];
		}
		lua_pop(L, 1);
	}
	luaL_pushresultsize(&b, j);
	return 1;
}

static int ref = LUA_NOREF;

static int l_settrans(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	if (ref != LUA_NOREF) {
		luaL_unref(L, LUA_REGISTRYINDEX, ref);
	}
	ref = luaL_ref(L, LUA_REGISTRYINDEX);
	return 0;
}

static int l_gettrans(lua_State *L)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	return 1;		
}

static int l_transliterate2(lua_State *L)
{
	size_t len;
	/* 1st argument must be a string */
	const char *s = luaL_checklstring(L, 1, &len);

	if (ref == LUA_NOREF) {
		lua_newtable(L);
	} else {
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	}

	luaL_Buffer b;
	char *p = luaL_buffinitsize(L, &b, len);
	int j = 0;
	for (int i = 0; i < len; i++) {
		lua_pushlstring(L, s + i, 1);
		int type = lua_gettable(L, 2);
		if (type == LUA_TSTRING) {
			p[j++] = lua_tostring(L, -1)[0];
		} else if (type == LUA_TNIL) {
			p[j++] = s[i];
		}
		lua_pop(L, 1);
	}
	luaL_pushresultsize(&b, j);
	return 1;
}

static int l_settrans2(lua_State *L)
{	
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_setfield(L, lua_upvalueindex(1), "trans");
	return 0;
}

static int l_gettrans2(lua_State *L)
{
	lua_getfield(L, lua_upvalueindex(1), "trans");
	return 1;
}

static int l_transliterate3(lua_State *L)
{
	size_t len;
	/* 1st argument must be a string */
	const char *s = luaL_checklstring(L, 1, &len);

	l_gettrans2(L);
	if (!lua_istable(L, 2)) {
		lua_pop(L, 1);
		return 1;
	}

	luaL_Buffer b;
	char *p = luaL_buffinitsize(L, &b, len);
	int j = 0;
	for (int i = 0; i < len; i++) {
		lua_pushlstring(L, s + i, 1);
		int type = lua_gettable(L, 2);
		if (type == LUA_TSTRING) {
			p[j++] = lua_tostring(L, -1)[0];
		} else if (type == LUA_TNIL) {
			p[j++] = s[i];
		}
		lua_pop(L, 1);
	}
	luaL_pushresultsize(&b, j);
	return 1;
}

static const struct luaL_Reg my_lib[] = {
	{ "filter", l_filter },
	{ "split",  l_split }, 
	{ "transliterate", l_transliterate },
	{ "settrans", l_settrans },
	{ "gettrans", l_gettrans },
	{ "transliterate2", l_transliterate2 },
	{ "settrans2", l_settrans2 },
	{ "gettrans2", l_gettrans2 },
	{ "transliterate3", l_transliterate3 },
	{ NULL,		NULL }
};

int luaopen_exe(lua_State *L)
{
//	luaL_newlib(L, my_lib);

//	just for exe_30_6
//	Ref: https://stackoverflow.com/questions/46021982/lua-shared-upvalue-example-in-c
	luaL_newlibtable(L, my_lib);
	lua_newtable(L);
	luaL_setfuncs(L, my_lib, 1);
	return 1;
}
