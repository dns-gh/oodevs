// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ReportFactory.h"
#include "ReportTemplate.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Agent.h"
#include "dispatcher/AgentKnowledge.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: ReportFactory constructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportFactory::ReportFactory( const dispatcher::Config& config, const dispatcher::Model_ABC& model )
    : model_( model )
{
    xml::xifstream main( config.GetPhysicalFile() );
    std::string reports;
    main >> xml::start( "physical" )
            >> xml::start( "reports" )
            >> xml::attribute( "file", reports );
    xml::xifstream xis( config.BuildPhysicalChildFile( reports ) );
    xis >> xml::start( "reports" )
            >> xml::list( "report", *this, &ReportFactory::ReadReport )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ReportFactory destructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportFactory::~ReportFactory()
{
    for( auto it = templates_.begin(); it != templates_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::ReadReport
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportFactory::ReadReport( xml::xistream& xis )
{
    ReportTemplate* report = new ReportTemplate( xis, *this );
    templates_[report->GetId()] = report;
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateMessage
// Created: SBO 2007-08-27
// -----------------------------------------------------------------------------
std::string ReportFactory::CreateMessage( const sword::Report& message ) const
{
    CIT_Templates it = templates_.find( message.type().id() );
    if( it == templates_.end() )
        return "Unknown report";
    if( message.has_parameters() )
        return it->second->RenderMessage( message.parameters() );
    else
        return it->second->GetMessage();
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::RenderParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
std::string ReportFactory::RenderParameter( const sword::MissionParameter& value ) const
{
    std::stringstream ss;

    const sword::MissionParameter_Value& param = value.value().Get(0);
    if( param.has_areal() )
        ss << param.areal();
    else if( param.has_agent() )
    {
        if( const dispatcher::Agent_ABC* agent = model_.Agents().Find( param.agent().id() ) )
            ss << agent->GetName().toAscii().constData() << " [" << agent->GetId() << "]";
    }
    else if( param.has_agentknowledge() )
    {
        if( const dispatcher::AgentKnowledge_ABC* knowledge = model_.AgentKnowledges().Find( param.agentknowledge().id() ) )
            ss << knowledge->GetName().toAscii().constData() << " [" << knowledge->GetId() << "]";
    }
//    else if( value.value().has_objectknowledge() )
//        ss << value.value().objectknowledge(); // $$$$ SBO 2007-08-27: resolve...
//    else if( value.value().has_crowdknowledge() )
//        ss << value.value().crowdknowledge(); // $$$$ SBO 2007-08-27: resolve...
//    else if( value.value().has_equipmenttype() )
//        ss << value.value().equipmenttype(); // $$$$ SBO 2007-08-27: resolve...
//    else if( value.value().has_dotationtype() )
//        ss << value.value().dotationtype(); // $$$$ SBO 2007-08-27: resolve...
//    else if( value.value().has_indirectfire() )
//        ss << value.value().indirectfire();
//    else if( value.value().has_acharstr() )
//        ss << value.value().acharstr();
    else
        return "[unhandled]";
    return ss.str();
}
