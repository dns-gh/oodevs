queryImplementation "getEnemiesInRange" 
{ 
    ["execute"] = function ( params )
        local res = {}
        local enemies = {}
        local myPosition = meKnowledge:getPosition()
        local porteeMax = integration.getMaxRangeToFireForPH( params.probabilityToHit )
        enemies = integration.getKnowledgesLivingAgentsInCircle( myPosition, porteeMax ) -- First filter: max range
        for _, enemy in pairs( enemies ) do
            if integration.isKnowledgeAgentValid( enemy ) and not integration.isAgentTacticallyDestroyed( enemy ) then
                local rDistanceAEni = 0 
                local porteeMaxEn = integration.getMaxRangeToFireOnAgent( enemy, params.probabilityToHit ) -- range depends on enemy volume
                local enyPosition = integration.getKnowledgeAgentPosition( enemy )
                if integration.isAgentFlying( enemy ) then -- flying enemy: compute 3D distance
                    local altitude = integration.getHeightForKnowledgeAgent( enemy )
                    rDistanceAEni = integration.compute3DDistance( myPosition, 0, enyPosition, altitude )
                else -- ground enemy: casual distance
                    rDistanceAEni = integration.computeDistance( myPosition, enyPosition )
                end
                if rDistanceAEni <= porteeMaxEn then
                    local ennyKn =  CreateKnowledge( world.Platoon, enemy )
                    if not ennyKn.capture and not ennyKn:isTransported() then
                        res[ #res + 1 ] = ennyKn
                    end
                end
            end
        end
        return res
    end
}