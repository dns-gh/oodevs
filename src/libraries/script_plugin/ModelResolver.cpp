// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ModelResolver.h"
#include "CommandString.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include <boost/lexical_cast.hpp>

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ModelResolver constructor
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
ModelResolver::ModelResolver( const dispatcher::Model& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelResolver destructor
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
ModelResolver::~ModelResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelResolver::Resolve
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ModelResolver::Resolve( const CommandString& command )
{
    return Resolve( command.Arg( 1 ), command.Arg( 2 ) );
}

// -----------------------------------------------------------------------------
// Name: ModelResolver::Resolve
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ModelResolver::Resolve( const std::string& type, const std::string& id )
{
    kernel::Entity_ABC* entity = 0;
    if( type == "agent" )
        entity = model_.agents_.Find( boost::lexical_cast< unsigned int >( id ) );
    else if( type == "automat" )
        entity = model_.automats_.Find( boost::lexical_cast< unsigned int >( id ) );
    return entity;
}
