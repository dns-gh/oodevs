-- Actions

dofile "resources/scripts/xml.lua"

orders =
{
    _BuildParameter = function( v )
        return
        {
            tag = "parameter",
            attributes = { name = v.name, type = v.type, value = v.value },
            children = v[1]
        }
    end,

    _AppendParameters = function( t, result )
        for _, v in ipairs( t or {} ) do
            result[ #result + 1 ] = orders._BuildParameter( v )
        end
    end,

    _DefaultParameters = function( t )
        local parameters =
        {
            { name = "direction dangereuse", type = "Direction", value = t.direction or 360 },
            { name = "Limas",                type = "PhaseLineList" },
            { name = "Limit 1",              type = "Limit", t.limit1 or {} },
            { name = "Limit 2",              type = "Limit", t.limit2 or {} },
            { name = "Renseignements",       type = "IntelligenceList" },
        }
        local result = {}
        orders._AppendParameters( parameters, result )
        return result
    end,

    _ToXml = function( t )
        local parameters = t.mission and orders._DefaultParameters( t ) or {}
        orders._AppendParameters( t.parameters, parameters )
        return xml.Serialize(
            {
                tag = "action",
                attributes = { id = t.mission or t.fragorder, name = "", target = t.target, type = t.mission and "mission" or "fragorder" },
                children = parameters
            }
        )
    end,

    Issue = function( t )
        actions:IssueXmlOrder( orders._ToXml( t ) )
    end,

    _ParameterLocation = function( points, type )
        local result = {}
        for _, v in ipairs( points or {} ) do
            result[ #result + 1 ] = { tag = "point", attributes = { coordinates = coord:ToUtm( v ) } }
        end
        return
        {
            tag = "location",
            attributes = { type = type },
            children = result
        }
    end,

    ParameterLocationPoint = function( point )
        return orders._ParameterLocation( { point }, "point" )
    end,

    ParameterLocationPolygon = function( points )
        return orders._ParameterLocation( points, "polygon" )
    end,

    ParameterLocationLine = function( points )
        return orders._ParameterLocation( points, "line" )
    end,

    ParameterLocationPointList = function( points )
        return orders._ParameterLocation( points, "pointlist" )
    end,

    missions =
    {

    },

    fragOrders =
    {
        ["Pion Appliquer feux"] = 358,
    },

    dotations =
    {
        ["Obus 155 OE CCR F1"] = 22,
        ["Ogre"]            = 23,
        ["Obus 122 OE"]     = 51,
        ["Obus 122 OE MPA"] = 52,
        ["Obus 122 ACED"]   = 53,
    }


-- Exemple d'utilisation
-- orders.Issue(
--                 {
--                     mission = 46,           -- fragorder = tirer.id
--                     target = myUnit:GetIdentifier(),
--                     direction = 42, -- optionnel
--                     parameters =
--                     {
--                         { name = "Position installation", type = "Point", { orders.ParameterLocationPoint( myUnit:GetCoordinates() ) } },
--                     }
--                 }
--             )
}
