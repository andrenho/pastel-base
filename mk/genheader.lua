#!/usr/bin/env luajit

local filename, format = unpack(arg)

local varname = filename:gsub("/", "_"):gsub("%.", "_"):gsub("-", "_")
local define = varname:upper() .. "_H_"

local filesize = 0
local file = io.open(filename, "rb")
if not file then
   error("Could not open " .. filename)
   os.exit(1)
end
local content = file:read("a")
file:close()

if format == "lua" then
   content = string.dump(assert(load(content)), false)
elseif format == "lua-strip" then
   content = string.dump(assert(load(content)), true)
elseif format ~= null then
   error("Invalid format " .. format)
end

print("#ifndef " .. define)
print("#define " .. define)
print([[

#include <stddef.h>
#include <stdint.h>
]])

print("static const uint8_t " .. varname .. "[] = {")
for i = 1, #content do
   local byte = string.byte(content, i)
   if i % 16 == 1 then io.write("   ") end
   io.write(string.format("0x%02X, ", byte))
   if i % 16 == 0 then print() end
end
print("0x00 };")
print("static const size_t " .. varname .. "_sz = " .. #content .. ";")
print()
print("static const char* " .. varname .. "_str = (const char *) " .. varname .. ";")

print()
print("#endif  // " .. define)