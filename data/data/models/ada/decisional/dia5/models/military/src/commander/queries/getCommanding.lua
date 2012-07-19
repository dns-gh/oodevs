queryImplementation "getCommanding" { ["execute"] = function ( params )
    local temp = {}
    if params.withPC then
        temp = DEC_Automate_PionsAvecPCCommunication()
    else
        temp = DEC_Automate_PionsSansPCCommunication()
    end

    local CreateKnowledge = CreateKnowledge
    local knowledges = {}
    local nTemp = #temp
    for i = 1, nTemp do
        local knowledge = temp[i]
        knowledges[ i ] = CreateKnowledge( world.PlatoonAlly, knowledge )
    end

    if params.role ~= "none" then --TODO replace by NIL when a queries will have nullable parameters
        return integration.filterPionWithRole( knowledges, params.role )
    else
        return knowledges
    end
end}
