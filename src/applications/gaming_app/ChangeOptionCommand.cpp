// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeOptionCommand.h"
#include "gaming/CommandHandler.h"
#include "gaming/Command.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Settings_ABC.h"
#include "clients_kernel/TriStateOption.h"
#include "clients_kernel/FourStateOption.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ChangeOptionCommand constructor
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
ChangeOptionCommand::ChangeOptionCommand( CommandHandler& handler, OptionsController& options )
    : handler_( handler )
    , options_( options )
{
    handler_.Register( "option", *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeOptionCommand destructor
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
ChangeOptionCommand::~ChangeOptionCommand()
{
    handler_.Unregister( "option", *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeOptionCommand::Receive
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
void ChangeOptionCommand::Receive( const Command& command )
{
    // /option tname value
    if( command.ArgumentCount() > 1 )
        ChangeOption( command.Argument( 1 )[0], command.Argument( 1 ).substr( 1 ), command.Argument( 2 ) );
}

// -----------------------------------------------------------------------------
// Name: ChangeOptionCommand::SetOption
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
template< typename T >
void ChangeOptionCommand::SetOption( const std::string& name, const std::string& value )
{
    options_.Change( name, boost::lexical_cast< T >( value ) );
}

// -----------------------------------------------------------------------------
// Name: ChangeOptionCommand::ChangeOption
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
void ChangeOptionCommand::ChangeOption( char type, const std::string& name, const std::string& value )
{
    if( type == Settings_ABC::intPrefix )
        SetOption< int >( name, value );
    else if( type == Settings_ABC::boolPrefix )
        SetOption< bool >( name, value );
    else if( type == Settings_ABC::floatPrefix )
        SetOption< float >( name, value );
    else if( type == Settings_ABC::tristatePrefix )
        options_.Change( name, TristateOption( value.c_str() ) );
    else if( type == Settings_ABC::fourstatePrefix )
        options_.Change( name, FourStateOption( value.c_str() ) );
    else if( type == Settings_ABC::stringPrefix )
        options_.Change( name, QString( value.c_str() ) );
}
