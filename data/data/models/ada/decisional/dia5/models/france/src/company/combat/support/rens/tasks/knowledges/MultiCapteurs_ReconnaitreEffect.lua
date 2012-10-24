return
{   
    getArea = function( self, params )
        return myself.taskParams.area
    end,

    getByNight = function( self, params )
        return false -- Action can take place day or night for ROHUM
    end,
    
    getFicelleDeVol = function( self, params )
        if myself.taskParams.ficelleDeVol and myself.taskParams.ficelleDeVol ~= NIL then
            return myself.taskParams.ficelleDeVol
        else
            local areaLocalisation = integration.computeLocationBarycenter( myself.taskParams.area.source )
            local point = CreateKnowledge( integration.ontology.types.point, areaLocalisation )
            return { point }    
        end
    end,
}