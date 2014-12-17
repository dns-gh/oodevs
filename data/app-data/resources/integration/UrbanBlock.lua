local maxConfinementDelay = 10 -- in ticks
local maxEvacuationDelay  = 10

--- Alerts the given urban block, and displays appropriate reports.
-- This method only alerts the given urban block if it is populated.
-- @param urbanBlock Urban block knowledge
-- @return Boolean, true
integration.alertUrbanBlock = function( urbanBlock )
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_Alert( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        reportFunction(eRC_ObjectiveAlerted )
    else
        reportFunction(eRC_NoPopulationInUrbanBlock )
        integration.displayTrace( "the urban block is not inhabited, population cannot be alerted" )
    end
    return true
end

--- Alerts the urbans blocks in the given area
-- @param area Simulation area
integration.alertUrbanBlockInArea = function( area )
    DEC_Agent_Alert( area )
end

--- Confines the urbans blocks in the given area
-- @param area Simulation area
integration.confineArea = function( area )
    DEC_Agent_Confine( area )
end

--- Undoes the alerting of the given urban block, and displays appropriate reports.
-- This method only undoes the alerting of the given urban block if it is populated.
-- @see integration.alertUrbanBlock
-- @param urbanBlock Urban block knowledge
-- @return Boolean, true
integration.undoAlertUrbanBlock = function( urbanBlock )
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_UndoAlert( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        reportFunction(eRC_ObjectiveIsNoLongerAlerted )
    else
        reportFunction(eRC_NoPopulationInUrbanBlock )
        integration.displayTrace( "the urban block is not inhabited" )
    end
    return true
end

--- Returns true if the given urban block is alerted, false otherwise.
-- @param urbanBlock Urban block knowledge
-- @return Boolean, whether or not the given urban block is alerted
integration.isUrbanBlockAlerted = function( urbanBlock )
    local localisation = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return DEC_Agent_IsAlerted( localisation )
end

--- Returns true if at least one population inside the given area is alerted, false otherwise.
-- @param area Simulation area
-- @return Boolean, whether or not the given area is alerted
integration.isPopInAreaAlerted = function( area )
    return DEC_Agent_IsAlerted( area )
end

--- Confines the given urban block, and displays appropriate reports.
--- This method must be called continuously on several
--- consecutive ticks for the confinement to take place.
-- This method only confines the given urban block if it is populated.
-- @param urbanBlock Urban block knowledge
-- @param confinementDelay Integer, the number of ticks needed for the confinement to take place.
-- By default, confinementDelay = 10
-- @return Boolean, true once the urban block is confined (or if it is not populated), false otherwise
integration.confineUrbanBlock = masalife.brain.integration.startStopAction( 
{ 
    start   = function( urbanBlock )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
                    urbanBlock.confinementDelay_ = 1
                    reportFunction(eRC_ZoneConfinementInProgress )
                else
                    integration.displayTrace( "the urban block is not inhabited, population cannot be confined" )
                    reportFunction(eRC_NoPopulationInUrbanBlock )
                end
              end,
    started = function( urbanBlock, confinementDelay )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source) then
                    if urbanBlock.confinementDelay_ == ( confinementDelay or maxConfinementDelay )
                        or DEC_Agent_IsAlerted( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) then
                        DEC_Agent_Confine( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) 
                        reportFunction(eRC_ObjectiveConfined )
                        return true
                    else
                        urbanBlock.confinementDelay_ = urbanBlock.confinementDelay_ + 1
                    end
                else
                    return true
                end
                return false 
              end, 
    stop    = function( urbanBlock ) end, -- NOTHING
} )

--- Undoes the confinement of the given urban block, and displays appropriate reports.
-- This method only undoes the confinement of the given urban block if it is populated.
-- @see integration.confineUrbanBlock
-- @param urbanBlock Urban block knowledge
-- @return Boolean, true
integration.undoConfineUrbanBlock = function( urbanBlock ) 
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_UndoConfine( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        reportFunction(eRC_ObjectiveIsNoLongerConfined )
    else
        integration.displayTrace( "the urban block is not inhabited" )
        reportFunction(eRC_NoPopulationInUrbanBlock )
    end
    return true
end

--- Returns true if the given urban block is confined, false otherwise.
-- @param urbanBlock Urban block knowledge
-- @return Boolean, whether or not the given urban block is confined
integration.isUrbanBlockConfined = function( urbanBlock )
    return DEC_Agent_IsConfined( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
end

--- Evacuates the given urban block, and displays appropriate reports.
--- This method must be called continuously on several
--- consecutive ticks for the evacuation to take place.
-- This method only evacuation the given urban block if it is populated.
-- @param urbanBlock Urban block knowledge
-- @param evacuationDelay Integer, the number of ticks needed for the evacuation to take place.
-- By default, evacuationDelay = 10
-- @return Boolean, true once the urban block is evacuated (or if it is not populated), false otherwise
integration.evacuateUrbanBlock = masalife.brain.integration.startStopAction( 
{ 
    start   = function( urbanBlock )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
                    urbanBlock.evacuationDelay_ = 1
                    reportFunction(eRC_ZoneEvacuationInProgress )
                else
                    integration.displayTrace( "the urban block is not inhabited" )
                    reportFunction(eRC_NoPopulationInUrbanBlock )
                end
              end,
    started = function( urbanBlock, evacuationDelay )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source) then
                    if urbanBlock.evacuationDelay_ == ( evacuationDelay or maxEvacuationDelay )
                    or DEC_Agent_IsAlerted( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) then
                        DEC_Agent_Evacuate( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) 
                        reportFunction(eRC_ObjectiveEvacuated )
                        return true
                    else
                        urbanBlock.evacuationDelay_ = urbanBlock.evacuationDelay_ + 1
                    end
                else
                    return true
                end
                return false 
              end, 
    stop    = function( urbanBlock ) end, -- NOTHING
})

--- Undoes the evacuation of the given urban block, and displays appropriate reports.
-- This method only undoes the evacuation of the given urban block if it is populated.
-- @see integration.evacuateUrbanBlock
-- @param urbanBlock Urban block knowledge
-- @return Boolean, true
integration.undoEvacuateUrbanBlock = function( urbanBlock )
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_UndoEvacuate( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        reportFunction(eRC_ObjectiveIsNoLongerEvacuated )
    else
        integration.displayTrace( "the urban block is not inhabited" )
        reportFunction(eRC_NoPopulationInUrbanBlock )
    end
    return true 
end

--- Returns true if the given urban block is evacuated, false otherwise.
-- @param urbanBlock Urban block knowledge
-- @return Boolean, whether or not the given urban block is evacuated
integration.isUrbanBlockEvacuated = function( urbanBlock )
    return DEC_Agent_IsEvacuated( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
end

--- Decontaminates the given urban block and displays a report.
-- @param urbanBlock Urban block knowledge
-- @return Boolean, true
integration.decontaminateUrbanBlock = function( urbanBlock )
    _DEC_DecontaminerZone( myself, DEC_PolygoneBlocUrbain( urbanBlock.source ) )
    reportFunction( eRC_DecontaminationDone )        
    return true
end

--- Creates a link between the given urban block and the provided resource node.
-- @param urbanBlock Urban block knowledge
-- @param resourceNode Resource node knowledge
-- @return Boolean, whether or not the creation has succeeded
integration.createResourceLinkWithUrbanBlock = function( urbanBlock, resourceNode )
    return DEC_ResourceNetwork_CreateLinkFromUrbanBlock( urbanBlock.source, resourceNode.source )
end

--- Returns the encompassing area of the given urban block.
-- @param Simulation urban block
-- @return Simulation area
integration.polylineUrbanBlock = function( urbanBlock )
    return DEC_PolygoneBlocUrbain( urbanBlock )
end

--- Returns true if the given position is inside the provided urban block, false otherwise.
-- @param position Simulation position
-- @param urbanBlock Simulation urban block
-- @return Boolean, whether or not the position is inside the urban block
integration.isPointInsideUrbanBlock = function( position, urbanBlock )
    return DEC_IsPointInUrbanBlock( position, urbanBlock )
end

--- Returns true if this entity is inside an urban block, false otherwise.
-- Note that this method can only be called by an agent.
-- @return Boolean, whether or not this entity is inside an urban block.
integration.isAgentInsideTown = function( )
    return _DEC_Agent_EnVille( myself )
end

--- Returns true if the given agent is inside the convex hull of all
-- urban blocks in the current map, false otherwise.
-- Note that this method can only be called by an agent.
-- @return Boolean, whether or not this entity is inside an urban block.
integration.AgentIsInTown = function( agent )
    return DEC_Agent_AgentEstEnVille( agent.source )
end

--- Returns a list of all urban blocks which are totally or partly 
--- inside the circle defined by the provided center and radius
-- @param center Simulation position, the center of the circle
-- @param radius Float, the radius of the circle
-- @return List of urban block knowledges
integration.getUrbanBlockInsideCircle = function( center, radius )
    return DEC_Connaissances_BlocUrbainDansCercle( center, radius )
end

--- Returns a list of all urban blocks which are totally inside the provided area
-- @param area Simulation area
-- @return List of urban block knowledges
integration.getUrbanBlockInArea = function( area )
    return DEC_UrbanBlocksInZone( area.source )
end

--- Returns the barycenter of the given urban block
-- @param urbanBlock Simulation urban block
-- @return Simulation position
integration.getUrbanBlockBarycenter = function( urbanBlock )
    return DEC_ConnaissanceUrbanBlock_Barycentre( urbanBlock )
end

--- Returns a list of all urban blocks which are inside or intersect the provided area
-- @param area Simulation area
-- @return List of urban block knowledges
integration.getUrbanBlockInsideOrIntersectArea = function( area )
    return _DEC_Connaissances_BlocUrbainDansOuIntersectentZone( myself, area )
end

--- Returns the urban block at the given position.
-- If there is no urban block at the given position, this method returns nil.
-- @param pos Simulation position
-- @return Simulation urban block (or nil)
integration.getUrbanBlockFromPosition = function( pos )
    return _DEC_Connaissances_BlocUrbainPourPosition( myself, pos )
end

------------------------------------------------------------------
-- CROWD METHODS ON URBAN BLOCK
------------------------------------------------------------------

--- Starts to damage a urban block, by decreasing its structural state.
-- @see integration.stopDamageUrbanBlock methods
-- This method can only be used by a crowd.
-- @param urbanBlock a urban block knowledge
-- @return Boolean, returns 'false'.
integration.startDamageUrbanBlock = function( urbanBlock )
    urbanBlock.actionDamage = _DEC_Population_DetruireBlocUrbain( myself, urbanBlock.source )
    _DEC_Population_ChangeUrbanDestructionState( myself, true )
    return false
end

--- Stop the action of damaging a urban block.
-- @see integration.startDamageUrbanBlock methods
-- This method can only be used by a crowd.
-- @param urbanBlock a urban block knowledge
-- @return Boolean, returns 'true'.
integration.stopDamageUrbanBlock = function( urbanBlock )
    if urbanBlock.actionDamage then
        urbanBlock.actionDamage = _DEC__StopAction( myself, urbanBlock.actionDamage )
        urbanBlock.actionDamage = nil
    end
    _DEC_Population_ChangeUrbanDestructionState( myself, false )
    return true
end

--- Remove all humans from a crowd
-- @param crowd Crowd knowledge
integration.removeAllHumansFromCrowd = function( crowd )
    _DEC_ReintegrateUrbanBlock( crowd )
end

--- Return the crowd level of angriness
-- @param crowd Crowd knowledge
-- @return integer
integration.getUrbanBlockAngriness = function( crowd )
    return _DEC_GetUrbanBlockAngriness( crowd )
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

--- Deprecated : use integration.getUrbanBlockInArea instead
integration.getUrbanBlockInsideArea = function( area )
    return _DEC_Connaissances_BlocUrbainDansZone( myself, area )
end