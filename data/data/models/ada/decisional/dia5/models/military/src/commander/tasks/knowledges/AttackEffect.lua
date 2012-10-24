return
{ 

    getObjectives = function( self, params, entity )
        local fuseau = self:getReachable( params )
        return { CreateKnowledge( world.Point, integration.computeArrivedPointForAOR(fuseau.source)) }
    end,

    getReachable = function( self, params )
      if not myself.leadData.currentAttackPosition then
        myself.leadData.currentAttackPosition = 0
        myself.leadData.fuseaux = {}
        local objective = params.objective
        if not objective then
          local fuseaux = integration.query.getFuseaux( myself.taskParams.maxNbrFront )
          for _,fuseau in pairs( fuseaux ) do
            myself.leadData.fuseaux[#myself.leadData.fuseaux +1] = CreateKnowledge( world.Fuseau, fuseau)
          end
       end
     end
        myself.leadData.currentAttackPosition = myself.leadData.currentAttackPosition % #myself.leadData.fuseaux + 1
        return myself.leadData.fuseaux[myself.leadData.currentAttackPosition]
    end,

    getNbrFront = function( self )
      local nbrEchelon = myself.taskParams.echelonNumber or 0
      if nbrEchelon == NIL or nbrEchelon == 0 then
        nbrEchelon = 1 -- default value
      end
      return integration.query.getNbrFront( nbrEchelon )
    end
}
