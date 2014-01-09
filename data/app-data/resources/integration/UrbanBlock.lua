local maxConfinementDelay = 10 -- 10 tick = 100 second
local maxEvacuationDelay  = 10

------------------------------------------------------------------
--- URBAN BLOCK SPECIFIC INTEGRATION METHODS
------------------------------------------------------------------
integration.alertUrbanBlock = function( urbanBlock )
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_Alert( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        integration.pionRC( eRC_ObjectiveAlerted )
    else
        integration.pionRC( eRC_NoPopulationInUrbanBlock )
        meKnowledge:sendMessage( "the urban block is not inhabited, population cannot be alerted" )
    end
    return true
end

integration.undoAlertUrbanBlock = function( urbanBlock )
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_UndoAlert( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        integration.pionRC( eRC_ObjectiveIsNoLongerAlerted )
    else
        integration.pionRC( eRC_NoPopulationInUrbanBlock )
        meKnowledge:sendMessage( "the urban block is not inhabited" )
    end
    return true
end

integration.isUrbanBlockAlerted = function( urbanBlock )
    local localisation = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return DEC_Agent_IsAlerted( localisation )
end

integration.confineUrbanBlock = masalife.brain.integration.startStopAction( 
{ 
    start   = function( urbanBlock )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
                    urbanBlock.confinementDelay_ = 1
                    integration.pionRC( eRC_ZoneConfinementInProgress )
                else
                    meKnowledge:sendMessage( "the urban block is not inhabited, population cannot be confined" )
                    integration.pionRC( eRC_NoPopulationInUrbanBlock )
                end
              end,
    started = function( urbanBlock )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source) then
                    if urbanBlock.confinementDelay_ == maxConfinementDelay 
                        or DEC_Agent_IsAlerted( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) then
                        DEC_Agent_Confine( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) 
                        integration.pionRC( eRC_ObjectiveConfined )
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

integration.undoConfineUrbanBlock = function( urbanBlock ) 
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_UndoConfine( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        integration.pionRC( eRC_ObjectiveIsNoLongerConfined )
    else
        meKnowledge:sendMessage( "the urban block is not inhabited" )
        integration.pionRC( eRC_NoPopulationInUrbanBlock )
    end
    return true
end

integration.isUrbanBlockConfined = function( urbanBlock )
    return DEC_Agent_IsConfined( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
end

integration.evacuateUrbanBlock = masalife.brain.integration.startStopAction( 
{ 
    start   = function( urbanBlock )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
                    urbanBlock.evacuationDelay_ = 1
                    integration.pionRC( eRC_ZoneEvacuationInProgress )
                else
                    meKnowledge:sendMessage( "the urban block is not inhabited" )
                    integration.pionRC( eRC_NoPopulationInUrbanBlock )
                end
              end,
    started = function( urbanBlock )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source) then
                    if urbanBlock.evacuationDelay_ == maxEvacuationDelay 
                    or DEC_Agent_IsAlerted( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) then
                        DEC_Agent_Evacuate( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) 
                        integration.pionRC( eRC_ObjectiveEvacuated )
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

integration.undoEvacuateUrbanBlock = function( urbanBlock )
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_UndoEvacuate( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        integration.pionRC( eRC_ObjectiveIsNoLongerEvacuated )
    else
        meKnowledge:sendMessage( "the urban block is not inhabited" )
        integration.pionRC( eRC_NoPopulationInUrbanBlock )
    end
    return true 
end

integration.isUrbanBlockEvacuated = function( urbanBlock )
    return DEC_Agent_IsEvacuated( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
end

-- Decontamination
integration.decontaminateUrbanBlock = function( urbanBlock )
    DEC_DecontaminerZone( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
    integration.pionRC( eRC_DecontaminationDone )        
    return true
end

integration.createResourceLinkWithUrbanBlock = function( urbanBlock, resourceNode )
    return DEC_ResourceNetwork_CreateLinkFromUrbanBlock( urbanBlock.source, resourceNode.source )
end

integration.polylineUrbanBlock = function( urbanBlock )
    return DEC_PolygoneBlocUrbain( urbanBlock )
end

integration.isPointInsideUrbanBlock = function( position, urbanBlock )
    return DEC_IsPointInUrbanBlock( position, urbanBlock )
end

integration.isAgentInsideTown = function( )
    return DEC_Agent_EnVille()
end

integration.getUrbanBlockInsideCircle = function( position, radius )
    return DEC_Connaissances_BlocUrbainDansCercle( position, radius )
end

integration.getUrbanBlockInsideArea = function( area ) -- deprecated : use integration.getUrbanBlockInArea instead
    return DEC_Connaissances_BlocUrbainDansZone( area )
end

integration.getUrbanBlockInArea = function( area )
    return DEC_UrbanBlocksInZone( area.source )
end

integration.getUrbanBlockBarycenter = function( urbanBlock )
    return DEC_ConnaissanceUrbanBlock_Barycentre( urbanBlock )
end