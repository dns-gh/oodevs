local knowledgeAssociation =
{
    [ eTypeLima_LD ] = world_elements.LD,
    [ eTypeLima_LC ] = world_elements.LC,
    [ eTypeLima_LCA ] = world_elements.LCA,
}

queryImplementation "getLimaOnPath"
{ 
    ["execute"] = function ( params )
        local listePointInterressant = DEC_GetPointsCategory()
        local res = {}
        for _,point in pairs( listePointInterressant or {} ) do
            if DEC_IsAvantPoint( point ) then
                local destPoint = DEC_GetDestPoint( point )
                local pointType = DEC_GetTypePoint( destPoint )
                if pointType == eTypePointLima then
                    local limaType = DEC_GetTypeLimaPoint( destPoint )
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
