// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_CommunicationFunctions_h_
#define __DEC_CommunicationFunctions_h_

namespace directia
{
    namespace brain
    {
        class Brain;
    }
}

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Path_ABC;
class MIL_Fuseau;
class MT_Vector2D;
class TER_Localisation;

// =============================================================================
/** @class  DEC_CommunicationFunctions
    @brief  DEC Communication functions
*/
// Created: LDC 2009-05-11
// =============================================================================
class DEC_CommunicationFunctions
{
public:
    static void Register( directia::brain::Brain& brain );

    static int F_Pion_GeteEtatDecPrudence( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatDecPrudence( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatNbc( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatNbc( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatDestruction( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatDestruction( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatFeu( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatFeu( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatAmbiance( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatAmbiance( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatRadio( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatRadio( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatRadar( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatRadar( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatDeplacement( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatDeplacement( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatOrdreCoordination( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatOrdreCoordination( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteConsigneTir( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteConsigneTir( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteConsigneTirPopulation( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteConsigneTirPopulation( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatSoutien( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatSoutien( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteEtatSituationEnnemi( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteEtatSituationEnnemi( DEC_Decision_ABC* pPion, int value );
    static bool F_Pion_GetbOrdreInterrompreMission( DEC_Decision_ABC* pPion );
    static void F_Pion_SetbOrdreInterrompreMission( DEC_Decision_ABC* pPion, bool value );
    static bool F_Pion_GetbOrdreDecrocher( DEC_Decision_ABC* pPion );
    static void F_Pion_SetbOrdreDecrocher( DEC_Decision_ABC* pPion, bool value );
    static bool F_Pion_GetbPasserSurLC( DEC_Decision_ABC* pPion );
    static void F_Pion_SetbPasserSurLC( DEC_Decision_ABC* pPion, bool value );
    static DEC_Decision_ABC* F_Pion_GetpionEnEscorte( DEC_Decision_ABC* pPion );
    static void F_Pion_SetpionEnEscorte( DEC_Decision_ABC* pPion, DEC_Decision_ABC* value );
    static bool F_Pion_GetNewEscorted( DEC_Decision_ABC* pPion );
    static void F_Pion_SetNewEscorted( DEC_Decision_ABC* pPion, bool value );
    static bool F_Pion_GetNeedReinforcement( DEC_Decision_ABC* pPion );
    static void F_Pion_SetNeedReinforcement( DEC_Decision_ABC* pPion, bool value );
    static boost::shared_ptr< DEC_Path_ABC > F_Pion_GetitMvt( DEC_Decision_ABC* pPion );
    static void F_Pion_SetitMvt( DEC_Decision_ABC* pPion, const boost::shared_ptr< DEC_Path_ABC >& value );
    static boost::shared_ptr< MT_Vector2D > F_Pion_GetobjectifEsquive( DEC_Decision_ABC* pPion );
    static void F_Pion_SetobjectifEsquive( DEC_Decision_ABC* pPion, MT_Vector2D* value );
    static boost::shared_ptr< DEC_Knowledge_Agent > F_Pion_GeteniEnCours( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteniEnCours( DEC_Decision_ABC* pPion, boost::shared_ptr< DEC_Knowledge_Agent > value );
    static std::string F_Pion_GetmissionPrecedente( DEC_Decision_ABC* pPion );
    static void F_Pion_SetmissionPrecedente( DEC_Decision_ABC* pPion, const std::string& value );
    static float F_Pion_GetrTenir( DEC_Decision_ABC* pPion );
    static void F_Pion_SetrTenir( DEC_Decision_ABC* pPion, float value );
    static int F_Pion_GeteTypeContact( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteTypeContact( DEC_Decision_ABC* pPion, int value );
    static int F_Pion_GeteNiveauAction( DEC_Decision_ABC* pPion );
    static void F_Pion_SeteNiveauAction( DEC_Decision_ABC* pPion, int value );
    static bool F_Pion_GetbDefenseStatique_Mobile( DEC_Decision_ABC* pPion );
    static void F_Pion_SetbDefenseStatique_Mobile( DEC_Decision_ABC* pPion, bool value );
    static const std::vector< DEC_Decision_ABC* >* F_Pion_GetselUnitesEnAppui( DEC_Decision_ABC* pPion );
    static void F_AddToselUnitesEnAppui( DEC_Decision_ABC* pPion );
    static void F_RemoveFromselUnitesEnAppui( DEC_Decision_ABC* pPion );
    static void F_Mot_EtatDesactiveSauvegarde( DEC_Decision_ABC* pPion, bool value );

    static int  F_GeteEtatPhaseMission( DEC_Decision_ABC* pAgent );
    static void F_SeteEtatPhaseMission( DEC_Decision_ABC* pAgent, int value );
    static int  F_GeteEtatLima( DEC_Decision_ABC* pAgent );
    static void F_SeteEtatLima( DEC_Decision_ABC* pAgent, int value );
    static int  F_GeteEtatDec( DEC_Decision_ABC* pAgent );
    static void F_SeteEtatDec( DEC_Decision_ABC* pAgent, int value );
    static int  F_GeteEtatEchelon( DEC_Decision_ABC* pAgent );
    static void F_SeteEtatEchelon( DEC_Decision_ABC* pAgent, int value );
    static bool F_GetbOrdreDecrocher( DEC_Decision_ABC* pAgent );
    static void F_SetbOrdreDecrocher( DEC_Decision_ABC* pAgent, bool value );
    static bool F_GetbOrdreTenirSurLR( DEC_Decision_ABC* pAgent );
    static void F_SetbOrdreTenirSurLR( DEC_Decision_ABC* pAgent, bool value );
    static bool F_GetbOrdreTenir( DEC_Decision_ABC* pAgent );
    static void F_SetbOrdreTenir( DEC_Decision_ABC* pAgent, bool value );

    static float F_Automat_GetrDestruction( DEC_Decision_ABC* pAutomat );
    static void F_Automat_SetrDestruction( DEC_Decision_ABC* pAutomat, float value );
    static const MIL_Fuseau* F_Automat_Getfuseau( DEC_Decision_ABC* pAutomat );
    static void F_Automat_Setfuseau( DEC_Decision_ABC* pAutomat, MIL_Fuseau* value );
    static TER_Localisation* F_Automat_Getzone( DEC_Decision_ABC* pAutomat );
    static void F_Automat_Setzone( DEC_Decision_ABC* pAutomat, TER_Localisation* value );
    static bool F_Automat_GetbOrdreAttendre( DEC_Decision_ABC* pAutomat );
    static void F_Automat_SetbOrdreAttendre( DEC_Decision_ABC* pAutomat, bool value );
    static bool F_Automat_GetbOrdrePoursuivre( DEC_Decision_ABC* pAutomat );
    static void F_Automat_SetbOrdrePoursuivre( DEC_Decision_ABC* pAutomat, bool value );
    static bool F_Automat_GetbOrdreRalentir( DEC_Decision_ABC* pAutomat );
    static void F_Automat_SetbOrdreRalentir( DEC_Decision_ABC* pAutomat, bool value );
};


#endif // __DEC_CommunicationFunctions_h_
