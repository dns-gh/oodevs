// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderListener.h"
#include "OrderDispatcher.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "ScopeEditor.h"

using namespace dispatcher;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OrderListener constructor
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
OrderListener::OrderListener( Database_ABC& database, const dispatcher::Model& model, const OrderTypes& types, dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_ ( publisher )
    , dispatcher_( new OrderDispatcher( database, types, model ) )
    , table_     ( database.OpenTable( "Orders" ) )
    , database_  ( database )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderListener destructor
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
OrderListener::~OrderListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderListener::Listen
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
void OrderListener::Listen()
{    
    Row_ABC* row = table_.Find( "processed=0" );
    if ( row != 0 )
    {
        while( row != 0 )
        {
            dispatcher_->Dispatch( publisher_, *row );
            MarkProcessed( *row );
            row = table_.GetNextRow();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OrderListener::MarkProcessed
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
void OrderListener::MarkProcessed( Row_ABC& row ) const
{    
    row.SetField( "processed", FieldVariant( -1 ) );    
    table_.UpdateRow( row );
}
