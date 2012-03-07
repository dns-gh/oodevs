-- ****************************************************************************
-- GetHeadPositionForCrowd
-- Get head position for a given crowd.
-- ****************************************************************************
queryImplementation "GetHeadPositionForCrowd" 
{ 
    [ "execute" ] = function ( params )
        local allRes = {}
        allRes[ 1 ] = CreateKnowledge( ontology.classes.Position, integration.getHeadPosition( params.crowd ) )

        --affichePositions( allRes )
        -- No position
        if not next( allRes ) then
            meKnowledge:sendReport( eRC_NoPositionsToReachTargets )
        end
        return allRes
    end
}