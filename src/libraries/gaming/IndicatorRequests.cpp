// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "IndicatorRequests.h"
#include "IndicatorRequest.h"
#include "clients_kernel/Controller.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IndicatorRequests constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequests::IndicatorRequests( Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests destructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequests::~IndicatorRequests()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::Purge
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
void IndicatorRequests::Purge()
{
    for( T_Requests::iterator it = requests_.begin(); it != requests_.end(); ++it )
        delete *it;
    requests_.resize( 0 );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::CreateRequest
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequest& IndicatorRequests::CreateRequest( const IndicatorDefinition_ABC& definition )
{
    requests_.push_back( new IndicatorRequest( controller_, definition, publisher_ ) );
    controller_.Update( *this );
    return *requests_.back();
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::CreateIterator
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
Iterator< const IndicatorRequest& > IndicatorRequests::CreateIterator()
{
    return new SimpleIterator< const IndicatorRequest&, T_Requests >( requests_ );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::Update
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void IndicatorRequests::Update( const ASN1T_MsgPlotResult& asnMsg )
{
    BOOST_FOREACH( T_Requests::value_type& request, requests_ )
        request->Update( asnMsg );
}
