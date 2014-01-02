--- Creates a resource link between an object and a resource node
-- Returns true if the creation succeeded, false otherwise (e.g. if the given object is invalid)
-- @param object Object knowledge
-- @param Resource node knowledge
-- @return Boolean
integration.createResourceLinkWithObject = function( object, resourceNode )
    return DEC_ResourceNetwork_CreateLinkFromObject( object.source, resourceNode.source )
end

--- Returns true if the given object is constructed, false otherwise
-- @param object Object knowledge
-- @return Boolean, whether or not the object is constructed
integration.isConstructed = function( object )
    if DEC_ConnaissanceObjet_EstConstuit( object.source ) == eTristate_True then
        return true
    end
    return false
end

--- Returns true if the given object knowledge is invalid, false otherwise
-- This function can be used to infer if the given object has been removed
-- @param object Object knowledge
-- @return Boolean, whether or not the given object knowledge is invalid
integration.isObjectRemoved = function( object )
    return not DEC_IsValidKnowledgeObject( object.source )
end

--- Returns the nearest position to the border of the provided object, at a specified distance
-- @param object The knowledge of the object
-- @param distance Optional, the minimal distance in meters between the returned position and the border of the object (20 by default)
integration.getObjectNearestBorderPosition = function( object, distance )
    distance = distance or 20 -- meters
    local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) , distance )
    return DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
end

--- Returns the given object's nearest position to this entity's position
-- @param object Object knowledge
-- @return Simulation position
integration.getObjectNearestPositionOnBorder = function( object )
    local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) , 0 )
    return DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
end

--- Returns positions around the given object to interact with it (e.g. to build it, to improve it, etc.).
--- The returned positions are guaranteed to be at least the provided distance (20 meters by default) from the border of the object.
-- This method returns roads intersections around the object, or the object border's closest position
-- to this entity's position if there aren't any.
-- @param object Object knowledge
-- @param distance Float, the minimum distance between each returned position and the border of the object (in meters)
-- @return List of simulation positions around the given object
integration.getObjectPositionsForWork = function( object, distance ) -- object is a buildable element (object or plannedWork)
    distance = distance or 20 -- meters
    
    object.positionsForWork = object.positionsForWork or {}
    
    if not object.positionsForWork[ distance ] then
        local localisation = DEC_Geometrie_AgrandirLocalisation( object:getLocalisation() , distance )
        local roadsIntersections = integration.findRoadIntersectionWithZone( localisation )
        if #roadsIntersections == 0 then -- no road around the object
            object.positionsForWork[ distance ] = { DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation ) }
        else
            object.positionsForWork[ distance ] = roadsIntersections
        end
    end
    return object.positionsForWork[ distance ]
end

--- Returns a list of simulation objects with a given capacity in a zone
-- @param capacityName Name of the capacity. The list of supported capacities is defined in ../schemas/(version)/physical/objects.xsd : All possible elements of T_ObjectCapacities - except geometries which defines the geometry of an object -. Examples include "avoidable", "burn", "bypassable"...
-- @param zone Knowledge of a geometry
-- @return List of simulation objects
integration.getObjectsKnowledgeInZoneWithCapacity = function( capacityName, zone )
    return DEC_ObjectKnowledge_GetObjectsInZone( meKnowledge.source, capacityName, zone.source )
end

--- Builds the given engineer object instantaneously and returns it
-- @param genObject Engineer object knowledge to build instantaneously
-- @param withoutReport Boolean if set to true don't display a report to indicate the beginning of the work
-- @return Object knowledge, the instantaneously built object
integration.buildObjectInstantaneously = function( engineerObject, withoutReport )
    if not withoutReport then
        integration.pionRC( eRC_FinTravauxObjet, engineerObject.source )
    end
    return DEC_ObjectKnowledge_BuildInstantaneously( myself, engineerObject.source )
end

--- Returns true if the given object can be used to filter crowds, false otherwise
-- @param object Object knowledge
-- @return Boolean
integration.canFilter = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "population-filter" )
end

--- Returns true if the given object can be used to decontaminate, false otherwise
-- @param object Object knowledge
-- @return Boolean
integration.canDecontaminateAgents = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "decontamination" )
end

--- Returns true if the given object can be used for logistic means, false otherwise
-- @param object Object knowledge
-- @return Boolean
integration.isCamp = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "logistic" )
end

--- Returns true if the given object is trafficable, false otherwise
-- @param object Object knowledge
-- @return Boolean
integration.isTrafficable = function( object )
    return DEC_ObjectKnowledge_HasCapacity( object.source, "trafficability" )
end

