// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "LogisticLink.h"
#include "Tools.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticLink constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLink::LogisticLink( const kernel::Entity_ABC& superior )
    : superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLink destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLink::~LogisticLink()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::Update
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void LogisticLink::Update( const sword::SeqOfDotationQuota& message, const tools::Resolver_ABC< kernel::DotationType >& dotationResolver )
{
    quotas_.resize( message.elem_size() );
    for( int i = 0; i < message.elem_size(); ++i )
        quotas_[ i ] = Dotation( dotationResolver.Get( message.elem( i ).resource().id() )
                               , message.elem( i ).quantity() );
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::Draw
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void LogisticLink::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& /*viewport*/, const GlTools_ABC& tools ) const
{
    tools.DrawCurvedArrow( where, superior_.Get< Positions >().GetPosition(), 0.3f );
}
