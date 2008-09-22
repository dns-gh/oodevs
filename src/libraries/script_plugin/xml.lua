-- Xml

xml = 
{
    _SerializeAttributes = function( t )
        local result = " "
        for k, v in pairs( t or {} ) do
            result = result .. k .. "='" .. v .. "' "
        end
        return result
    end,
    
    Serialize = function( t )
        local result = "<" .. t.tag .. xml._SerializeAttributes( t.attributes ) .. ">\n"
        for _, child in ipairs( t.children or {} ) do
            result = result .. xml.Serialize( child )
        end
        result = result .. "</" .. t.tag .. ">\n"
        return result
    end
}
    