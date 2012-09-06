local AddPointKnowledge = function( points, result, newResult, knowledges )
    local CheckEqualPositions = function( point, result )
        for _, p in pairs( result ) do
            if masalife.brain.core.class.isOfType( p, world.Point ) then
                if DEC_Geometrie_PositionsEgales( point, p.source ) then 
                    return p
                end
            end
        end
    end
    for _, point in pairs( points ) do
        local pointToAdd = nil
        pointToAdd = CheckEqualPositions( point, result ) or CheckEqualPositions( point, newResult )
        if not pointToAdd then
            knowledges[ #knowledges + 1 ] = CreateKnowledge( world.Point, point )
        else
            knowledges[ #knowledges + 1 ] = pointToAdd
        end
    end
end

queryImplementation "getPositionsToSupport" 
{ 
    [ "execute" ] = function ( params )
        local result = knowledgeManager.getQueryResult( "getPositionsToSupport" )
        local newResult = {}
        if params.retrogradeContext then
            for _, element in pairs ( params.elementsToSupport ) do
                local pos = integration.getPositionToSupportFriend( element )
                if pos ~= nil then
                    newResult[ #newResult + 1 ] = pos
                end
            end
            return newResult
        else
            local knowledges = {}
            local newPositions = {}
            local DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition = DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition
            local DEC_Tir_PorteeMaxPourTirer = DEC_Tir_PorteeMaxPourTirer
            local DEC_Connaissances_BlocUrbainDansCercle = DEC_Connaissances_BlocUrbainDansCercle
            local DEC_Geometrie_CalculerLocalisationsBU = DEC_Geometrie_CalculerLocalisationsBU
            if ( knowledgeManager.bCanCallStaticQuery or params.dynamic ) then
               local rangeDistanceMax = DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 2 -- indirect fire case
               local rangeDistanceMin = DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition() / 3
               if rangeDistanceMax <= 0 then -- direct fire case
                   rangeDistanceMax = DEC_Tir_PorteeMaxPourTirer( 0.7 ) / 2
                   rangeDistanceMin = DEC_Tir_PorteeMaxPourTirer( 0.8 ) / 2
               end
                -- Pour toutes les connaissances, garder celles qui sont bonnes pour soutenir
               for _, objective in pairs ( params.elementsToSupport ) do
                   local foundAPositionForThisObjective = false
                   if not objective:isNearby() then 
                       knowledges[ #knowledges + 1 ] = CreateProxyKnowledge( 
                          world.SupportingArea, objective,{ distanceMin = rangeDistanceMin , distanceMax = rangeDistanceMax } )
                   else
                       -- Les blocs urbains
                       local urbanknowledges = DEC_Connaissances_BlocUrbainDansCercle( objective:getPosition(), 100 )
                       -- Les positions autours des BUs et son barycentre (un point dans le BU)
                       for _, bu in pairs ( urbanknowledges ) do
                           AddPointKnowledge( DEC_Geometrie_CalculerLocalisationsBU( bu ), result, newResult, knowledges )
                       end
                   end
                   -- Verify that position are good to support unit
                   for _, element in pairs ( knowledges ) do
                       if element:isSupportingFor( objective ) then
                           foundAPositionForThisObjective = true
                           if not exists( newResult, element ) then
                               newResult[ #newResult + 1 ] = CreateKnowledge( world.Point, DEC_Geometrie_CopiePoint(element:getPosition()))
                           end
                       end
                   end
                   -- si pas de position
                   if not foundAPositionForThisObjective then
                       newResult[ #newResult + 1 ] = CreateProxyKnowledge( 
                          world.SupportingArea, objective, {distanceMin = rangeDistanceMin , distanceMax = rangeDistanceMax } )
                   end
                end
                knowledgeManager.setQueryResult( "getPositionsToSupport", newResult )
            end
            return newResult
        end
    end
}
