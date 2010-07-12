// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/Decision/DEC_DataBase.h"
#include "simulation_kernel/PopulationFactory.h"
#include "simulation_kernel/Entities/Populations/MIL_Population.h"
#include "simulation_kernel/Entities/Populations/MIL_PopulationType.h"
#include "simulation_kernel/FormationFactory_ABC.h"

#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PopulationFactory )

// -----------------------------------------------------------------------------
// Name: PopulationFactory constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
PopulationFactory::PopulationFactory( DEC_DataBase& database )
    : database_( database )
{

    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFactory destructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
PopulationFactory::~PopulationFactory()
{

    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFactory::Create
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
MIL_Population& PopulationFactory::Create( xml::xistream& xis, MIL_Army& army )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );

    const MIL_PopulationType* pType = MIL_PopulationType::Find( strType );
    if( !pType )
        xis.error( "Unknown population type" );

    MIL_Population& population = *new MIL_Population( xis, *pType, army, database_ );
    Register( population.GetID(), population );
    return population;
}

// -----------------------------------------------------------------------------
// Name: PopulationFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void PopulationFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < PopulationFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void PopulationFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object < PopulationFactory_ABC >( *this );
}