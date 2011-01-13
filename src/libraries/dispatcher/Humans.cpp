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
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Humans::Humans( const sword::HumanDotations_HumanDotation& asnMsg )
    : nRank_                    ( asnMsg.rank() )
    , nNbrTotal_                ( asnMsg.total() )
    , nNbrOperational_          ( asnMsg.operational() )
    , nNbrDead_                 ( asnMsg.dead() )
    , nNbrWounded_              ( asnMsg.wounded() )
    , nNbrMentalDiseased_       ( asnMsg.mentally_wounded() )
    , nNbrNBC_                  ( asnMsg.contaminated() )
    , nNbrInLogisticMedical_    ( asnMsg.healing() )
    , nNbrInLogisticMaintenance_( asnMsg.maintenance() )
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
void Humans::Update( const sword::HumanDotations_HumanDotation& message )
{
    nNbrTotal_                = message.total();
    nNbrOperational_          = message.operational();
    nNbrDead_                 = message.dead();
    nNbrWounded_              = message.wounded();
    nNbrMentalDiseased_       = message.mentally_wounded();
    nNbrNBC_                  = message.contaminated();
    nNbrInLogisticMedical_    = message.healing();
    nNbrInLogisticMaintenance_= message.maintenance();
}

// -----------------------------------------------------------------------------
// Name: Humans::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Humans::Send( sword::HumanDotations_HumanDotation& message ) const
{
    message.set_rank               ( nRank_ );
    message.set_total              ( nNbrTotal_ );
    message.set_operational        ( nNbrOperational_ );
    message.set_dead               ( nNbrDead_ );
    message.set_wounded            ( nNbrWounded_ );
    message.set_mentally_wounded   ( nNbrMentalDiseased_ );
    message.set_contaminated       ( nNbrNBC_ );
    message.set_healing            ( nNbrInLogisticMedical_ );
    message.set_maintenance        ( nNbrInLogisticMaintenance_ );
    message.set_unevacuated_wounded( 0 ); //$$$$ RPD TO IMPLEMENT
}
