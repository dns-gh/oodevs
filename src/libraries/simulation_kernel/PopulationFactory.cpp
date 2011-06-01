// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PopulationFactory.h"
#include "FormationFactory_ABC.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "MT_Tools/MT_ScipioException.h"
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PopulationFactory )

// -----------------------------------------------------------------------------
// Name: PopulationFactory constructor
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
PopulationFactory::PopulationFactory( unsigned int gcPause, unsigned int gcMult )
    : gcPause_( gcPause )
    , gcMult_ ( gcMult )
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
MIL_Population& PopulationFactory::Create( xml::xistream& xis, MIL_Army_ABC& army )
{
    const MIL_PopulationType* pType = MIL_PopulationType::Find( xis.attribute< std::string >( "type" ) );
    if( !pType )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown population type" );
    MIL_Population& population = *new MIL_Population( xis, *pType, army, gcPause_, gcMult_ );
    Register( population.GetID(), population );
    return population;
}

// -----------------------------------------------------------------------------
// Name: PopulationFactory::Create
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
MIL_Population& PopulationFactory::Create( const std::string& type, const MT_Vector2D& point, int number, const std::string& name, MIL_Army_ABC& army, UrbanObjectWrapper* pUrbanObject /*= 0*/ )
{
    const MIL_PopulationType* pType = MIL_PopulationType::Find( type );
    if( !pType )
        throw std::runtime_error( "Unknown population type" );
    MIL_Population& population = *new MIL_Population( *pType, army, point, number, name, gcPause_, gcMult_ );
    Register( population.GetID(), population );
    if( pUrbanObject )
        populationFromUrbanObjectResolver_.Register( pUrbanObject, population );
    return population;
}

// -----------------------------------------------------------------------------
// Name: PopulationFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void PopulationFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PopulationFactory_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void PopulationFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< PopulationFactory_ABC >( *this );
}
