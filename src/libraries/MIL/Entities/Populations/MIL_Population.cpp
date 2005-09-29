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
#include "DEC_PopulationDecision.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "Network/NET_ASN_Messages.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population( const MIL_PopulationType& type, uint nID, MIL_InputArchive& archive )
    : type_        ( type )
    , nID_         ( nID )
    , pArmy_       ( 0 )
    , strName_     ( type.GetName() )
    , pDecision_   ( 0 )
    , orderManager_( *this )
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

    pDecision_ = new DEC_PopulationDecision( *this );
    
    concentrations_.insert( new MIL_PopulationConcentration( *this, archive ) );    
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
    asnMsg.GetAsnMsg().type_population = type_.GetID();
    asnMsg.GetAsnMsg().oid_camp        = pArmy_->GetID();
    asnMsg.GetAsnMsg().nom             = strName_.c_str(); // !! pointeur sur const char*   
    asnMsg.Send();

    for( CIT_ConcentrationSet itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        (**itConcentration).SendCreation();

    for( CIT_FlowSet itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
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

    for( CIT_ConcentrationSet itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        (**itConcentration).SendFullState();

    for( CIT_FlowSet itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).SendFullState();
}
