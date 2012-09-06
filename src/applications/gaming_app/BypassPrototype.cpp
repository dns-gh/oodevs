// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "BypassPrototype.h"
#include "clients_kernel/Units.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"
#include "clients_gui/LoadableSpinBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: BypassPrototype constructor
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
BypassPrototype::BypassPrototype(  QWidget* parent, actions::parameters::ParameterList*& attributesList  )
    : BypassPrototype_ABC( parent )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype destructor
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
BypassPrototype::~BypassPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassPrototype::Commit
// Created: NPT 2012-09-05
// -----------------------------------------------------------------------------
void BypassPrototype::Commit( const kernel::Team_ABC& )
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "bypass" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::bypass );
    list.AddQuantity( "Value", bypassConstructionPercentage_->value() );
}
