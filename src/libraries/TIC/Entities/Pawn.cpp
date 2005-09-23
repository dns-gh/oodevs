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

#include "TIC_pch.h"
#include "Entities/Pawn.h"
#include "Entities/Automat.h"
#include "Entities/Platform.h"
#include "Entities/Types/AutomatType.h"
#include "Entities/Types/PawnType.h"
#include "Entities/Types/PlatformType.h"
#include "Entities/Formations/FormationManager.h"
#include "Entities/Formations/Formation_ABC.h"
#include "Tools/Path.h"
#include "Tools/Position.h"
#include "Tools/PositionManager.h"

using namespace TIC;

Pawn::T_PawnMap      Pawn::pawns_;
Pawn::T_EntityIdSet  Pawn::disaggregPawns_;
TerrainData          Pawn::roadTerrain_;

//-----------------------------------------------------------------------------
// Name: Pawn::Pawn
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Pawn::Pawn( const ASN1T_MsgPionCreation& asnMsg )
    : ConcreteEntity    ()
    , nId_              ( asnMsg.oid_pion )
    , strName_          ( asnMsg.nom )
    , pType_            ( PawnType::Find( asnMsg.type_pion ) )
    , eState_           ( EState::eStateOperational )
    , pAutomat_         ( Automat::Find( asnMsg.oid_automate ) )
    , bIsAggregated_    ( true  )
    , bConfigDisaggreg_ ( disaggregPawns_.find( nId_ ) != disaggregPawns_.end() )
    , bIsPc_            ( false )
    , bIsOnRoad_        ( false )
    , bIsLoaded_        ( false )
    , bIsTransported_   ( false )
    , pReinforcedPawn_  ( 0 )
{
    assert( pType_ );
    assert( pAutomat_ );
    pAutomat_->AttachPawn( *this );

    // create pawn associated platforms
    for( PawnType::CIT_PawnPlatformMultiSet it = pType_->platformTypes_.begin(); it != pType_->platformTypes_.end(); ++it )
        childPlatforms_.push_back( new Platform( **it, *this ) );

    if( MustBeDisaggregated() )
        Disaggregate();
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
    , eState_           ( EState::eStateOperational )
    , pAutomat_         ( &automat )
    , bIsAggregated_    ( true  )
    , bConfigDisaggreg_ ( disaggregPawns_.find( nId_ ) != disaggregPawns_.end() )
    , bIsPc_            ( true )
    , bIsOnRoad_        ( false )
    , bIsLoaded_        ( false )
    , bIsTransported_   ( false )
    , pReinforcedPawn_  ( 0 )
{
    // retrieve PC type for the parent automat
    const AutomatType *pAutomatType = AutomatType::Find( asnMsg.type_automate );
    assert( pAutomatType );
    pType_ = &pAutomatType->GetPcType();
    assert( pType_ );

    assert( pAutomat_ );
    pAutomat_->AttachPawn( *this );

    // create pawn associated platforms
    for( PawnType::CIT_PawnPlatformMultiSet it = pType_->platformTypes_.begin(); it != pType_->platformTypes_.end(); ++it )
        childPlatforms_.push_back( new Platform( **it, *this ) );

    if( MustBeDisaggregated() )
        Disaggregate();
}

//-----------------------------------------------------------------------------
// Name: Pawn::~Pawn
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Pawn::~Pawn()
{
    lends_.clear();
    reinforcingPawns_.clear();
    transportedPawns_.clear();
    for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); ++it )
        delete *it;
    childPlatforms_.clear();
}

//-----------------------------------------------------------------------------
// Name: Pawn::Initialize
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void Pawn::Initialize()
{
    // initialize road terrain type
    roadTerrain_.Merge( TerrainData::Motorway  () );
    roadTerrain_.Merge( TerrainData::LargeRoad () );
    roadTerrain_.Merge( TerrainData::MediumRoad() );
    roadTerrain_.Merge( TerrainData::SmallRoad () );
    roadTerrain_.Merge( TerrainData::Bridge    () );
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
    disaggregPawns_.clear();
}

//-----------------------------------------------------------------------------
// Name: Pawn::Aggregate
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void Pawn::Aggregate()
{
    if( !bIsAggregated_ )
    {
        SendPlatformPositionToMos( true );
        bIsAggregated_ = true;
    }
}

