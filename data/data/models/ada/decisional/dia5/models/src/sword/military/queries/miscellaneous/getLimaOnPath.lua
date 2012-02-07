local knowledgeAssociation =
{
    [ eTypeLima_LD ] = sword.military.world.LD,
    [ eTypeLima_LC ] = sword.military.world.LC,
    [ eTypeLima_LCA ] = sword.military.world.LCA,
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
