-- *****************************************************************************
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
--
-- *****************************************************************************

--------------------------------------------------------------------------------
--
-- Orders
--
-- Provides tools for creating missions and fragmentary orders
--
--------------------------------------------------------------------------------

dofile "resources/scripts/xml.lua"

--------------------------------------------------------------------------------
-- Location
--------------------------------------------------------------------------------
Location = {}
Location.__index = Location

function Location.create( type, maxOccurs )
    local new = {}
    setmetatable( new, Location )
    new.type = type
    new[ "min-occurs" ] = "1"
    new[ "max-occurs" ] = maxOccurs
    new.points = {}
    return new
end

function Location:AddPoint( coordinates )
    local point = coordinates
    if #coordinates == 2 then
        point = coord:ToUtm( coordinates )
    end
    self.points[#self.points + 1] = point
end

function Location:ToXml()
    local points = {}
    for _, v in ipairs( self.points or {} ) do
        points[ #points + 1 ] = { tag = "point", attributes = { coordinates = v } }
    end
    return
    {
        tag = "location",
        attributes = { type = self.type },
        children = points
    }
end

--------------------------------------------------------------------------------
-- LocationParameter
--------------------------------------------------------------------------------
LocationParameter = {}
LocationParameter.__index = LocationParameter

function LocationParameter.create( name, points )
    local new = {}
    setmetatable( new, LocationParameter )
    new.type = "Location"
    new.name = name
    new.children = { Location.create( "polygon", "1" ) }
    for _,v in ipairs( points ) do
        new.children[1]:AddPoint( v )
    end
    return new
end

--------------------------------------------------------------------------------
-- Point
--------------------------------------------------------------------------------
Point = {}
Point.__index = Point

function Point.create( coordinates )
    local new = Location.create( "point", "1" )
    new:AddPoint( coordinates )
    return new
end

--------------------------------------------------------------------------------
-- Polygon
--------------------------------------------------------------------------------
Polygon = {}
Polygon.__index = Polygon

function Polygon.create()
    return Location.create( "polygon", "1" )
end

function Polygon.create( name, type, points )
    local new = {}
    setmetatable( new, Polygon )
    new.type = type
    new.name = name
    new.children = { Location.create( "polygon", "1" ) }
    for _,v in ipairs( points ) do
        new.children[1]:AddPoint( v )
    end
    return new
end

--------------------------------------------------------------------------------
-- PolygonList
--------------------------------------------------------------------------------
PolygonList = {}
PolygonList.__index = PolygonList

function PolygonList.create( name, points )
    local new = {}
    setmetatable( new, PolygonList )
    new.type = "Polygon"
    new[ "min-occurs" ] = "1"
    new[ "max-occurs" ] = "unbounded"
    new.name = name
    new.children = { Polygon.create( "Position 1", "location", points ) }
    return new
end

--------------------------------------------------------------------------------
-- GenObject
--------------------------------------------------------------------------------
GenObject = {}
GenObject.__index = GenObject

function GenObject.create( name, points )
    local new = {}
    setmetatable( new, GenObject )
    new.type = "obstacle"
    new.value = "abattis"
    new.name = name
    new.children = {}
    new.children[1] = { name="Obstacle type", type="obstacletype", value="0" }
    new.children[2] = Polygon.create( "Location", "location", points )
    return new
end

--------------------------------------------------------------------------------
-- GenObjectList
--------------------------------------------------------------------------------
GenObjectList = {}
GenObjectList.__index = GenObjectList

function GenObjectList.create( name, points )
    local new = {}
    setmetatable( new, GenObjectList )
    new.type = "GenObject"
    new[ "min-occurs" ] = "1"
    new[ "max-occurs" ] = "unbounded"
    new.name = name
    new.children = { GenObject.create( "Obstacle 1", points ) }
    return new
end

--------------------------------------------------------------------------------
-- AgentList
--------------------------------------------------------------------------------
AgentList = {}
AgentList.__index = AgentList

function AgentList.create( name, type, agents )
    local new = {}
    setmetatable( new, AgentList )
    new.type = type
    new[ "min-occurs" ] = "1"
    new[ "max-occurs" ] = "unbounded"
    new.name = name
    new.children = {}
    for _, v in ipairs( agents ) do
        new.children[#new.children + 1] = { name = name, type = "agent", value = v }
    end
    return new
end

--------------------------------------------------------------------------------
-- AgentKnowledgeList
--------------------------------------------------------------------------------
AgentKnowledgeList = {}
AgentKnowledgeList.__index = AgentKnowledgeList

function AgentKnowledgeList.create( name, type, agentKnowledges )
    local new = {}
    setmetatable( new, AgentKnowledgeList )
    new.type = type
    new[ "min-occurs" ] = "1"
    new[ "max-occurs" ] = "unbounded"
    new.name = name
    new.children = {}
    for _, v in ipairs( agentKnowledges ) do
        new.children[#new.children + 1] = { name = name, type = "agentknowledge", value = v }
    end
    return new
end


--------------------------------------------------------------------------------
-- Line
--------------------------------------------------------------------------------
Line = {}
Line.__index = Line

function Line.create()
    return Location.create( "line", "1" )
end

--------------------------------------------------------------------------------
-- PointList
--------------------------------------------------------------------------------
PointList = {}
PointList.__index = PointList

function PointList.create()
    return Location.create( "point", "unbounded" )
end

--------------------------------------------------------------------------------
-- Path
--------------------------------------------------------------------------------
Path = {}
Path.__index = Path

function Path.create( name )
    local new = {}
    setmetatable( new, Path )
    new.name = name
    new.type = "Path"
    new.children = {}
    return new
end

function Path:AddPoint( name, coordinates )
    self.children[#self.children + 1] = { name = name, type = "pathpoint", children = { Point.create( coordinates ) } }
    return self
end

--------------------------------------------------------------------------------
-- PointLocation
--------------------------------------------------------------------------------
PointLocation = {}
PointLocation.__index = PointLocation

function PointLocation.create( name, coordinates, type )
    local new = {}
    setmetatable( new, PointLocation )
    new.name = name
    new.type = type
    new.children = { Point.create( coordinates ) }
    return new
end

--------------------------------------------------------------------------------
-- Order
--------------------------------------------------------------------------------
Order = {}
Order.__index = Order

-- Constructor
function Order.create( target, id, type )
    local new = {}
    setmetatable( new, Order )
    new.id = id
    new.target = target
    new.type = type
    new.parameters = {}
    return new
end

-- Definition of parameter
function Order:With( t )
    for i, v in ipairs( self.parameters or {} ) do
        if v.name == t.name then
            self.parameters[i] = { name = t.name, type = t.type, value = t.value, children = t.children or {} }
            self.parameters[i][ "min-occurs" ] = t[ "min-occurs" ]
            self.parameters[i][ "max-occurs" ] = t[ "max-occurs" ]
            return self
        end
    end
    local newIndex = #self.parameters + 1
    self.parameters[newIndex] = { name = t.name, type = t.type, value = t.value, children = t.children or {} }
    self.parameters[newIndex][ "min-occurs" ] = t[ "min-occurs" ]
    self.parameters[newIndex][ "max-occurs" ] = t[ "max-occurs" ]
    return self
end

-- Xml serialization
function Order:ToXml()
    local attributesTable = { id = self.id, name = "", target = self.target, type = self.type, time = 0 }
    attributesTable[ "min-occurs" ] = self[ "min-occurs" ]
    attributesTable[ "max-occurs" ] = self[ "max-occurs" ]
    return xml.Serialize(
        {
            tag = "action",
            attributes = attributesTable,
            children = Order._MakeXmlParameters( self.parameters )
        }
    )
end

-- Parameter serialization tools
function Order._MakeXmlParameter( parameter )
    local attributesTable = { name = parameter.name, type = parameter.type, value = parameter.value }
    attributesTable[ "min-occurs" ] = parameter[ "min-occurs" ]
    attributesTable[ "max-occurs" ] = parameter[ "max-occurs" ]
return {
    tag = "parameter",
    attributes = attributesTable,
    children = Order._MakeXmlParameters( parameter.children )
}
end

function Order._MakeXmlParameters( parameters )
    local result = {}
    for _, v in ipairs( parameters or {} ) do
        if v.ToXml ~= nil then
            result[#result + 1] = v:ToXml()
        else
            result[#result + 1] = Order._MakeXmlParameter( v )
        end
    end
    return result
end

-- Order execution
function Order:Issue()
    actions:IssueXmlOrder( self:ToXml() )
end

--------------------------------------------------------------------------------
-- Mission
-- Order with default parameters
--------------------------------------------------------------------------------
Mission = {}
Mission.__index = Mission

function Mission.create( target, id )
    local phaseLineParameter = { name = "Phase lines", type = "PhaseLine" }
    phaseLineParameter[ "min-occurs" ] = "1"
    phaseLineParameter[ "max-occurs" ] = "unbounded"
    local new = Order.create( target, id, "mission" )
        :With( { name = "Danger direction", type = "Heading", value = 360 } )
        :With( phaseLineParameter )
        :With( { name = "Boundary limit 1", type = "Limit" } )
        :With( { name = "Boundary limit 2", type = "Limit" } )
    return new
end

--------------------------------------------------------------------------------
-- FragOrder
--------------------------------------------------------------------------------
FragOrder = {}
FragOrder.__index = FragOrder

function FragOrder.create( target, id )
    return Order.create( target, id, "fragorder" )
end
