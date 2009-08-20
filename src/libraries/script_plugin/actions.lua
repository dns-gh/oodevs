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

function Location.create( type )
	local new = {}
	setmetatable( new, Location )
	new.type = type
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
-- Point
--------------------------------------------------------------------------------
Point = {}
Point.__index = Point

function Point.create( coordinates )
	local new = Location.create( "point" )
	new:AddPoint( coordinates )
	return new
end

--------------------------------------------------------------------------------
-- Polygon
--------------------------------------------------------------------------------
Polygon = {}
Polygon.__index = Polygon

function Polygon.create()
	return Location.create( "polygon" )
end

--------------------------------------------------------------------------------
-- Line
--------------------------------------------------------------------------------
Line = {}
Line.__index = Line

function Line.create()
	return Location.create( "line" )
end

--------------------------------------------------------------------------------
-- PointList
--------------------------------------------------------------------------------
PointList = {}
PointList.__index = PointList

function PointList.create()
	return Location.create( "pointlist" )
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
			return self
		end
	end
	self.parameters[#self.parameters + 1] = { name = t.name, type = t.type, value = t.value, children = t.children or {} }
	return self
end

-- Xml serialization
function Order:ToXml()
	return xml.Serialize(
		{
			tag = "action",
			attributes = { id = self.id, name = "", target = self.target, type = self.type },
			children = Order._MakeXmlParameters( self.parameters )
		}
	)
end

-- Parameter serialization tools
function Order._MakeXmlParameter( parameter )
return {
	tag = "parameter",
	attributes = { name = parameter.name, type = parameter.type, value = parameter.value },
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
	local new = Order.create( target, id, "mission" )
		:With( { name = "Danger direction", type = "Direction", value = 360 } )
		:With( { name = "Phase lines",      type = "PhaseLineList" } )
		:With( { name = "Boundary limit 1", type = "Limit" } )
		:With( { name = "Boundary limit 2", type = "Limit" } )
		:With( { name = "Intelligences",    type = "IntelligenceList" } )
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
