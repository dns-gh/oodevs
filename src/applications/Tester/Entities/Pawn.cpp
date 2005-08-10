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
#include "Entities/Types/AutomatType.h"
#include "Entities/Types/PawnType.h"
#include "Tools/Path.h"
#include "Tools/Position.h"
#include "Tools/Location.h"
#include "TacticalLines/TacticalLineManager.h"
#include "Actions/Missions/Mission_Pawn_Type.h"

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
    , nLeftLimit_       ( 0 )
    , nRightLimit_      ( 0 )

{
    assert( pType_ );
    assert( pAutomat_ );
    pAutomat_->AttachPawn( *this );
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
    , nLeftLimit_       ( 0 )
    , nRightLimit_      ( 0 )
{
    // retrieve PC type for the parent automat
    const AutomatType *pAutomatType = AutomatType::Find( asnMsg.type_automate );
    assert( pAutomatType );
    pType_ = &pAutomatType->GetPcType();
    assert( pType_ );

    assert( pAutomat_ );
    pAutomat_->AttachPawn( *this );
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
    Mission_Pawn_Type::Initialize();
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
    Mission_Pawn_Type::Terminate();
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
// Name: Pawn::GetTestParam_Position
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Position& Pawn::GetTestParam_Point() const
{
    double rX = position_.X();
    double rY = position_.Y();

    rX += 2000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
    rY += 2000.0 * ( rand() * 1.0 / RAND_MAX - 0.5 );
    Position* pPos = new Position();
    pPos->SetSimCoordinates( rX, rY );
    return *pPos;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParam_LeftLimit
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
T_EntityId Pawn::GetTestParam_LeftLimit()
{
    // if left limit is not ser get an existing limit which is not the right limit
    if( nLeftLimit_ == 0 )
        nLeftLimit_ = TacticalLineManager::GetLimitIdExcluding( nRightLimit_ );
    // at least world border limits should exist
    assert( nLeftLimit_ );
    return nLeftLimit_;
 }

// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParam_RightLimit
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
T_EntityId Pawn::GetTestParam_RightLimit()
{
    // if right limit is not ser get an existing limit which is not the left limit
    if( nRightLimit_ == 0 )
        nRightLimit_ = TacticalLineManager::GetLimitIdExcluding( nLeftLimit_ );
    // at least world border limits should exist
    assert( nRightLimit_ );
    return nRightLimit_;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParam_Limas
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
T_IdVector& Pawn::GetTestParam_Limas() const
{
    return *new T_IdVector();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParam_Direction
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
uint Pawn::GetTestParam_Direction() const
{
    return GetTestParam_Numeric( 0, 359 );
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParam_Path
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
Path& Pawn::GetTestParam_Path() const
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
// Name: Pawn::GetTestParam_Bool
// Created: SBO 2005-08-05
// -----------------------------------------------------------------------------
bool Pawn::GetTestParam_Bool() const
{
    return GetTestParam_Numeric( 0, 1 ) ? true : false;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParam_AgentList
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
T_IdVector& Pawn::GetTestParam_AgentList() const
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
// Name: pawn::GetTestParam_Enumeration
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
uint Pawn::GetTestParam_Enumeration( uint nMin, uint nMax ) const
{
    return GetTestParam_Numeric( nMin, nMax );
}

// -----------------------------------------------------------------------------
// Name: pawn::GetTestParam_Numeric
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
int Pawn::GetTestParam_Numeric( int nMin /* = 0 */, int nMax /* = RAND_MAX */ ) const
{
    if( nMin > nMax )
        return 0;
    return ( uint )( rand() * ( double )( nMax - nMin ) / RAND_MAX ) + nMin;
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParam_AgentKnowledgeList
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_IdVector& Pawn::GetTestParam_AgentKnowledgeList() const
{
    return pAutomat_->GetKnowledgeGroup().GetTestParam_Targets();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParam_ObjectKnowledgeList
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_IdVector& Pawn::GetTestParam_ObjectKnowledgeList() const
{
    return pAutomat_->GetTeam().GetTestParam_Objects();
}

// -----------------------------------------------------------------------------
// Name: Pawn::GetTestParam_Location
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
Location& Pawn::GetTestParam_Location() const
{
    return Location::GetTestParam_Location( position_ );
}
