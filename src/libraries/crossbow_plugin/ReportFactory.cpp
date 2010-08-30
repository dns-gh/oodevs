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
    for( CIT_Templates it = templates_.begin(); it != templates_.end(); ++it )
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
std::string ReportFactory::CreateMessage( const MsgsSimToClient::MsgReport& message ) const
{
    CIT_Templates it;
    if( message.cr().has_automat() )
        CIT_Templates it = templates_.find( message.cr().automat().id() );
    else if( message.cr().has_population() )
        CIT_Templates it = templates_.find( message.cr().population().id() );
    else if( message.cr().has_unit() )
        CIT_Templates it = templates_.find( message.cr().unit().id() );
    else if( message.cr().has_formation() )
        CIT_Templates it = templates_.find( message.cr().formation().id() );
    else
        return "Unknown report";
    return it->second->RenderMessage( message.parametres() );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::RenderParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
std::string ReportFactory::RenderParameter( const Common::MsgMissionParameter& value ) const
{
    std::stringstream ss;

    if (value.value().has_areal() )
        ss << value.value().areal();
    else if (value.value().has_unit() )
    {
        if( const dispatcher::Agent_ABC* agent = model_.Agents().Find( value.value().unit().id() ) )
            ss << agent->GetName().ascii() << " [" << agent->GetId() << "]";
    }
    else if (value.value().has_unitknowledge() )
    {
        if( const dispatcher::AgentKnowledge_ABC* knowledge = model_.AgentKnowledges().Find( value.value().unitknowledge().id() ) )
            ss << knowledge->GetName().ascii() << " [" << knowledge->GetId() << "]";
    }
//    else if( value.value().has_objectknowledge() )
//        ss << value.value().objectknowledge(); // $$$$ SBO 2007-08-27: resolve...
//    else if( value.value().has_populationknowledge() )
//        ss << value.value().populationknowledge(); // $$$$ SBO 2007-08-27: resolve...
//    else if( value.value().has_equipmenttype() )
//        ss << value.value().equipmenttype(); // $$$$ SBO 2007-08-27: resolve...
//    else if( value.value().has_dotationtype() )
//        ss << value.value().dotationtype(); // $$$$ SBO 2007-08-27: resolve...
//    else if( value.value().has_tirindirect() )
//        ss << value.value().tirindirect();
//    else if( value.value().has_acharstr() )
//        ss << value.value().acharstr();
    else
        return "[unhandled]";
    return ss.str();
}
