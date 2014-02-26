// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DelayPrototype.h"
#include "clients_kernel/Units.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"
#include "clients_gui/LoadableTimeEdit.h"

using namespace sword;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: DelayPrototype constructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayPrototype::DelayPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : DelayPrototype_ABC( parent )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayPrototype destructor
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
DelayPrototype::~DelayPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DelayPrototype::Commit
// Created: JSR 2010-07-06
// -----------------------------------------------------------------------------
void DelayPrototype::Commit( const kernel::Team_ABC& )
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "Delay" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_effect_delay );
    list.AddQuantity( "Time", delayTime_->Seconds() );
}
