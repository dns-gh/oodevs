// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Pawn.cpp $
// $Author: Sbo $
// $Modtime: 21/07/05 17:06 $
// $Revision: 38 $
// $Workfile: Pawn.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"
#include "Entities/Pawn.h"
#include "Entities/Automat.h"
#include "Entities/KnowledgeGroup.h"
#include "Entities/Team.h"
#include "Types/TypeManager.h"
#include "Types/Entities/AutomatType.h"
#include "Types/Entities/PawnType.h"
#include "Types/Entities/PawnModel.h"
#include "Tools/Path.h"
#include "Tools/Position.h"
#include "Actions/Scheduler.h"
#include "Actions/Missions/Mission_Pawn_Type.h"
#include "Workspace.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: Pawn::Pawn
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Pawn::Pawn( const Workspace& workspace, const ASN1T_MsgPionCreation& asnMsg )
    : Testable_Entity ( workspace )
    , nId_            ( asnMsg.oid_pion )
    , strName_        ( asnMsg.nom )
    , pType_          ( workspace.GetTypeManager().FindPawnType( asnMsg.type_pion ) )
    , pAutomat_       ( workspace.GetEntityManager().FindAutomat( asnMsg.oid_automate ) )
    , bIsPc_          ( false )
    , position_       ()
    , nDirection_     ( 0 )
    , nHeight_        ( 0 )
    , rSpeed_         ( 0. )
    , nOpState_       ( 0 )
    , bIsLoaded_      ( false )
{
    assert( pType_ );
    assert( pAutomat_ );
    pAutomat_->AttachPawn( *this );
}

//-----------------------------------------------------------------------------
// Name: Pawn::Pawn
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
Pawn::Pawn( const Workspace& workspace, const ASN1T_MsgAutomateCreation& asnMsg, Automat& automat )
    : Testable_Entity ( workspace )
    , nId_            ( asnMsg.oid_automate )
    , strName_        ( asnMsg.nom )
    , pType_          ( 0 )
    , pAutomat_       ( &automat )
    , bIsPc_          ( true )
    , position_       ()
    , nDirection_     ( 0 )
    , nHeight_        ( 0 )
    , rSpeed_         ( 0. )
    , nOpState_       ( 0 )
    , bIsLoaded_      ( false )
{
    // retrieve PC type for the parent automat
    const AutomatType* pAutomatType = workspace.GetTypeManager().FindAutomatType( asnMsg.type_automate );
    assert( pAutomatType );
    pType_ = &pAutomatType->GetPcType();
    assert( pType_ );
    automat.AttachPawn( *this );
}

//-----------------------------------------------------------------------------
// Name: Pawn::~Pawn
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Pawn::~Pawn()
{
    path_.Clear();
}

//-----------------------------------------------------------------------------
// Name: Pawn::OnAutomatChanged
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void Pawn::OnAutomatChanged( Automat& automat )
{
    assert( pAutomat_ );
    pAutomat_->DetachPawn( *this );
    automat.AttachPawn( *this );
    pAutomat_ = &automat;
}

//-----------------------------------------------------------------------------
// Name: Pawn::OnAttributeUpdated
// Created: SBO 2005-05-16
//-----------------------------------------------------------------------------
void Pawn::OnAttributeUpdated( const ASN1T_MsgUnitAttributes& asnMsg )
{
    if( asnMsg.m.embarquePresent )
        bIsLoaded_ = ( asnMsg.embarque ? true : false );

    if( asnMsg.m.vitessePresent )
        rSpeed_ = asnMsg.vitesse;

    if( asnMsg.m.directionPresent )
        nDirection_ = asnMsg.direction;

    if( asnMsg.m.positionPresent )
        position_ = std::string( (const char*)asnMsg.position.data, 15 );

    if( asnMsg.m.hauteurPresent )
        nHeight_ = asnMsg.hauteur;
}

//-----------------------------------------------------------------------------
// Name: Pawn::OnAttributeUpdated
// Created: SBO 2005-05-16
//-----------------------------------------------------------------------------
void Pawn::OnAttributeUpdated( const ASN1T_MsgUnitDotations& /*asnMsg*/ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Pawn::OnReceivePathfind
// Created: SBO 2005-06-15
//-----------------------------------------------------------------------------
void Pawn::OnReceivePathfind( const ASN1T_MsgUnitPathFind& asnMsg )
{
    // clear any previous pathfind
    path_.Clear();

    // store new pathfind
    for( uint i = 0; i < asnMsg.itineraire.vecteur_point.n; ++i )
    {
        Position* pPos = new Position( std::string( (const char*)asnMsg.itineraire.vecteur_point.elem[i].data, 15 ) );
        path_.AddNode( *pPos );
    }
}

//-----------------------------------------------------------------------------
// Name: Pawn::OnReceiveTerrainType
// Created: SBO 2005-06-15
//-----------------------------------------------------------------------------
void Pawn::OnReceiveTerrainType( DIN::DIN_Input& /*input*/ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// SCHEDULING
//-----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: Pawn::ScheduleAllMissions
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void Pawn::ScheduleAllMissions( Scheduler& scheduler )
{
    assert( pType_ );
    pType_->GetModel().ScheduleAllMissions( *this, scheduler );
}

// -----------------------------------------------------------------------------
// Name: Pawn::ScheduleMission
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void Pawn::ScheduleMission( Scheduler& scheduler, const std::string& strMissionName )
{
    assert( pType_ );
    pType_->GetModel().ScheduleMission( *this, scheduler, strMissionName );
}

