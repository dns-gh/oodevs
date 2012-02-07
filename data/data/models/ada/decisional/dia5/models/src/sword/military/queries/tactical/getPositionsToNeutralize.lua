queryImplementation "getPositionsToNeutralize"
{
    ["execute"] = function ( params )
        local allRes = {}
        local blocksInCircle = DEC_Connaissances_BlocUrbainDansCercle( meKnowledge:getPosition(), 100 )
        for _, block in pairs( blocksInCircle ) do
            allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.UrbanBlock, block )  
        end
        allRes = integration.tableIsNeutralizingFor( allRes, params.elementsToNeutralize )
        for _, objective in pairs( params.elementsToNeutralize ) do
            allRes[ #allRes + 1 ] = CreateProxyKnowledge( sword.military.world.EngagementArea, objective )
        end
        return allRes
    end
}
