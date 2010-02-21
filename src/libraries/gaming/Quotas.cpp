// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Quotas.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Quotas constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Quotas::Quotas( Controller& controller, const tools::Resolver_ABC< DotationType >& dotationResolver )
    : controller_( controller )
    , dotationResolver_( dotationResolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Quotas destructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Quotas::~Quotas()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Quotas::DoUpdate
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Quotas::DoUpdate( const MsgsSimToClient::MsgLogSupplyQuotas& message )
{
    quotas_.resize( message.quotas().elem_size() );
    for( int i = 0; i < message.quotas().elem_size(); ++i )
        quotas_[ i ] = Dotation( dotationResolver_.Get( message.quotas().elem( i ).ressource_id() )
                               , message.quotas().elem( i ).quota_disponible() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Quotas::IsRelevant
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
bool Quotas::IsRelevant() const
{
    return !quotas_.empty();
}
