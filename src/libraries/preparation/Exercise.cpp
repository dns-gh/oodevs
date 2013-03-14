// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Exercise.h"
#include "clients_kernel/Controller.h"
#include "tools/ExerciseConfig.h"
#include "tools/SchemaWriter_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>


// -----------------------------------------------------------------------------
// Name: Exercise constructor
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
Exercise::Exercise( kernel::Controller& controller )
    : controller_    ( controller )
    , actionPlanning_( "" )
    , isValid_       ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Exercise destructor
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
Exercise::~Exercise()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Exercise::Load
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
void Exercise::Load( xml::xistream& xis )
{
    std::string name;
    xis >> xml::start( "exercise" )
            >> xml::optional >> xml::start( "meta" )
                >> xml::optional >> xml::start( "briefing" )
                    >> xml::list( "text", *this, &Exercise::ReadBriefing )
                >> xml::end // end briefing
                >> xml::optional >> xml::start( "resources" )
                    >> xml::list( "resource", *this, &Exercise::ReadResource )
                >> xml::end // end resources
                >> xml::optional >> xml::start( "orders" )
                    >> xml::list( "order", *this, &Exercise::ReadOrderFile )
                >> xml::end // end orders
            >> xml::end // end meta
            >> xml::optional >> xml::start( "action-planning" )
                >> xml::attribute< tools::Path >( "file", actionPlanning_ )
            >> xml::end // end action-planning
        >> xml::end; // end exercise

    name_ = name.c_str();
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::ReadBriefing
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::ReadBriefing( xml::xistream& xis )
{
    std::string lang, text;
    xis >> xml::attribute( "lang", lang )
        >> text;
    briefings_[ lang.c_str() ] = text.c_str();
}

// -----------------------------------------------------------------------------
// Name: Exercise::ReadResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::ReadResource( xml::xistream& xis )
{
    std::string name;
    tools::Path file;
    xis >> xml::attribute( "name", name )
        >> xml::attribute( "file", file );
    resources_[ name.c_str() ] = file;
}

// -----------------------------------------------------------------------------
// Name: Exercise::ReadOrderFile
// Created: JSR 2012-03-01
// -----------------------------------------------------------------------------
void Exercise::ReadOrderFile( xml::xistream& xis )
{
    const tools::Path file = xis.attribute< tools::Path >( "file" );
    orderFiles_.push_back( file );
}

// -----------------------------------------------------------------------------
// Name: Exercise::Purge
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
void Exercise::Purge()
{
    name_ = "";
    briefings_.clear();
    resources_.clear();
    orderFiles_.clear();
    actionPlanning_.Clear();
    isValid_ = true;
    settings_.Purge();
    controller_.Update( *this );
}

namespace
{
    void CopyNode( const std::string& name, xml::xistream& xis, xml::xostream& xos )
    {
        if( name != "meta" && name != "action-planning" && name != "settings" && name != "terrain" && name != "urbanstate" && name != "urban" )
            xos << xml::content( name, xis );
    }
    void CopyFromFile( const tools::Path& file, xml::xostream& xos )
    {
        tools::Xifstream xis( file );
        xis >> xml::start( "exercise" )
                >> xml::list( boost::bind( &CopyNode, _2, _3, boost::ref( xos ) ) )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: Exercise::Serialize
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
void Exercise::Serialize( const tools::ExerciseConfig& config, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    tools::Path file = config.GetExerciseFile();
    tools::Xofstream xos( file );
    xos << xml::start( "exercise" );
    schemaWriter.WriteExerciseSchema( xos, "exercise" );
    xos << xml::attribute( "valid", isValid_ )
        << xml::start( "meta" );
    SerializeBriefings( xos );
    SerializeResources( xos );
    SerializeOrderFiles( xos );
    xos     << xml::end;
    CopyFromFile( file, xos );
    xos << xml::start( "urban" )
            << xml::attribute( "file", config.GetUrbanFileName() )
        << xml::end
        << xml::start( "settings" )
            << xml::attribute( "file", config.GetSettingsFileName() )
        << xml::end
        << xml::start( "terrain" )
            << xml::attribute( "name", config.GetTerrainName() )
        << xml::end;
    if( !actionPlanning_.IsEmpty() )
        xos << xml::start( "action-planning" ) << xml::attribute( "file", actionPlanning_ ) << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Exercise::SerializeBriefings
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::SerializeBriefings( xml::xostream& xos ) const
{
    xos << xml::start( "briefing" );
    for( auto it = briefings_.begin(); it != briefings_.end(); ++it )
        if( !it->second.isEmpty() )
            xos << xml::start( "text" )
                    << xml::attribute( "lang", it->first.toStdString() )
                    << xml::cdata( it->second.toStdString() )
                << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Exercise::SerializeResources
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::SerializeResources( xml::xostream& xos ) const
{
    xos << xml::start( "resources" );
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
        xos << xml::start( "resource" )
                << xml::attribute( "name", it->first.toStdString() )
                << xml::attribute( "file", it->second )
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Exercise::SerializeOrderFiles
// Created: JSR 2012-03-01
// -----------------------------------------------------------------------------
void Exercise::SerializeOrderFiles( xml::xostream& xos ) const
{
    if( orderFiles_.empty() )
        return;
    xos << xml::start( "orders" );
    for( auto it = orderFiles_.begin(); it != orderFiles_.end(); ++it )
        xos << xml::start( "order" )
                << xml::attribute( "file", *it )
            << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Exercise::GetSettings
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
tools::ExerciseSettings& Exercise::GetSettings()
{
    return settings_;
}

// -----------------------------------------------------------------------------
// Name: Exercise::GetName
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
QString Exercise::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Exercise::SetName
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::SetName( const QString& name )
{
    name_ = name;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::SetBriefing
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::SetBriefing( const QString& lang, const QString& text )
{
    briefings_[ lang ] = text;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::AddResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::AddResource( const QString& name, const tools::Path& file )
{
    resources_[ name ] = file;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::AddOrderFile
// Created: JSR 2012-03-01
// -----------------------------------------------------------------------------
void Exercise::AddOrderFile( const tools::Path& file )
{
    orderFiles_.push_back( file );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::SetActionPlanning
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Exercise::SetActionPlanning( const tools::Path& filename )
{
    if( actionPlanning_.IsEmpty() )
        actionPlanning_ = filename;
}

// -----------------------------------------------------------------------------
// Name: Exercise::SetExerciseValidity
// Created: JSR 2012-01-09
// -----------------------------------------------------------------------------
void Exercise::SetExerciseValidity( bool isValid )
{
    isValid_ = isValid;
}

// -----------------------------------------------------------------------------
// Name: Exercise::ClearResources
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::ClearResources()
{
    resources_.clear();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::ClearOrderFiles
// Created: JSR 2012-03-01
// -----------------------------------------------------------------------------
void Exercise::ClearOrderFiles()
{
    orderFiles_.clear();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::Accept
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::Accept( ExerciseVisitor_ABC& visitor ) const
{
    for( auto it = briefings_.begin(); it != briefings_.end(); ++it )
        visitor.VisitBriefing( it->first, it->second );
    for( auto it = resources_.begin(); it != resources_.end(); ++it )
        visitor.VisitResource( it->first, it->second );
    for( auto it = orderFiles_.begin(); it != orderFiles_.end(); ++it )
        visitor.VisitOrderFile( *it );
}
