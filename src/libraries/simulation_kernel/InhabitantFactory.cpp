// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "simulation_kernel/InhabitantFactory.h"
#include "simulation_kernel/Entities/Inhabitants/MIL_Inhabitant.h"
#include "simulation_kernel/Entities/Inhabitants/MIL_InhabitantType.h"
#include "simulation_kernel/FormationFactory_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( InhabitantFactory )

// -----------------------------------------------------------------------------
// Name: InhabitantFactory constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
InhabitantFactory::InhabitantFactory()
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
    std::string strType( xis.attribute< std::string >( "type" ) );
    const MIL_InhabitantType* pType = MIL_InhabitantType::Find( strType );
    if( !pType )
        xis.error( "Unknown population type" );
    MIL_Inhabitant& inhabitant = *new MIL_Inhabitant( xis, *pType, army );
    Register( inhabitant.GetID(), inhabitant );
    return inhabitant;
}

// -----------------------------------------------------------------------------
// Name: InhabitantFactory::Create
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant& InhabitantFactory::Create( const std::string& type, const MT_Vector2D& point, int number, const std::string& name, MIL_Army_ABC& army )
{
    const MIL_InhabitantType* pType = MIL_InhabitantType::Find( type );
    if( !pType )
        throw std::runtime_error( "Unknown population type" );
    MIL_Inhabitant& inhabitant = *new MIL_Inhabitant( *pType, army, point, number, name );
    Register( inhabitant.GetID(), inhabitant );
    return inhabitant;
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