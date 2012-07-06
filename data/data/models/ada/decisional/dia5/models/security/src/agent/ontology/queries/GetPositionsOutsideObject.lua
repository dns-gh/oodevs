-- ****************************************************************************
-- GetPositionsOutsideObject
-- Get a position outside the given object
-- ****************************************************************************
queryImplementation "GetPositionsOutsideObject" 
{ 
    [ "execute" ] = function ( params )
        local result = {}
        local simPos = integration.getObjectNearestBorderPosition( params.object )
        result[ #result + 1 ] = CreateKnowledge( agent.ontology.classes.Position, simPos )
        if not next( result ) then
            meKnowledge:sendReport( eRC_NoKeyPositionsFoundInArea )
        end
        return result
    end
}