// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerModel.h"
#include "DrawingFactory_ABC.h"
#include "DrawerShape.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "tools/GeneralConfig.h"
#include "tools/SchemaWriter_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerModel constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerModel::DrawerModel( kernel::Controllers& controllers, const DrawingFactory_ABC& factory, const kernel::EntityResolver_ABC& resolver )
    : controllers_( controllers )
    , factory_    ( factory )
    , resolver_   ( resolver )
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
void DrawerModel::Load( const tools::Loader_ABC& /*fileLoader*/, const tools::Path& file ) // modif
{
    tools::Xifstream xis( file );
    xis >> xml::start( "shapes" );
    const tools::Path schema = xis.attribute< tools::Path >( "xsi:noNamespaceSchemaLocation", "" );
    if( !schema.IsEmpty() )
        tools::Xifstream( file, xml::external_grammar( tools::GeneralConfig::BuildResourceChildFile( schema ).ToUTF8().c_str() ) );
    ReadShapes( xis );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ReadShapes
// Created: LGY 2011-02-10
// -----------------------------------------------------------------------------
void DrawerModel::ReadShapes( xml::xistream& xis )
{
    xis >> xml::list( "formation", *this, &DrawerModel::ReadFormation )
        >> xml::list( "automat", *this, &DrawerModel::ReadAutomat )
        >> xml::list( "shape", boost::bind( &DrawerModel::ReadShape, this, _1, static_cast< const kernel::Entity_ABC* >( 0 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ReadFormation
// Created: JSR 2011-06-29
// -----------------------------------------------------------------------------
void DrawerModel::ReadFormation( xml::xistream& xis )
{
    const kernel::Entity_ABC* formation = resolver_.FindFormation( xis.attribute< unsigned int >( "id" ) );
    xis >> xml::list( "shape", *this, &DrawerModel::ReadShape, formation );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ReadAutomat
// Created: JSR 2011-06-29
// -----------------------------------------------------------------------------
void DrawerModel::ReadAutomat( xml::xistream& xis )
{
    const kernel::Entity_ABC* automat = resolver_.FindAutomat( xis.attribute< unsigned int >( "id" ) );
    xis >> xml::list( "shape", *this, &DrawerModel::ReadShape, automat );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ReadShape
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerModel::ReadShape( xml::xistream& xis, const kernel::Entity_ABC* diffusionEntity )
{
    try
    {
        factory_.CreateShape( xis, diffusionEntity );
    }
    catch( ... )
    {
        throw MASA_EXCEPTION( tools::translate( "DrawerModel", "Unable to load a drawing" ).toStdString() );
    }
}

namespace
{
    typedef std::set< const kernel::Drawing_ABC* > T_Drawings;
    typedef std::map< unsigned long, T_Drawings > T_DrawingsMap;

    void SerializeDrawings( xml::xostream& xos, const T_Drawings& drawings )
    {
        std::for_each( drawings.begin(), drawings.end(), boost::bind( &kernel::Drawing_ABC::Serialize, _1, boost::ref( xos ) ) );
    }

    void SerializeDrawingsMap( xml::xostream& xos, const T_DrawingsMap& map, const std::string& tag )
    {
        for( T_DrawingsMap::const_iterator it = map.begin(); it != map.end(); ++it )
        {
            xos << xml::start( tag )
                << xml::attribute( "id", it->first );
            SerializeDrawings( xos, it->second );
            xos << xml::end;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Serialize
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerModel::Serialize( const tools::Path& filename, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    T_DrawingsMap formationMap;
    T_DrawingsMap automatMap;
    T_Drawings notDiffused;
    tools::Iterator< const kernel::Drawing_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Drawing_ABC& drawing = it.NextElement();
        const kernel::Entity_ABC* diffusionEntity = drawing.GetDiffusionEntity();
        if( diffusionEntity )
        {
            if( diffusionEntity->GetTypeName() == kernel::Formation_ABC::typeName_ )
                formationMap[ diffusionEntity->GetId() ].insert( &drawing );
            else if( diffusionEntity->GetTypeName() == kernel::Automat_ABC::typeName_ )
                automatMap[ diffusionEntity->GetId() ].insert( &drawing );
            else
                notDiffused.insert( &drawing );
        }
        else
            notDiffused.insert( &drawing );
    }

    tools::Xofstream xos( filename );
    xos << xml::start( "shapes" );
    schemaWriter.WriteExerciseSchema( xos, "drawings" );
    SerializeDrawingsMap( xos, formationMap, "formation" );
    SerializeDrawingsMap( xos, automatMap, "automat" );
    SerializeDrawings( xos, notDiffused );
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
void DrawerModel::NotifyCreated( const kernel::Drawing_ABC& shape )
{
    Register( shape.GetId(), const_cast< kernel::Drawing_ABC& >( shape ) ); // $$$$ SBO 2008-06-05: bof
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::NotifyDeleted
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
void DrawerModel::NotifyDeleted( const kernel::Drawing_ABC& shape )
{
    if( kernel::Drawing_ABC* drawing = Find( shape.GetId() ) )
        Remove( shape.GetId() );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Create
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void DrawerModel::Create( const DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity,
                          E_Dash_style dashStyle, kernel::Location_ABC& location ) const
{
    factory_.CreateShape( style, color, entity, dashStyle, location );
}
