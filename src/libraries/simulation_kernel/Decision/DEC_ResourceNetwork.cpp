// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_ResourceNetwork.h"
#include "protocol/protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_ResourceNetwork )

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetwork constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
DEC_ResourceNetwork::DEC_ResourceNetwork()
    : objectId_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetwork constructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
DEC_ResourceNetwork::DEC_ResourceNetwork( const sword::ResourceNetworkElement& message )
    : objectId_( message.object().id() )
    , resource_( message.resource().name() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetwork constructor
// Created: JSR 2012-01-17
// -----------------------------------------------------------------------------
DEC_ResourceNetwork::DEC_ResourceNetwork( unsigned int objectId, const std::string& resource )
    : objectId_( objectId )
    , resource_( resource )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetwork destructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
DEC_ResourceNetwork::~DEC_ResourceNetwork()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetwork::Serialize
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void DEC_ResourceNetwork::Serialize( sword::ResourceNetworkElement& message ) const
{
    message.mutable_object()->set_id( objectId_ );
    message.mutable_resource()->set_name( resource_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetwork::GetObjectId
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
unsigned int DEC_ResourceNetwork::GetObjectId() const
{
    return objectId_;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetwork::GetResource
// Created: JSR 2011-05-04
// -----------------------------------------------------------------------------
const std::string& DEC_ResourceNetwork::GetResource() const
{
    return resource_;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetwork::GetTypeName
// Created: LMT 2011-05-12
// -----------------------------------------------------------------------------
const std::string& DEC_ResourceNetwork::GetTypeName() const
{
    static const std::string result( "boost::shared_ptr<class DEC_ResourceNetwork>" );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_ResourceNetwork::serialize
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_ResourceNetwork::serialize( Archive& file, const unsigned int )
{
    file & objectId_;
    file & resource_;
}
