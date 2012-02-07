return
{
    init = function( self, params )        
        myself.leadData.firePositionIndex = 0
    end,

    getReachable = function( self, params )
        return params.objective
    end,
    
    getFirePositions = function( self, params )
        if not myself.leadData.firePositions then
            if not params.angle or params.angle == NIL then
                self.angle = 180
            else
                self.angle = params.angle
            end
            if not params.modeDeploiement or params.modeDeploiement == NIL then
                self.modeDeploiement = eDeploiementEn_carre
            else
                self.modeDeploiement = params.modeDeploiement
            end
            if params.positions ~= NIL then -- si les positions sont renseign�es, on occupe ces positions
                myself.leadData.firePositions = params.positions
            else -- le mode de deploiment est toujours renseign�
                myself.leadData.firePositions = integration.getFirePositions( self.modeDeploiement, params.objective, self.angle )
            end
        end
        myself.leadData.firePositionIndex = myself.leadData.firePositionIndex % #myself.leadData.firePositions + 1        
        return myself.leadData.firePositions[ myself.leadData.firePositionIndex ]
    end,
}
