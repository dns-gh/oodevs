// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SupplyResourceRequest.h"
#include "clients_kernel/DotationType.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyResourceRequest constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
SupplyResourceRequest::SupplyResourceRequest( const DotationType& type, unsigned int requested, unsigned int granted, unsigned int convoyed )
    : type_( &type )
    , requested_( requested )
    , granted_( granted )
    , convoyed_( convoyed )
    , delivered_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceRequest destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
SupplyResourceRequest::~SupplyResourceRequest()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceRequest::GetTypeName
// Created: MMC 2012-10-30
// -----------------------------------------------------------------------------
QString SupplyResourceRequest::GetTypeName() const
{
    if( !type_ )
        return QString();
    return QString::fromStdString( type_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceRequest::GetRequested
// Created: MMC 2012-10-30
// -----------------------------------------------------------------------------
unsigned int SupplyResourceRequest::GetRequested() const
{
    return requested_;
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceRequest::GetRequested
// Created: MMC 2012-10-30
// -----------------------------------------------------------------------------
unsigned int SupplyResourceRequest::GetGranted() const
{
    return granted_;
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceRequest::GetRequested
// Created: MMC 2012-10-30
// -----------------------------------------------------------------------------
unsigned int SupplyResourceRequest::GetConvoyed() const
{
    return convoyed_;
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceRequest::IsDelivered
// Created: LGY 2014-01-24
// -----------------------------------------------------------------------------
bool SupplyResourceRequest::IsDelivered() const
{
    return delivered_;
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceRequest::Deliver
// Created: LGY 2014-01-24
// -----------------------------------------------------------------------------
void SupplyResourceRequest::Deliver()
{
    delivered_ = true;
}

// -----------------------------------------------------------------------------
// Name: SupplyResourceRequest::Update
// Created: LGY 2014-01-24
// -----------------------------------------------------------------------------
void SupplyResourceRequest::Update( const ::google::protobuf::RepeatedPtrField< ::sword::SupplyResourceRequest >& resources )
{
    auto it = std::find_if( resources.begin(), resources.end(),
        [&]( const ::sword::SupplyResourceRequest& value )->bool {
            return value.resource().id() == type_->GetId(); } );
    if( it == resources.end() )
        Deliver();
    else
    {
        requested_ = it->requested();
        granted_ = it->granted();
        convoyed_ = it->convoyed();
        delivered_ = false;
    }
}
