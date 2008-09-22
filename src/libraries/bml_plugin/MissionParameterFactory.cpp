// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterFactory.h"
#include "MissionParameterLimit.h"
#include "MissionParameterPhaseLine.h"
#include "MissionParameterHeading.h"
#include "MissionParameterPolygon.h"
#include "MissionParameterPath.h"
#include "MissionParameterPoint.h"
#include "MissionParameterAutomat.h"
#include "MissionParameterAgent.h"
#include "Mission.h"
#include "MissionMapping.h"
#include "SerializationTools.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/OrderParameter.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Model.h"
#include <xeumeuleu/xml.h>
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterFactory constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterFactory::MissionParameterFactory( const Mission& mission, const kernel::MissionType& type, const dispatcher::Automat& automat )
    : mission_( mission )
    , type_( type )
    , automat_( &automat )
    , agent_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterFactory constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterFactory::MissionParameterFactory( const Mission& mission, const kernel::MissionType& type, const dispatcher::Agent& agent )
    : mission_( mission )
    , type_( type )
    , automat_( 0 )
    , agent_( &agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterFactory destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameterFactory::~MissionParameterFactory()
{
    // NOTHING
}

namespace
{
    const kernel::OrderParameter* GetParameterByType( const kernel::MissionType& type, const Mission& mission, const std::string& code )
    {
        const std::string parameterType = GetParameterTypeFromCode( code );
        kernel::Iterator< const kernel::OrderParameter& > it( type.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const kernel::OrderParameter& parameter = it.NextElement();
            if( boost::algorithm::to_lower_copy( parameter.GetType() ) == parameterType && !mission.IsSet( parameter ) )
                return &parameter;
        }
        return 0;
    }
}

namespace
{
    template< typename Entity >
    struct NameFinder
    {
        explicit NameFinder( const std::string& name ) : name_( name ), result_( 0 ) {}
        void operator()( const Entity& entity ) const
        {
            if( entity.name_ == name_ )
                result_ = &entity;
        }
        std::string name_;
        mutable const Entity* result_;
    };

    const dispatcher::Automat* FindAutomat( const dispatcher::Model& model, const std::string& name )
    {
        NameFinder< dispatcher::Automat > automatFinder( name );
        model.automats_.Apply( automatFinder );
        return automatFinder.result_;
    }

    const dispatcher::Agent* FindAgent( const dispatcher::Model& model, const std::string& name )
    {
        NameFinder< dispatcher::Agent > agentFinder( name );
        model.agents_.Apply( agentFinder );
        return agentFinder.result_;
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameterFactory::CreateParameter
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
MissionParameter_ABC* MissionParameterFactory::CreateParameter( xml::xistream& xis, const dispatcher::Model& model ) const
{
    std::string name, code, oid;
    xis >> xml::start( NS( "C_BML_Who", "cbml" ) )
            >> xml::optional()
                >> xml::start( NS( "WhoInstance", "cbml" ) )
                    >> xml::start( NS( "ControlFeatureType", "cbml" ) )
                        >> xml::content( NS( "NameText", "jc3iedm" ), name )
                        >> xml::content( NS( "CategoryCode", "jc3iedm" ), code )
                    >> xml::end()
                >> xml::end()
            >> xml::optional()
                >> xml::start( NS( "WhoRef", "cbml" ) )
                    >> xml::start( NS( "UnitRef", "cbml" ) )
                        >> xml::content( NS( "OID", "jc3iedm" ), oid )
                    >> xml::end()
                >> xml::end()
        >> xml::end();
    
    const dispatcher::Agent* agent = 0;
    const dispatcher::Automat* automat = 0;
    if( !oid.empty() )
    {
        automat = FindAutomat( model, oid );
        if( !automat )
            agent = FindAgent( model, oid );
        code = automat ? "automate" : ( agent ? "agent" : "" );
    }

    const kernel::OrderParameter* definition = GetParameterByType( type_, mission_, code );
    if( definition == 0 )
        throw std::runtime_error( __FUNCTION__ ": Unknown parameter" );
    
    xis >> xml::optional()
            >> xml::start( NS( "C_BML_Where", "cbml" ) )
                >> xml::start( NS( "WhereInstance", "cbml" ) );
    MissionParameter_ABC* param = 0;
	std::string type = boost::algorithm::to_lower_copy( definition->GetType() );
    if( type == "limit" )
        param = new MissionParameterLimit( xis, *definition );
    else if( type == "phaselinelist" )
        param = new MissionParameterPhaseLine( xis, *definition, name );
    else if( type == "direction" )
    {
        if( agent_ )
            param = new MissionParameterHeading( xis, *definition, *agent_ );
        else
            param = new MissionParameterHeading( xis, *definition, *automat_ );
    }
    else if( type == "polygon" )
        param = new MissionParameterPolygon( xis, *definition );
    else if( type == "path" )
        param = new MissionParameterPath( xis, *definition );
    else if( type == "point" )
        param = new MissionParameterPoint( xis, *definition );
    else if( type == "automate" )
        param = new MissionParameterAutomat( *definition, *automat );
    else if( type == "agent" )
        param = new MissionParameterAgent( *definition, *agent );
    if( !param )
        throw std::runtime_error( std::string( __FUNCTION__ ": Unsupported mission parameter: " ) + definition->GetType() );
    xis     >> xml::end()
        >> xml::end();
    return param;
}
