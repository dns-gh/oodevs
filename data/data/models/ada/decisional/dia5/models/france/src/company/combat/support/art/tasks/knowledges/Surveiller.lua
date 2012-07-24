return
{
    init = function( self, params )
        myself.leadData.deployPositionIndex = 0
        meKnowledge:setROE( eRoeStateRestrictedFire )
    end,
    getReachable = function( self, params )
        return params.objective
    end,
    getDeployPositions = function( self, params )
        if not myself.leadData.deployPositions then
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
            if params.positions ~= NIL then -- si les positions sont renseignées, on occupe ces positions
                myself.leadData.deployPositions = params.positions
            else -- le mode de deploiment est toujours renseigné
                myself.leadData.deployPositions = integration.getFirePositions( self.modeDeploiement, params.objective, self.angle )
            end
        end
        myself.leadData.deployPositionIndex = myself.leadData.deployPositionIndex % #myself.leadData.deployPositions + 1        
        return myself.leadData.deployPositions[ myself.leadData.deployPositionIndex ]
    end,
}
