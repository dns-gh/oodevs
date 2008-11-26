// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "ReportFactory.h"
#include "ReportTemplate.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/AgentKnowledge.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace plugins::gearth;

// -----------------------------------------------------------------------------
// Name: ReportFactory constructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportFactory::ReportFactory( const dispatcher::Config& config, const dispatcher::Model& model )
    : model_( model )
{
    xml::xifstream main( config.GetPhysicalFile() );
    std::string reports;
    main >> start( "physical" )
            >> start( "reports" )
            >> attribute( "file", reports );
    xifstream xis( config.BuildPhysicalChildFile( reports ) );
    xis >> start( "reports" )
            >> list( "report", *this, &ReportFactory::ReadReport )
        >> end();
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
std::string ReportFactory::CreateMessage( const ASN1T_MsgReport& asn ) const
{
    CIT_Templates it = templates_.find( asn.cr );
    if( it == templates_.end() )
        return "Unknown report";
    return it->second->RenderMessage( asn.parametres );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::RenderParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
std::string ReportFactory::RenderParameter( const ASN1T_MissionParameter& value ) const
{
    std::stringstream ss;
    switch( value.value.t )
    {
    case T_MissionParameter_value_aReal:
        ss << value.value.u.aReal;
        break;
    case T_MissionParameter_value_unit:
        {
            if( const dispatcher::Agent* agent = model_.agents_.Find( value.value.u.unit ) )
                ss << agent->GetName().ascii() << " [" << agent->GetId() << "]";
            break;
        }
    case T_MissionParameter_value_unitKnowledge:
        {
            if( const dispatcher::AgentKnowledge* knowledge = model_.agentKnowledges_.Find( value.value.u.unitKnowledge ) )
                ss << knowledge->GetName().ascii() << " [" << knowledge->GetId() << "]";
            break;
        }
    case T_MissionParameter_value_objectKnowledge:
        ss << value.value.u.objectKnowledge; // $$$$ SBO 2007-08-27: resolve...
        break;
    case T_MissionParameter_value_populationKnowledge:
        ss << value.value.u.populationKnowledge; // $$$$ SBO 2007-08-27: resolve...
        break;
    case T_MissionParameter_value_equipmentType:
        ss << value.value.u.equipmentType; // $$$$ SBO 2007-08-27: resolve...
        break;
    case T_MissionParameter_value_dotationType:
        ss << value.value.u.dotationType; // $$$$ SBO 2007-08-27: resolve...
        break;
    case T_MissionParameter_value_tirIndirect:
        ss << value.value.u.tirIndirect;
    case T_MissionParameter_value_aCharStr:
        ss << value.value.u.aCharStr;
    default:
        return "[unhandled]";
    }
    return ss.str();
}
