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
#include "Connector.h"
#include "OrderDispatcher.h"

using namespace dispatcher;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OrderListener constructor
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
OrderListener::OrderListener( Connector& connector, const kernel::OrderTypes& types, const Model& model )
    : dispatcher_   ( new OrderDispatcher( connector, types, model ) )
    , table_        ( connector.GetTable( "Orders" ) )
    , waitingOrdersFilter_()
    , cursor_       ()
{
    waitingOrdersFilter_.CreateInstance( CLSID_QueryFilter );
    waitingOrdersFilter_->put_WhereClause( CComBSTR( "processed=false" ) );
}

// -----------------------------------------------------------------------------
// Name: OrderListener destructor
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
OrderListener::~OrderListener()
{
    waitingOrdersFilter_.Release();
}
/*
namespace 
{
    class RunCursor
    {
        RunCursor( ICursorPtr cursor ) : cursor_ ( cursor ) {}
        
        template< Functor > void Apply( OrderListener& listener, Functor& func )
        {
            IRowPtr row;
            cursor_->NextRow( &row );
            while( row != 0 )
            {
                listener.func();                
                
                ProcessOrder( row );
                cursor_->NextRow( &row );
            }
        }
    private:
        ICursorPtr cursor_;
    };
}
*/
// -----------------------------------------------------------------------------
// Name: OrderListener::Listen
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
void OrderListener::Listen( Publisher_ABC& publisher )
{
    IRowPtr row;
    HRESULT res = table_->Search( waitingOrdersFilter_, false, &cursor_ );
    if( FAILED( res ) )
        throw std::runtime_error( "Search failed" ); // $$$$ SBO 2007-05-30: 
    
    cursor_->NextRow( &row );
    while( row != 0 )
    {
        dispatcher_->Dispatch( publisher, row );
        MarkProcessed( row );
        cursor_->NextRow( &row );
    }    
}

// -----------------------------------------------------------------------------
// Name: OrderListener::MarkProcessed
// Created: SBO 2007-05-30
// -----------------------------------------------------------------------------
void OrderListener::MarkProcessed( IRowPtr row ) const
{
    IFieldsPtr fields;
    row->get_Fields( &fields );
    long index;
    fields->FindField( CComBSTR( "processed" ), &index );
    row->put_Value( index, CComVariant( true ) );
    row->Store();
}
