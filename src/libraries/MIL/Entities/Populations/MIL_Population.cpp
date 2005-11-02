// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "MIL_Population.h"

#include "MIL_PopulationType.h"
#include "MIL_PopulationConcentration.h"
#include "MIL_PopulationFlow.h"
#include "MIL_PopulationAttitude.h"
#include "DEC_PopulationDecision.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "Network/NET_ASN_Messages.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_GUID( MIL_Population, "MIL_Population" )

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population( const MIL_PopulationType& type, uint nID, MIL_InputArchive& archive )
    : PHY_Actor               ()
    , pType_                  ( &type )
    , nID_                    ( nID )
    , pArmy_                  ( 0 )
    , strName_                ( type.GetName() )
    , pDecision_              ( 0 )
    , orderManager_           ( *this )
    , pDefaultAttitude_       ( 0 )
    , bPionMaxSpeedOverloaded_( false )
    , rOverloadedPionMaxSpeed_( 0. )
{
    archive.ReadField( "Nom", strName_, MIL_InputArchive::eNothing );

    std::stringstream strTmp;
    strTmp << strName_ << " - [" << nID_ << "]";
    strName_ = strTmp.str();

    std::string strArmy;
    archive.ReadField( "Camp", strArmy );
    pArmy_ = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( strArmy );
    if( !pArmy_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown army", archive.GetContext() );

    std::string strAttitude;
    archive.ReadField( "Attitude", strAttitude );
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( strAttitude );
    if( !pDefaultAttitude_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown attitude", archive.GetContext() );

    pDecision_ = new DEC_PopulationDecision( *this );
    
    concentrations_.push_back( new MIL_PopulationConcentration( *this, archive ) );    
}

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population()
    : PHY_Actor               ()
    , pType_                  ( 0 )
    , nID_                    ( 0 )
    , pArmy_                  ( 0 )
    , strName_                ()
    , pDecision_              ( 0 )
    , orderManager_           ( *this )
    , pDefaultAttitude_       ( 0 )
    , bPionMaxSpeedOverloaded_( false )
    , rOverloadedPionMaxSpeed_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Population destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population::~MIL_Population()
{
    delete pDecision_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::load
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_Population::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< PHY_Actor >( *this );

    uint nTypeID;
    file >> nTypeID;
    pType_ = MIL_PopulationType::Find( nTypeID );
    assert( pType_ );

    file >> nID_
         >> const_cast< MIL_Army*& >( pArmy_ )
         >> strName_;

    if ( !MIL_EntityManager::populationIDManager_.IsMosIDValid( nID_ ) )
        MIL_EntityManager::populationIDManager_.LockSimID( nID_ );

    uint nAttitudeID;
    file >> nAttitudeID;
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( nAttitudeID );
    assert( pDefaultAttitude_ );

    file >> concentrations_
         >> flows_
         >> trashedConcentrations_
         >> trashedFlows_
         >> bPionMaxSpeedOverloaded_
         >> rOverloadedPionMaxSpeed_;

    pDecision_ = new DEC_PopulationDecision( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_Population::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< PHY_Actor >( *this );

    file << pType_->GetID()
         << nID_
         << pArmy_
         << strName_
         << pDefaultAttitude_->GetID()
         << concentrations_
         << flows_
         << trashedConcentrations_
         << trashedFlows_
         << bPionMaxSpeedOverloaded_
         << rOverloadedPionMaxSpeed_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateDecision
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
void MIL_Population::UpdateDecision()
{
    assert( pDecision_ );
    pDecision_->UpdateDecision();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_Population::UpdateState()
{
    for( CIT_ConcentrationVector it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
        delete *it;
    trashedConcentrations_.clear();

    for( CIT_FlowVector it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
        delete *it;
    trashedFlows_.clear();

    // Flows    
    for( IT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); )
    {
        MIL_PopulationFlow* pFlow = *itFlow;
        if( !pFlow->Update() )
        {
            itFlow = flows_.erase( itFlow );
            trashedFlows_.push_back( pFlow );
        }
        else 
            ++ itFlow;
    }

    // Concentrations
    for( IT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); )
    {
        MIL_PopulationConcentration* pConcentration = *itConcentration;
        if( !pConcentration->Update() )
        {
            itConcentration = concentrations_.erase( itConcentration );
            trashedConcentrations_.push_back( pConcentration );
        }
        else 
            ++ itConcentration;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_Population::Clean()
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        (**itConcentration).Clean();

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Move
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
void MIL_Population::Move( const MT_Vector2D& destination )
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        (**itConcentration).Move( destination );

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).Move( destination );
}

// =============================================================================
// FLOWS / CONCENTRATION MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::CreateFlow
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
MIL_PopulationFlow& MIL_Population::CreateFlow( MIL_PopulationConcentration& concentration )
{
    MIL_PopulationFlow* pFlow = new MIL_PopulationFlow( *this, concentration );
    flows_.push_back( pFlow );
    return *pFlow;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetConcentration
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
MIL_PopulationConcentration& MIL_Population::GetConcentration( const MT_Vector2D& position )
{
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
    {
        if( (**it).IsNearPosition( position ) )
            return **it;
    }

    MIL_PopulationConcentration* pConcentration = new MIL_PopulationConcentration( *this, position );
    concentrations_.push_back( pConcentration );
    return *pConcentration;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetPionMaxSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetMaxSpeed() const
{
    assert( pType_ );
    return pType_->GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDefaultFlowDensity
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetDefaultFlowDensity() const
{
    assert( pType_ );
    return pType_->GetDefaultFlowDensity();
}
// -----------------------------------------------------------------------------
// Name: MIL_Population::GetPionMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetPionMaxSpeed( const MIL_PopulationAttitude& attitude, MT_Float rDensity, const PHY_Volume& pionVolume ) const
{
    if( bPionMaxSpeedOverloaded_ )
        return rOverloadedPionMaxSpeed_;
    else
        return pType_->GetPionMaxSpeed( attitude, rDensity, pionVolume );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_Population::SendCreation() const
{
    NET_ASN_MsgPopulationCreation asnMsg;
    asnMsg.GetAsnMsg().oid_population  = nID_;
    asnMsg.GetAsnMsg().type_population = pType_->GetID();
    asnMsg.GetAsnMsg().oid_camp        = pArmy_->GetID();
    asnMsg.GetAsnMsg().nom             = strName_.c_str(); // !! pointeur sur const char*   
    asnMsg.Send();

    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        (**itConcentration).SendCreation();

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_Population::SendFullState() const
{
    NET_ASN_MsgPopulationUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_population = nID_;
    asnMsg.Send();

    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        (**itConcentration).SendFullState();

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateNetwork
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_Population::UpdateNetwork()
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        (**itConcentration).SendChangedState();

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).SendChangedState();
}

