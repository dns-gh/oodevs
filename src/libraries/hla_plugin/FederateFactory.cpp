// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FederateFactory.h"
#include "FederateAmbassadorFactory_ABC.h"
#include "Federate_ABC.h"
#include <tools/Path.h>
#include <hla/RtiAmbassador_ABC.h>
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: FederateFactory::CreateFederate
// Created: AHC 2011-11-09
// -----------------------------------------------------------------------------
std::unique_ptr< Federate_ABC > FederateFactory::CreateFederate( xml::xisubstream xis, hla::RtiAmbassador_ABC& ambassador,
        const FederateAmbassadorFactory_ABC& factory, const tools::Path& pluginDirectory )
{
    std::unique_ptr< Federate_ABC > federate = factory.Create( ambassador, xis.attribute< std::string >( "name", "SWORD" ), xis.attribute< int >( "lookahead", -1 ) );
    if( !federate->Connect() )
        throw MASA_EXCEPTION( "Could not connect to '" + xis.attribute< std::string >( "lrcSettings", "" ) + "'" );
    const std::string name = xis.attribute< std::string >( "federation", "Federation" );
    const bool joined = federate->Join( name, xis.attribute< bool >( "time-constrained", true ), xis.attribute< bool >( "time-regulating", true ) );
    if( !joined )
    {
        if( xis.attribute< bool >( "creation", false ) )
        {
            std::string fom = xis.attribute< std::string >( "fom", "ASI_FOM_v2.0.8_2010.xml" );
            std::vector< std::string > fomFiles;

            boost::split(fomFiles, fom, boost::is_any_of(",;"));

            if (fomFiles.size() > 1)
            {
                if( !federate->Create( name, fomFiles ) )
                    throw MASA_EXCEPTION( "Could not create the federation '" + name + "'" );
            }
            else
            {
                tools::Path fomPath = tools::Path::FromUTF8( fom );
                if( !fomPath.IsAbsolute() )
                {
                    fomPath = pluginDirectory / fomPath;
                    fomPath.Normalize();
                }
                if( !federate->Create( name, fomPath.ToUTF8() ) )
                    throw MASA_EXCEPTION( "Could not create the federation '" + name + "'" );
            }


            if( !federate->Join( name, xis.attribute< bool >( "time-constrained", true ), xis.attribute< bool >( "time-regulating", true ) ) )
                throw MASA_EXCEPTION( "Could not join the federation '" + name + "'" );
        }
        else
            throw MASA_EXCEPTION( "Could not join the federation '" + name + "'" );
    }
    return federate;
}
