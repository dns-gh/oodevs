// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_CommunicationFunctions.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include <directia/Brain.h>

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::Register
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::Register( directia::Brain& brain )
{
    brain.RegisterFunction( "F_Pion_GeteEtatDec", &F_GeteEtatDec );
    brain.RegisterFunction( "F_Pion_SeteEtatDec", &F_SeteEtatDec );
    brain.RegisterFunction( "F_Pion_GeteEtatDecPrudence", &F_Pion_GeteEtatDecPrudence );
    brain.RegisterFunction( "F_Pion_SeteEtatDecPrudence", &F_Pion_SeteEtatDecPrudence );
    brain.RegisterFunction( "F_Pion_GeteEtatLima", &F_GeteEtatLima );
    brain.RegisterFunction( "F_Pion_SeteEtatLima", &F_SeteEtatLima );
    brain.RegisterFunction( "F_Pion_GeteEtatNbc", &F_Pion_GeteEtatNbc );
    brain.RegisterFunction( "F_Pion_SeteEtatNbc", &F_Pion_SeteEtatNbc );
    brain.RegisterFunction( "F_Pion_GeteEtatDestruction", &F_Pion_GeteEtatDestruction );
    brain.RegisterFunction( "F_Pion_SeteEtatDestruction", &F_Pion_SeteEtatDestruction );
    brain.RegisterFunction( "F_Pion_GeteEtatFeu", &F_Pion_GeteEtatFeu );
    brain.RegisterFunction( "F_Pion_SeteEtatFeu", &F_Pion_SeteEtatFeu );
    brain.RegisterFunction( "F_Pion_GeteEtatAmbiance", &F_Pion_GeteEtatAmbiance );
    brain.RegisterFunction( "F_Pion_SeteEtatAmbiance", &F_Pion_SeteEtatAmbiance );
    brain.RegisterFunction( "F_Pion_GeteEtatRadio", &F_Pion_GeteEtatRadio );
    brain.RegisterFunction( "F_Pion_SeteEtatRadio", &F_Pion_SeteEtatRadio );
    brain.RegisterFunction( "F_Pion_GeteEtatRadar", &F_Pion_GeteEtatRadar );
    brain.RegisterFunction( "F_Pion_SeteEtatRadar", &F_Pion_SeteEtatRadar );
    brain.RegisterFunction( "F_Pion_GeteEtatDeplacement", &F_Pion_GeteEtatDeplacement );
    brain.RegisterFunction( "F_Pion_SeteEtatDeplacement", &F_Pion_SeteEtatDeplacement );
    brain.RegisterFunction( "F_Pion_GeteEtatOrdreCoordination", &F_Pion_GeteEtatOrdreCoordination );
    brain.RegisterFunction( "F_Pion_SeteEtatOrdreCoordination", &F_Pion_SeteEtatOrdreCoordination );
    brain.RegisterFunction( "F_Pion_GeteConsigneTir", &F_Pion_GeteConsigneTir );
    brain.RegisterFunction( "F_Pion_SeteConsigneTir", &F_Pion_SeteConsigneTir );
    brain.RegisterFunction( "F_Pion_GeteConsigneTirPopulation", &F_Pion_GeteConsigneTirPopulation );
    brain.RegisterFunction( "F_Pion_SeteConsigneTirPopulation", &F_Pion_SeteConsigneTirPopulation );
    brain.RegisterFunction( "F_Pion_GeteEtatEchelon", &F_GeteEtatEchelon );
    brain.RegisterFunction( "F_Pion_SeteEtatEchelon", &F_SeteEtatEchelon );
    brain.RegisterFunction( "F_Pion_GeteEtatSoutien", &F_Pion_GeteEtatSoutien );
    brain.RegisterFunction( "F_Pion_SeteEtatSoutien", &F_Pion_SeteEtatSoutien );
    brain.RegisterFunction( "F_Pion_GeteEtatSituationEnnemi", &F_Pion_GeteEtatSituationEnnemi );
    brain.RegisterFunction( "F_Pion_SeteEtatSituationEnnemi", &F_Pion_SeteEtatSituationEnnemi );
    brain.RegisterFunction( "F_Pion_GeteEtatPhaseMission", &F_GeteEtatPhaseMission );
    brain.RegisterFunction( "F_Pion_SeteEtatPhaseMission", &F_SeteEtatPhaseMission );
    brain.RegisterFunction( "F_Pion_GetbOrdreInterrompreMission", &F_Pion_GetbOrdreInterrompreMission );
    brain.RegisterFunction( "F_Pion_SetbOrdreInterrompreMission", &F_Pion_SetbOrdreInterrompreMission );
    brain.RegisterFunction( "F_Pion_GetbOrdreDecrocher", &F_GetbOrdreDecrocher );
    brain.RegisterFunction( "F_Pion_SetbOrdreDecrocher", &F_SetbOrdreDecrocher );
    brain.RegisterFunction( "F_Pion_GetbOrdreTenirSurLR", &F_GetbOrdreTenirSurLR );
    brain.RegisterFunction( "F_Pion_SetbOrdreTenirSurLR", &F_SetbOrdreTenirSurLR );
    brain.RegisterFunction( "F_Pion_GetbOrdreTenir", &F_GetbOrdreTenir );
    brain.RegisterFunction( "F_Pion_SetbOrdreTenir", &F_SetbOrdreTenir );
    brain.RegisterFunction( "F_Pion_GetbPasserSurLC", &F_Pion_GetbPasserSurLC );
    brain.RegisterFunction( "F_Pion_SetbPasserSurLC", &F_Pion_SetbPasserSurLC );
    brain.RegisterFunction( "F_Pion_GetpionEnEscorte",* &F_Pion_GetpionEnEscorte );
    brain.RegisterFunction( "F_Pion_SetpionEnEscorte", &F_Pion_SetpionEnEscorte );
    brain.RegisterFunction( "F_Pion_GetitMvt", &F_Pion_GetitMvt );
    brain.RegisterFunction( "F_Pion_SetitMvt", &F_Pion_SetitMvt );
    brain.RegisterFunction( "F_Pion_GetobjectifEsquive", &F_Pion_GetobjectifEsquive );
    brain.RegisterFunction( "F_Pion_SetobjectifEsquive", &F_Pion_SetobjectifEsquive );
    brain.RegisterFunction( "F_Pion_GeteniEnCours", &F_Pion_GeteniEnCours );
    brain.RegisterFunction( "F_Pion_SeteniEnCours", &F_Pion_SeteniEnCours );
    brain.RegisterFunction( "F_Pion_GetmissionPrecedente", &F_Pion_GetmissionPrecedente );
    brain.RegisterFunction( "F_Pion_SetmissionPrecedente", &F_Pion_SetmissionPrecedente );
    brain.RegisterFunction( "F_Pion_GetrTenir", &F_Pion_GetrTenir );
    brain.RegisterFunction( "F_Pion_SetrTenir", &F_Pion_SetrTenir );
    brain.RegisterFunction( "F_Pion_GeteTypeContact", &F_Pion_GeteTypeContact );
    brain.RegisterFunction( "F_Pion_SeteTypeContact", &F_Pion_SeteTypeContact );
    brain.RegisterFunction( "F_Pion_GeteNiveauAction", &F_Pion_GeteNiveauAction );
    brain.RegisterFunction( "F_Pion_SeteNiveauAction", &F_Pion_SeteNiveauAction );
    brain.RegisterFunction( "F_Pion_GetbDefenseStatique_Mobile", &F_Pion_GetbDefenseStatique_Mobile );
    brain.RegisterFunction( "F_Pion_SetbDefenseStatique_Mobile", &F_Pion_SetbDefenseStatique_Mobile );
    brain.RegisterFunction( "F_Pion_GetselUnitesEnAppui", &F_Pion_GetselUnitesEnAppui );
    brain.RegisterFunction( "F_AddToselUnitesEnAppui", &F_AddToselUnitesEnAppui );
    brain.RegisterFunction( "F_RemoveFromselUnitesEnAppui", &F_RemoveFromselUnitesEnAppui );

    brain.RegisterFunction( "F_Automat_GeteEtatPhaseMission", &F_GeteEtatPhaseMission );
    brain.RegisterFunction( "F_Automat_SeteEtatPhaseMission", &F_SeteEtatPhaseMission );
    brain.RegisterFunction( "F_Automat_GeteEtatLima", &F_GeteEtatLima );
    brain.RegisterFunction( "F_Automat_SeteEtatLima", &F_SeteEtatLima );
    brain.RegisterFunction( "F_Automat_GeteEtatDec", &F_GeteEtatDec );
    brain.RegisterFunction( "F_Automat_SeteEtatDec", &F_SeteEtatDec );
    brain.RegisterFunction( "F_Automat_GetrDestruction", &F_Automat_GetrDestruction );
    brain.RegisterFunction( "F_Automat_SetrDestruction", &F_Automat_SetrDestruction );
    brain.RegisterFunction( "F_Automat_GeteEtatEchelon", &F_GeteEtatEchelon );
    brain.RegisterFunction( "F_Automat_SeteEtatEchelon", &F_SeteEtatEchelon );
    brain.RegisterFunction( "F_Automat_Getfuseau", &F_Automat_Getfuseau );
    brain.RegisterFunction( "F_Automat_Setfuseau", &F_Automat_Setfuseau );
    brain.RegisterFunction( "F_Automat_Getzone", &F_Automat_Getzone );
    brain.RegisterFunction( "F_Automat_Setzone", &F_Automat_Setzone );
    brain.RegisterFunction( "F_Automat_GetbOrdreAttendre", &F_Automat_GetbOrdreAttendre );
    brain.RegisterFunction( "F_Automat_SetbOrdreAttendre", &F_Automat_SetbOrdreAttendre );
    brain.RegisterFunction( "F_Automat_GetbOrdrePoursuivre", &F_Automat_GetbOrdrePoursuivre );
    brain.RegisterFunction( "F_Automat_SetbOrdrePoursuivre", &F_Automat_SetbOrdrePoursuivre );
    brain.RegisterFunction( "F_Automat_GetbOrdreRalentir", &F_Automat_GetbOrdreRalentir );
    brain.RegisterFunction( "F_Automat_SetbOrdreRalentir", &F_Automat_SetbOrdreRalentir );
    brain.RegisterFunction( "F_Automat_GetbOrdreDecrocher", &F_GetbOrdreDecrocher );
    brain.RegisterFunction( "F_Automat_SetbOrdreDecrocher", &F_SetbOrdreDecrocher );
    brain.RegisterFunction( "F_Automat_GetbOrdreTenirSurLR", &F_GetbOrdreTenirSurLR );
    brain.RegisterFunction( "F_Automat_SetbOrdreTenirSurLR", &F_SetbOrdreTenirSurLR );
    brain.RegisterFunction( "F_Automat_GetbOrdreTenir", &F_GetbOrdreTenir );
    brain.RegisterFunction( "F_Automat_SetbOrdreTenir", &F_SetbOrdreTenir );
}

