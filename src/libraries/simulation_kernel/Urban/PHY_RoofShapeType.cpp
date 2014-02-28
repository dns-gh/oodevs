// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoofShapeType.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/ptr_container/ptr_map.hpp>

namespace
{
    boost::ptr_map< std::string, PHY_RoofShapeType > roofShapes;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoofShapeType::Initialize
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_RoofShapeType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing roof shapes" );
    xis >> xml::start( "urban" )
            >> xml::start( "urban-block-types" )
                >> xml::start( "roof-shape-types" )
                    >> xml::list( "roof-shape-type", &PHY_RoofShapeType::ReadRoofShape )
                >> xml::end
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoofShapeType::Terminate
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_RoofShapeType::Terminate()
{
    ::roofShapes.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoofShapeType::Find
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const PHY_RoofShapeType* PHY_RoofShapeType::Find( const std::string& strName )
{
    auto it = ::roofShapes.find( strName );
    if( it == ::roofShapes.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoofShapeType constructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_RoofShapeType::PHY_RoofShapeType( const std::string& strName )
    : strName_( strName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoofShapeType destructor
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
PHY_RoofShapeType::~PHY_RoofShapeType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoofShapeType::ReadRoofShape
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
void PHY_RoofShapeType::ReadRoofShape( xml::xistream& xis )
{
    std::string shape = xis.attribute< std::string >( "name" );
    if( ::roofShapes.count( shape ) )
        throw MASA_EXCEPTION( xis.context() + "Roof shape " + shape + " already defined" );
    auto next = std::auto_ptr< PHY_RoofShapeType >( new PHY_RoofShapeType( shape ) );
    ::roofShapes.insert( shape, next );
}