//-----------------------------------------------------------------------------
// Name: Pawn::Disaggregate
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
void Pawn::Disaggregate()
{
    if(    ! bIsTransported_                                             // do not disaggreg when transported
        && !( pReinforcedPawn_  && pReinforcedPawn_->IsAggregated() ) )  // do not disaggreg when reinforcing an aggregated pawn
        bIsAggregated_ = false;
    else
        Aggregate();
}

//-----------------------------------------------------------------------------
// Name: Pawn::UpdateDisaggregationStatus
// Created: SBO 2005-06-18
//-----------------------------------------------------------------------------
void Pawn::UpdateDisaggregationStatus()
{
    if( MustBeDisaggregated() || PositionManager::IsInAnArea( position_ ) )
            Disaggregate();
        else
            Aggregate();
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

    if( asnMsg.m.pion_renforcePresent )
        pReinforcedPawn_ = Pawn::Find( asnMsg.pion_renforce );

    if( asnMsg.m.pion_transporteurPresent )
        bIsTransported_ = Pawn::Find( asnMsg.pion_transporteur ) ? true : false;

    if( asnMsg.m.pions_renforcantPresent )
    {
        for( CIT_PawnVector it = reinforcingPawns_.begin(); it != reinforcingPawns_.end(); ++it )
        {
            ( *it )->bIsOnRoad_ = bIsOnRoad_;
            ( *it )->path_      = path_;
        }
        reinforcingPawns_.clear();
        for( uint i = 0; i < asnMsg.pions_renforcant.n; ++i )
        {
            Pawn* pPawn = Pawn::Find( asnMsg.pions_renforcant.elem[i] );
            assert( pPawn );
            reinforcingPawns_.push_back( pPawn );
        }
    }

    if( asnMsg.m.pions_transportesPresent )
    {
        for( CIT_PawnVector it = transportedPawns_.begin(); it != transportedPawns_.end(); ++it )
        {
            ( *it )->bIsOnRoad_ = bIsOnRoad_;
            ( *it )->path_      = path_;
        }
        transportedPawns_.clear();
        for( uint i = 0; i < asnMsg.pions_transportes.n; ++i )
        {
            Pawn* pPawn = Pawn::Find( asnMsg.pions_transportes.elem[i] );
            assert( pPawn );
            transportedPawns_.push_back( pPawn );
        }
    }

    if( asnMsg.m.vitessePresent )
    {
        rSpeed_ = asnMsg.vitesse;
        for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); ++it )
            if( ( *it )->CanMove() )
                ( *it )->rSpeed_ = rSpeed_;
            else
                ( *it )->rSpeed_ = 0.;
    }

    if( asnMsg.m.directionPresent )
    {
        nDirection_ = asnMsg.direction;
        for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); ++it )
            if( ( *it )->CanMove() )
                ( *it )->nDirection_ = nDirection_;
    }

    if( asnMsg.m.positionPresent )
        position_ = std::string( (const char*)asnMsg.position.data, 15 );

    if( asnMsg.m.hauteurPresent )
    {
        nHeight_ = asnMsg.hauteur;
        for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); it++ )
            if( ( *it )->CanMove() )
                ( *it )->nHeight_ = nHeight_;
            else
                ( *it )->nHeight_ = 0;
    }

    if( asnMsg.m.etat_operationnelPresent )
    {
        if( asnMsg.etat_operationnel >= 100 )
            eState_ = eStateOperational;
        else
            eState_ = eStateFixable;
    }
    if( asnMsg.m.mortPresent && asnMsg.mort )
        eState_ = eStateDestroyed;
/*
    if( asnMsg.m.etat_opPresent )
    {
        if( asnMsg.etat_op >= 100 )
            eState_ = eStateOperational;
        else
            eState_ = eStateFixable;
    }
    if( asnMsg.etatPresent && asnMsg.etat == EnumUnitState::mort )
        eState_ = eStateDestroyed;
*/
}

