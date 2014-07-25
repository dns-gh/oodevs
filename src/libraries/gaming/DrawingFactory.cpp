// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "DrawingFactory.h"
#include "Drawing.h"
#include "DrawingTools.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_gui/DrawingHelper.h"
#include "clients_gui/DrawingPositions.h"
#include "clients_gui/DrawingTemplate.h"

// -----------------------------------------------------------------------------
// Name: DrawingFactory constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingFactory::DrawingFactory( kernel::Controllers& controllers, const gui::DrawingTypes& types, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& resolver )
    : controllers_( controllers )
    , types_      ( types )
    , publisher_  ( publisher )
    , converter_  ( converter )
    , resolver_   ( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingFactory::~DrawingFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
kernel::Drawing_ABC* DrawingFactory::CreateShape( const sword::ShapeCreation& message ) const
{
    const kernel::Entity_ABC* diffusionEntity = 0;
    if( message.shape().has_diffusion() )
    {
        if( message.shape().diffusion().has_automat() )
            diffusionEntity = resolver_.FindAutomat( message.shape().diffusion().automat().id() );
        else if( message.shape().diffusion().has_formation() )
            diffusionEntity = resolver_.FindFormation( message.shape().diffusion().formation().id() );
    }

    gui::DrawingPositions* location = new gui::DrawingPositions();
    const auto& points = message.shape().points();
    for( int i = 0; i < points.elem_size(); ++i )
        location->AddPoint( converter_.ConvertToXY( points.elem(i) ) );

    Drawing* drawing = new Drawing( controllers_, message, diffusionEntity, types_, *location, publisher_, converter_ );
    drawing->Attach< kernel::Positions >( *location );
    drawing->Polish();
    return drawing;
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingFactory::CreateShape( const gui::DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity,
                                  gui::E_Dash_style dashStyle, kernel::Location_ABC& location, const QString& name ) const
{
    plugins::messenger::ShapeCreationRequest message;
    auto* shape = message().mutable_shape();
    shape->set_category( style.GetCategory().toStdString() );
    shape->mutable_color()->set_red( color.red() );
    shape->mutable_color()->set_green( color.green() );
    shape->mutable_color()->set_blue( color.blue() );
    shape->set_pattern( style.GetName().toStdString() );
    shape->set_name( name.toStdString() );
    if( dashStyle != gui::eSolid )
        shape->set_pen_style( sword::EnumPenStyle( dashStyle ) );
    if( entity )
    {
        if( entity->GetTypeName() == kernel::Automat_ABC::typeName_ )
            message().mutable_shape()->mutable_diffusion()->mutable_automat()->set_id( entity->GetId() );
        else if( entity->GetTypeName() == kernel::Formation_ABC::typeName_ )
            message().mutable_shape()->mutable_diffusion()->mutable_formation()->set_id( entity->GetId() );
    }
    tools::SerializeLocation( message, converter_, location );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: DrawingFactory::CreateShape
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingFactory::CreateShape( xml::xistream& xis, const kernel::Entity_ABC* entity ) const
{
    const auto& style = gui::ReadStyle( xis, types_ );
    std::unique_ptr< kernel::Location_ABC > location( style.CreateLocation() );
    gui::ReadLocation( xis, *location, converter_ );
    CreateShape( style, gui::ReadColor( xis ), entity, gui::ReadDashStyle( xis ), *location,
        xis.attribute< std::string >( "name", style.GetName().toStdString() ).c_str() );
}
