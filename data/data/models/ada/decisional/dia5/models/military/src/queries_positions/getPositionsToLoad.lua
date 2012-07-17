queryImplementation "getPositionsToLoad" 
{ 
    [ "execute" ] = function( params )
        local allRes = {}
        for _, objective in pairs ( params.elementsToLoad ) do
            local position = objective:getMyPosition()
            if position:isLoadingFor( objective ) then
                if not exists( allRes, position ) then
                    allRes[ #allRes + 1 ] = position
                end
            end
        end
        return allRes
    end
}