//-----------------------------------------------------------------------------
// Name: Pawn::OnAttributeUpdated
// Created: SBO 2005-05-16
//-----------------------------------------------------------------------------
void Pawn::OnAttributeUpdated( const ASN1T_MsgUnitDotations& asnMsg )
{
    if( asnMsg.m.prets_equipementPresent )
    {
        T_LendMap previousLends = lends_;

        // return lends to pawns
        for( CIT_LendMap it = lends_.begin(); it != lends_.end(); ++it )
        {
            // insert back the platform into pawn's childs
            childPlatforms_.push_back( it->first );
            it->first->pPawn_ = this;
            // find and erase platform from receiver's childs
            Pawn* pRecv = Pawn::Find( it->second->GetId() );
            assert( pRecv );
            for( IT_PlatformVector itP = pRecv->childPlatforms_.begin(); itP != pRecv->childPlatforms_.end(); ++itP )
                if( *itP == it->first )
                {
                    pRecv->childPlatforms_.erase( itP );
                    break;
                }
        }
        // clear lends list
        lends_.clear();
        // manage new lends
        Pawn* pPawn = Pawn::Find( asnMsg.oid_pion );
        assert( pPawn );
        for( uint i = 0; i < asnMsg.prets_equipement.n; ++i )
        {
            const ASN1T_PretEquipement& lend = asnMsg.prets_equipement.elem[i];
            Pawn* pLender = Pawn::Find( lend.oid_pion_emprunteur );
            assert( pLender );
            const PlatformType* pType = PlatformType::Find( lend.type_equipement );
            assert( pType );
            if( !pPawn->LendPlatform( *pLender, *pType, lend.nombre, previousLends ) )
                MT_LOG_ERROR_MSG( "Cannot lend all requested platform(s) to pawn" );
        }
        previousLends.clear();
    }

    if( asnMsg.m.dotation_eff_materielPresent )
    {
        for( uint i = 0; i < asnMsg.dotation_eff_materiel.n; ++i )
        {
            const ASN1T_DotationEquipement& equip = asnMsg.dotation_eff_materiel.elem[ i ];
            const PlatformType* pType = PlatformType::Find( equip.type_equipement );
            assert( pType );

            uint nState     = ( uint )Platform::EPlatformState::ePlatformStatePrisoner;
            uint nNbOfState = equip.nb_prisonniers;
            for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); ++it )
            {
                while( nNbOfState == 0 )
                {
                    ++nState;
                    if( nState == ( uint )Platform::EPlatformState::ePlatformStateDestroyed )
                        nNbOfState = equip.nb_indisponibles;
                    else if( nState == ( uint )Platform::EPlatformState::ePlatformStateFixable )
                        nNbOfState = equip.nb_reparables;
                    else if( nState == ( uint )Platform::EPlatformState::ePlatformStateFixing )
                        nNbOfState = equip.nb_dans_chaine_maintenance;
                    else if( nState == ( uint )Platform::EPlatformState::ePlatformStateOk )
                        nNbOfState = equip.nb_disponibles;
                    else
                        break;
                }
                if( nNbOfState == 0 )
                    break;
                if( &( *it )->pType_->GetType() == pType )
                {
                    ( *it )->eState_ = ( Platform::EPlatformState )nState;
                    --nNbOfState;
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: Pawn::LendPlatform
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
bool Pawn::LendPlatform( Pawn& receiver, const PlatformType& platformType, uint nNbr, T_LendMap& previousLends )
{
    uint nPlatformLent = 0;
    // recycle from previously lent platform
    for( IT_LendMap it = previousLends.begin(); it != previousLends.end(); )
        if( it->second == &receiver && &it->first->pType_->GetType() == &platformType )
        {
            lends_.insert( std::make_pair( it->first, &receiver ) );
            receiver.childPlatforms_.push_back( it->first );
            it->first->pPawn_ = &receiver;
            IT_PlatformVector itP = std::find( childPlatforms_.begin(), childPlatforms_.end(), it->first );
            assert( itP != childPlatforms_.end() );
            childPlatforms_.erase( itP );
            it = previousLends.erase( it );
            if( ++nPlatformLent == nNbr )
                return true;
        }
        else
            ++it;

    // if more lends are needed, get them from pawn
    for( IT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); )
        if( &( *it )->pType_->GetType() == &platformType )
        {
            // keep track of original pawn for the platform
            lends_.insert( std::make_pair( *it, &receiver ) );
            // insert platform into receiving pawn
            receiver.childPlatforms_.push_back( *it );
            ( *it )->pPawn_ = &receiver;
            // remove platform from lending pawn
            it = childPlatforms_.erase( it );
            if( ++nPlatformLent == nNbr )
                return true;
        }
        else
            ++it;
    return false;
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
void Pawn::OnReceiveTerrainType( DIN::DIN_Input& input )
{
    char  cArea;
    char  cLeft;
    char  cRight;
    short nLinear;
    input >> cArea;
    input >> cLeft;
    input >> cRight;
    input >> nLinear;

    TerrainData terData( cArea, cLeft, cRight, nLinear );
    if( terData == terrain_ )
        return;

    terrain_.SetArea  ( cArea   );
    terrain_.SetLeft  ( cLeft   );
    terrain_.SetRight ( cRight  );
    terrain_.SetLinear( nLinear );
    bIsOnRoad_ = terrain_.ContainsOne( roadTerrain_ );

    // update transported pawns to same terrain type and path
    if( transportedPawns_.size() > 0 )
        for( CIT_PawnVector it = transportedPawns_.begin(); it != transportedPawns_.end(); ++it )
        {
            ( *it )->bIsOnRoad_ = bIsOnRoad_;
            ( *it )->path_      = path_;
        }

    // update reinforcing pawns to same terrain type and path
    if( reinforcingPawns_.size() > 0 )
        for( CIT_PawnVector it = reinforcingPawns_.begin(); it != reinforcingPawns_.end(); ++it )
        {
            ( *it )->bIsOnRoad_ = bIsOnRoad_;
            ( *it )->path_      = path_;
        }
}

//-----------------------------------------------------------------------------
// Name: Pawn::UpdatePlatformPositions
// Created: SBO 2005-07-05
//-----------------------------------------------------------------------------
void Pawn::UpdatePlatformPositions()
{
    // update disaggregation status (need to be done before because of reinforcement)
    for( CIT_PawnMap it = pawns_.begin(); it != pawns_.end(); ++it )
        it->second->UpdateDisaggregationStatus();

    for( CIT_PawnMap it = pawns_.begin(); it != pawns_.end(); ++it )
    {
        // do it again for reinforcing units whose status depends on reinforcing pawn's
        it->second->UpdateDisaggregationStatus();

        it->second->UpdatePlatformPosition();
        // debug to display platforms on MOS
        if( !it->second->bIsAggregated_ )
            it->second->SendPlatformPositionToMos( false );
    }
}

//-----------------------------------------------------------------------------
// Name: Pawn::UpdatePlatformPosition
// Created: SBO 2005-05-31
//-----------------------------------------------------------------------------
void Pawn::UpdatePlatformPosition() const
{
    // onroad formation
    if( bIsOnRoad_ )
        SetFormation( FormationManager::GetFormation( FormationManager::eFormationRoad ) );
    // offroad formation
    else
    {
        // don't use diamond formation when only 2 platforms
        if( GetPlatformCount() == 2 )
            SetFormation( FormationManager::GetFormation( FormationManager::eFormationLine ) );
        else
            SetFormation( FormationManager::GetFormation( FormationManager::eFormationDiamond ) );
    }
}

//-----------------------------------------------------------------------------
// Name: Pawn::SetFormation
// Created: SBO 2005-06-16
//-----------------------------------------------------------------------------
void Pawn::SetFormation( Formation_ABC& formation ) const
{
    // use a special platform count only for "moveable" platforms
    uint nPlatformCount = 0;
    if( bIsAggregated_ )
        nPlatformCount = 0;
    for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); ++it )
        if( ( *it )->MustBeDisaggregated() && ( *it )->CanMove() )
            nPlatformCount++;

    formation.Begin( *this, nPlatformCount );

    if( !formation.IsReverse() )
    {
        for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); ++it )
            if( ( *it )->MustBeDisaggregated() && ( *it )->CanMove() )
                formation.ApplyTo( **it );
            else if( ( *it )->CanMove() )
                ( *it )->Follow( *this );
    }
    else
    {
        for( CRIT_PlatformVector it = childPlatforms_.rbegin(); it != childPlatforms_.rend(); ++it )
            if( ( *it )->MustBeDisaggregated() && ( *it )->CanMove() )
                formation.ApplyTo( **it );
            else if( ( *it )->CanMove() )
                ( *it )->Follow( *this );
    }

    formation.End();
}

//-----------------------------------------------------------------------------
// Name: Pawn::SendPlatformPositionToMos
// Created: SBO 2005-06-16
// Note: Debug, send platform position in order to be displayed on MOS_Light2
//-----------------------------------------------------------------------------
void Pawn::SendPlatformPositionToMos( bool bClear ) const
{
    MessageManager& msgMgr = TicManager::GetTicManager().GetNetworkManager().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << (uint32)nId_;
    if( bClear )
        dinMsg << (uint32)0;
    else
    {
        dinMsg << (uint32)GetPlatformCount();
        for( CIT_PlatformVector it = childPlatforms_.begin(); it != childPlatforms_.end(); ++it )
            if( ( *it )->MustBeDisaggregated() )
            {
                dinMsg << ( double )( *it )->position_.X();
                dinMsg << ( double )( *it )->position_.Y();
            }
    }
    msgMgr.SendMsgDebugDrawPoints( dinMsg );
}