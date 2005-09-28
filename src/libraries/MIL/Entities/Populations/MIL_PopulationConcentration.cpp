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
#include "MIL_PopulationConcentration.h"

#include "MIL_PopulationAttitude.h"
#include "MIL_Population.h"
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

MIL_MOSIDManager MIL_PopulationConcentration::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::MIL_PopulationConcentration( const MIL_Population& population, MIL_InputArchive& archive )
    : population_     ( population )
    , nID_            ( idManager_.GetFreeSimID() )
    , position_       ()
    , nNbrAliveHumans_( 0 )
    , nNbrDeadHumans_ ( 0 )
    , pAttitude_      ( 0 )
{
    // Position - $$$ DEGEU
    std::string strPosition;
    archive.ReadField( "Position", strPosition );
    MIL_Tools::ConvertCoordMosToSim( strPosition, position_ );

    archive.ReadField( "NombreHumains", nNbrAliveHumans_ );

    std::string strAttitude;
    archive.ReadField( "Attitude", strAttitude );
    pAttitude_ = MIL_PopulationAttitude::Find( strAttitude );
    if( !pAttitude_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown attitude", archive.GetContext() );

//    SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_PopulationConcentration::~MIL_PopulationConcentration()
{
    idManager_.ReleaseSimID( nID_ );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendCreation() const
{
    NET_ASN_MsgPopulationConcentrationCreation asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = population_.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationConcentration::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_PopulationConcentration::SendFullState() const
{
    assert( pAttitude_ );

    NET_ASN_MsgPopulationConcentrationUpdate asnMsg;
    asnMsg.GetAsnMsg().oid_concentration = nID_;
    asnMsg.GetAsnMsg().oid_population    = population_.GetID();

    asnMsg.GetAsnMsg().m.formePresent              = 1;
    asnMsg.GetAsnMsg().m.attitudePresent           = 1;
    asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
    asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1;

    NET_ASN_Tools::WritePoint( position_, asnMsg.GetAsnMsg().forme ); //$$$$$$
    asnMsg.GetAsnMsg().attitude = pAttitude_->GetAsnID();
    asnMsg.GetAsnMsg().nb_humains_morts   = nNbrDeadHumans_;
    asnMsg.GetAsnMsg().nb_humains_vivants = nNbrAliveHumans_;

    asnMsg.Send();

    NET_ASN_Tools::Delete( asnMsg.GetAsnMsg().forme );
}