local radius = 150 -- in meters. Defined the radius of cricle in witch we need 
                   -- to find terrain poisition to destroy elements
local nbParts = 3 -- Why 3 ? Why not, it's sounds good
queryImplementation "getPositionsToIntercept"
{
    ["execute"] = function ( params )
        local blocksInCircle
        local allRes={}

        -- ZU environnement,  get all the BU in circle around elements to destroy
        for _, objective in pairs( params.objectives ) do
            local elementPosition = objective:getPosition()
            if elementPosition then
                blocksInCircle = DEC_Connaissances_BlocUrbainDansCercle( elementPosition, radius )
                for _, block in pairs( blocksInCircle ) do
                    allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.UrbanBlock, block )
                end
            end
        end

        -- Area case, add positions in the area : if I had at least one position in BU I don't try to find another position
        if #allRes < 1 then
            for _, objective in pairs( params.objectives ) do
                if masalife.brain.core.class.isOfType( objective, sword.military.world.Area) then
                    local subAreas = DEC_Geometry_SplitLocalisation( objective.source, nbParts, nil )
                    for _, subArea in pairs( subAreas.first ) do
                         local pos = DEC_Geometrie_CalculerBarycentreLocalisation( subArea )
                         allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.Point, pos )
                    end
                end
            end
        end

        return allRes
    end
}
