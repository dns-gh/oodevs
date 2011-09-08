// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
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
TrafficabilityPrototype::TrafficabilityPrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : TrafficabilityPrototype_ABC( parent )
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
void TrafficabilityPrototype::Commit()
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "Trafficability" );
	list.AddQuantity( "Trafficability", trafficability_->text().toDouble() );
}
