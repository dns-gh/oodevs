queryImplementation "getFragOrders"
{
    ["execute"] = function ( params )
        local CreateKnowledge = CreateKnowledge
        local res = {}
        local ordres_recus = DEC_GetOrdersCategory()
        if ordres_recus then
            local nOrdres = #ordres_recus
            for i = 1, nOrdres do
                local x = ordres_recus[ i ]
                res[ i ] = CreateKnowledge( world.classes.FragOrder, x )
            end
        end
        for _,x in pairs( myself.fragOrders or emptyTable ) do
            res[ #res + 1 ] = x
        end
        return res
    end
}