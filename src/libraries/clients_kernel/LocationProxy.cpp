// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "LocationProxy.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LocationProxy constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
LocationProxy::LocationProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationProxy destructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
LocationProxy::~LocationProxy()
{
    // NOTHING
}

namespace
{
    void DoNothing( Location_ABC* )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::SetLocation
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void LocationProxy::SetLocation( Location_ABC& location )
{
    location_ = boost::shared_ptr< Location_ABC >( &location, &DoNothing );
    Update();
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::SetLocation
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void LocationProxy::SetLocation( std::auto_ptr< Location_ABC >& location )
{
    location_.reset( location.release() );
    Update();
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::PopPoint
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void LocationProxy::PopPoint()
{
    if( location_ )
        location_->PopPoint();
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::AddPoint
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void LocationProxy::AddPoint( const geometry::Point2f& point )
{
    if( location_ )
        location_->AddPoint( point );
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::Translate
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void LocationProxy::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
{
    if( location_ )
        location_->Translate( from, translation, precision );
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::Clone
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
Location_ABC& LocationProxy::Clone() const
{
    if( location_ )
        return location_->Clone();
    throw MASA_EXCEPTION( " Location proxy was not initialized." );
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::GetTypeName
// Created: JSR 2012-02-17
// -----------------------------------------------------------------------------
std::string LocationProxy::GetTypeName() const
{
    return location_ ? location_->GetTypeName() : "";
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::Accept
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void LocationProxy::Accept( LocationVisitor_ABC& visitor ) const
{
    if( location_ )
        location_->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::GetName
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
QString LocationProxy::GetName() const
{
    return location_ ? location_->GetName() : "";
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::IsValid
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
bool LocationProxy::IsValid() const
{
    return location_ ? location_->IsValid() : false;
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::IsDone
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
bool LocationProxy::IsDone() const
{
    return location_ ? location_->IsDone() : false;
}

// -----------------------------------------------------------------------------
// Name: LocationProxy::Update
// Created: LGY 2014-05-14
// -----------------------------------------------------------------------------
void LocationProxy::Update()
{
    // NOTHING
}
