local CheckEqualPositions = function( point, result )
    for _, p in pairs( result ) do
        if masalife.brain.core.class.isOfType( p, world.Point ) then
            if DEC_Geometrie_PositionsEgales( point, p.source ) then 
                return p
            end
        end
    end
end
local AddPoint = function( point, result, newResult )
    local pointToAdd = nil
    if point then
        pointToAdd = CheckEqualPositions( point, result )
        if not pointToAdd then
            newResult[ #newResult + 1 ] = CreateKnowledge( world.Point, point )
        else
            newResult[ #newResult + 1 ] = pointToAdd
        end
    end
end

local AddUrbanKnowledge = function( blocks, result, newResult, knowledges )
    local CheckEqualUrbanKnoweldges = function( block, result )
        for _, p in pairs( result ) do
            if masalife.brain.core.class.isOfType( p, world.UrbanBlock ) then
                if DEC_Geometrie_PositionsEgales( DEC_ConnaissanceUrbanBlock_Barycentre( block ) , DEC_ConnaissanceUrbanBlock_Barycentre( p.source ) ) then 
                    return p
                end
            end
        end
    end
    for _, block in pairs( blocks ) do
        local blockToAdd = nil
        blockToAdd = CheckEqualUrbanKnoweldges( block, result ) or CheckEqualUrbanKnoweldges( block, newResult )
        if not blockToAdd then
            knowledges[ #knowledges + 1 ] = CreateKnowledge( world.UrbanBlock, block )  
        else
            knowledges[ #knowledges + 1 ] = blockToAdd
        end
    end
end

-- ****************************************************************************
-- getPositionsToCover
-- Try to find cover and concealment positions to safeguard from elements issued 
-- as parameters.
-- ****************************************************************************
-- All distances in meters
local radius = 100 -- Defined the radius of circle in witch we need 
                   -- to find terrain position to safeguard from elements
local distanceToFindCrossroads       = 300 
local distanceToFindStreetsPositions = 60 -- positions in streets around crossroads

queryImplementation "getPositionsToCover" 
{ 
    [ "execute" ] = function ( params )
        local result = knowledgeManager.getQueryResult( "getPositionsToCover" )
        local newResult = {}
        local knowledges = {}
        local newPositions = {}
        local urbanknowledges= {}

        -- -------------------------------------------------------------------------------- 
        -- Urban blocks
        -- --------------------------------------------------------------------------------
        urbanknowledges = DEC_Connaissances_BlocUrbainDansCercle(  meKnowledge:getPosition(), radius )
        AddUrbanKnowledge( urbanknowledges, result, newResult, knowledges )

        -- -------------------------------------------------------------------------------- 
        -- Crossroads and streets positions
        -- --------------------------------------------------------------------------------
        if next( urbanknowledges ) and DEC_Agent_EnVille() then -- only if agent is in urban terrain
            newPositions = DEC_FindSafetyPositions( distanceToFindCrossroads, distanceToFindStreetsPositions )
            for _, pos in pairs ( newPositions ) do
               knowledges[ #knowledges + 1 ] = CreateKnowledge( world.Point, pos )
            end
        end

        -- -------------------------------------------------------------------------------- 
        -- Check is positions are good positions to find cover and concealment depending on
        -- targets (the elements to safeguard from).
        -- --------------------------------------------------------------------------------
        for _, element in pairs ( params.elementsToCoverFrom ) do
            for _, pos in pairs ( knowledges ) do
                if pos:isSafeguardingFor( element ) then
                    if not exists( newResult, pos ) then
                        newResult[ #newResult + 1 ] = pos
                    end
                end
            end
            if #newResult == 0 then
                local distanceToFindOpenFieldPositions = DEC_Tir_PorteeMaxPourEtreTireParUnite(element.source, 0.5)
                if distanceToFindOpenFieldPositions ~= 0 then
                    local point = DEC_Geometrie_CalculerPositionSurete(element.source, distanceToFindOpenFieldPositions)
                    AddPoint( point, result, newResult )
                end
                -- Add unit position 
                local unitPosition = meKnowledge:getPosition()
                AddPoint( unitPosition, result, newResult )

                if DEC_HasMission( meKnowledge.source ) then
                    local mission = DEC_GetRawMission( meKnowledge.source )
                    local dirDanger = DEC_GetDirectionDanger( mission )
 
                    local positionSurete = DEC_Geometrie_PositionTranslateDir( meKnowledge:getPosition(), dirDanger, - distanceToFindOpenFieldPositions )

                    AddPoint( positionSurete, result, newResult )
                end
            end
        end
       
        -- -------------------------------------------------------------------------------- 
        -- No position (empty open terrain?) 
        -- --------------------------------------------------------------------------------
        if next( params.elementsToCoverFrom ) and not next( newResult ) then
            meKnowledge:RC( eRC_NoPositionsToGetCoverFromEntities )
        end
        knowledgeManager.setQueryResult( "getPositionsToCover", newResult )
        return newResult
    end
}

