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
#include "Entities/Types/AutomatType.h"
#include "Entities/Types/PawnType.h"
#include "Tools/Path.h"
#include "Tools/Position.h"
#include "TacticalLines/TacticalLineManager.h"

using namespace TEST;

Pawn::T_PawnMap      Pawn::pawns_;

//-----------------------------------------------------------------------------
// Name: Pawn::Pawn
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Pawn::Pawn( const ASN1T_MsgPionCreation& asnMsg )
    : ConcreteEntity    ()
    , nId_              ( asnMsg.oid_pion )
    , strName_          ( asnMsg.nom )
    , pType_            ( PawnType::Find( asnMsg.type_pion ) )
    , pAutomat_         ( Automat::Find( asnMsg.oid_automate ) )
    , bIsPc_            ( false )
    , bIsLoaded_        ( false )
    , limits_           ()
{
    assert( pType_ );
    assert( pAutomat_ );
    pAutomat_->AttachPawn( *this );
    limits_[ 0 ] = 0;
    limits_[ 1 ] = 0;
}

//-----------------------------------------------------------------------------
// Name: Pawn::Pawn
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
Pawn::Pawn( const ASN1T_MsgAutomateCreation& asnMsg, Automat& automat )
    : ConcreteEntity    ()
    , nId_              ( asnMsg.oid_automate )
    , strName_          ( asnMsg.nom )
    , pType_            ( 0 )
    , pAutomat_         ( &automat )
    , bIsPc_            ( true )
    , bIsLoaded_        ( false )
    , limits_           ()
{
    // retrieve PC type for the parent automat
    const AutomatType *pAutomatType = AutomatType::Find( asnMsg.type_automate );
    assert( pAutomatType );
    pType_ = &pAutomatType->GetPcType();
    assert( pType_ );

    assert( pAutomat_ );
    pAutomat_->AttachPawn( *this );
    limits_[ 0 ] = 0;
    limits_[ 1 ] = 0;
}

//-----------------------------------------------------------------------------
// Name: Pawn::~Pawn
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Pawn::~Pawn()
{
}

//-----------------------------------------------------------------------------
// Name: Pawn::Initialize
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void Pawn::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: Pawn::Terminate
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void Pawn::Terminate()
{
    for( CIT_PawnMap it = pawns_.begin(); it != pawns_.end(); ++it )
        delete it->second;
    pawns_.clear();
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
// TEST PARAMETERS
//-----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParameter_Position
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Position& Pawn::GetTP_Position() const
{
    Position* pos = new Position( position_.GetLatitude(), position_.GetLongitude() );
    return *pos;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_LeftLimit
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
uint Pawn::GetTP_LeftLimit()
{
    // if left limit is not ser get an existing limit which is not the right limit
    if( limits_[ 0 ] == 0 )
        limits_[ 0 ] = TacticalLineManager::GetLimitIdExcluding( limits_[ 1 ] );
    // at least world border limits should exist
    assert( limits_[ 0 ] );
    return limits_[ 0 ];
 }

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_RightLimit
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
uint Pawn::GetTP_RightLimit()
{
    // if right limit is not ser get an existing limit which is not the left limit
    if( limits_[ 1 ] == 0 )
        limits_[ 1 ] = TacticalLineManager::GetLimitIdExcluding( limits_[ 0 ] );
    // at least world border limits should exist
    assert( limits_[ 1 ] );
    return limits_[ 1 ];
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_Limas
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
T_IdVector& Pawn::GetTP_Limas() const
{
    return *new T_IdVector();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_DangerDirection
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
uint Pawn::GetTP_DangerDirection() const
{
    return ( uint )( rand() * 360.0 / RAND_MAX );
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_Path
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
Path& Pawn::GetTP_Path() const
{
    Path* p = new Path();

    double rX = position_.X();
    double rY = position_.Y();

    for( uint i = 0; i < 5; ++i )
    {
        rX += 2000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
        rY += 2000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
        Position* pNode = new Position();
        pNode->SetSimCoordinates( rX, rY );
        p->AddNode( *pNode );
    }
    return *p;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_PathType
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
uint Pawn::GetTP_PathType() const
{
    // 6 different types
    return ( uint )( rand() * 5.0 / RAND_MAX );
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_UnLoaded
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
bool Pawn::GetTP_IsUnLoaded() const
{
    return !bIsLoaded_;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_VerouillageVision
/** @return 
*/
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
uint Pawn::GetTP_VerouillageVision() const
{
    // 3 different types
    return ( uint )( rand() * 2.0 / RAND_MAX );
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_VisionPoint
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
Position& Pawn::GetTP_VisionPoint() const
{
    Position* pos = new Position( position_.GetLatitude(), position_.GetLongitude() );
    return *pos;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_Direction
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
uint Pawn::GetTP_Direction() const
{
    return nDirection_;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_AgentList
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
T_IdVector& Pawn::GetTP_PawnList() const
{
    // return a vector of up to 5 pawn ids
    T_IdVector* pPawns = new T_IdVector();
    uint i = 0;
    for( CIT_PawnMap it = pawns_.begin(); it != pawns_.end() && i < 5; ++it )
        if( it->first != GetId() )
        {
            pPawns->push_back( it->first );
            ++i;
        }
    return *pPawns;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTP_ObjectType
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
uint Pawn::GetTP_ObjectType() const
{
    // 37 different object types
    return ( uint )( rand() * 36.0 / RAND_MAX );
}

// -----------------------------------------------------------------------------
// Name: pawn::GetTP_IsIndirectFire
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
bool Pawn::GetTP_IsIndirectFire() const
{
    return rand() < RAND_MAX / 2;
}

// -----------------------------------------------------------------------------
// Name: pawn::GetTP_MunitionType
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
uint Pawn::GetTP_MunitionType() const
{
    // 6 different munition types
    return ( uint )( rand() * 5.0 / RAND_MAX );
}

// -----------------------------------------------------------------------------
// Name: pawn::GetTP_NbObus
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
uint Pawn::GetTP_NbObus() const
{
    // 1 to 10
    return ( uint )( rand() * 9.0 / RAND_MAX ) + 1;
}

// -----------------------------------------------------------------------------
// Name: pawn::GetTP_Targets
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
T_IdVector& Pawn::GetTP_Targets() const
{
    return pAutomat_->GetKnowledgeGroup().GetTP_Targets();
}
