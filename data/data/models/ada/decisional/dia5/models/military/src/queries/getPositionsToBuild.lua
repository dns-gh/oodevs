queryImplementation "getPositionsToBuild" 
{ 
    ["execute"] = function ( params )
        local allRes = {}
        local positionNextTo --  nearest position around element to build
        local point, objectPosition, scaledObject
  
        for _, element in pairs ( params.elementsToBuild ) do     
          scaledObject = integration.enlargeLocation( element:getLocalisation() , 50 )
          if myself.taskParams.meetingPoint and myself.taskParams.meetingPoint ~= NIL then
              positionNextTo = myself.taskParams.meetingPoint.source
          else
              positionNextTo = meKnowledge:getPosition()
          end
          objectPosition = integration.computeNearestBorder( positionNextTo, scaledObject )
          point = CreateKnowledge( world.Point, objectPosition )
          
          if point:isBuildingFor( element ) then
              allRes[ #allRes + 1 ] = point
          end
        end
        return allRes
    end
}
