// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerModel.h"
#include "DrawingFactory_ABC.h"
#include "DrawerShape.h"
#include "clients_kernel/Controllers.h"
#include "tools/GeneralConfig.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerModel constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerModel::DrawerModel( kernel::Controllers& controllers, const DrawingFactory_ABC& factory )
    : controllers_( controllers )
    , factory_    ( factory )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel destructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerModel::~DrawerModel()
{
    controllers_.Unregister( *this );
    Purge();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Load
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerModel::Load( const std::string& filename )
{
    xml::xifstream xis( filename );
    xis >> xml::start( "shapes" );
    const std::string schema = xis.attribute< std::string >( "xsi:noNamespaceSchemaLocation", "" );
    if( schema.empty() )
        ReadShapes( xis );
    else
    {
        xml::xifstream xif( filename, xml::external_grammar( tools::GeneralConfig::BuildResourceChildFile( schema ) ) );
        ReadShapes( xif );
    }
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ReadShapes
// Created: LGY 2011-02-10
// -----------------------------------------------------------------------------
void DrawerModel::ReadShapes( xml::xistream& xis )
{
    xis  >> xml::list( "shape", *this, &DrawerModel::ReadShape );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ReadShape
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerModel::ReadShape( xml::xistream& xis )
{
    try
    {
        factory_.CreateShape( xis );
    }
    catch( ... )
    {
        // $$$$ SBO 2008-06-04: invalid drawing
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Save
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerModel::Save( const std::string& filename ) const
{
    xml::xofstream xos( filename );
    xos << xml::start( "shapes" )
        << xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" )
        << xml::attribute( "xsi:noNamespaceSchemaLocation", "schemas/exercise/drawings.xsd" );
    std::for_each( elements_.begin(), elements_.end(), boost::bind( &Drawing_ABC::Serialize, boost::bind( &T_Elements::value_type::second, _1 ), boost::ref( xos ) ) );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Purge
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::NotifyCreated
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
void DrawerModel::NotifyCreated( const Drawing_ABC& shape )
{
    Register( shape.GetId(), const_cast< Drawing_ABC& >( shape ) ); // $$$$ SBO 2008-06-05: bof
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::NotifyDeleted
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
void DrawerModel::NotifyDeleted( const Drawing_ABC& shape )
{
    Delete( shape.GetId() );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Create
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
Drawing_ABC* DrawerModel::Create( const DrawingTemplate& style, const QColor& color ) const
{
    return factory_.CreateShape( style, color );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Delete
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void DrawerModel::Delete( unsigned long id )
{
    Remove( id );
}
