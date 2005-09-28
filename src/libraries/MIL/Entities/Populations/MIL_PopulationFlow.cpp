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
#include "MIL_PopulationFlow.h"

#include "MIL_Population.h"
#include "Network/NET_ASN_Messages.h"

MIL_MOSIDManager MIL_PopulationFlow::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::MIL_PopulationFlow( const MIL_Population& population )
    : population_( population )
    , nID_       ( idManager_.GetFreeSimID() )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationFlow::~MIL_PopulationFlow()
{
    idManager_.ReleaseSimID( nID_ );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendCreation() const
{
    NET_ASN_MsgPopulationFluxCreation asnMsg;
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = population_.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationFlow::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationFlow::SendFullState() const
{
    assert( false );

    NET_ASN_MsgPopulationFluxUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_flux       = nID_;
    asnMsg.GetAsnMsg().oid_population = population_.GetID();

    //$$$

    asnMsg.Send();
}
