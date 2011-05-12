// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "StockPrototype.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"
#include "clients_kernel/DotationType.h"
#include "clients_gui/LoadableSpinBox.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: StockPrototype constructor
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
StockPrototype::StockPrototype( QWidget* parent, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, actions::parameters::ParameterList*& attributesList )
    : StockPrototype_ABC( parent, resolver )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockPrototype destructor
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
StockPrototype::~StockPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockPrototype::Commit
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
void StockPrototype::Commit()
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "Stock" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_stock );
    actions::parameters::ParameterList& dotations = list.AddList( "dotations" );

    BOOST_FOREACH( DotationWidget* dotationWidget, dotationWidgets_ )
    {
        actions::parameters::ParameterList& dotation = dotations.AddList( "dotation" );
        dotation.AddIdentifier( "DotationId", dotationWidget->dotationType_->GetValue()->GetId() );
        dotation.AddQuantity( "Stock", dotationWidget->stock_->value() );
        dotation.AddQuantity( "MaxStock", dotationWidget->maxStock_->value() );
    }
}
