--Default Geometry Implementation

--- Returns a position translated by a direction at a given distance
-- @param position Starting simulation position
-- @param direction Simulation direction (vector)
-- @param dist Float, the distance of the translation
-- @return Translated simulation position
integration.positionTranslateDir = function ( position, direction, dist )
    return DEC_Geometrie_PositionTranslateDir( position, direction, dist )
end

--- Returns a list of points at the border of the area
-- @param location Simulation area
-- @return List of simulation positions
integration.locationInPoints = function( location )
    return DEC_Geometrie_ListePointsLocalisation( location )
end

--- Returns true if the provided position is in the area of responsibility,
--- returns false otherwise
-- @param areaOfResponsibility Simulation area of responsibility
-- @param position Simulation position
-- @return Boolean, whether or not the position is in the AOR
integration.isPointInAOR_WithParam = function( areaOfResponsibility, position )
    return DEC_Geometrie_EstPointDansFuseau_AvecParamFuseau( areaOfResponsibility, position )
end

--- Returns a position at the end of the provided area of responsibility
-- @param areaOfResponsibility Simulation area of responsibility
-- @return Simulation position at the end of the AOR
integration.computeArrivedPointForAOR = function( areaOfResponsibility )
    return DEC_Geometrie_CalculerPointArriveePourFuseau( areaOfResponsibility )
end

--- Returns the barycenter of the provided area
-- Note that the barycenter may be outside of the area
-- @param location Simulation area
-- @return Simulation position
integration.computeLocationBarycenter = function( location )
    return DEC_Geometrie_CalculerBarycentreLocalisation( location )
end

--- Returns the barycenter of the provided list of points
-- @param listPoints List of simulation positions
-- @return Simulation position
integration.computePointsListBarycenter = function( listPoints )
    return DEC_Geometrie_CalculerBarycentreListePoints( listPoints )
end

--- Returns a position on the middle limit at the provided advance
--- along the area of responsibility.
-- @param areaOfResponsibility Simulation area of responsibility
-- @param advance Float, the advance along the AOR
-- @return Simulation position
integration.computePointOnAOR = function( areaOfResponsibility, advance )
    return DEC_Geometrie_CalculerPointSurFuseau( areaOfResponsibility, advance )
end

--- Returns a list of all road intersections with the border of the given area
-- Returns an empty list if there are none
-- @param area Simulation area
-- @return List of simulation positions
integration.findRoadIntersectionWithZone = function( area )
    return DEC_Geometrie_FindRoadIntersectionWithZone( area )
end

--- Returns true if the given position is inside the area, false otherwise
-- @param position Simulation position
-- @param location Simulation area
-- @return Boolean, whether or not the position is in the location
integration.isPointInsideLocation = function( position, location )
    return DEC_Geometrie_EstPointDansLocalisation( position, location )
end

--- Returns a circle with the given position as its center and 
--- the given radius as its radius
-- @param point Simulation position, the center of the circle
-- @param radius Float, the radius of the circle
-- @return Simulation circle
integration.createLocationCircle = function( point, radius )
    return DEC_Geometrie_CreerLocalisationCercle( point, radius )
end

--- Returns the advance of the provided agent 
--- along the current area of responsibility.
-- @param agent Simulation agent
-- @return Float standing for the advance of the agent along the AOR
integration.advanceAlongAOR = function( agent )
    return DEC_Geometrie_AdvanceAlongFuseau( agent )
end

--- Returns the advance of the given point along the danger direction of the provided agent
-- @param agent Directia agent
-- @param point Simulation position to be translated
-- @return Float
integration.advanceAlongDangerDirection = function( agent, point )
    return DEC_Geometrie_PositionAdvanceAlongDangerDirection( agent.source, point )
end

--- Returns the distance between two positions,
--- taking into account their respective heights
-- @param firstPosition First simulation position
-- @param firstHeight Float, the height of the first position
-- @param secondPosition Second simulation position
-- @param secondHeight Float, the height of the second position
-- @return Float, the distance between the positions
integration.compute3DDistance = function( firstPosition, firstHeight, secondPosition, secondHeight )
    return DEC_Geometrie_Distance3D( firstPosition, firstHeight, secondPosition, secondHeight )
