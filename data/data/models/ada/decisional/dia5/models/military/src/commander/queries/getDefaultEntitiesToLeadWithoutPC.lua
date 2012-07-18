queryImplementation "getDefaultEntitiesToLeadWithoutPC" { ["execute"] = function ( params )
    local allRes = {}
    local temp = DEC_Automate_PionsSansPC()
    local CreateKnowledge = CreateKnowledge
    local nTemp = #temp
    for i = 1, nTemp do
        allRes[ i ] = CreateKnowledge( world.PlatoonAlly, nTemp )
    end
    return allRes
end}
