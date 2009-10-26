queryImplementation "getCrossroads" { ["execute"] = function ( params )
    local allRes = {}
    for _, objective in pairs( params.objectives ) do
        local res = {}
        crossroads = DEC_Carrefours( objective.source, 10000 )
	    for i = 1, #crossroads do 
	    	res[ i ] = knowledges.create( net.masagroup.world.concrete.Point, crossroads[i] )
	    end
        allRes[ objective ] = res
    end
    return allRes
end}