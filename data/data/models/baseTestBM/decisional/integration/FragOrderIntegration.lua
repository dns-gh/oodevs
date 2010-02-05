--Frag order Implementation
integration.isROE = function( self )
  return self.source:GetType()=="Rep_OrderConduite_ChangerReglesEngagement"
end

integration.updateROE = function( self )
  DEC_Agent_ChangeEtatROE(self.source:GetorderConduiteChangerReglesEngagement_())
  DEC_RemoveFromOrdersCategory(self.source)
  DEC_DeleteRepresentation( self.source )
end
