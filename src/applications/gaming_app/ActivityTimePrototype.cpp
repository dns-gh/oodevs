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
#include "clients_kernel/Units.h"
#include "protocol/Protocol.h"

using namespace Common;

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
ActivityTimePrototype::ActivityTimePrototype( QWidget* parent, MsgsClientToSim::MsgMagicActionCreateObject& msg )
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
    msg_.mutable_attributes()->mutable_activity_time()->set_value( 
        3600 * activityTime_->time().hour() + 
        60 * activityTime_->time().minute() +
        activityTime_->time().second() );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void ActivityTimePrototype::Clean()
{
    if( msg_.attributes().has_activity_time() )
        msg_.mutable_attributes()->clear_activity_time();
}
