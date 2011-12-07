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
#include <directia/brain/Brain.h>

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::Register
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::Register( directia::brain::Brain& brain )
{
    //
    brain[ "F_Pion_GeteEtatDec" ] = &F_GeteEtatDec;
    brain[ "F_Pion_SeteEtatDec" ] = &F_SeteEtatDec;
    brain[ "F_Pion_GeteEtatDecPrudence" ] = &F_Pion_GeteEtatDecPrudence;
    brain[ "F_Pion_SeteEtatDecPrudence" ] = &F_Pion_SeteEtatDecPrudence;
    brain[ "F_Pion_GeteEtatLima" ] = &F_GeteEtatLima;
    brain[ "F_Pion_SeteEtatLima" ] = &F_SeteEtatLima;
    brain[ "F_Pion_GeteEtatNbc" ] = &F_Pion_GeteEtatNbc;
    brain[ "F_Pion_SeteEtatNbc" ] = &F_Pion_SeteEtatNbc;
    brain[ "F_Pion_GeteEtatDestruction" ] = &F_Pion_GeteEtatDestruction;
    brain[ "F_Pion_SeteEtatDestruction" ] = &F_Pion_SeteEtatDestruction;
    brain[ "F_Pion_GeteEtatFeu" ] = &F_Pion_GeteEtatFeu;
    brain[ "F_Pion_SeteEtatFeu" ] = &F_Pion_SeteEtatFeu;
    brain[ "F_Pion_GeteEtatAmbiance" ] = &F_Pion_GeteEtatAmbiance;
    brain[ "F_Pion_SeteEtatAmbiance" ] = &F_Pion_SeteEtatAmbiance;
    brain[ "F_Pion_GeteEtatRadio" ] = &F_Pion_GeteEtatRadio;
    brain[ "F_Pion_SeteEtatRadio" ] = &F_Pion_SeteEtatRadio;
    brain[ "F_Pion_GeteEtatRadar" ] = &F_Pion_GeteEtatRadar;
    brain[ "F_Pion_SeteEtatRadar" ] = &F_Pion_SeteEtatRadar;
    brain[ "F_Pion_GeteEtatDeplacement" ] = &F_Pion_GeteEtatDeplacement;
    brain[ "F_Pion_SeteEtatDeplacement" ] = &F_Pion_SeteEtatDeplacement;
    brain[ "F_Pion_GeteEtatOrdreCoordination" ] = &F_Pion_GeteEtatOrdreCoordination;
    brain[ "F_Pion_SeteEtatOrdreCoordination" ] = &F_Pion_SeteEtatOrdreCoordination;
    brain[ "F_Pion_GeteConsigneTir" ] = &F_Pion_GeteConsigneTir;
    brain[ "F_Pion_SeteConsigneTir" ] = &F_Pion_SeteConsigneTir;
    brain[ "F_Pion_GeteConsigneTirPopulation" ] = &F_Pion_GeteConsigneTirPopulation;
    brain[ "F_Pion_SeteConsigneTirPopulation" ] = &F_Pion_SeteConsigneTirPopulation;
    brain[ "F_Pion_GeteEtatEchelon" ] = &F_GeteEtatEchelon;
    brain[ "F_Pion_SeteEtatEchelon" ] = &F_SeteEtatEchelon;
    brain[ "F_Pion_GeteEtatSoutien" ] = &F_Pion_GeteEtatSoutien;
    brain[ "F_Pion_SeteEtatSoutien" ] = &F_Pion_SeteEtatSoutien;
    brain[ "F_Pion_GeteEtatSituationEnnemi" ] = &F_Pion_GeteEtatSituationEnnemi;
    brain[ "F_Pion_SeteEtatSituationEnnemi" ] = &F_Pion_SeteEtatSituationEnnemi;
    brain[ "F_Pion_GeteEtatPhaseMission" ] = &F_GeteEtatPhaseMission;
    brain[ "F_Pion_SeteEtatPhaseMission" ] = &F_SeteEtatPhaseMission;
    //
    brain[ "F_Pion_GetbOrdreInterrompreMission" ] = &F_Pion_GetbOrdreInterrompreMission;
    brain[ "F_Pion_SetbOrdreInterrompreMission" ] = &F_Pion_SetbOrdreInterrompreMission;
    brain[ "F_Pion_GetbOrdreDecrocher" ] = &F_GetbOrdreDecrocher;
    brain[ "F_Pion_SetbOrdreDecrocher" ] = &F_SetbOrdreDecrocher;
    brain[ "F_Pion_GetbOrdreTenirSurLR" ] = &F_GetbOrdreTenirSurLR;
    brain[ "F_Pion_SetbOrdreTenirSurLR" ] = &F_SetbOrdreTenirSurLR;
    brain[ "F_Pion_GetbOrdreTenir" ] = &F_GetbOrdreTenir;
    brain[ "F_Pion_SetbOrdreTenir" ] = &F_SetbOrdreTenir;
    brain[ "F_Pion_GetbPasserSurLC" ] = &F_Pion_GetbPasserSurLC;
    brain[ "F_Pion_SetbPasserSurLC" ] = &F_Pion_SetbPasserSurLC;
    brain[ "F_Pion_GetNewEscorted" ] = &F_Pion_GetNewEscorted;
    brain[ "F_Pion_SetNewEscorted" ] = &F_Pion_SetNewEscorted;
    brain[ "F_Pion_GetNeedReinforcement" ] = &F_Pion_GetNeedReinforcement;
    brain[ "F_Pion_SetNeedReinforcement" ] = &F_Pion_SetNeedReinforcement;
    brain[ "F_Pion_GetEnemyAttackedBy" ] = &F_Pion_GetEnemyAttackedBy;
    brain[ "F_Pion_GetpionEnEscorte" ] = &F_Pion_GetpionEnEscorte;
    brain[ "F_Pion_SetpionEnEscorte" ] = &F_Pion_SetpionEnEscorte;
    brain[ "F_Pion_GetitMvt" ] = &F_Pion_GetitMvt;
    brain[ "F_Pion_SetitMvt" ] = &F_Pion_SetitMvt;
    brain[ "F_Pion_GetobjectifEsquive" ] = &F_Pion_GetobjectifEsquive;
    brain[ "F_Pion_SetobjectifEsquive" ] = &F_Pion_SetobjectifEsquive;
    brain[ "F_Pion_GeteniEnCours" ] = &F_Pion_GeteniEnCours;
    brain[ "F_Pion_SeteniEnCours" ] = &F_Pion_SeteniEnCours;
    brain[ "F_Pion_GetmissionPrecedente" ] = &F_Pion_GetmissionPrecedente;
    brain[ "F_Pion_SetmissionPrecedente" ] = &F_Pion_SetmissionPrecedente;
    brain[ "F_Pion_GetrTenir" ] = &F_Pion_GetrTenir;
    brain[ "F_Pion_SetrTenir" ] = &F_Pion_SetrTenir;
    //
    brain[ "F_Pion_GeteTypeContact" ] = &F_Pion_GeteTypeContact;
    brain[ "F_Pion_SeteTypeContact" ] = &F_Pion_SeteTypeContact;
    brain[ "F_Pion_GeteNiveauAction" ] = &F_Pion_GeteNiveauAction;
    brain[ "F_Pion_SeteNiveauAction" ] = &F_Pion_SeteNiveauAction;
    //
    brain[ "F_Pion_GetbDefenseStatique_Mobile" ] = &F_Pion_GetbDefenseStatique_Mobile;
    brain[ "F_Pion_SetbDefenseStatique_Mobile" ] = &F_Pion_SetbDefenseStatique_Mobile;
    brain[ "F_Pion_GetselUnitesEnAppui" ] = &F_Pion_GetselUnitesEnAppui;
    brain[ "F_AddToselUnitesEnAppui" ] = &F_AddToselUnitesEnAppui;
    brain[ "F_RemoveFromselUnitesEnAppui" ] = &F_RemoveFromselUnitesEnAppui;
    brain[ "F_Mot_EtatDesactiveSauvegarde" ] = &F_Mot_EtatDesactiveSauvegarde;

    //
    brain[ "F_Automat_GeteEtatPhaseMission" ] = &F_GeteEtatPhaseMission;
    brain[ "F_Automat_SeteEtatPhaseMission" ] = &F_SeteEtatPhaseMission;
    brain[ "F_Automat_GeteEtatLima" ] = &F_GeteEtatLima;
    brain[ "F_Automat_SeteEtatLima" ] = &F_SeteEtatLima;
    brain[ "F_Automat_GeteEtatDec" ] = &F_GeteEtatDec;
    brain[ "F_Automat_SeteEtatDec" ] = &F_SeteEtatDec;
    //
    brain[ "F_Automat_GetrDestruction" ] = &F_Automat_GetrDestruction;
    brain[ "F_Automat_SetrDestruction" ] = &F_Automat_SetrDestruction;
    //
    brain[ "F_Automat_GeteEtatEchelon" ] = &F_GeteEtatEchelon;
    brain[ "F_Automat_SeteEtatEchelon" ] = &F_SeteEtatEchelon;
    //
    brain[ "F_Automat_Getfuseau" ] = &F_Automat_Getfuseau;
    brain[ "F_Automat_Setfuseau" ] = &F_Automat_Setfuseau;
    brain[ "F_Automat_Getzone" ] = &F_Automat_Getzone;
    brain[ "F_Automat_Setzone" ] = &F_Automat_Setzone;
    brain[ "F_Automat_GetbOrdreAttendre" ] = &F_Automat_GetbOrdreAttendre;
    brain[ "F_Automat_SetbOrdreAttendre" ] = &F_Automat_SetbOrdreAttendre;
    brain[ "F_Automat_GetbOrdrePoursuivre" ] = &F_Automat_GetbOrdrePoursuivre;
    brain[ "F_Automat_SetbOrdrePoursuivre" ] = &F_Automat_SetbOrdrePoursuivre;
    brain[ "F_Automat_GetbOrdreRalentir" ] = &F_Automat_GetbOrdreRalentir;
    brain[ "F_Automat_SetbOrdreRalentir" ] = &F_Automat_SetbOrdreRalentir;
    brain[ "F_Automat_GetbOrdreDecrocher" ] = &F_GetbOrdreDecrocher;
    brain[ "F_Automat_SetbOrdreDecrocher" ] = &F_SetbOrdreDecrocher;
    brain[ "F_Automat_GetbOrdreTenirSurLR" ] = &F_GetbOrdreTenirSurLR;
    brain[ "F_Automat_SetbOrdreTenirSurLR" ] = &F_SetbOrdreTenirSurLR;
    brain[ "F_Automat_GetbOrdreTenir" ] = &F_GetbOrdreTenir;
    brain[ "F_Automat_SetbOrdreTenir" ] = &F_SetbOrdreTenir;
}

