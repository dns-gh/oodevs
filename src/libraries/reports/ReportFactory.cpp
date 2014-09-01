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
#include "Stages.h"
#include "Trace.h"
#include "RcEntityResolver_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Tools.h"
#include "protocol/Helpers.h"
#include "protocol/Simulation.h"
#include "tools/DefaultLoader.h"
#include "tools/ExerciseConfig.h"
#include "tools/FileWrapper.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/XmlStreamOperators.h"
#include <tools/Path.h>
#include <tools/Exception.h>
#include <xeumeuleu/xml.hpp>
#include <boost/optional.hpp>
#include <boost/smart_ptr/make_shared.hpp>
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
    , stages_           ( new Stages() )
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
    stages_->Load( config );
    const tools::Path reports = config.GetOptionalPhysicalChildFile( "reports" );
    if( reports.IsEmpty() )
        throw MASA_EXCEPTION( "cannot load reports file: " + reports.ToUTF8() );
    tools::NullFileLoaderObserver observer;
    tools::DefaultLoader loader( observer );
    std::unique_ptr< xml::xistream > xis = loader.LoadFile( reports );
    *xis >> xml::start( "reports" )
            >> xml::list( "report", *this, &ReportFactory::ReadReport );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::Purge
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
void ReportFactory::Purge()
{
    stages_->Purge();
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
    QString date( d.data().c_str() );
    date.insert( 13, ':' );
    date.insert( 11, ':' );
    date.insert( 6, '-' );
    date.insert( 4, '-' );
    return QDateTime::fromString( date, Qt::ISODate );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateReport
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
boost::shared_ptr< Report > ReportFactory::CreateReport( const kernel::Entity_ABC& agent, const sword::Report& message ) const
{
    ReportTemplate* report = Find( message.type().id() );
    if( !report )
        return boost::shared_ptr< Report >();
    Report::E_Type type = Report::eRC;
    if( message.category() == sword::Report::information )
        type = Report::eMessage;
    else if( message.category() == sword::Report::exceptional_event )
        type = Report::eEvent;
    else if( message.category() == sword::Report::warning )
        type = Report::eWarning;
    return boost::make_shared< Report >( agent, type, report->RenderMessage( message ), GetTime( message.time() ) );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::FormatReport
// Created: LDC 2010-03-17
// -----------------------------------------------------------------------------
std::string ReportFactory::FormatReport( const sword::Report& message ) const
{
    ReportTemplate* report = Find( message.type().id() );
    if( report )
        return report->RenderMessage( message ).toStdString();
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateTrace
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
Report* ReportFactory::CreateTrace( const kernel::Entity_ABC& agent, const sword::Trace& message ) const
{
    if( !time_ )
        throw MASA_EXCEPTION( "No time, can't generate trace" );
    return new Trace( agent, *time_, message );
}

namespace
{
    boost::optional< std::string > FindLimaType( protocol::mapping::PhaseLineType::value_type type )
    {
        const auto& map = protocol::mapping::PhaseLineType::data_;
        for( size_t i = 0; i < protocol::mapping::PhaseLineType::size_; ++i )
            if( map[i].type == type )
                return map[i].name;
        return boost::none;
    }
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::RenderParameter
// Created: SBO 2006-12-07
// -----------------------------------------------------------------------------
QString ReportFactory::RenderParameter( const sword::MissionParameter_Value& value ) const
{
    if( value.has_areal() )
        return QString::number( value.areal() );
    if( value.has_intvalue() )
        return QString::number( value.intvalue() );
    if( value.has_agent() )
        return rcResolver_.CreateLink( Agent_ABC::typeName_, value.agent().id() );
    if( value.has_automat() )
        return rcResolver_.CreateLink( Automat_ABC::typeName_, value.automat().id() );
    if( value.has_agentknowledge() )
        return rcResolver_.CreateLink( AgentKnowledge_ABC::typeName_, value.agentknowledge().id() );
    if( value.has_objectknowledge() )
        return rcResolver_.CreateLink( Object_ABC::typeName_, value.objectknowledge().id() );
    if( value.has_crowdknowledge() )
        return rcResolver_.CreateLink( Population_ABC::typeName_, value.crowdknowledge().id() );
    if( value.has_equipmenttype() )
        return equipmentResolver_.Get( value.equipmenttype().id() ).GetName().c_str();
    if( value.has_resourcetype() )
        return dotationResolver_.Get( value.resourcetype().id() ).GetName().c_str();
    if( value.has_indirectfire() )
        return QString::number( value.indirectfire().id() );
    if( value.has_acharstr() )
        return QString( value.acharstr().c_str() );
    if( value.has_stage() )
        return QString( stages_->FindTranslation( value.stage().c_str() ).c_str() );
    if( value.has_phase_line_function() )
    {
        auto limaType = FindLimaType( value.phase_line_function() );
        if( limaType )
            return QString::fromStdString( *limaType );
    }
    throw MASA_EXCEPTION( tools::translate( "ReportFactory", "Unhandled report parameter type: '%1'." ).arg( value.GetDescriptor()->full_name().c_str() ).toStdString() );
}
