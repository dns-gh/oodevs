local knowledgeAssociation =
{
    [ eTypeLima_LD ] = world.LD,
    [ eTypeLima_LC ] = world.LC,
    [ eTypeLima_LCA ] = world.LCA,
}

queryImplementation "getLimaOnPath"
{ 
    ["execute"] = function ( params )
        local listePointInterressant = integration.getPointsCategory()
        local res = {}
        for _,point in pairs( listePointInterressant or {} ) do
            if integration.isBeforePoint( point ) then
                local destPoint = integration.getDestinationPoint( point )
                local pointType = integration.getTypePoint( destPoint )
                if pointType == eTypePointLima then
                    local limaType = integration.getTypeLimaFromPoint( destPoint )
                    local limaTypeEnum = knowledgeAssociation[ limaType ]
                    if limaTypeEnum then
                        res[ #res + 1 ] = CreateKnowledge( limaTypeEnum , point, { limaType_ = limaType } )
                    end
                end
            end
        end
        return res
    end
}
