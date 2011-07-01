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
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DotationType.h"
#include "protocol/Simulation.h"

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
void StockAttribute::Update( const DotationType& type, const sword::StockResource& resource )
{
    StockDotation& dotation = stockDotations_[ &type ];
    dotation.stock_ = resource.current();
    if( resource.has_maximum() )
        dotation.maxStock_ = resource.maximum();
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void StockAttribute::UpdateData( const T& message )
{
    if( message.has_stock() )
        for( int i = 0; i < message.stock().resources_size(); ++i )
        {
            const sword::StockResource& resource = message.stock().resources( i );
            const DotationType* type = resolver_.Find( resource.resource().id() );
            if( type )
                Update( *type, resource );
            controller_.Update( *(StockAttribute_ABC*)this );
        }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void StockAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void StockAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

