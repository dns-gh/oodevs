queryImplementation "getCrowdInArea" 
{ 
    ["execute"] = function ( params )
        local allRes = {}
        local crowds = DEC_Connaissances_Populations()
        for _,crowd in pairs( crowds ) do
             if DEC_ConnaissancePopulation_EstDansZone( crowd, params.area.source ) then
                allRes[ #allRes + 1 ] = CreateKnowledge( world_elements.Crowd, crowd )
             end
        end
        return allRes
    end
}