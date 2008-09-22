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
#include "Tools.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "tools/ExerciseConfig.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ReportFactory constructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportFactory::ReportFactory( const RcEntityResolver_ABC& rcResolver
                            , const Resolver_ABC< DotationType >&  dotationResolver
                            , const Resolver_ABC< EquipmentType >& equipmentResolver
                            , const Simulation& simulation )
    : rcResolver_       ( rcResolver )
    , dotationResolver_ ( dotationResolver )
    , equipmentResolver_( equipmentResolver )
    , simulation_       ( simulation )
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
void ReportFactory::Load( const tools::ExerciseConfig& config  )
{
    xml::xifstream scipio( config.GetPhysicalFile() );
    std::string reports;
    scipio >> start( "physical" )
                >> start( "reports" )
                >> attribute( "file", reports );
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

namespace
{
    // $$$$ AGE 2007-10-19: caca et ^c^v
    QDateTime GetTime( const ASN1T_DateTime& d )
    {
        const std::string date( (const char*)d.data, d.numocts );
        QString extended( date.c_str() );
        extended.insert( 13, ':' ); extended.insert( 11, ':' ); 
        extended.insert(  6, '-' ); extended.insert(  4, '-' );
        return QDateTime::fromString( extended, Qt::ISODate );
    }
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateReport
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
Report* ReportFactory::CreateReport( const kernel::Entity_ABC& agent, const ASN1T_MsgReport& asn ) const
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
    return new Report( agent, type, report->RenderMessage( asn.parametres ), GetTime( asn.time ) );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateTrace
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
Report* ReportFactory::CreateTrace( const kernel::Entity_ABC& agent, const ASN1T_MsgTrace& message ) const
{
    return new Trace( agent, simulation_, message );
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
        return equipmentResolver_.Get( value.value.u.equipmentType ).GetName().c_str();
    case T_MissionParameter_value_dotationType:
        return dotationResolver_.Get( value.value.u.dotationType ).GetCategory().c_str();
    case T_MissionParameter_value_tirIndirect:
        return QString::number( value.value.u.tirIndirect );
    case T_MissionParameter_value_aCharStr:
        return QString( value.value.u.aCharStr );
    default:
        throw std::runtime_error( tools::translate( "ReportFactory", "Unhandled report parameter type: '%1'." ).arg( value.value.t ).ascii() );
    }
    return "";
}
