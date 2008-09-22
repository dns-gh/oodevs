-- Geometry

local pointMetaTable =
{
    __add = function( l, r )
        return { (l[1] + r[1]), (l[2] + r[2]) }
    end,
    
    __sub = function( l, r )
        return { l[1] - r[1], l[2] - r[2] }
    end,

    __mul = function( l, r )
        local ln, rn = tonumber( l ), tonumber( r )
        return ( ln and { r[1] * ln, r[2] * ln } )
            or ( rn and { l[1] * rn, l[2] * rn } )
    end,

    __div = function( l, r )
        local rn = tonumber( r )
        return { l[1] / rn, l[2] / rn }
    end
}
    
function MakePoint( t )
    return setmetatable( t, pointMetaTable )
end