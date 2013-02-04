queryImplementation "getPositionsToRepair" 
{ 
    ["execute"] = function ( params )
        local allRes = {}
        for _, objective in pairs( params.elementsToRepair ) do
            if masalife.brain.core.class.isOfType( objective, sword.military.world.UrbanBlock ) then
                allRes[ #allRes + 1 ] = objective:getMyPosition()
            else
               scaledObject = DEC_Geometrie_AgrandirLocalisation( objective:getLocalisation() , 50 )
               if myself.taskParams.meetingPoint and myself.taskParams.meetingPoint ~= NIL then
                   positionNextTo = myself.taskParams.meetingPoint:getPosition()
               else
                   positionNextTo = meKnowledge:getPosition()
               end
               objectPosition = DEC_Geometrie_ComputeNearestBorder( positionNextTo, scaledObject )
               point = CreateKnowledge( sword.military.world.Point, objectPosition )
               allRes[ #allRes + 1 ] = point
            end
        end
        return allRes
    end
}