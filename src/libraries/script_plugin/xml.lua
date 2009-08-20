-- Xml

require "table"

xml =
{
	_SortAttributes = function( t )
		local result = {}
		for k, v in pairs( t or {} ) do
			if k ~= nil then
				local i = 1
				while i < #result + 1 and k > result[i] do
					i = i + 1
				end
				table.insert( result, i, k .. "=\"" .. v .. "\"" )
			end
		end
		return result
	end,

    _SerializeAttributes = function( t )
		local attributes = xml._SortAttributes( t )
		local result = ""
		for i, v in ipairs( attributes ) do
			result = result .. " " .. v
		end
        return result
    end,

    Serialize = function( t )
		local rootField = "<" .. t.tag .. xml._SerializeAttributes( t.attributes )
		local result = ""
        for _, child in ipairs( t.children or {} ) do
            result = result .. xml.Serialize( child )
        end
		if result ~= "" then
			return rootField .. ">\n" .. result .. "</" .. t.tag .. ">\n"
		end
        return rootField .. "/>\n"
    end
}
