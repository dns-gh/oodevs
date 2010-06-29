// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Humans.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Humans::Humans( const MsgsSimToClient::HumanDotations_HumanDotation& asnMsg )
    : nRank_                    ( asnMsg.rang() )
    , nNbrTotal_                ( asnMsg.nb_total() )
    , nNbrOperational_          ( asnMsg.nb_operationnels() )
    , nNbrDead_                 ( asnMsg.nb_morts() )
    , nNbrWounded_              ( asnMsg.nb_blesses() )
    , nNbrMentalDiseased_       ( asnMsg.nb_blesses_mentaux() )
    , nNbrNBC_                  ( asnMsg.nb_contamines_nbc() )
    , nNbrInLogisticMedical_    ( asnMsg.nb_dans_chaine_sante() )
    , nNbrInLogisticMaintenance_( asnMsg.nb_utilises_pour_maintenance() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Humans::~Humans()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Humans::Update( const MsgsSimToClient::HumanDotations_HumanDotation& message )
{
    nNbrTotal_                = message.nb_total();
    nNbrOperational_          = message.nb_operationnels();
    nNbrDead_                 = message.nb_morts();
    nNbrWounded_              = message.nb_blesses();
    nNbrMentalDiseased_       = message.nb_blesses_mentaux();
    nNbrNBC_                  = message.nb_contamines_nbc();
    nNbrInLogisticMedical_    = message.nb_dans_chaine_sante();
    nNbrInLogisticMaintenance_= message.nb_utilises_pour_maintenance();
}

// -----------------------------------------------------------------------------
// Name: Humans::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Humans::Send( MsgsSimToClient::HumanDotations_HumanDotation& message ) const
{
    message.set_rang                      ( nRank_ );
    message.set_nb_total                  ( nNbrTotal_ );
    message.set_nb_operationnels          ( nNbrOperational_ );
    message.set_nb_morts                  ( nNbrDead_ );
    message.set_nb_blesses                ( nNbrWounded_ );
    message.set_nb_blesses_mentaux        ( nNbrMentalDiseased_ );
    message.set_nb_contamines_nbc         ( nNbrNBC_ );
    message.set_nb_dans_chaine_sante      ( nNbrInLogisticMedical_ );
    message.set_nb_utilises_pour_maintenance( nNbrInLogisticMaintenance_ );
}
