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

PHY_RoofShapeType::T_RoofShapeMap PHY_RoofShapeType::roofShapes_;

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
    for( CIT_RoofShapeMap itRoofShape = roofShapes_.begin(); itRoofShape != roofShapes_.end(); ++itRoofShape )
        delete itRoofShape->second;
    roofShapes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoofShapeType::Find
// Created: JSR 2011-02-17
// -----------------------------------------------------------------------------
const PHY_RoofShapeType* PHY_RoofShapeType::Find( const std::string& strName )
{
    CIT_RoofShapeMap itRoofShape = roofShapes_.find( strName );
    if( itRoofShape == roofShapes_.end() )
        return 0;
    return itRoofShape->second;
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
    std::string strRoofShape = xis.attribute< std::string >( "name" );
    const PHY_RoofShapeType*& pRoofShape = roofShapes_[ strRoofShape ];
    if( pRoofShape )
        xis.error( "Roof shape " + strRoofShape + " already defined" );
    pRoofShape = new PHY_RoofShapeType( strRoofShape );
}
