// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ActivityTimePrototype.h"
#include "clients_kernel/Units.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"
#include "clients_gui/LoadableTimeEdit.h"

using namespace sword;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ActivityTimePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
ActivityTimePrototype::ActivityTimePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : ActivityTimePrototype_ABC( parent )
    , attributesList_( attributesList )
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
void ActivityTimePrototype::Commit( const kernel::Team_ABC& )
{
    int time = 3600 * activityTime_->time().hour() +
               60 * activityTime_->time().minute() +
               activityTime_->time().second();
    actions::parameters::ParameterList& list = attributesList_->AddList( "ActivityTime" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_activity_time );
    list.AddQuantity( "Time", time );
}
