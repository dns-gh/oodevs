// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_ResourceNetworkTypeParameter.h"
#include "MIL.h"
#include "CheckPoints/SerializationTools.h"
#include "protocol/protocol.h"
#include "Urban/PHY_ResourceNetworkType.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_ResourceNetworkTypeParameter )

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkTypeParameter constructor
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
MIL_ResourceNetworkTypeParameter::MIL_ResourceNetworkTypeParameter()
    : pNetworkType_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkTypeParameter constructor
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
MIL_ResourceNetworkTypeParameter::MIL_ResourceNetworkTypeParameter( const sword::ResourceNetworkType& message )
    : pNetworkType_( PHY_ResourceNetworkType::Find( message.name() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkTypeParameter constructor
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
MIL_ResourceNetworkTypeParameter::MIL_ResourceNetworkTypeParameter( const PHY_ResourceNetworkType* pNetworkType )
    : pNetworkType_( pNetworkType )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkTypeParameter destructor
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
MIL_ResourceNetworkTypeParameter::~MIL_ResourceNetworkTypeParameter()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkTypeParameter::IsOfType
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
bool MIL_ResourceNetworkTypeParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eResourceNetworkType;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkTypeParameter::ToResourceNetworkType
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
bool MIL_ResourceNetworkTypeParameter::ToResourceNetworkType( const PHY_ResourceNetworkType*& value ) const
{
    value = pNetworkType_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkTypeParameter::ToElement
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
bool MIL_ResourceNetworkTypeParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    if( !pNetworkType_ )
        return false;
    elem.mutable_resourcenetworktype()->set_name( pNetworkType_->GetName() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkTypeParameter::load
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
void MIL_ResourceNetworkTypeParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::string name;
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> name;
    pNetworkType_ = PHY_ResourceNetworkType::Find( name );
}

// -----------------------------------------------------------------------------
// Name: MIL_ResourceNetworkTypeParameter::save
// Created: ABR 2012-02-15
// -----------------------------------------------------------------------------
void MIL_ResourceNetworkTypeParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    if( !pNetworkType_ )
    {
        file << boost::serialization::base_object< MIL_BaseParameter >( *this );
        file << "";
    }
    else
    {
        file << boost::serialization::base_object< MIL_BaseParameter >( *this );
        file << pNetworkType_->GetName();
    }
}
