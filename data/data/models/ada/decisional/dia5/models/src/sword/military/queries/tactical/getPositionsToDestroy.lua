local radius = 100 -- in meters. Defined the radius of circle in witch we need 
                   -- to find terrain poisition to destroy elements
queryImplementation "getPositionsToDestroy"
{
    ["execute"] = function ( params )
        local blocksInCircle
        local allRes={}
        local newResult = {}

        -- ZU environnement,  get all the BU in circle around elements to destroy
        for _, element in pairs( params.elementsToDestroy ) do
            local elementPosition = element:getPosition()
            if elementPosition then
                blocksInCircle = DEC_Connaissances_BlocUrbainDansCercle( elementPosition, radius )
                for _, block in pairs( blocksInCircle ) do
                    allRes[ #allRes + 1 ] = CreateKnowledge( sword.military.world.UrbanBlock, block )
                end
            end
        end

        -- check if position allow units to destroy each element
        for _, element in pairs( params.elementsToDestroy ) do
            if element:isValid() then
                for _, res in pairs ( allRes ) do
                    if res:isDestroyingFor( element ) then
                        if not exists( newResult, res ) then
                            newResult[ #newResult + 1 ] = res
                        end
                    end
                end
            end
        end

        -- Open terrain, no position, get a tracking position as a destroying position
        if not next( newResult ) then
            for _, objective in pairs( params.elementsToDestroy ) do
                if objective:isValid() then
                    local lastOption = CreateProxyKnowledge( sword.military.world.EngagementArea, objective )
                    if integration.isElementInAOR( lastOption.proxy ) then
                        newResult[ #newResult + 1 ] = lastOption
                    else
                        newResult[ #newResult + 1 ] = integration.query.getNearestPositionInAOR( lastOption.proxy )                          
                    end
                end
            end
        end
        return newResult
    end
}