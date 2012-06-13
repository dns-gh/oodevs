// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TrafficabilityPrototype.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"
#include "clients_gui/LoadableLineEdit.h"

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype constructor
// Created: CMA 2011-09-05
// -----------------------------------------------------------------------------
TrafficabilityPrototype::TrafficabilityPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList, double maxWeight )
    : TrafficabilityPrototype_ABC( parent, maxWeight )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype destructor
// Created: CMA 2011-09-05
// -----------------------------------------------------------------------------
TrafficabilityPrototype::~TrafficabilityPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TrafficabilityPrototype::Commit
// Created: CMA 2011-09-05
// -----------------------------------------------------------------------------
void TrafficabilityPrototype::Commit( const kernel::Team_ABC& )
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "Trafficability" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_trafficability );
    list.AddNumeric( "MaxWeight", maxWeight_->text().toFloat() );
}
