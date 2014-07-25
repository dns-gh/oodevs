// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Drawing.h"
#include "DrawingTools.h"
#include "clients_gui/DrawingTypes.h"
#include "clients_gui/DrawingCategory.h"
#include "clients_gui/DrawingTemplate.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LocationProxy.h"
#include "protocol/Protocol.h"

namespace
{
    gui::E_Dash_style Convert( const sword::ShapeCreation& message )
    {
        if( message.shape().has_pen_style() )
            return gui::E_Dash_style( message.shape().pen_style() );
        return gui::eSolid;
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::Drawing( kernel::Controllers& controllers, const sword::ShapeCreation& message, const kernel::Entity_ABC* entity, const gui::DrawingTypes& types,
                  kernel::LocationProxy& proxy, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter )
    : gui::DrawerShape( controllers, message.id().id(), message.shape().name().c_str(),
                         types.Get( message.shape().category().c_str() ).GetTemplate( message.shape().pattern() ),
                         QColor( message.shape().color().red(), message.shape().color().green(), message.shape().color().blue() ),
                         entity, proxy, converter, Convert( message ) )
    , publisher_ ( publisher )
    , converter_ ( converter )
    , controller_( controllers.controller_ )
{
    SetLocation( message.shape().points() );
    SetText( message.shape() );
}

// -----------------------------------------------------------------------------
// Name: Drawing destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
Drawing::~Drawing()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing::SetText
// Created: LGY 2014-07-23
// -----------------------------------------------------------------------------
void Drawing::SetText( const sword::Shape& shape )
{
    if( shape.has_text() && shape.has_font() )
    {
        QFont font;
        font.fromString( shape.font().c_str() );
        location_.AddText( shape.text().c_str(), font );
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing::SetLocation
// Created: SBO 2008-06-09
// -----------------------------------------------------------------------------
void Drawing::SetLocation( const sword::CoordLatLongList& list )
{
    std::unique_ptr< kernel::Location_ABC > location( style_.CreateLocation() );
    location_.SetLocation( location );
    location.release();
    for( int i = 0; i < list.elem_size(); ++i )
        location_.AddPoint( converter_.ConvertToXY( list.elem( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: Drawing::NotifyDestruction
// Created: JSR 2011-06-30
// -----------------------------------------------------------------------------
void Drawing::NotifyDestruction() const
{
    plugins::messenger::ShapeDestructionRequest message;
    message().mutable_id()->set_id( GetId() );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Drawing::Update
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::Update()
{
    SendUpdateRequest();
}

// -----------------------------------------------------------------------------
// Name: Drawing::SendUpdateRequest
// Created: LGY 2014-05-16
// -----------------------------------------------------------------------------
void Drawing::SendUpdateRequest() const
{
    plugins::messenger::ShapeUpdateRequest message;
    message().mutable_shape()->set_id( GetId() );
    message().set_category( style_.GetCategory().toStdString() );
    message().mutable_color()->set_red( color_.red() );
    message().mutable_color()->set_green( color_.green() );
    message().mutable_color()->set_blue( color_.blue() );
    message().set_pattern( style_.GetName().toStdString() );
    message().set_name( name_ );

    tools::SerializeLocation( message, converter_, location_ );
    message.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: Drawing::DoUpdate
// Created: SBO 2008-06-05
// -----------------------------------------------------------------------------
void Drawing::DoUpdate( const sword::ShapeUpdate& message )
{
    const auto& shape = message.shape();
     // $$$$ SBO 2008-06-09: can only change color and shape
    if( shape.has_color()  )
        ChangeColor( QColor( shape.color().red(), shape.color().green(),shape.color().blue() ) );
    if(shape.has_points() )
        SetLocation( shape.points() );
    if( shape.has_name() )
        name_ = shape.name().c_str();

    SetText( shape );
    controller_.Update( gui::DictionaryUpdated( *this, tools::translate( "EntityImplementation", "Info" ) ) );
    gui::DrawerShape::Update();
}

// -----------------------------------------------------------------------------
// Name: Drawing::Rename
// Created: LGY 2014-05-16
// -----------------------------------------------------------------------------
void Drawing::Rename( const QString& name )
{
    gui::DrawerShape::Rename( name );
    SendUpdateRequest();
}
