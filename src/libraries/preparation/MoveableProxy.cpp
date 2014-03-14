// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "MoveableProxy.h"

// -----------------------------------------------------------------------------
// Name: MoveableProxy constructor
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
MoveableProxy::MoveableProxy( kernel::Moveable_ABC& moveable )
    : moveable_( &moveable )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MoveableProxy destructor
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
MoveableProxy::~MoveableProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MoveableProxy::Move
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
void MoveableProxy::Move( const geometry::Point2f& position )
{
    moveable_->Move( position );
}

// -----------------------------------------------------------------------------
// Name: MoveableProxy::GetPosition
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
geometry::Point2f MoveableProxy::GetPosition( bool aggregated ) const
{
    return moveable_->GetPosition( aggregated );
}

// -----------------------------------------------------------------------------
// Name: MoveableProxy::GetHeight
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
float MoveableProxy::GetHeight( bool ) const
{
    throw MASA_EXCEPTION( "not implemented" );
}

// -----------------------------------------------------------------------------
// Name: MoveableProxy::IsIn
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
bool MoveableProxy::IsIn( const geometry::Rectangle2f& /*rectangle*/ ) const
{
    throw MASA_EXCEPTION( "not implemented" );
}

// -----------------------------------------------------------------------------
// Name: MoveableProxy::GetBoundingBox
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
geometry::Rectangle2f MoveableProxy::GetBoundingBox() const
{
    throw MASA_EXCEPTION( "not implemented" );
}

// -----------------------------------------------------------------------------
// Name: MoveableProxy::Accept
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
void MoveableProxy::Accept( kernel::LocationVisitor_ABC& /*visitor*/ ) const
{
    throw MASA_EXCEPTION( "not implemented" );
}

// -----------------------------------------------------------------------------
// Name: MoveableProxy::CanAggregate
// Created: LGY 2012-05-07
// -----------------------------------------------------------------------------
bool MoveableProxy::CanAggregate() const
{
    throw MASA_EXCEPTION( "not implemented" );
}
