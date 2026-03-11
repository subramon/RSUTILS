local S = {}
-- Function to serialize a table into a string format that is valid Lua code
S.serialize_tbl = function(o)
    local lua_format = {}
    local lookup = {}

    local function recurse(v, level)
        local str = ""
        local tabs = string.rep("  ", level)
        if type(v) == "number" then
            str = str .. v
        elseif type(v) == "string" then
            str = str .. string.format("%q", v) -- Use %q for proper escaping
        elseif type(v) == "boolean" then
            str = str .. tostring(v)
        elseif type(v) == "table" then
            if lookup[v] then
                error("Table contains cycles or shared references, cannot serialize with this function")
            end
            lookup[v] = true
            str = str .. "{\n"
            for k, val in pairs(v) do
                str = str .. tabs .. "  "
                -- Handle key type: if not a valid identifier, use square brackets
                if type(k) == "number" then
                    str = str .. "[" .. k .. "] = "
                elseif type(k) == "string" and k:match("^[a-zA-Z_][a-zA-Z0-9_]*$") then
                    str = str .. k .. " = "
                else
                    str = str .. "[" .. string.format("%q", k) .. "] = "
                end
                str = str .. recurse(val, level + 1) .. ",\n"
            end
            str = str .. tabs .. "}"
            lookup[v] = nil
        else
            error("Cannot serialize type: " .. type(v))
        end
        return str
    end

    return recurse(o, 0)
end

-- Function to save the serialized table to a file
S.save_tbl = function(file_name, table_data)
    local file = io.open(file_name, "w")
    if file then
        -- Write "return" so the file content can be loaded as a return value
        file:write("return " .. S.serialize_tbl(table_data))
        file:close()
        return true
    end
    return false, "Could not open file for writing."
end

-- Function to load the table from a file
S.load_tbl = function(file_name)
    -- loadfile returns a function that executes the file's content
    local loader = loadfile(file_name)
    if loader then
        -- Execute the function to get the returned table
        return loader()
    end
    return nil, "Could not load file or file does not exist."
end

return S
