// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CrossingSitePrototype.h"
#include "clients_kernel/Units.h"
#include "clients_gui/LoadableSpinBox.h"
#include "clients_gui/LoadableCheckBox.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype constructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::CrossingSitePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : CrossingSitePrototype_ABC( parent )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype destructor
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CrossingSitePrototype::~CrossingSitePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSitePrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CrossingSitePrototype::Commit()
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "CrossingSite" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_crossing_site );
    list.AddQuantity( "Width", width_->value() );
    list.AddQuantity( "Depth", depth_->value() );
    list.AddQuantity( "FlowRate", speed_->value() );
    list.AddBool( "BanksRequireFitting", needsConstruction_->isChecked() );
}
