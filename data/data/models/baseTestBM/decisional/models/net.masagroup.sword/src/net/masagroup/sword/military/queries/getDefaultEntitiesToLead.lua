queryImplementation "getDefaultEntitiesToLead" { ["execute"] = function ( params )
    local allRes = {}
    local temp = DEC_Automate_PionsAvecPC()
    for _,i in pairs(temp) do
        allRes[ #allRes + 1 ] = CreateKnowledge( "net.masagroup.sword.military.world.Section" ,i, { 0, 0, 0 } )
    end
    return allRes
end}