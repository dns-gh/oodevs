// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ReportFactory.h"
#include "ReportTemplate.h"
#include "Trace.h"
#include "RcEntityResolver_ABC.h"
#include "Agent.h"
#include "AgentKnowledge_ABC.h"
#include "ObjectKnowledge_ABC.h"
#include "PopulationKnowledge_ABC.h"
#include "Tools.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/ExerciseConfig.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ReportFactory constructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportFactory::ReportFactory( const RcEntityResolver_ABC& rcResolver
                            , const Resolver_ABC< DotationType >&  dotationResolver
                            , const Resolver_ABC< EquipmentType >& equipmentResolver )
    : rcResolver_       ( rcResolver )
    , dotationResolver_ ( dotationResolver )
    , equipmentResolver_( equipmentResolver )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ReportFactory destructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportFactory::~ReportFactory()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::Load
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportFactory::Load( const kernel::ExerciseConfig& config  )
{
    xml::xifstream scipio( config.GetPhysicalFile() );
    std::string reports;
    scipio >> start( "physical" )
                >> content( "ComptesRendus", reports );
    xifstream xis( config.BuildPhysicalChildFile( reports ) );
    xis >> start( "reports" )
            >> list( "report", *this, &ReportFactory::ReadReport )
        >> end();
}
    
// -----------------------------------------------------------------------------
// Name: ReportFactory::Purge
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportFactory::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::ReadReport
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportFactory::ReadReport( xml::xistream& xis )
{
    ReportTemplate* report = new ReportTemplate( xis, *this );
    Register( report->GetId(), *report );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateReport
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
Report* ReportFactory::CreateReport( const kernel::Entity_ABC& agent, const Simulation& simulation, const ASN1T_MsgReport& asn ) const
{
    ReportTemplate* report = Find( asn.cr );
    if( !report )
        return 0;
    Report::E_Type type = Report::eRC;
    if( asn.type == EnumReportType::message )
        type = Report::eMessage;
    else if( asn.type == EnumReportType::evenement_exceptionnel )
        type = Report::eEvent;
    else if( asn.type == EnumReportType::warning )
        type = Report::eWarning;
    return new Report( agent, simulation, type, report->RenderMessage( asn.parametres ) );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateTrace
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
Report* ReportFactory::CreateTrace( const kernel::Entity_ABC& agent, const Simulation& simulation, const ASN1T_MsgTrace& message ) const
{
    return new Trace( agent, simulation, message );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::RenderParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
QString ReportFactory::RenderParameter( const ASN1T_MissionParameter& value ) const
{
    switch( value.value.t )
    {
    case T_MissionParameter_value_aReal:
        return QString::number( value.value.u.aReal );
    case T_MissionParameter_value_unit:
        return rcResolver_.CreateLink( Agent::typeName_, value.value.u.unit );
    case T_MissionParameter_value_unitKnowledge:
        return rcResolver_.CreateLink( AgentKnowledge_ABC::typeName_, value.value.u.unitKnowledge );
    case T_MissionParameter_value_objectKnowledge:
        return rcResolver_.CreateLink( ObjectKnowledge_ABC::typeName_, value.value.u.objectKnowledge );
    case T_MissionParameter_value_populationKnowledge:
        return rcResolver_.CreateLink( PopulationKnowledge_ABC::typeName_, value.value.u.populationKnowledge );
    case T_MissionParameter_value_equipmentType:
        return equipmentResolver_.Get( value.value.u.equipmentType ).GetName();
    case T_MissionParameter_value_dotationType:
        return dotationResolver_.Get( value.value.u.dotationType ).GetCategory();
    case T_MissionParameter_value_tirIndirect:
        return QString::number( value.value.u.tirIndirect );
    case T_MissionParameter_value_aCharStr:
        return QString( value.value.u.aCharStr );
    default:
        throw std::runtime_error( tools::translate( "ReportFactory", "Unhandled report parameter type: '%1'." ).arg( value.value.t ).ascii() );
    }
    return "";
}
