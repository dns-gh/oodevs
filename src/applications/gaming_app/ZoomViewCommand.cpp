// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ZoomViewCommand.h"
#include "gaming/Command.h"
#include "gaming/CommandHandler.h"
#include "clients_gui/View_ABC.h"
#include "clients_gui/XyParser.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <Qt3Support/q3url.h>

// -----------------------------------------------------------------------------
// Name: ZoomViewCommand constructor
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
ZoomViewCommand::ZoomViewCommand( CommandHandler& handler, gui::View_ABC& view,
                                  const kernel::CoordinateConverter_ABC& converter )
    : handler_( handler )
    , view_( view )
    , converter_( converter )
{
    handler_.Register( "zoom", *this );
    handler_.Register( "center", *this );
}

// -----------------------------------------------------------------------------
// Name: ZoomViewCommand destructor
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
ZoomViewCommand::~ZoomViewCommand()
{
    handler_.Unregister( "zoom", *this );
    handler_.Unregister( "center", *this );
}

// -----------------------------------------------------------------------------
// Name: ZoomViewCommand::Receive
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
void ZoomViewCommand::Receive( const Command& command )
{
    if( command.ArgumentCount() < 1 )
        return;

    if( command.Name() == "zoom" )
        view_.Zoom( boost::lexical_cast< float >( command.Argument( 1 ) ) );
    else if( command.Name() == "center" )
    {
        const Q3Url url( command.Argument( 1 ).c_str() );
        if( url.protocol() == "loc" )
            Center( url.toString( false, false ).mid( 6 ).toStdString() ); // $$$$ SBO 2008-10-21: strlen( "loc://" )
    }
}

// -----------------------------------------------------------------------------
// Name: ZoomViewCommand::Center
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
void ZoomViewCommand::Center( const std::string& arg )
{
    gui::XyParser parser( converter_ );
    QString content = arg.c_str();
    QString hint;
    geometry::Point2f result;
    if( parser.Parse( content, result, hint ) )
        view_.CenterOn( result );
}
