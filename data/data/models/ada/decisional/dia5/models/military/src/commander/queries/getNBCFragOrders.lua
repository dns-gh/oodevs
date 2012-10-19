queryImplementation "getNBCFragOrders"
{ 
    ["execute"] = function ( params )

        local fragOrders = {}

        -- A frag order has been retreived
        local fragOrderWw = CreateFragOrder( "ActivateNBCProtection" )
        if fragOrderWw ~= nil then
            return { CreateKnowledge( world.FragOrder, fragOrderWw ) }
        end
        local fragOrderFr = CreateFragOrder( "Rep_OrderConduite_MettreTenueNBC" )
        if fragOrderFr ~= nil then
            return { CreateKnowledge( world.FragOrder, fragOrderFr ) }
        end
        return {}
    end
}