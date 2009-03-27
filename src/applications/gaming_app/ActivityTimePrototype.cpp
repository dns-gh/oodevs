// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ActivityTimePrototype.h"
#include "game_asn/SimulationSenders.h"
#include "clients_kernel/Units.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
ActivityTimePrototype::ActivityTimePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg )
    : ActivityTimePrototype_ABC ( parent )
    , msg_ ( msg )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype destructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
ActivityTimePrototype::~ActivityTimePrototype()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ActivityTimePrototype::Commit()
{
    msg_.attributes.m.activity_timePresent = 1;
    msg_.attributes.activity_time.value = 
        3600 * activityTime_->time().hour() + 
        60 * activityTime_->time().minute() +
        activityTime_->time().second();
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ActivityTimePrototype::Clean()
{
    msg_.attributes.m.activity_timePresent = 0;
}