--- Returns positions around the provided object
-- @param object Object knowledge
-- @return List of simulation positions
integration.getPositionsAroundObject = function( object )
    local CreateKnowledge = CreateKnowledge
    local typePoint = integration.ontology.types.point
    
    local result = {}
    local simPoints = object:getPositions() 
    for _, simPoint in pairs( simPoints ) do
        result[ #result + 1 ] = CreateKnowledge( typePoint, simPoint )
    end
    local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_ConnaissanceObjet_Localisation( object.source ) , 20 )
    result[ #result + 1 ] = CreateKnowledge( typePoint, DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation ) )
    return result
end

--- Returns all the object knowledges inside the circle defined by
--- the provided center and radius with at least one type from the given list
-- @param position Simulation position, center of the circle
-- @param distance Float, radius of the circle (in meters)
-- @param objectTypeList List of strings
-- @return List of object knowledges
integration.getKnowledgesObjectsInCircle = function( position, distance, objectTypeList )
    return DEC_Knowledges_ObjectsInCircle( position, distance, objectTypeList )
end

--- Returns the given object's localisation
-- @param Simulation object
-- @return Simulation area
integration.getKnowledgeObjectLocation = function( object )
    return DEC_ConnaissanceObjet_Localisation( object )
end

--- Returns the given engineer object's localisation
-- @param Simulation engineer object
-- @return Simulation area
integration.getGenObjectLocation = function( genObject )
    return DEC_GenObject_Localisation( genObject )
end

--- Returns the given engineer object's type
-- @param Simulation engineer object
-- @return String
integration.getGenObjectType = function( genObject )
    return DEC_GenObject_Type( genObject )
end

--- Returns true if the provided object has the given capacity, false otherwise
-- @param Simulation object
-- @param String, the name of the capacity
-- @return Boolean, whether or not the object has the capacity
integration.hasCapacityKnowledgeObject = function( object, capacity )
    return DEC_ObjectKnowledge_HasCapacity( object, capacity )
end

--- Returns the given object's type
-- @param Simulation object
-- @return String, the type of the object
integration.getKnowledgeObjectType = function( object )
    return DEC_ConnaissanceObjet_Type( object )
end

--- Returns the next non-bypassed object on this entity's path.
-- The returned object is guaranteed to have at least one of the types from
-- the list parameter (if the provided list is non-empty).
-- @params model Deprecated, unused
-- @params distance Deprecated, unused
-- @params paramsList List of strings
-- @return Simulation object
integration.getNextObjectOnPath = function( model, distance, paramsList )
    return DEC_GetNextObjectOnPath( model, distance, paramsList )
end

--- Returns the next object on this entity's path.
-- The returned object is guaranteed to have at least one of the types from
-- the list parameter (if the provided list is non-empty).
-- @params model Deprecated
-- @params distance Deprecated
-- @params paramsList List of strings
-- @return Simulation object
integration.getNextObjectOnPathWithBypassed = function( model, distance, paramsList )
    return DEC_GetNextObjectOnPathWithBypassed( model, distance, paramsList )
end

--- Returns true if the given engineer object is not activated, false otherwise
-- @see integration.isManeuverObstacle
-- @see integration.isActifManeuverObstacle
-- @params genObject Engineer object knowledge
-- @return Boolean
integration.isManeuverGenObject = function( genObject )
    return DEC_GenObject_TypeObstacleManoeuvre( genObject )
end

--- Returns true if the given object is valid, false otherwise
-- @params object Object knowledge
-- @return Boolean, whether or not the object is valid
integration.isValidKnowledgeObject = function( object )
    return DEC_IsValidKnowledgeObject( object )
end

--- Queues the given agent knowledge for decontamination in the given decontamination plot
-- @param unit Directia agent
-- @param object Object knowledge
-- @param decontaminationPlot Object knowledge with a decontamination capacity
integration.addKnowledgeToDecontaminationList = function( unit, object )
    DEC_Agent_DecontamineConnaissance( unit.source, object.source )
end

--- Queues this entity for decontamination in the given decontamination plot
-- @param decontaminationPlot Object knowledge with a decontamination capacity
integration.requestForDecontamination = function( decontaminationPlot )
    DEC_ConnaissanceObjet_DemandeDeDecontamination( decontaminationPlot.source )
end

--- Queues the given agent knowledge for decontamination in the given decontamination plot
-- @param unit Directia agent knowledge
-- @param object Object knowledge
-- @param decontaminationPlot Object knowledge with a decontamination capacity
integration.addToDecontaminationList = function( unit, object )
    DEC_ConnaissanceObjet_DemandeDeDecontaminationSurPion(unit.source, object.source)
end

-- Returns true if the given engineer object needs improvement, false otherwise
-- @param genObject Engineer object knowledge
-- @return Boolean, whether or not the engineer object needs improvement
integration.genObjectNeedsImprovement = function( genObject )
    return DEC_GenObject_Mining( genObject.source )
end

-- Returns true if the given object needs improvement, false otherwise
-- @param object Object knowledge
-- @return Boolean, whether or not the object needs improvement
integration.objectNeedsImprovement = function( object )
    return DEC_ObjectKnowledge_MustBeMined( object.source )
end

--- Returns a position outside the convex hull of the provided set of objects, at a specified distance.
-- Returns nil if the list of objects is empty
-- @param objects List of object knowledges
-- @param distanceFromBorder Optional, the minimal distance in meters between 
-- the returned position and the border of the convex hull (20 by default)
-- @return Simulation position outside the convex hull, or nil if no object was provided
integration.getPositionOutsideObjects = function( objects, distanceFromBorder )
    distanceFromBorder = distanceFromBorder or 20 -- meters
    local localisations = {}
    for i = 1, #objects do
        localisations[ i ] = DEC_ConnaissanceObjet_Localisation( objects[ i ].source )
    end 
    if #localisations > 0 then
        local localisation = DEC_Geometrie_AgrandirLocalisation( DEC_Geometrie_ConvexHull( localisations ) , distanceFromBorder )
        local simPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), localisation )
        return CreateKnowledge( integration.ontology.types.point, simPosition )
    else
        return nil
    end
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

-- Deprecated, use integration.getGenObjectLocation instead
integration.getKnowledgeGenObjectLocation = function( genObject )
    return DEC_GenObjectKnowledge_Localisation( genObject )
end
