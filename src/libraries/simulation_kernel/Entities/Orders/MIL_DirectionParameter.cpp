// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DirectionParameter.h"
#include "MIL.h"
#include "MT_Tools/MT_Vector2D.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_DirectionParameter )

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter constructor
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
MIL_DirectionParameter::MIL_DirectionParameter()
    : heading_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter constructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_DirectionParameter::MIL_DirectionParameter( const sword::Heading& heading )
    : heading_( heading.heading() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter constructor
// Created: PSN
// -----------------------------------------------------------------------------
MIL_DirectionParameter::MIL_DirectionParameter( boost::shared_ptr< MT_Vector2D >& dir )
{
    double tan = dir->rY_ / dir->rX_;
    double atan = std::atan(tan);
    heading_ = (int)(atan * 360. / ( 2 * MT_PI ));
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter destructor
// Created: LDC 2009-05-26
// -----------------------------------------------------------------------------
MIL_DirectionParameter::~MIL_DirectionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_DirectionParameter::IsOfType( MIL_ParameterType_ABC::E_Type type ) const
{
    return type == MIL_ParameterType_ABC::eDirection;
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter::ToDirection
// Created: MGD 2010-05-03
// -----------------------------------------------------------------------------
bool MIL_DirectionParameter::ToDirection( boost::shared_ptr< MT_Vector2D >& dir ) const
{
    MT_Vector2D direction( 0,1 );
    direction.Rotate( ( heading_ / 360.f ) * MT_PI * 2 );
    dir.reset( new MT_Vector2D( direction ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter::ToElement
// Created: MGD 2010-11-19
// -----------------------------------------------------------------------------
bool MIL_DirectionParameter::ToElement( sword::MissionParameter_Value& elem ) const
{
    elem.mutable_heading()->set_heading( heading_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter::load
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void MIL_DirectionParameter::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_BaseParameter >( *this );
    file >> heading_;
}

// -----------------------------------------------------------------------------
// Name: MIL_DirectionParameter::save
// Created: LGY 2011-08-22
// -----------------------------------------------------------------------------
void MIL_DirectionParameter::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_BaseParameter >( *this );
    file << heading_;
}
