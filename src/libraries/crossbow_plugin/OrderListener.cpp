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
#include <sstream>

using namespace dispatcher;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OrderListener constructor
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
OrderListener::OrderListener( Database_ABC& database, const dispatcher::Model& model, const OrderTypes& types, dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_ ( publisher )
    , dispatcher_( new OrderDispatcher( database, types, model ) )
    , validation_( *database.OpenTable( "OrdersValidation" ) )
    , database_  ( database )
    , ref_ ( 0 )
{
    database_.ClearTable( "Orders" ); // Clear table row data    
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
    long ref = ref_;
    {
        std::stringstream ss;
        ss << "OBJECTID > " << ref_ << " AND Checked=-1";
        std::auto_ptr< Table_ABC > table( database_.OpenTable( "Orders" ) );
        Row_ABC* row = table->Find( ss.str() );
        while( row != 0 )
        {
            Copy( *row );
            ref_ = row->GetID();
            row = table->GetNextRow();
        }        
    }
    if ( ref != ref_ )
    {        
        Row_ABC* row = validation_.Find( "Checked=0" );
    //    Row_ABC* row = table_.Find( "[Orders].[OBJECTID] NOT IN ( SELECT [OrdersValidation].[OrderID] FROM OrdersValidation);" );     
        while( row != 0 )
        {
            dispatcher_->Dispatch( publisher_, *row );
            MarkProcessed( *row );            
            row = validation_.GetNextRow();
        }        
    }
}

// -----------------------------------------------------------------------------
// Name: OrderListener::Copy
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
void OrderListener::Copy( Row_ABC& row )
{
    Row_ABC& created = validation_.CreateRow();
    created.SetField( "OrderID", row.GetID() );
    created.SetField( "Checked", FieldVariant( 0 ) );
    created.SetField( "Name", row.GetField( "Name" ) );
    created.SetField( "TargetID", row.GetField( "TargetID" ) );    
    validation_.UpdateRow( created );    
}

// -----------------------------------------------------------------------------
// Name: OrderListener::MarkProcessed
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
void OrderListener::MarkProcessed( Row_ABC& row ) const
{
//    Table_ABC& table = database_.OpenTable( "OrdersValidation" );
//    Row_ABC& created = table.CreateRow();
//    created.SetField( "OrderID", FieldVariant( row.GetID() ) );        
    row.SetField( "Checked", FieldVariant( -1 ) );
    validation_.UpdateRow( row );
//    table.UpdateRow( created );    
}
