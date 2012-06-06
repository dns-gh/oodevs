local maxConfinementDelay = 10
local maxEvacuationDelay = 10

------------------------------------------------------------------
--- URBAN BLOCK SPECIFIC INTEGRATION METHODS
------------------------------------------------------------------
integration.alertUrbanBlock = function( urbanBlock )
    DEC_Agent_Alert( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
    meKnowledge:RC( eRC_ObjectiveAlerted )
    return true
end

integration.undoAlertUrbanBlock = function( urbanBlock )
    DEC_Agent_UndoAlert( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
    meKnowledge:RC( eRC_ObjectiveIsNoLongerAlerted )
    return true
end

integration.isUrbanBlockAlerted = function( urbanBlock )
    local localisation = DEC_PolygoneBlocUrbain( urbanBlock.source )
    return DEC_Agent_IsAlerted( localisation )
end

integration.confineUrbanBlock = masalife.brain.integration.startStopAction( 
{ 
    start = function( urbanBlock )
        urbanBlock.confinementDelay_ = 1
        meKnowledge:RC( eRC_ZoneConfinementInProgress )
    end,
    started = function( urbanBlock )
        local isAlerted = DEC_Agent_IsAlerted( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
        if isAlerted then
            meKnowledge:sendMessage( "inhabitants are prepared to be evacuated" )
        end
        if urbanBlock.confinementDelay_ == maxConfinementDelay or isAlerted then
            DEC_Agent_Confine( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) 
            meKnowledge:RC( eRC_ObjectiveConfined )
            return true
        else
            urbanBlock.confinementDelay_ = urbanBlock.confinementDelay_ + 1
        end
        return false 
    end, 
    stop = function( urbanBlock ) end, -- NOTHING
} )

integration.undoConfineUrbanBlock = function( urbanBlock ) 
    DEC_Agent_UndoConfine( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
    meKnowledge:RC( eRC_ObjectiveIsNoLongerConfined )
    return true
end

integration.isUrbanBlockConfined = function( urbanBlock )
    return DEC_Agent_IsConfined( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
end

integration.evacuateUrbanBlock = masalife.brain.integration.startStopAction( 
{ 
    start = function( urbanBlock )
        urbanBlock.evacuationDelay_ = 1
        meKnowledge:RC( eRC_ZoneEvacuationInProgress )
    end,
    started = function( urbanBlock )
        if urbanBlock.evacuationDelay_ == maxEvacuationDelay 
            or DEC_Agent_IsAlerted( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) then
            DEC_Agent_Evacuate( DEC_PolygoneBlocUrbain( urbanBlock.source ) ) 
            meKnowledge:RC( eRC_ObjectiveEvacuated )
            return true
        else
            urbanBlock.evacuationDelay_ = urbanBlock.evacuationDelay_ + 1
        end
        return false 
    end, 
    stop = function( urbanBlock ) end, -- NOTHING
})

integration.undoEvacuateUrbanBlock = function( urbanBlock )
    DEC_Agent_UndoEvacuate( DEC_PolygoneBlocUrbain( urbanBlock.source ) )
    meKnowledge:RC( eRC_ObjectiveIsNoLongerEvacuated )
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