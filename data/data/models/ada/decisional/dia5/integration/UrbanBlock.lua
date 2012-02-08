local maxConfinementDelay = 10
local maxEvacuationDelay = 10

------------------------------------------------------------------
--- URBAN BLOCK SPECIFIC INTEGRATION METHODS
------------------------------------------------------------------
integration.alertUrbanBlock = function( urbanBlock )
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_Alert( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        meKnowledge:RC( eRC_ObjectiveAlerted )
    else
        meKnowledge:RC( eRC_NoPopulationInUrbanBlock )
    end
    return true
end

integration.undoAlertUrbanBlock = function( urbanBlock )
    if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source ) then
        DEC_Agent_UndoAlert( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        meKnowledge:RC( eRC_ObjectiveIsNoLongerAlerted )
    else
        meKnowledge:RC( eRC_NoPopulationInUrbanBlock )
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
                    meKnowledge:RC( eRC_ZoneConfinementInProgress )
                else
                    meKnowledge:RC( eRC_NoPopulationInUrbanBlock )
                end
              end,
    started = function( urbanBlock )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source) then
                    if urbanBlock.confinementDelay_ == maxConfinementDelay 
                        or DEC_Agent_IsAlerted( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) then
                        DEC_Agent_Confine( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) 
                        meKnowledge:RC( eRC_ObjectiveConfined )
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
        meKnowledge:RC( eRC_ObjectiveIsNoLongerConfined )
    else
        meKnowledge:RC( eRC_NoPopulationInUrbanBlock )
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
                    meKnowledge:RC( eRC_ZoneEvacuationInProgress )
                else
                    meKnowledge:RC( eRC_NoPopulationInUrbanBlock )
                end
              end,
    started = function( urbanBlock )
                if DEC_Agent_UrbanBlockIsPopulated( urbanBlock.source) then
                    if urbanBlock.evacuationDelay_ == maxEvacuationDelay or DEC_Agent_IsAlerted( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) then
                        DEC_Agent_Evacuate( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) 
                        meKnowledge:RC( eRC_ObjectiveEvacuated )
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
        meKnowledge:RC( eRC_ObjectiveIsNoLongerEvacuated )
    else
        meKnowledge:RC( eRC_NoPopulationInUrbanBlock )
    end
    return true 
end

integration.isUrbanBlockEvacuated = function( urbanBlock )
    return DEC_Agent_IsEvacuated( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
end

-- Decontamination
integration.decontaminateUrbanBlock = function( urbanBlock )            
    DEC_DecontaminerZone( DEC_PolygoneBlocUrbain( urbanBlock.source ) )           
    return true
end