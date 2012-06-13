------------------------------------------------------------------
--- Logistic Integration
------------------------------------------------------------------
integration.getActionCouranteConvoi = function()
    return DEC_Ravitaillement_Convoi_ActionCourante()
end

integration.getRavitailleur = function()
    return DEC_Ravitaillement_Convoi_Ravitailleur()
end
integration.getDestinataireCourant = function()
    return DEC_Ravitaillement_Convoi_DestinataireCourant()
end
integration.getTransporter = function()
    return DEC_Ravitaillement_Convoi_Transporteur()
end
integration.getItineraireVersProchaineDestination = function()
    return DEC_Ravitaillement_Convoi_ItineraireVersProchaineDestination()
end
integration.deplacementVersRavitailleurEffectue = function()
    DEC_Ravitaillement_Convoi_DeplacementVersRavitailleurEffectue()
end

integration.deplacementVersDestinataireEffectue = function()
    DEC_Ravitaillement_Convoi_DeplacementVersDestinataireEffectue()
end

integration.deplacementVersTransporteurEffectue = function()
    DEC_Ravitaillement_Convoi_DeplacementVersTransporteurEffectue()
end

integration.finMissionConvoi = function()
    DEC_Ravitaillement_Convoi_FinMission()
end

integration.isMedical = function( agent )
    return DEC_Agent_CanPerformHealthEvacuation( agent.source ) -- works with myself
end