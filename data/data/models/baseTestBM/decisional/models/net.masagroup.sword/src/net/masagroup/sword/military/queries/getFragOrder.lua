queryImplementation "getFragOrder" { ["execute"] = function ( params )
    local res = {}
    	local ordres_recus = DEC_GetOrdersCategory()
        for _,x in pairs( ordres_recus or {} ) do
        	res[#res+1] = CreateKnowledgeWithoutPos( net.masagroup.sword.military.world.FragOrder, x )
        end
    return res
end}