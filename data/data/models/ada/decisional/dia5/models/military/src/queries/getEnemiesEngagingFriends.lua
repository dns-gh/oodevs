queryImplementation "getEnemiesEngagingFriends"
{
    ["execute"] = function ( params )
        local allRes = knowledgeManager.getQueryResult( "getEnemiesEngagingFriends" )
        local res = {}
        local tableKn = {}
        local entities = {}
        if knowledgeManager.bCanCallStaticQuery or params.dynamic then
            allRes = {}
            for _, entity in pairs( params.friends ) do
                res = {}
                entities = {}
                if masalife.brain.core.class.isOfType( entity, world.Company ) then -- on peut appuyer des automates alli�s
                    local entitiesFromAutomat = integration.getEntitiesFromAutomat( entity, "none", true)
                    for j = 1, #entitiesFromAutomat do
                        entities[#entities + 1] = entitiesFromAutomat[j]
                    end
                else -- on peut appuyer des unit�s alli�s
                    entities[#entities + 1] = entity
                end
                for k = 1, #entities do
                    res = DEC_Connaissances_UnitesPrenantAPartieSurAmi( entities[k].source )
                    for _, i in pairs( res ) do
                        if not exists( tableKn, i ) then
                            allRes[ #allRes + 1 ] = CreateKnowledge( world.Platoon, i )
                            tableKn[ #tableKn ] = i
                        end
                    end
                end
            end
            knowledgeManager.setQueryResult( "getEnemiesEngagingFriends", allRes )
        end
        return allRes
    end
}