// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InhabitantFactory.h"
#include "Entities/Inhabitants/MIL_Inhabitant.h"
#include "Entities/Inhabitants/MIL_InhabitantType.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( InhabitantFactory )

// -----------------------------------------------------------------------------
// Name: InhabitantFactory constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
InhabitantFactory::InhabitantFactory( const MIL_Config& config )
    : config_( config )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantFactory destructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
InhabitantFactory::~InhabitantFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantFactory::Create
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant& InhabitantFactory::Create( xml::xistream& xis, MIL_Army_ABC& army )
{
    const MIL_InhabitantType* pType = MIL_InhabitantType::Find( xis.attribute< std::string >( "type" ) );
    if( !pType )
        throw MASA_EXCEPTION( "Unknown population type" );
    MIL_Inhabitant& inhabitant = *new MIL_Inhabitant( xis, *pType, army, config_ );
    Register( inhabitant.GetID(), inhabitant );
    return inhabitant;
}

// -----------------------------------------------------------------------------
// Name: InhabitantFactory::Finalize
// Created: LMT 2011-07-21
// -----------------------------------------------------------------------------
void InhabitantFactory::Finalize()
{
    Apply( boost::bind( &MIL_Inhabitant::Finalize, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: InhabitantFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void InhabitantFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < InhabitantFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void InhabitantFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object < InhabitantFactory_ABC >( *this );
}
