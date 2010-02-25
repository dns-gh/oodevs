--Safeguarding Implementation

integration.isSafeguardingFor = function( self, objectives )
   for _, objective in pairs( objectives ) do
      if kBase.me.body:computeProtectionCapability( self, objective ) > 0 and ( objective:getEstimateAttrition( self ) == 0  or kBase.me.body:computePerceptionCapability( self, objective ) == 0 ) then
        return true
      end
   end
   return false
end
