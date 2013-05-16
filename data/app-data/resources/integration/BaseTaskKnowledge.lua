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
        
        hasDotation = function( self, entity, obstacle )
            return integration.hasDotationToBuildObject( entity.source, tostring( obstacle:getType() ) )
        end,

        canDoIt = function( self, entity, obstacle )
            local localisation = obstacle:getLocalisation()
            if localisation ~= nil then
                return integration.canBuildObjectWhenLoadedWithLocation( 
                       entity.source, tostring( obstacle:getType() ), localisation )
            else
                return false
        end
    end,
    }

    for i,j in pairs( result ) do
        t[i] = j
    end

    return t
end