#pragma warning( disable : 4100 )

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GeteEtatDec
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_GeteEtatDec( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_GeteEtatDec" );
    return pAgent->GeteEtatDec();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SeteEtatDec
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SeteEtatDec( DEC_Decision_ABC* pAgent, int value )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_SeteEtatDec" );
    pAgent->SeteEtatDec( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatDecPrudence
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatDecPrudence( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatDecPrudence not called on DEC_RolePion_Decision" );
    return pion->GeteEtatDecPrudence();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatDecPrudence
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatDecPrudence( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatDecPrudence not called on DEC_RolePion_Decision" );
    pion->SeteEtatDecPrudence( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GeteEtatLima
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_GeteEtatLima( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_GeteEtatLima" );
    return pAgent->GeteEtatLima();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SeteEtatLima
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SeteEtatLima( DEC_Decision_ABC* pAgent, int value )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_SeteEtatLima" );
    pAgent->SeteEtatLima( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatNbc
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatNbc( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatNbc not called on DEC_RolePion_Decision" );
    return pion->GeteEtatNbc();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatNbc
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatNbc( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatNbc not called on DEC_RolePion_Decision" );
    pion->SeteEtatNbc( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatDestruction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatDestruction( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatDestruction not called on DEC_RolePion_Decision" );
    return pion->GeteEtatDestruction();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatDestruction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatDestruction( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatDestruction not called on DEC_RolePion_Decision" );
    pion->SeteEtatDestruction( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatFeu
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatFeu( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatFeu not called on DEC_RolePion_Decision" );
    return pion->GeteEtatFeu();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatFeu
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatFeu( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatFeu not called on DEC_RolePion_Decision" );
    pion->SeteEtatFeu( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatAmbiance
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatAmbiance( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatAmbiance not called on DEC_RolePion_Decision" );
    return pion->GeteEtatAmbiance();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatAmbiance
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatAmbiance( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatAmbiance not called on DEC_RolePion_Decision" );
    pion->SeteEtatAmbiance( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatRadio
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatRadio( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatRadio not called on DEC_RolePion_Decision" );
    return pion->GeteEtatRadio();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatRadio
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatRadio( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatRadio not called on DEC_RolePion_Decision" );
    pion->SeteEtatRadio( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatRadar
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatRadar( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatRadar not called on DEC_RolePion_Decision" );
    return pion->GeteEtatRadar();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatRadar
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatRadar( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatRadar not called on DEC_RolePion_Decision" );
    pion->SeteEtatRadar( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatDeplacement
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatDeplacement( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatDeplacement not called on DEC_RolePion_Decision" );
    return pion->GeteEtatDeplacement();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatDeplacement
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatDeplacement( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatDeplacement not called on DEC_RolePion_Decision" );
    pion->SeteEtatDeplacement( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatOrdreCoordination
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatOrdreCoordination( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatOrdreCoordination not called on DEC_RolePion_Decision" );
    return pion->GeteEtatOrdreCoordination();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatOrdreCoordination
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatOrdreCoordination( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatOrdreCoordination not called on DEC_RolePion_Decision" );
    pion->SeteEtatOrdreCoordination( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteConsigneTir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteConsigneTir( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteConsigneTir not called on DEC_RolePion_Decision" );
    return pion->GeteConsigneTir();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteConsigneTir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteConsigneTir( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteConsigneTir not called on DEC_RolePion_Decision" );
    pion->SeteConsigneTir( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteConsigneTirPopulation
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteConsigneTirPopulation( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteConsigneTirPopulation not called on DEC_RolePion_Decision" );
    return pion->GeteConsigneTirPopulation();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteConsigneTirPopulation
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteConsigneTirPopulation( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteConsigneTirPopulation not called on DEC_RolePion_Decision" );
    pion->SeteConsigneTirPopulation( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GeteEtatEchelon
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_GeteEtatEchelon( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_GeteEtatEchelon" );
    return pAgent->GeteEtatEchelon();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SeteEtatEchelon
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SeteEtatEchelon( DEC_Decision_ABC* pAgent, int value )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_SeteEtatEchelon" );
    pAgent->SeteEtatEchelon( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatSoutien
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatSoutien( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatSoutien not called on DEC_RolePion_Decision" );
    return pion->GeteEtatSoutien();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatSoutien
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatSoutien( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatSoutien not called on DEC_RolePion_Decision" );
    pion->SeteEtatSoutien( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteEtatSituationEnnemi
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteEtatSituationEnnemi( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteEtatSituationEnnemi not called on DEC_RolePion_Decision" );
    return pion->GeteEtatSituationEnnemi();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteEtatSituationEnnemi
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteEtatSituationEnnemi( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteEtatSituationEnnemi not called on DEC_RolePion_Decision" );
    pion->SeteEtatSituationEnnemi( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GeteEtatPhaseMission
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_GeteEtatPhaseMission( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_GeteEtatPhaseMission" );
    return pAgent->GeteEtatPhaseMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SeteEtatPhaseMission
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SeteEtatPhaseMission( DEC_Decision_ABC* pAgent, int value )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_SeteEtatPhaseMission" );
    pAgent->SeteEtatPhaseMission( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetbOrdreInterrompreMission
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Pion_GetbOrdreInterrompreMission( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GetbOrdreInterrompreMission not called on DEC_RolePion_Decision" );
    return pion->GetbOrdreInterrompreMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetbOrdreInterrompreMission
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetbOrdreInterrompreMission( DEC_Decision_ABC* pPion, bool value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SetbOrdreInterrompreMission not called on DEC_RolePion_Decision" );
    pion->SetbOrdreInterrompreMission( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GetbOrdreDecrocher
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_GetbOrdreDecrocher( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_GetbOrdreDecrocher" );
    return pAgent->GetbOrdreDecrocher();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SetbOrdreDecrocher
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SetbOrdreDecrocher( DEC_Decision_ABC* pAgent, bool value )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_SetbOrdreDecrocher" );
    pAgent->SetbOrdreDecrocher( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GetbOrdreTenirSurLR
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_GetbOrdreTenirSurLR( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_GetbOrdreTenirSurLR" );
    return pAgent->GetbOrdreTenirSurLR();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SetbOrdreTenirSurLR
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SetbOrdreTenirSurLR( DEC_Decision_ABC* pAgent, bool value )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_SetbOrdreTenirSurLR" );
    pAgent->SetbOrdreTenirSurLR( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_GetbOrdreTenir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_GetbOrdreTenir( DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_GetbOrdreTenir" );
    return pAgent->GetbOrdreTenir();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_SetbOrdreTenir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_SetbOrdreTenir( DEC_Decision_ABC* pAgent, bool value )
{
    if( !pAgent )
        throw std::runtime_error( "Invalid parameter in F_SetbOrdreTenir" );
    pAgent->SetbOrdreTenir( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetbPasserSurLC
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Pion_GetbPasserSurLC( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GetbPasserSurLC not called on DEC_RolePion_Decision" );
    return pion->GetbPasserSurLC();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetbPasserSurLC
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetbPasserSurLC( DEC_Decision_ABC* pPion, bool value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SetbPasserSurLC not called on DEC_RolePion_Decision" );
    pion->SetbPasserSurLC( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetNewEscorted
// Created: LDC 2010-30-12
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Pion_GetNewEscorted( DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw std::runtime_error( "Invalid parameter in F_Pion_GetNewEscorted" );
    return pPion->GetVariable< bool >( "g_bNewEscorted" );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetNewEscorted
// Created: LDC 2010-30-12
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetNewEscorted( DEC_Decision_ABC* pPion, bool value )
{
    if( !pPion )
        throw std::runtime_error( "Invalid parameter in F_Pion_SetNewEscorted" );
    pPion->SetVariable( "g_bNewEscorted" ,value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetNeedReinforcement
// Created: LMT 2011-06-28
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Pion_GetNeedReinforcement( DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw std::runtime_error( "Invalid parameter in F_Pion_GetNeedReinforcement" );
    return pPion->GetVariable< bool >( "g_needReinforcement" );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetNeedReinforcement
// Created: LMT 2011-06-28
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetNeedReinforcement( DEC_Decision_ABC* pPion, bool value )
{
    if( !pPion )
        throw std::runtime_error( "Invalid parameter in F_Pion_SetNeedReinforcement" );
    pPion->SetVariable( "g_needReinforcement" ,value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetEnemyAttackedBy
// Created: DDA 2011-10-12
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_CommunicationFunctions::F_Pion_GetEnemyAttackedBy( DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw std::runtime_error( "Invalid parameter in F_Pion_GetEnemyAttackedBy" );
    return pPion->GetVariable< boost::shared_ptr< DEC_Knowledge_Agent > >( "g_myEnemy");
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetpionEnEscorte
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_CommunicationFunctions::F_Pion_GetpionEnEscorte( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GetpionEnEscorte not called on DEC_RolePion_Decision" );
    return pion->GetpionEnEscorte();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetpionEnEscorte
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetpionEnEscorte( DEC_Decision_ABC* pPion, DEC_Decision_ABC* value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SetpionEnEscorte not called on DEC_RolePion_Decision" );
    pion->SetpionEnEscorte( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetitMvt
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Path_ABC > DEC_CommunicationFunctions::F_Pion_GetitMvt( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GetitMvt not called on DEC_RolePion_Decision" );
    return pion->GetitMvt();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetitMvt
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetitMvt( DEC_Decision_ABC* pPion, const boost::shared_ptr< DEC_Path_ABC >& value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "shared_ptr not called on DEC_RolePion_Decision" );
    pion->SetitMvt( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetobjectifEsquive
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_CommunicationFunctions::F_Pion_GetobjectifEsquive( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GetobjectifEsquive not called on DEC_RolePion_Decision" );
    return pion->GetobjectifEsquive();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetobjectifEsquive
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetobjectifEsquive( DEC_Decision_ABC* pPion, MT_Vector2D* value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SetobjectifEsquive not called on DEC_RolePion_Decision" );
    pion->SetobjectifEsquive( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteniEnCours
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_CommunicationFunctions::F_Pion_GeteniEnCours( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteniEnCours not called on DEC_RolePion_Decision" );
    return pion->GeteniEnCours();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteniEnCours
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteniEnCours( DEC_Decision_ABC* pPion, boost::shared_ptr< DEC_Knowledge_Agent > value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "shared_ptr not called on DEC_RolePion_Decision" );
    pion->SeteniEnCours( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::string
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
std::string DEC_CommunicationFunctions::F_Pion_GetmissionPrecedente( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GetmissionPrecedente not called on DEC_RolePion_Decision" );
    return pion->GetmissionPrecedente();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetmissionPrecedente
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetmissionPrecedente( DEC_Decision_ABC* pPion, const std::string& value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "string not called on DEC_RolePion_Decision" );
    pion->SetmissionPrecedente( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetrTenir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
float DEC_CommunicationFunctions::F_Pion_GetrTenir( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GetrTenir not called on DEC_RolePion_Decision" );
    return pion->GetrTenir();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetrTenir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetrTenir( DEC_Decision_ABC* pPion, float value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SetrTenir not called on DEC_RolePion_Decision" );
    pion->SetrTenir( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteTypeContact
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteTypeContact( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteTypeContact not called on DEC_RolePion_Decision" );
    return pion->GeteTypeContact();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteTypeContact
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteTypeContact( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteTypeContact not called on DEC_RolePion_Decision" );
    pion->SeteTypeContact( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GeteNiveauAction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
int DEC_CommunicationFunctions::F_Pion_GeteNiveauAction( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GeteNiveauAction not called on DEC_RolePion_Decision" );
    return pion->GeteNiveauAction();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SeteNiveauAction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SeteNiveauAction( DEC_Decision_ABC* pPion, int value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SeteNiveauAction not called on DEC_RolePion_Decision" );
    pion->SeteNiveauAction( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_GetbDefenseStatique_Mobile
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Pion_GetbDefenseStatique_Mobile( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GetbDefenseStatique_Mobile not called on DEC_RolePion_Decision" );
    return pion->GetbDefenseStatique_Mobile();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Pion_SetbDefenseStatique_Mobile
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Pion_SetbDefenseStatique_Mobile( DEC_Decision_ABC* pPion, bool value )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_SetbDefenseStatique_Mobile not called on DEC_RolePion_Decision" );
    pion->SetbDefenseStatique_Mobile( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::vector
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
const std::vector< DEC_Decision_ABC* >* DEC_CommunicationFunctions::F_Pion_GetselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_Pion_GetselUnitesEnAppui not called on DEC_RolePion_Decision" );
    return pion->GetselUnitesEnAppui();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_AddToselUnitesEnAppui
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_AddToselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_AddToselUnitesEnAppui not called on DEC_RolePion_Decision" );
    pion->AddToselUnitesEnAppui( pPion );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_RemoveFromselUnitesEnAppui
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_RemoveFromselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    DEC_RolePion_Decision* pion = dynamic_cast< DEC_RolePion_Decision* >( pPion );
    if( !pion )
        throw std::runtime_error( "F_RemoveFromselUnitesEnAppui not called on DEC_RolePion_Decision" );
    pion->RemoveFromselUnitesEnAppui( pPion );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Mot_EtatDesactiveSauvegarde
// Created: LDC 2010-01-20
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Mot_EtatDesactiveSauvegarde( DEC_Decision_ABC* pPion, bool value )
{
    if( !pPion )
        throw std::runtime_error( "Invalid parameter in F_Mot_EtatDesactiveSauvegarde" );
    pPion->SetVariable( "g_bDesactiveSauvegarde" ,value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_GetrDestruction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
float DEC_CommunicationFunctions::F_Automat_GetrDestruction( DEC_Decision_ABC* pAutomat )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( "Invalid parameter in F_Automat_GetrDestruction" );
    return automat->GetrDestruction();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_SetrDestruction
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_SetrDestruction( DEC_Decision_ABC* pAutomat, float value )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( "Invalid parameter in F_Automat_SetrDestruction" );
    automat->SetrDestruction( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_Getfuseau
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
const MIL_Fuseau* DEC_CommunicationFunctions::F_Automat_Getfuseau( DEC_Decision_ABC* pAutomat )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( "Invalid parameter in F_Automat_Getfuseau" );
    return automat->Getfuseau();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_Setfuseau
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_Setfuseau( DEC_Decision_ABC* pAutomat, MIL_Fuseau* value )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( "Invalid parameter in F_Automat_Setfuseau" );
    automat->Setfuseau( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::
//F_Automat_Getzone Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
TER_Localisation* DEC_CommunicationFunctions::F_Automat_Getzone( DEC_Decision_ABC* pAutomat )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return automat->Getzone();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_Setzone
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_Setzone( DEC_Decision_ABC* pAutomat, TER_Localisation* value )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    automat->Setzone( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_GetbOrdreAttendre
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Automat_GetbOrdreAttendre( DEC_Decision_ABC* pAutomat )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return automat->GetbOrdreAttendre();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_SetbOrdreAttendre
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_SetbOrdreAttendre( DEC_Decision_ABC* pAutomat, bool value )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    automat->SetbOrdreAttendre( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_GetbOrdrePoursuivre
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Automat_GetbOrdrePoursuivre( DEC_Decision_ABC* pAutomat )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return automat->GetbOrdrePoursuivre();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_SetbOrdrePoursuivre
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_SetbOrdrePoursuivre( DEC_Decision_ABC* pAutomat, bool value )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    automat->SetbOrdrePoursuivre( value );
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_GetbOrdreRalentir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
bool DEC_CommunicationFunctions::F_Automat_GetbOrdreRalentir( DEC_Decision_ABC* pAutomat )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return automat->GetbOrdreRalentir();
}

// -----------------------------------------------------------------------------
// Name: DEC_CommunicationFunctions::F_Automat_SetbOrdreRalentir
// Created: LDC 2009-05-11
// -----------------------------------------------------------------------------
void DEC_CommunicationFunctions::F_Automat_SetbOrdreRalentir( DEC_Decision_ABC* pAutomat, bool value )
{
    DEC_AutomateDecision* automat = dynamic_cast< DEC_AutomateDecision* >( pAutomat );
    if( !automat )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    automat->SetbOrdreRalentir( value );
}
