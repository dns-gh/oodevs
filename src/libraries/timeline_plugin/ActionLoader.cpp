// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "ActionLoader.h"
#include "Publisher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::timeline;

// -----------------------------------------------------------------------------
// Name: ActionLoader constructor
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
ActionLoader::ActionLoader( long scenarioId, long actorId, Publisher_ABC& publisher )
    : publisher_ ( publisher )
    , scenarioId_( scenarioId )
    , actorId_   ( actorId )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionLoader destructor
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
ActionLoader::~ActionLoader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionLoader::Load
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
void ActionLoader::Load( const std::string& filename )
{
    std::string errors;
    xml::xifstream xis( filename );
    xis >> xml::start( "actions" )
            >> xml::list( "action", *this, &ActionLoader::ReadAction, errors )
        >> xml::end();
    if( !errors.empty() )
        throw std::exception( std::string( "The order file contains error(s), some actions could not be loaded:\n" + errors ).c_str() );
    MT_LOG_INFO_MSG( "TimelinePlugin : load recorded orders from " + filename )
}

namespace
{
    std::string BuildMessage( xml::xistream& xis )
    {
        std::stringstream ss;
        const std::string type( xis.attribute< std::string >( "type" ) );

        if ( type == "mission" )
        {
        ss << "Unit [" << xis.attribute< std::string >( "target" ) << "] is requested to execute "
            << xis.attribute< std::string >( "type" ) << ": " << "'"
            << xis.attribute< std::string >( "name" ) << "'";
        } 
        else if ( type == "magicobject" )
        {
            const std::string name( xis.attribute< std::string >( "name" ) );

            // JCR: The first parameter should be the type of the object
            xis >> xml::start( "parameter" );
            ss << "Magic action [" << type << "] is requested to create " 
               << xis.attribute< std::string >( "value" ) << ": '" << name << "'";
        }
        else
        {
            ss << "Magic action [" << xis.attribute< std::string >( "type" ) << "] is requested to be executed " 
                << ": " << "'" << xis.attribute< std::string >( "name" ) << "'";
        }
        return ss.str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionLoader::ReadAction
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
void ActionLoader::ReadAction( xml::xistream& xis, std::string& error )
{
    try
    {
        xml::xostringstream xos;
        xos << xml::start( "CommitScenario" ) << xml::attribute( "xmlns", "urn:masa:taranis:scenario:1.0" )
                << xml::attribute( "xmlns:noNamespaceSchemaLocation", "CommitScenario.xsd" )
                << xml::attribute( "id", scenarioId_ )
                << xml::start( "CommitTask" ) << xml::attribute( "timestamp", 0 )
                    << xml::start( "task" ) << xml::attribute( "id", -1 ) << xml::attribute( "name", BuildMessage( xml::xisubstream( xis ) ) ) 
                    << xml::attribute( "actor", actorId_ ) << xml::attribute( "type", "action" )
                        << xml::start( "action" )
                            << xml::xisubstream( xis )
                        << xml::end
                    << xml::end
                << xml::end
                << xml::start( "CommitScheduler" ) << xml::attribute( "timestamp", 0 )
                    << xml::start( "schedule" )
                        << xml::attribute( "id", -1 ) << xml::attribute( "task", -1 )
                        << xml::attribute( "time", xis.attribute< std::string >( "time" ) ) << xml::attribute( "status", "pending" )
                    << xml::end
                << xml::end
            << xml::end;
        publisher_.PushReport( xos.str() );
    }
    catch( std::exception& e )
    {
        error += std::string( e.what() ) + "\n";
    }
}
