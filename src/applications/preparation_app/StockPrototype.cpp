// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "StockPrototype.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/LoadableSpinBox.h"
#include "preparation/StockAttribute.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StockPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
StockPrototype::StockPrototype( QWidget* parent, const tools::Resolver_ABC< DotationType, std::string >& resolver, Object_ABC*& creation )
    : StockPrototype_ABC( parent, resolver )
    , creation_( creation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
StockPrototype::~StockPrototype()
{
}

// -----------------------------------------------------------------------------
// Name: StockPrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void StockPrototype::Commit()
{
    if( creation_ )
    {
        PropertiesDictionary& dico = creation_->Get< PropertiesDictionary >();
        StockAttribute* attribute = new StockAttribute( dico );
        BOOST_FOREACH( DotationWidget* dotationWidget, dotationWidgets_ )
            attribute->SetDotation( *dotationWidget->dotationType_->GetValue(), dotationWidget->stock_->value(), dotationWidget->maxStock_->value() );
        creation_->Attach( *attribute );
    }
}

