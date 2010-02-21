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
#include <xeumeuleu/xml.h>
#include <google/protobuf/Descriptor.h>

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ReportFactory constructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportFactory::ReportFactory( const RcEntityResolver_ABC& rcResolver
                            , const tools::Resolver_ABC< DotationType >&  dotationResolver
                            , const tools::Resolver_ABC< EquipmentType >& equipmentResolver
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
    QDateTime GetTime( const Common::MsgDateTime& d )
    {
        const std::string date( (const char*)d.data().c_str() );
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
Report* ReportFactory::CreateReport( const kernel::Entity_ABC& agent, const MsgsSimToClient::MsgReport& message ) const
{
    ReportTemplate* report = Find( message.cr() );
    if( !report )
        return 0;
    Report::E_Type type = Report::eRC;
    if( message.type() == MsgsSimToClient::EnumReportType::message )
        type = Report::eMessage;
    else if( message.type() == MsgsSimToClient::EnumReportType::evenement_exceptionnel )
        type = Report::eEvent;
    else if( message.type() == MsgsSimToClient::EnumReportType::warning )
        type = Report::eWarning;
    return new Report( agent, type, report->RenderMessage( message.parametres() ), GetTime( message.time() ) );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateTrace
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
Report* ReportFactory::CreateTrace( const kernel::Entity_ABC& agent, const MsgsSimToClient::MsgTrace& message ) const
{
    return new Trace( agent, simulation_, message );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::RenderParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
QString ReportFactory::RenderParameter( const Common::MsgMissionParameter& value ) const
{
    if( value.has_value() )
    {
        if( value.value().has_areal() )
            return QString::number( value.value().areal() );
        if( value.value().has_unit() )
            return rcResolver_.CreateLink( Agent::typeName_, value.value().unit().oid() );
        if( value.value().has_unitknowledge() )
            return rcResolver_.CreateLink( AgentKnowledge_ABC::typeName_, value.value().unitknowledge().oid() );
        if( value.value().has_objectknowledge() )
            return rcResolver_.CreateLink( ObjectKnowledge_ABC::typeName_, value.value().objectknowledge().oid() );
        if( value.value().has_populationknowledge() )
            return rcResolver_.CreateLink( PopulationKnowledge_ABC::typeName_, value.value().populationknowledge().oid() );
        if( value.value().has_equipmenttype() )
            return equipmentResolver_.Get( value.value().equipmenttype().equipment() ).GetName().c_str();
        if( value.value().has_dotationtype() )
            return dotationResolver_.Get( value.value().dotationtype().oid() ).GetCategory().c_str();
        if( value.value().has_tirindirect() )
            return QString::number( value.value().tirindirect() );
        if( value.value().has_acharstr() )
            return QString( value.value().acharstr().c_str() );
    }
    throw std::runtime_error( tools::translate( "ReportFactory", "Unhandled report parameter type: '%1'." ).arg( value.GetDescriptor()->full_name().c_str() ).ascii() );
    return "";
}
