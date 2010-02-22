--Frag order Implementation
integration.isROE = function( self )
  return self.source:GetType()=="Rep_OrderConduite_ChangerReglesEngagement"
end

integration.isTask = function( self )
  orderType = self.source:GetType()
  return orderType == "net.masagroup.sword.military.tasks.Illuminate" or orderType =="net.masagroup.sword.military.tasks.ApplyIndirectFire"
end

integration.updateROE = function( self )
  DEC_Agent_ChangeEtatROE(self.source:GetorderConduiteChangerReglesEngagement_())
  DEC_RemoveFromOrdersCategory(self.source)
  DEC_DeleteRepresentation( self.source )
end

integration.startFragOrderTask = function( self )
  --TODO MGD Temp HardCoded fragOrder
  mission = {}
  
  orderType = self.source:GetType()
  if orderType == "net.masagroup.sword.military.tasks.Illuminate" then
    mission.entities = { CreateKnowledge( net.masagroup.sword.military.world.section, self.source:GetAgentKnowledge_() ) }
  elseif orderType =="net.masagroup.sword.military.tasks.ApplyIndirectFire" then
    mission.entity = CreateKnowledge( net.masagroup.sword.military.world.section, self.source:GetAgentKnowledge_() )
    mission.munition = self.source:Getmunitions_()
    mission.interventionType = self.source:GetnbIT_()
  end
  
  behavior_model.startTask( orderType, mission )
  
  DEC_RemoveFromOrdersCategory(self.source)
  DEC_DeleteRepresentation( self.source )
end
