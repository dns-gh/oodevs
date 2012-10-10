-- ****************************************************************************
-- getPositionsToReach
-- Try to find reaching positions for elements to reach issued as paramters.
-- ****************************************************************************
queryImplementation "getPositionsToReach" 
{ 
    [ "execute" ] = function ( params )
        local newResult = {}

        -- -------------------------------------------------------------------------------- 
        --  Simple position
        -- --------------------------------------------------------------------------------
        local position, scaledObject, objectPosition
        for _, element in pairs ( params.elementsToReach ) do
            if masalife.brain.core.class.isOfType( element, sword.military.world.Object) and element:isAvoidable() then
                scaledObject = DEC_Geometrie_AgrandirLocalisation(  element:getLocalisation() , 50 )
                objectPosition = DEC_Geometrie_ComputeNearestBorder( meKnowledge:getPosition(), scaledObject )
                position = CreateKnowledge( sword.military.world.Point, objectPosition )
            else
                position = element:getMyPosition() -- reaching element
            end
            if position:isReachingFor( element ) then
                newResult[ #newResult + 1 ] = position
            end -- cas des BU non traficable par exemple
        end

        -- -------------------------------------------------------------------------------- 
        --  No positions, try to find accessible ones
        -- --------------------------------------------------------------------------------
        if not next( newResult ) then
            for _, elementToReach in pairs ( params.elementsToReach ) do
                local simPositions = elementToReach:getPositions()
                for _, simPos in pairs( simPositions ) do
                    local kPos = CreateKnowledge( sword.military.world.Point, simPos )
                    if kPos:isReachingFor( elementToReach ) then
                        newResult[ #newResult + 1 ] = kPos
                    end
                end
            end
        end
       -- affichePositions( newResult )
        return newResult
    end
}