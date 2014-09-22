// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ResourceNetworkNodeParameter.h"
#include "Decision/DEC_ResourceNetwork.h"
#include "protocol/protocol.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ResourceNetworkNodeParameter )

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkNodeParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_ResourceNetworkNodeParameter::MIL_ResourceNetworkNodeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkNodeParameter constructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
MIL_ResourceNetworkNodeParameter::MIL_ResourceNetworkNodeParameter( const sword::ResourceNetworkElement& message )
    : pResourceNetwork_( new DEC_ResourceNetwork( message ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkNodeParameter constructor
// Created: LMT 2011-05-12
// -----------------------------------------------------------------------------
MIL_ResourceNetworkNodeParameter::MIL_ResourceNetworkNodeParameter( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
    : pResourceNetwork_( resourceNetwork )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkNodeParameter destructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
MIL_ResourceNetworkNodeParameter::~MIL_ResourceNetworkNodeParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkNodeParameter::IsOfType
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
bool MIL_ResourceNetworkNodeParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eResourceNetworkNode;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkNodeParameter::ToResourceNetworkNode
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
bool MIL_ResourceNetworkNodeParameter::ToResourceNetworkNode( boost::shared_ptr< DEC_ResourceNetwork >& value ) const
{
    value = pResourceNetwork_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkNodeParameter::ToElement
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
bool MIL_ResourceNetworkNodeParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    pResourceNetwork_->Serialize( *elem.mutable_resourcenetworknode() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkNodeParameter::load
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_ResourceNetworkNodeParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> pResourceNetwork_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkNodeParameter::save
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_ResourceNetworkNodeParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << pResourceNetwork_;
}

