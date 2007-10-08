// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionRequests.h"
#include "AfterActionRequest.h"
#include "clients_kernel/Controller.h"
#include <boost/bind.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionRequests constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequests::AfterActionRequests( Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequests destructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequests::~AfterActionRequests()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequests::Purge
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
void AfterActionRequests::Purge()
{
    for( T_Requests::iterator it = requests_.begin(); it != requests_.end(); ++it )
        delete *it;
    requests_.resize( 0 );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequests::CreateRequest
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequests::CreateRequest( const AfterActionFunction& function )
{
    requests_.push_back( new AfterActionRequest( controller_, function, publisher_ ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequests::CreateIterator
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
Iterator< const AfterActionRequest& > AfterActionRequests::CreateIterator()
{
    return new SimpleIterator< const AfterActionRequest&, T_Requests >( requests_ );
}

// -----------------------------------------------------------------------------
// Name: AfterActionRequests::Update
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionRequests::Update( const ASN1T_MsgIndicatorResult& asnMsg )
{
    std::for_each( requests_.begin(), requests_.end(), 
                   boost::bind( &AfterActionRequest::Update, _1, boost::ref( asnMsg ) ) );
}