end

--- Returns the 2D distance between two positions
-- @param firstPosition First simulation position
-- @param secondPosition Second simulation position
-- @return Float, the distance between the positions
integration.computeDistance = function( firstPosition, secondPosition )
    return DEC_Geometrie_Distance( firstPosition, secondPosition )
end

--- Returns the advance of the provided position
--- along the current area of responsibility.
-- @param position Simulation position
-- @return Float, the advance of the position along the AOR
integration.positionAdvanceAlongAOR = function( position )
    return DEC_Geometrie_PositionAdvanceAlongAOR( myself, position )
end

--- Returns an homothetic transformation of the location so that the border
--- of the new area will be at the provided distance of the former border.
-- @param location Simulation area
-- @param distance Float, the distance used in the homothety
-- @return Enlarged simulation area
integration.enlargeLocation = function( location, distance )
    return DEC_Geometrie_AgrandirLocalisation( location, distance )
end

--- Returns the nearest point to the provided position on the border of the object
-- @param position Simulation position
-- @param object Simulation object
-- @return Simulation position on the border of the object
integration.computeNearestBorder = function( position, scaledObject )
    return DEC_Geometrie_ComputeNearestBorder( position, scaledObject )
end

--- Returns true if the positions are the same, false otherwise
-- Positions are considered equal if the distance is inferior to a threshold (10 meters)
-- @param firstPosition First simulation position
-- @param secondPosition Second simulation position
-- @return Boolean, whether or not the two positions are equal
integration.positionsEqual = function( firstPosition, secondPosition )
    return DEC_Geometrie_PositionsEgales( firstPosition, secondPosition )
end

--- Returns positions inside and at the border of the provided urban block
-- @param urbanBlock Simulation urban block
-- @return List of simulation positions
integration.computeUrbanBlockLocations = function( urbanBlock )
    return DEC_Geometrie_CalculerLocalisationsBU( urbanBlock )
end

--- Returns the closest point to the caller's position on the border of
--- the provided area and inside the current area of responsibility.
--- Returns nil if there is no such position.
-- @param area Simulation area
-- @return Simulation position 
integration.computePointNearLocationsInsideAOR = function( area )
    return DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau( area )
end

--- Returns an area with the provided list of positions as vertices
-- @param positionList List of simulation vertices
-- @return Simulation area
integration.createLocationPolyline = function( positionsList )
    return DEC_Geometrie_CreerLocalisationPolyligne( positionsList )
end

--- Returns a vector with the two provided positions as extremities
-- @param firstPosition First simulation position
-- @param secondPosition Second simulation position
-- @return Simulation direction (vector)
integration.createDirection = function( firstPosition, secondPosition )
    return DEC_Geometrie_CreerDirection( firstPosition, secondPosition )
end

--- Returns the X coordinate of the provided position
-- @param position Simulation position
-- @return Float, the X coordinate of the position
integration.geometryX = function( position )
    return position:DEC_Geometrie_X()
end

--- Returns the Y coordinate of the provided position
-- @param position Simulation position
-- @return Float, the Y coordinate of the position
integration.geometryY = function( position )
    return position:DEC_Geometrie_Y()
end

--- Returns a position at the provided safety distance from the enemy agent.
--- This method uses the current danger direction to find a safe position.
-- Note that this method can only be called from a unit brain.
-- @param enemyAgent Simulation agent
-- @param safetyDistance Float, the safety distance
-- @return Simulation position
integration.computeSafetyPosition = function( enemyAgent, safetyDistance )
    return DEC_Geometrie_CalculerPositionSurete( enemyAgent, safetyDistance )
end

--- Splits the current area of responsibility along its global direction
--- in sections of the provided length. Returns a list of these sections.
-- @param length Float, the length of each section
-- @return List of simulation areas of responsibility
integration.splitAORInSections = function( length )
    return DEC_Geometrie_DecoupeFuseauEnTroncons( length )
end

