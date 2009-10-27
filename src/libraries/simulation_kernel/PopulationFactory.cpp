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

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( PopulationFactory, "AutomateFactory" )

// -----------------------------------------------------------------------------
// Name: PopulationFactory constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
PopulationFactory::PopulationFactory( DEC_DataBase& database )
    : database_( database )
{

}

// -----------------------------------------------------------------------------
// Name: PopulationFactory destructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
PopulationFactory::~PopulationFactory()
{

}

// -----------------------------------------------------------------------------
// Name: PopulationFactory::Create
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
MIL_Population& PopulationFactory::Create( xml::xistream& xis, MIL_Army& army )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );

    const MIL_PopulationType* pType = MIL_PopulationType::Find( strType );//@TODO MGD move to internal resolver
    if( !pType )
        xis.error( "Unknown population type" );

    MIL_Population& population = *new MIL_Population( xis, *pType, army, database_ );
    Register( population.GetID(), population );
    return population;
}