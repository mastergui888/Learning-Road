local my_lib = require("exe")

--[[
-- exe_30_1
local filter = my_lib.filter

local t = filter({1,3,20,-4,5}, function(x) return x < 5 end)
for _, v in ipairs(t) do
	io.write(string.format("%s ", v))
end
print()
--]]

--[[
-- exe_30_2
local split = my_lib.split
local str = "Hello\0Hello\0Hi:H\0Helll\0222\0\0\0:"
local t =  split(str, ":")
for _, v in ipairs(t) do
	io.write(string.format("\"%s\";len=%d\n", v, #v))
end
--]]

--[[
-- exe_30_3
local transliterate = my_lib.transliterate
local str = "abcdefghijkmn"
local tab = { a = "A", b = "B", d = false, k = "K" }
print(transliterate(str, tab))
--]]

--[[
-- exe_30_4
local set = my_lib.settrans
local get = my_lib.gettrans
local transliterate2 = my_lib.transliterate2
local str = "abcdefghijkmn"
local tab = { a = "A", b = "B", d = false, k = "K" }
set(tab)
set({})
local t = get(tab);
for k, v in pairs(t) do
	print(k, v)
end
print(transliterate2(str))
--]]

-- exe_30_5
local set2 = my_lib.settrans2
local get2 = my_lib.gettrans2
local transliterate3 = my_lib.transliterate3
local str = "abcdefghijkmn"
local t = get2() or {}
print(transliterate3(str))
set2({a = "A", b = "B", d = false, k = "K"})
local t = get2()
print(transliterate3(str))
