local function url_encode(str)
    if str then
        str = string.gsub(str, "\n", "\r\n")
        -- Replace unsafe characters with percent-encoding
        str = string.gsub(str, "([^%w %-%._~])", function(c)
            return string.format("%%%02X", string.byte(c))
        end)
        -- Convert spaces to '+' (or change to "%20" depending on your requirements)
        str = string.gsub(str, " ", "+")
    end
    return str
end
return url_encode
