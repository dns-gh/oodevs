-------------------------------------------------------------------------------
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
-------------------------------------------------------------------------------

tableRC = {}
tableRC[ eRC_BM_DebutMission ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_BM_FinMission ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_BM_DebutPhase ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_BM_FinPhase ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutAnimationObjet ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinAnimationObjet ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutDegagement ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutTravauxObjet ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinTravauxObjet ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TirDirectSurCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TirIndirectSurCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_EnnemiDetruit ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_EnnemiTactiquementDetruit ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinTirDirectSurCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinTirIndirectSurCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutNeutralisationCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinNeutralisationCible ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ObjetDejaOccupe ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ReussiteRenforcement ] = function( typeMessage, id, list ) return DEC_RC_Pion_Automate( myself, typeMessage, id, list[1], list[2] ) end
tableRC[ eRC_EchecRenforcement ] = function( typeMessage, id, list ) return DEC_RC_Pion_Automate( myself, typeMessage, id, list[1], list[2] ) end
tableRC[ eRC_ReussiteRenforcementPion ] = function( typeMessage, id, list ) return DEC_RC_Pion_Pion( myself, typeMessage, id, list[1], list[2] ) end
tableRC[ eRC_EchecRenforcementPion ] = function( typeMessage, id, list ) return DEC_RC_Pion_Pion( myself, typeMessage, id, list[1], list[2] ) end
tableRC[ eRC_SurLima ] = function( typeMessage, id, list ) return DEC_RC_Id( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_AttitudePopulation ] = function( typeMessage, id, list ) return DEC_RC_Id( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_MissionImpossibleReason ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ObjetNonContournable ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_LeaveLima ] = function( typeMessage, id, list ) return DEC_RC_Id( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutValorisation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinValorisation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_DebutDevalorisation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FinDevalorisation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ObstacleEnAttenteActivation ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_criticalIntelligence ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_RiposteContrePopulation ] = function( typeMessage, id, list ) return DEC_RC_PopulationKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TerroristCaptured ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TakeHostage ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TimeInUndergroundNetwork ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_PreparationMission ] = function( typeMessage, id, list ) return DEC_RC_Float( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_LimaParameterNotPresent ] = function( typeMessage, id, list ) return DEC_RC_Id( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TerroristDropped ] = function( typeMessage, id, list ) return DEC_RC_AgentKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ObstacleNonActivable ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_ActivationObstacleDeManoeuvre ] = function( typeMessage, id, list ) return DEC_RC_ObjectKnowledge( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_FollowingUnitIsDead ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_CombatSupportUnitIsDead ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TirIndirectLessInterventionType ] = function( typeMessage, id, list ) return DEC_RC_String( myself, typeMessage, id, list[1] ) end
tableRC[ eRC_TransportRoundTrip ] = function( typeMessage, id, list ) return DEC_RC_Float( myself, typeMessage, id, list[1], list[2] ) end
tableRC[ eRC_TransportRoundTrip ] = function( typeMessage, id, list ) return DEC_RC_Float( myself, typeMessage, id, list[1], list[2] ) end
