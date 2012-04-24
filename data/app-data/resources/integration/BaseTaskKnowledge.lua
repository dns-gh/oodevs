taskKnowledge = {}

initTaskKnowledge = function( result )
    local t = 
    {
        getPionEfficiency = function( self )
            return ePionEfficiencyCommon
        end,
        computeEffectsLevels = function( self )
            return nil
        end,
    }

    for i,j in pairs( result ) do
        t[i] = j
    end

    return t
end
