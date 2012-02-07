queryImplementation "getEnemiesEngagingFriends"
{
    ["execute"] = function ( params )
        local allRes = knowledgeManager.getQueryResult( "getEnemiesEngagingFriends" )
        local res = {}
        local tableKn = {}
        if knowledgeManager.bCanCallStaticQuery or params.dynamic then
            allRes = {}
            for _, entity in pairs( params.friends ) do
                res = {}
                res = DEC_Connaissances_UnitesPrenantAPartieSurAmi( entity.source )
                res[#res] = F_Pion_GetEnemyAttackedBy( entity.source )
                for _, i in pairs( res ) do
                    if not exists( tableKn, i ) then
                        allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.Platoon, i )
                        tableKn[ #tableKn ] = i
                    end
                end
            end
            knowledgeManager.setQueryResult( "getEnemiesEngagingFriends", allRes )
        end
        return allRes
    end
}