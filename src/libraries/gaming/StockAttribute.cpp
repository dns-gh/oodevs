// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "StockAttribute.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DotationType.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StockAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
StockAttribute::StockAttribute( Controller& controller, const tools::Resolver_ABC< DotationType >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
StockAttribute::~StockAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Update
// Created: JCR 2009-06-05
// -----------------------------------------------------------------------------
void StockAttribute::Update( const DotationType& type, const ASN1T_StockResource& resource )
{
    typedef T_StockDotation::iterator IT_StockDotation;
    typedef std::pair< T_DotationState, T_DotationState > T_StockResourceValue;

    T_StockResourceValue value = stock_[ &type ];
    value.first = resource.current;
    if( resource.m.maxPresent )
        value.second = resource.max;
    stock_[ &type ] = value;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void StockAttribute::UpdateData( const T& message )
{
    if ( message.m.stockPresent )
    {
        for( unsigned i = 0; i < message.stock.resources.n; ++i )
        {
            ASN1T_StockResource* resource = message.stock.resources.elem + i;
            const DotationType* type = resolver_.Find( resource->dotation_type );
            if( type )
                Update( *type, *resource );
            controller_.Update( *(StockAttribute_ABC*)this );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void StockAttribute::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void StockAttribute::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    UpdateData( message.attributes );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void StockAttribute::DoUpdate( const ASN1T_MsgObjectCreation& message )
{
    UpdateData( message.attributes );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void StockAttribute::Display( Displayer_ABC& displayer ) const
{
    typedef T_StockDotation::const_iterator CIT_StockDotation;
    Displayer_ABC& local = 
        displayer.Group( tools::translate( "Stock", "Stock resource attribute" ) );
    
    for ( CIT_StockDotation it = stock_.begin(); it != stock_.end(); ++it )
    {
        local.Start( it->first->GetName() )
                .Add( "Dotation: " ).Add( it->second.first )
                                    .Add( " / " ).Add( it->second.second )
             .End();
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void StockAttribute::DisplayInSummary( Displayer_ABC& displayer ) const
{
    Display( displayer );
}
