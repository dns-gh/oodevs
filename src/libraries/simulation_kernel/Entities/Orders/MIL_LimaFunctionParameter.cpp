// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LimaFunctionParameter.h"
#include "protocol/Protocol.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_LimaFunctionParameter )

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunctionParameter constructor
// Created: MMC 2013-10-14
// -----------------------------------------------------------------------------
MIL_LimaFunctionParameter::MIL_LimaFunctionParameter()
    : id_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunctionParameter constructor
// Created: MMC 2013-10-14
// -----------------------------------------------------------------------------
MIL_LimaFunctionParameter::MIL_LimaFunctionParameter( const sword::PhaseLineOrder::Function& phaseLinefunction )
    : id_( static_cast< int >( phaseLinefunction ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunctionParameter destructor
// Created: MMC 2013-10-14
// -----------------------------------------------------------------------------
MIL_LimaFunctionParameter::~MIL_LimaFunctionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunctionParameter::IsOfType
// Created: MMC 2013-10-14
// -----------------------------------------------------------------------------
bool MIL_LimaFunctionParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eLimaFunction;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunctionParameter::ToIndirectFire
// Created: MMC 2013-10-14
// -----------------------------------------------------------------------------
bool MIL_LimaFunctionParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.set_phase_line_function( sword::PhaseLineOrder::Function( id_ ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunctionParameter::load
// Created: MMC 2013-10-14
// -----------------------------------------------------------------------------
void MIL_LimaFunctionParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> id_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaFunctionParameter::save
// Created: MMC 2013-10-14
// -----------------------------------------------------------------------------
void MIL_LimaFunctionParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << id_;
}