--- Returns points in the middle of the provided area of responsibility.
-- The returned points constitute an itinerary along the AOR
-- @param AOR knowledge of an area of responsibility
-- @return List of simulation points in the middle of the AOR
integration.computeMiddlePointsInAOR = function( AOR )
    return DEC_Fuseau_ComputeMiddlePointsInAOR( AOR.source )
end

--- Returns the closest position to reach the provided area of responsibility for the given agent
-- @param agent Directia agent
-- @param AOR Simulation area of responsibility
-- @return Simulation position at the end of the AOR
integration.computeArrivalPoint( agent, AOR )
    return DEC_Geometrie_CalculerEntreeFuseau( agent, AOR )
end

--- Clips the provided area to the area of responsibility, and returns the result
-- Returns nil if there is no intersection between the area and the area of responsibility
-- @param area Masalife knowledge of an area
-- @param areaOfResponsibility Masalife knowledge of an area of responsibility
-- @return Clipped simulation area
integration.clipperLocalisation = function( area, areaOfResponsibility )
    return DEC_Geometrie_ClipperLocalisation( area.source, areaOfResponsibility.source )
end

--- Converts an area of responsibility into an area, and returns the result
-- @param areaOfResponsibility Masalife knowledge of an area of responsibility
-- @return Simulation area
integration.convertAORtoLocalisation = function ( areaOfResponsibility ) 
    return integration.getAreaFromAOR( areaOfResponsibility.source )
end

--- Returns the convex hull of a list of locations.
-- @param locationList List of simulation locations
-- @return Simulation convex hull of the provided locations.
integration.getConvexHull = function( locationList )
    return DEC_Geometrie_ConvexHull( locationList )
end

--- Returns a random position inside the disc defined by the given center and radius.
-- @param center Point knowledge (center of the disc), defining a "getPosition" method returning a simulation point.
-- @param radius Float, the radius of the disc
-- @return Point knowledge
integration.randomPositionInCircle = function( center, radius )
    return CreateKnowledge( integration.ontology.types.point, DEC_Geometrie_PositionAleatoireDansCercle( center:getPosition(), radius ) )
end

--- Returns a random position on the circle defined by the given center and radius.
-- @param center Point knowledge (center of the circle), defining a "getPosition" method returning a simulation point.
-- @param radius Float, the radius of the circle
-- @return Point knowledge
integration.randomPositionOnCircle = function( center, radius )
    return CreateKnowledge( integration.ontology.types.point, DEC_Geometrie_PositionAleatoireSurCercle( center:getPosition(), radius ) )
end

