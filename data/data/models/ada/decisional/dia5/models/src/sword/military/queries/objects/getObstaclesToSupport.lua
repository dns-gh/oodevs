queryImplementation "getObstaclesToSupport"
{ 
    ["execute"] = function ( params )
        local allRes = {}
        local obstacles = meKnowledge:getObstaclesOnAllyPath()
        local nObstacles = #obstacles
        for i = 1, nObstacles do
            local content = obstacles[i]
            if content.engineerObject.bypassable ~= false then
                local positionPion = DEC_Geometrie_PositionAdvanceAlongFuseau( content.platoon:getPosition() )
                local positionObjet = DEC_Geometrie_PositionAdvanceAlongFuseau( content.engineerObject:getPosition() )
                if( positionPion <= positionObjet ) and not existsInside( allRes, content.engineerObject ) then
                    allRes[ #allRes + 1 ] = content.engineerObject
                end
            end
        end
        return allRes
    end
}
