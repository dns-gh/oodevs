queryImplementation "getDetectedUnits" 
{ 
    [ "execute" ] = function( params )

        local detectedUnits = {}
        local simUnits = DEC_Connaissances_UnitesDetecteesDansFuseau()
        for _, simUnit in pairs( simUnits ) do
            local unit = CreateKnowledge( sword.military.world.Platoon, simUnit )
            if unit:isNearby() and not unit:isTransported() and unit:identificationLevel() < 60 then
               detectedUnits[ #detectedUnits + 1 ] = unit
            end
        end
        return detectedUnits

    end
}