#pragma warning( disable : 4100 )

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GeteEtatDec
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_GeteEtatDec( DEC_Decision_ABC* pAgent )
{
    return pAgent->GeteEtatDec();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SeteEtatDec
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SeteEtatDec( DEC_Decision_ABC* pAgent, int value )
{
    pAgent->SeteEtatDec( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatDecPrudence
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatDecPrudence( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatDecPrudence();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatDecPrudence
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatDecPrudence( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatDecPrudence( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GeteEtatLima
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_GeteEtatLima( DEC_Decision_ABC* pAgent )
{
    return pAgent->GeteEtatLima();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SeteEtatLima
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SeteEtatLima( DEC_Decision_ABC* pAgent, int value )
{
    pAgent->SeteEtatLima( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatNbc
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatNbc( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatNbc();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatNbc
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatNbc( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatNbc( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatDestruction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatDestruction( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatDestruction();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatDestruction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatDestruction( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatDestruction( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatFeu
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatFeu( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatFeu();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatFeu
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatFeu( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatFeu( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatAmbiance
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatAmbiance( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatAmbiance();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatAmbiance
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatAmbiance( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatAmbiance( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatRadio
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatRadio( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatRadio();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatRadio
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatRadio( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatRadio( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatRadar
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatRadar( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatRadar();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatRadar
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatRadar( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatRadar( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatDeplacement
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatDeplacement( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatDeplacement();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatDeplacement
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatDeplacement( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatDeplacement( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatOrdreCoordination
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatOrdreCoordination( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatOrdreCoordination();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatOrdreCoordination
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatOrdreCoordination( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatOrdreCoordination( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteConsigneTir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteConsigneTir( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteConsigneTir();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteConsigneTir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteConsigneTir( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteConsigneTir( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteConsigneTirPopulation
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteConsigneTirPopulation( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteConsigneTirPopulation();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteConsigneTirPopulation
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteConsigneTirPopulation( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteConsigneTirPopulation( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GeteEtatEchelon
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_GeteEtatEchelon( DEC_Decision_ABC* pAgent )
{
    return pAgent->GeteEtatEchelon();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SeteEtatEchelon
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SeteEtatEchelon( DEC_Decision_ABC* pAgent, int value )
{
    pAgent->SeteEtatEchelon( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatSoutien
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatSoutien( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatSoutien();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatSoutien
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatSoutien( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatSoutien( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatSituationEnnemi
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatSituationEnnemi( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteEtatSituationEnnemi();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatSituationEnnemi
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatSituationEnnemi( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteEtatSituationEnnemi( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GeteEtatPhaseMission
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_GeteEtatPhaseMission( DEC_Decision_ABC* pAgent )
{
    return pAgent->GeteEtatPhaseMission();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SeteEtatPhaseMission
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SeteEtatPhaseMission( DEC_Decision_ABC* pAgent, int value )
{
    pAgent->SeteEtatPhaseMission( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetbOrdreInterrompreMission
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Pion_GetbOrdreInterrompreMission( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GetbOrdreInterrompreMission();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetbOrdreInterrompreMission
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetbOrdreInterrompreMission( DEC_Decision_ABC* pPion, bool value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SetbOrdreInterrompreMission( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GetbOrdreDecrocher
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_GetbOrdreDecrocher( DEC_Decision_ABC* pAgent )
{
    return pAgent->GetbOrdreDecrocher();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SetbOrdreDecrocher
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SetbOrdreDecrocher( DEC_Decision_ABC* pAgent, bool value )
{
    pAgent->SetbOrdreDecrocher( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GetbOrdreTenirSurLR
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_GetbOrdreTenirSurLR( DEC_Decision_ABC* pAgent )
{
    return pAgent->GetbOrdreTenirSurLR();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SetbOrdreTenirSurLR
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SetbOrdreTenirSurLR( DEC_Decision_ABC* pAgent, bool value )
{
    pAgent->SetbOrdreTenirSurLR( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GetbOrdreTenir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_GetbOrdreTenir( DEC_Decision_ABC* pAgent )
{
    return pAgent->GetbOrdreTenir();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SetbOrdreTenir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SetbOrdreTenir( DEC_Decision_ABC* pAgent, bool value )
{
    pAgent->SetbOrdreTenir( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetbPasserSurLC
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Pion_GetbPasserSurLC( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GetbPasserSurLC();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetbPasserSurLC
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetbPasserSurLC( DEC_Decision_ABC* pPion, bool value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SetbPasserSurLC( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::
//F_Pion_GetpionEnEscorte Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_CommunicationFunctions::F_Pion_GetpionEnEscorte( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GetpionEnEscorte();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetpionEnEscorte
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetpionEnEscorte( DEC_Decision_ABC* pPion, DEC_Decision_ABC* value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SetpionEnEscorte( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::
//F_Pion_GetitMvt Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Path_ABC > DEC_CommunicationFunctions::F_Pion_GetitMvt( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GetitMvt();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetitMvt
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetitMvt( DEC_Decision_ABC* pPion, const boost::shared_ptr< DEC_Path_ABC >& value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SetitMvt( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::
//F_Pion_GetobjectifEsquive Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_CommunicationFunctions::F_Pion_GetobjectifEsquive( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GetobjectifEsquive();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetobjectifEsquive
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetobjectifEsquive( DEC_Decision_ABC* pPion, MT_Vector2D* value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SetobjectifEsquive( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::
//F_Pion_GeteniEnCours Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_CommunicationFunctions::F_Pion_GeteniEnCours( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteniEnCours();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteniEnCours
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteniEnCours( DEC_Decision_ABC* pPion, boost::shared_ptr< DEC_Knowledge_Agent > value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteniEnCours( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::string 
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
std::string DEC_CommunicationFunctions::F_Pion_GetmissionPrecedente( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GetmissionPrecedente();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetmissionPrecedente
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetmissionPrecedente( DEC_Decision_ABC* pPion, const std::string& value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SetmissionPrecedente( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetrTenir
// Created: LDC 2009-05-11
// return pPion->etrTenir-----------------------------------();
float DEC_CommunicationFunctions::F_Pion_GetrTenir( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GetrTenir();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetrTenir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetrTenir( DEC_Decision_ABC* pPion, float value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SetrTenir( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteTypeContact
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteTypeContact( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteTypeContact();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteTypeContact
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteTypeContact( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteTypeContact( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteNiveauAction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteNiveauAction( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GeteNiveauAction();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteNiveauAction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteNiveauAction( DEC_Decision_ABC* pPion, int value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SeteNiveauAction( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetbDefenseStatique_Mobile
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Pion_GetbDefenseStatique_Mobile( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GetbDefenseStatique_Mobile();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetbDefenseStatique_Mobile
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetbDefenseStatique_Mobile( DEC_Decision_ABC* pPion, bool value )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->SetbDefenseStatique_Mobile( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::vector
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
const std::vector< DEC_Decision_ABC* >* DEC_CommunicationFunctions::F_Pion_GetselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    return dynamic_cast< DEC_RolePion_Decision* >( pPion )->GetselUnitesEnAppui();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_AddToselUnitesEnAppui
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_AddToselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->AddToselUnitesEnAppui( pPion );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_RemoveFromselUnitesEnAppui
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_RemoveFromselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    dynamic_cast< DEC_RolePion_Decision* >( pPion )->RemoveFromselUnitesEnAppui( pPion );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_GetrDestruction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
float DEC_CommunicationFunctions::F_Automat_GetrDestruction( DEC_Decision_ABC* pAutomat )
{
    return dynamic_cast< DEC_AutomateDecision* >( pAutomat )->GetrDestruction();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_SetrDestruction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_SetrDestruction( DEC_Decision_ABC* pAutomat, float value )
{
    dynamic_cast< DEC_AutomateDecision* >( pAutomat )->SetrDestruction( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::
//F_Automat_Getfuseau Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
MIL_Fuseau* DEC_CommunicationFunctions::F_Automat_Getfuseau( DEC_Decision_ABC* pAutomat )
{
    return dynamic_cast< DEC_AutomateDecision* >( pAutomat )->Getfuseau();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_Setfuseau
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_Setfuseau( DEC_Decision_ABC* pAutomat, MIL_Fuseau* value )
{
    dynamic_cast< DEC_AutomateDecision* >( pAutomat )->Setfuseau( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::
//F_Automat_Getzone Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
TER_Localisation* DEC_CommunicationFunctions::F_Automat_Getzone( DEC_Decision_ABC* pAutomat )
{
    return dynamic_cast< DEC_AutomateDecision* >( pAutomat )->Getzone();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_Setzone
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_Setzone( DEC_Decision_ABC* pAutomat, TER_Localisation* value )
{
    dynamic_cast< DEC_AutomateDecision* >( pAutomat )->Setzone( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_GetbOrdreAttendre
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Automat_GetbOrdreAttendre( DEC_Decision_ABC* pAutomat )
{
    return dynamic_cast< DEC_AutomateDecision* >( pAutomat )->GetbOrdreAttendre();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_SetbOrdreAttendre
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_SetbOrdreAttendre( DEC_Decision_ABC* pAutomat, bool value )
{
    dynamic_cast< DEC_AutomateDecision* >( pAutomat )->SetbOrdreAttendre( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_GetbOrdrePoursuivre
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Automat_GetbOrdrePoursuivre( DEC_Decision_ABC* pAutomat )
{
    return dynamic_cast< DEC_AutomateDecision* >( pAutomat )->GetbOrdrePoursuivre();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_SetbOrdrePoursuivre
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_SetbOrdrePoursuivre( DEC_Decision_ABC* pAutomat, bool value )
{
    dynamic_cast< DEC_AutomateDecision* >( pAutomat )->SetbOrdrePoursuivre( value );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_GetbOrdreRalentir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Automat_GetbOrdreRalentir( DEC_Decision_ABC* pAutomat )
{
    return dynamic_cast< DEC_AutomateDecision* >( pAutomat )->GetbOrdreRalentir();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_SetbOrdreRalentir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_SetbOrdreRalentir( DEC_Decision_ABC* pAutomat, bool value )
{
    dynamic_cast< DEC_AutomateDecision* >( pAutomat )->SetbOrdreRalentir( value );
}
