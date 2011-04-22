// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ReportFactory.h"
#include "ReportTemplate.h"
#include "Trace.h"
#include "RcEntityResolver_ABC.h"
#include "Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "protocol/Protocol.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#pragma warning( pop )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ReportFactory constructor
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
ReportFactory::ReportFactory( const RcEntityResolver_ABC& rcResolver
                            , const tools::Resolver_ABC< DotationType >&  dotationResolver
                            , const tools::Resolver_ABC< EquipmentType >& equipmentResolver
                            , const kernel::Time_ABC* time )
    : rcResolver_       ( rcResolver )
    , dotationResolver_ ( dotationResolver )
    , equipmentResolver_( equipmentResolver )
    , time_             ( time )
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
    scipio >> xml::start( "physical" )
                >> xml::start( "reports" )
                    >> xml::attribute( "file", reports );
    xml::xifstream xis( config.BuildPhysicalChildFile( reports ) );
    xis >> xml::start( "reports" )
            >> xml::list( "report", *this, &ReportFactory::ReadReport )
        >> xml::end;
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
// Name: ReportFactory::GetTime
// Created: LDC 2010-03-18
// $$$$ AGE 2007-10-19: caca et ^c^v
// -----------------------------------------------------------------------------
QDateTime ReportFactory::GetTime( const sword::DateTime& d ) const
{
    const std::string date( (const char*)d.data().c_str() );
    QString extended( date.c_str() );
    extended.insert( 13, ':' ); extended.insert( 11, ':' );
    extended.insert(  6, '-' ); extended.insert(  4, '-' );
    return QDateTime::fromString( extended, Qt::ISODate );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateReport
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
Report* ReportFactory::CreateReport( const kernel::Entity_ABC& agent, const sword::Report& message ) const
{
    ReportTemplate* report = Find( message.type().id() );
    if( !report )
        return 0;
    Report::E_Type type = Report::eRC;
    if( message.category() == sword::Report::information )
        type = Report::eMessage;
    else if( message.category() == sword::Report::exceptional_event )
        type = Report::eEvent;
    else if( message.category() == sword::Report::warning )
        type = Report::eWarning;
    return new Report( agent, type, report->RenderMessage( message ), GetTime( message.time() ) );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::FormatReport
// Created: LDC 2010-03-17
// -----------------------------------------------------------------------------
std::string ReportFactory::FormatReport( const sword::Report& message ) const
{
    ReportTemplate* report = Find( message.type().id() );
    if( report )
        return report->RenderMessage( message ).ascii();
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateTrace
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
Report* ReportFactory::CreateTrace( const kernel::Entity_ABC& agent, const sword::Trace& message ) const
{
    if( !time_ )
        throw std::runtime_error( "No time, can't generate trace" );
    return new Trace( agent, *time_, message );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::RenderParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
QString ReportFactory::RenderParameter( const sword::MissionParameter_Value& value ) const
{
    if( value.has_areal() )
        return QString::number( value.areal() );
    if( value.has_agent() )
        return rcResolver_.CreateLink( Agent_ABC::typeName_, value.agent().id() );
    if( value.has_automat() )
        return rcResolver_.CreateLink( Automat_ABC::typeName_, value.automat().id() );
    if( value.has_agentknowledge() )
        return rcResolver_.CreateLink( AgentKnowledge_ABC::typeName_, value.agentknowledge().id() );
    if( value.has_objectknowledge() )
        return rcResolver_.CreateLink( ObjectKnowledge_ABC::typeName_, value.objectknowledge().id() );
    if( value.has_crowdknowledge() )
        return rcResolver_.CreateLink( PopulationKnowledge_ABC::typeName_, value.crowdknowledge().id() );
    if( value.has_equipmenttype() )
        return equipmentResolver_.Get( value.equipmenttype().id() ).GetName().c_str();
    if( value.has_resourcetype() )
        return dotationResolver_.Get( value.resourcetype().id() ).GetName().c_str();
    if( value.has_indirectfire() )
        return QString::number( value.indirectfire().id() );
    if( value.has_acharstr() )
        return QString( value.acharstr().c_str() );
    throw std::runtime_error( tools::translate( "ReportFactory", "Unhandled report parameter type: '%1'." ).arg( value.GetDescriptor()->full_name().c_str() ).ascii() );
}