--- Splits the given area into the provided number of parts,
--- and sets the list of sub-areas knowledges in the myself.leadData.subAreas list variable.
-- If the given area is outside of the current area of responsibility, then it will not
-- be split, and will be the only area set in the myself.leadData.subAreas list variable.
-- This method can only be called by a company with an ongoing mission.
-- @see integration.geometrySplitLocalisation
-- @param area Area knowledge, the area to be split
-- @param numberOfParts Integer, the number of sub-areas to create
integration.splitArea = function( area, numberOfParts )
    local subAreas = integration.geometrySplitLocalisation( area.source, numberOfParts )
    subAreas = subAreas.first
    local integration = integration
    local myself = myself
    myself.leadData.subAreas = {}
    for _, localArea in pairs( subAreas ) do
        myself.leadData.subAreas[ #myself.leadData.subAreas + 1 ] = CreateKnowledge( integration.ontology.types.area, localArea )
    end
    if #subAreas == 0 then
        myself.leadData.subAreas[ #myself.leadData.subAreas + 1 ] = area -- cas ou la zone est hors limite
    end
end

--- Splits the given area into the provided number of parts using the given direction,
--- and returns the resulting list of sub-areas knowledges.
-- If there is a current area of responsibility, then only the part of the given area
-- inside the current area of responsibility will be used. If the given area has no 
-- intersection with the current area of responsibility, then the splitting will not take place,
-- and the returned list of sub-areas will be empty.
-- @param localisation Simulation area, the area to be split
-- @param numberOfParts Integer, the number of sub-areas to create. If negative, the splitting will not take place, 
-- and the returned list of sub-areas will be empty.
-- @param direction Simulation direction (vector), the direction with respect to which the splitting will take place
-- (the north-south direction by default).
-- @return Table with two keys :
-- <ul> <li> "first" : List of area knowledges, the created sub-areas  </li>
-- <li> "second" : Integer, the error code among one of the following : </li>
-- <ul> <li> 1 : The area has no intersection with the current area of responsibility
-- <li> 2 : The numberOfParts parameter is negative </li>
-- <li> 3 : The splitting was successful </li> </ul> </ul>
integration.geometrySplitLocalisation = function( localisation, numberOfParts, direction )
    return DEC_Geometry_SplitLocalisation( localisation, numberOfParts, direction )
end

--- Sets the forward line computer to start the computation of the distance
--- between the subordinates of the company and the forward line
-- @see settleDistance
-- @return The forward line computer
startSettleCalcul = function()
    local listePions = integration.getAgentsWithHQ()
    return DEC_Geometrie_StartCalculLignesAvantEtArrierePourPion( DEC_GetAutomate( meKnowledge.source ), listePions )
end

--- Computes the distance between the caller and the provided line computer
-- @param lineComputer The previously set forward line computer
-- @return Float, the distance between the caller and the line
settleDistance = function( lineComputer )
    return DEC_Geometrie_CalculerDistanceLigneAvant( lineComputer, meKnowledge.source )
end

--- Picks the terrain data given the provided location. Returns the type of terrain
--- at this point, divided into linear types (roads, rivers, ...) and area types (forest, urban, ...)
-- @param point Simulation point, the location where to pick the terrain data
-- @return Table with two members :
-- <ul><li> first : The area part of the terrain data, as a bit-field integer</li>
-- <ul>
-- <li>unknown    = 0</li>
-- <li>forest     = 1</li>
-- <li>plantation = 2</li>
-- <li>swamp      = 4</li>
-- <li>urban      = 8</li>
-- <li>water      = 16</li>
-- <li>dune       = 32</li>
-- <li>ice        = 64</li>
-- <li>mountain   = 128</li></ul>
-- <li> second : The linear part of the terrain data, as a bit-field integer</li>
-- <ul>
-- <li>cliff       = 1</li>
-- <li>motorway    = 2</li>
-- <li>largeroad   = 4</li>
-- <li>mediumroad  = 8</li>
-- <li>smallroad   = 16</li>
-- <li>bridge      = 32</li>
-- <li>railroad    = 64</li>
-- <li>largeriver  = 128</li>
-- <li>mediumriver = 256</li>
-- <li>smallriver  = 512</li>
-- <li>crossroad   = 1024</li>
-- <li>street      = 2048</li>
-- <li>avenue      = 4096</li>
-- <li>underpass   = 8192</li>
-- <li>metro       = 16384</li></ul>
-- </ul>
-- Example: a point with urban as area terrain type, smallriver and bridge as linear terrain type, will return
-- { first = 8, second = 544 } (bridge 32 +  smallriver 512)
integration.getTerrainData = function( point )
    return DEC_Geometrie_GetTerrainData( point )
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

-- Deprecated
stopSettleCalcul = function( line )
    DEC_Geometrie_StopCalculLignesAvantEtArriere( line )
end

-- Deprecated, use integration.positionAdvanceAlongAOR instead
integration.positionAdvanceAlongAORAutomat = function( position )
    return DEC_Geometrie_PositionAdvanceAlongFuseauAutomat( position )
end

DEC_Geometrie_PositionAdvanceAlongFuseau = function( position )
    return DEC_Geometrie_PositionAdvanceAlongAOR( myself, position )
end

DEC_Geometrie_PositionAdvanceAlongFuseauAutomat = function( position )
    return DEC_Geometrie_PositionAdvanceAlongAOR( myself, position )
end

--- Returns the corresponding area of the given AOR
-- @param AOR Sim AOR
-- @return Sim area
integration.getAreaFromAOR = function ( AOR )
    return DEC_Geometrie_ConvertirFuseauEnLocalisation( AOR )
end

integration.postionAdvanceAlongAOR = integration.positionAdvanceAlongAOR

--- Deprecated
integration.getPositionAlongDangerDirection = integration.advanceAlongDangerDirection