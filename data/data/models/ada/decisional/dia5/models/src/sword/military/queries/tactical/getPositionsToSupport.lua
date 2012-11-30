local AddPointKnowledge = function( points, result, newResult, knowledges )
    local CheckEqualPositions = function( point, result )
        for _, p in pairs( result ) do
            if masalife.brain.core.class.isOfType( p, sword.military.world.Point ) then
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
            knowledges[ #knowledges + 1 ] = CreateKnowledge( sword.military.world.Point, point )
        else
            knowledges[ #knowledges + 1 ] = pointToAdd
        end
    end
end

local AddCoverPositions = function( length, knowledges, objective )
    local compartments = {}
    if DEC_Agent_AutomateEstEmbraye() then
        compartments = DEC_Geometrie_DecoupeFuseauEnTroncons( length )
        for _, compartment in pairs ( compartments ) do
            knowledges[ #knowledges + 1 ] = CreateKnowledge( sword.military.world.Area, compartment )
        end
    end
end

local createAndCheckPositionsKnowledge = function( knowledges, newResult, objective )
    for _, element in pairs ( knowledges ) do
        if element:isSupportingFor( objective ) then
            if not exists( newResult, element ) then
                newResult[ #newResult + 1 ] = CreateKnowledge( sword.military.world.Point, DEC_Geometrie_CopiePoint(element:getPosition()))
            end
        end
    end
end

queryImplementation "getPositionsToSupport" 
{ 
    [ "execute" ] = function ( params )
        local result = knowledgeManager.getQueryResult( "getPositionsToSupport" )
        local newResult = {}
        local knowledges = {}
        local newPositions = {}
        -- -------------------------------------------------------------------------------- 
        -- Retrograde mission cases (Screen / Delay)
        -- --------------------------------------------------------------------------------
        if params.retrogradeContext then
            for _, element in pairs ( params.elementsToSupport ) do
                local pos = integration.getPositionToSupportFriend( element )
                if pos ~= nil then
                    newResult[ #newResult + 1 ] = pos
                end
            end
            return newResult

        -- -------------------------------------------------------------------------------- 
        -- Other cases
        -- --------------------------------------------------------------------------------
        else
            local DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition = DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition
            local DEC_Tir_PorteeMaxPourTirer = DEC_Tir_PorteeMaxPourTirer
            local DEC_Connaissances_BlocUrbainDansCercle = DEC_Connaissances_BlocUrbainDansCercle
            local DEC_Geometrie_CalculerLocalisationsBU = DEC_Geometrie_CalculerLocalisationsBU
            if ( knowledgeManager.bCanCallStaticQuery or params.dynamic ) then
               local rangeDistance = integration.firingRangeToSupport( self )
               local rangeDistanceMin = rangeDistance[1]
               local rangeDistanceMax = rangeDistance[2]
                -- For each objective to support, find position
                for _, objective in pairs ( params.elementsToSupport ) do
                    local foundAPositionForThisObjective = false
                    if not objective:isNearby() then 
                        knowledges[ #knowledges + 1 ] = CreateProxyKnowledge( 
                           sword.military.world.SupportingArea, objective,{distanceMin = rangeDistanceMin , distanceMax = rangeDistanceMax } )
                    else
                        -- Urbans blocks
                        local urbanknowledges = DEC_Connaissances_BlocUrbainDansCercle( objective:getPosition(), 100 )
                        -- Les positions autours des BUs et son barycentre (un point dans le BU)
                        for _, bu in pairs ( urbanknowledges ) do
                            AddPointKnowledge( DEC_Geometrie_CalculerLocalisationsBU( bu ), result, newResult, knowledges )
                        end
                    end
    
                    -- check that positions are good to support unit
                    createAndCheckPositionsKnowledge( knowledges, newResult, objective )
                    
                     -- no position found before
                    if #newResult < 1 then
                        local lastOption = CreateProxyKnowledge( 
                           sword.military.world.SupportingArea, objective, {distanceMin = rangeDistanceMin , distanceMax = rangeDistanceMax } )
                        if integration.isElementInAOR( lastOption.proxy ) then
                            newResult[ #newResult + 1 ] = lastOption
                        else
                            local compartmentPositions = {}
                            AddCoverPositions( rangeDistanceMin, compartmentPositions, objective )
                            createAndCheckPositionsKnowledge( compartmentPositions, newResult, objective )
                            if #newResult < 1 then -- the less worst
                                newResult[ #newResult + 1 ] = integration.query.getNearestPositionInAOR( lastOption.proxy )
                            end
                        end
                    end
                end
                
                knowledgeManager.setQueryResult( "getPositionsToSupport", newResult )
            end
            return newResult
        end
    end
}
