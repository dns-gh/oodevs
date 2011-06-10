// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ResourceNetworkParameter.h"
#include "Decision/DEC_ResourceNetwork.h"
#include "protocol/protocol.h"
#include "Checkpoints/SerializationTools.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ResourceNetworkParameter )

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_ResourceNetworkParameter::MIL_ResourceNetworkParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkParameter constructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
MIL_ResourceNetworkParameter::MIL_ResourceNetworkParameter( const sword::ResourceNetworkElement& message )
    : pResourceNetwork_( new DEC_ResourceNetwork( message ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkParameter constructor
// Created: LMT 2011-05-12
// -----------------------------------------------------------------------------
MIL_ResourceNetworkParameter::MIL_ResourceNetworkParameter( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork )
    : pResourceNetwork_( resourceNetwork )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkParameter destructor
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
MIL_ResourceNetworkParameter::~MIL_ResourceNetworkParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkParameter::IsOfType
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
bool MIL_ResourceNetworkParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eResourceNetwork;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkParameter::ToResourceNetwork
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
bool MIL_ResourceNetworkParameter::ToResourceNetwork( boost::shared_ptr< DEC_ResourceNetwork >& value ) const
{
    value = pResourceNetwork_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkParameter::ToElement
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
bool MIL_ResourceNetworkParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    pResourceNetwork_->Serialize( *elem.mutable_resourcenetwork() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkParameter::load
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_ResourceNetworkParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this )
         >> pResourceNetwork_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkParameter::save
// Created: LGY 2011-06-10
// -----------------------------------------------------------------------------
void MIL_ResourceNetworkParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this )
         << pResourceNetwork_;
}
