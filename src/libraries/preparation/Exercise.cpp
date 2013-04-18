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
#include "tools/XmlCrc32Signature.h"
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: Exercise constructor
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
Exercise::Exercise( kernel::Controller& controller )
    : controller_    ( controller )
    , actionPlanning_( "" )
    , settings_      ()
    , needSaving_( false )
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
// Name: Exercise::ReadAllowedSicVersions
// Created: JSR 2013-02-25
// -----------------------------------------------------------------------------
bool Exercise::ReadAllowedSicVersions()
{
    allowedSicVersions_[ eSicAtlas ].clear();
    allowedSicVersions_[ eSicDQPManager ].clear();
    allowedSicVersions_[ eSicSICF ].clear();
    allowedSicVersions_[ eSicSIR ].clear();

    const std::string sicVersionFile = tools::GeneralConfig::BuildResourceChildFile( "sicVersion.xml" );
    if( bfs::exists( sicVersionFile ) )
    {
        xml::xifstream xis( sicVersionFile );
        xis >> xml::start( "sic-version" )
                >> xml::list( *this, &Exercise::ReadSicVersions )
            >> xml::end;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Exercise::ReadSicVersions
// Created: JSR 2013-02-25
// -----------------------------------------------------------------------------
void Exercise::ReadSicVersions( const std::string& tag, xml::xistream& xis )
{
    xis >> xml::list( "version", *this, &Exercise::ReadSicVersion, tag );
}

namespace
{
    Exercise::E_Sics ConvertFromString( const std::string& sic )
    {
        if( sic == "atlas" )
            return Exercise::eSicAtlas;
        if( sic == "dqp-manager" )
            return Exercise::eSicDQPManager;
        if( sic == "sicf" )
            return Exercise::eSicSICF;
        if( sic == "sir" )
            return Exercise::eSicSIR;
        return Exercise::eSicNbr;
    }
}

// -----------------------------------------------------------------------------
// Name: Exercise::ReadSicVersion
// Created: JSR 2013-02-26
// -----------------------------------------------------------------------------
void Exercise::ReadSicVersion( xml::xistream& xis, const std::string& strSic )
{
    E_Sics sic = ConvertFromString( strSic );
    if( sic < eSicNbr )
        allowedSicVersions_[ sic ].push_back( T_AllowedSicVersion( xis.attribute< int >( "revision" ), xis.attribute< std::string >( "name" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Exercise::Load
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
void Exercise::Load( xml::xistream& xis )
{
    needSaving_ = false;
    bool sicPresent = ReadAllowedSicVersions();
    sicVersions_[ eSicAtlas ].clear();
    sicVersions_[ eSicDQPManager ].clear();
    sicVersions_[ eSicSICF ].clear();
    sicVersions_[ eSicSIR ].clear();
    std::string name;
    xis >> xml::start( "exercise" )
            >> xml::optional >> xml::start( "meta" )
                >> xml::optional >> xml::start( "briefing" )
                    >> xml::list( "text", *this, &Exercise::ReadBriefing )
                >> xml::end // end briefing
                >> xml::optional >> xml::start( "resources" )
                    >> xml::list( "resource", *this, &Exercise::ReadResource )
                >> xml::end // end resources
            >> xml::end // end meta
            >> xml::optional >> xml::start( "action-planning" )
                >> xml::attribute( "file", actionPlanning_ )
            >> xml::end; // end action-planning
    if( sicPresent )
        xis >> xml::optional >> xml::start( "sic" )
                >> xml::attribute( "atlas", sicVersions_[ eSicAtlas ] )
                >> xml::attribute( "dqp-manager", sicVersions_[ eSicDQPManager ] )
                >> xml::attribute( "sicf", sicVersions_[ eSicSICF ] )
                >> xml::attribute( "sir", sicVersions_[ eSicSIR ] )
            >> xml::end; // sic
    xis >> xml::end; // end exercise

    if( sicPresent )
    {
        CheckSic( eSicAtlas );
        CheckSic( eSicDQPManager );
        CheckSic( eSicSICF );
        CheckSic( eSicSIR );
    }
    name_ = name.c_str();
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::CheckSic
// Created: JSR 2013-02-26
// -----------------------------------------------------------------------------
void Exercise::CheckSic( E_Sics sic )
{
    const T_AllowedSicVersions& sicVersions = allowedSicVersions_[ sic ];
    if( sicVersions.empty() )
        return;
    std::string defaultName;
    unsigned int revision = std::numeric_limits< unsigned int >::max();
    for( int i = 0; i < sicVersions.size(); ++i )
    {
        const T_AllowedSicVersion& sicVersion = sicVersions[ i ];
        if( sicVersion.name_ == sicVersions_[ sic ] )
            return;
        if( sicVersion.revision_ < revision )
        {
            revision = sicVersion.revision_;
            defaultName = sicVersion.name_;
        }
    }
    sicVersions_[ sic ] = defaultName;
    needSaving_ = true;
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
    briefings_[lang.c_str()] = text.c_str();
}

// -----------------------------------------------------------------------------
// Name: Exercise::ReadResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::ReadResource( xml::xistream& xis )
{
    std::string name, file;
    xis >> xml::attribute( "name", name )
        >> xml::attribute( "file", file );
    resources_[name.c_str()] = file.c_str();
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
    actionPlanning_.clear();
    settings_.Purge();
    controller_.Update( *this );
}

namespace
{
    void CopyNode( const std::string& name, xml::xistream& xis, xml::xostream& xos )
    {
        if( name != "meta" && name != "action-planning" && name != "settings" && name != "drawings" && name != "terrain" && name != "sic" )
            xos << xml::content( name, xis );
    }
    void CopyFromFile( const std::string& file, xml::xostream& xos )
    {
        xml::xifstream xis( file );
        xis >> xml::start( "exercise" )
                >> xml::list( boost::bind( &CopyNode, _2, _3, boost::ref( xos ) ) )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: Exercise::SerializeAndSign
// Created: SBO 2010-03-08
// -----------------------------------------------------------------------------
void Exercise::SerializeAndSign( const tools::ExerciseConfig& config, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    std::string file = config.GetExerciseFile();
    xml::xofstream xos( file, xml::encoding( "UTF-8" ) );
    xos << xml::start( "exercise" );
    schemaWriter.WriteExerciseSchema( xos, "exercise" );
    xos << xml::start( "meta" );
    SerializeBriefings( xos );
    SerializeResources( xos );
    xos     << xml::end;
    CopyFromFile( file, xos );
    xos << xml::start( "terrain" ) << xml::attribute( "name", config.GetTerrainName() ) << xml::end;
    xos << xml::start( "drawings" ) << xml::attribute( "file", config.GetDrawingsFileName() ) << xml::end;
    if( !actionPlanning_.empty() )
        xos << xml::start( "action-planning" ) << xml::attribute( "file", actionPlanning_ ) << xml::end;
    if( !sicVersions_[ eSicAtlas ].empty() || !sicVersions_[ eSicDQPManager ].empty() ||
        !sicVersions_[ eSicSICF ].empty() || !sicVersions_[ eSicSIR ].empty() )
        xos << xml::start( "sic" )
                << xml::attribute( "atlas", sicVersions_[ eSicAtlas ] )
                << xml::attribute( "dqp_manager", sicVersions_[ eSicDQPManager ] )
                << xml::attribute( "sicf", sicVersions_[ eSicSICF ] )
                << xml::attribute( "sir", sicVersions_[ eSicSIR ] )
            << xml::end; // sic
    xos << xml::end;
    tools::WriteXmlCrc32Signature( file );
}

// -----------------------------------------------------------------------------
// Name: Exercise::NeedsSaving
// Created: JSR 2013-02-26
// -----------------------------------------------------------------------------
bool Exercise::NeedsSaving() const
{
    return needSaving_;
}

// -----------------------------------------------------------------------------
// Name: Exercise::SerializeBriefings
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::SerializeBriefings( xml::xostream& xos ) const
{
    xos << xml::start( "briefing" );
    for( T_Resources::const_iterator it = briefings_.begin(); it != briefings_.end(); ++it )
        if( !it->second.isEmpty() )
            xos << xml::start( "text" )
                    << xml::attribute( "lang", it->first.toAscii().constData() )
                    << xml::cdata( it->second.toAscii().constData() )
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
    for( T_Resources::const_iterator it = resources_.begin(); it != resources_.end(); ++it )
        xos << xml::start( "resource" )
                << xml::attribute( "name", it->first.toAscii().constData() )
                << xml::attribute( "file", it->second.toAscii().constData() )
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
    briefings_[lang] = text;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::AddResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::AddResource( const QString& name, const QString& file )
{
    resources_[name] = file;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Exercise::SetActionPlanning
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Exercise::SetActionPlanning( const std::string& filename )
{
    actionPlanning_ = filename;
}

// -----------------------------------------------------------------------------
// Name: Exercise::GetActionPlanning
// Created: LDC 2012-09-21
// -----------------------------------------------------------------------------
std::string Exercise::GetActionPlanning() const
{
    return actionPlanning_;
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
// Name: Exercise::Accept
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void Exercise::Accept( ExerciseVisitor_ABC& visitor ) const
{
    for( T_Resources::const_iterator it = briefings_.begin(); it != briefings_.end(); ++it )
        visitor.VisitBriefing( it->first, it->second );
    for( T_Resources::const_iterator it = resources_.begin(); it != resources_.end(); ++it )
        visitor.VisitResource( it->first, it->second );
    AcceptSic( visitor, eSicSICF );
    AcceptSic( visitor, eSicSIR );
    AcceptSic( visitor, eSicAtlas );
    AcceptSic( visitor, eSicDQPManager );
}

// -----------------------------------------------------------------------------
// Name: Exercise::AcceptSic
// Created: JSR 2013-02-26
// -----------------------------------------------------------------------------
void Exercise::AcceptSic( ExerciseVisitor_ABC& visitor, E_Sics sic ) const
{
    const T_AllowedSicVersions& sicVersion = allowedSicVersions_[ sic ];
    for( auto it = sicVersion.begin(); it != sicVersion.end(); ++it )
        visitor.VisitSicVersion( sic, it->revision_, it->name_, it->name_ == sicVersions_[ sic ] );
}

// -----------------------------------------------------------------------------
// Name: Exercise::SetSicVersion
// Created: JSR 2013-02-26
// -----------------------------------------------------------------------------
void Exercise::SetSicVersion( E_Sics sic, unsigned int revision )
{
    const T_AllowedSicVersions& sicVersion = allowedSicVersions_[ sic ];
    for( auto it = sicVersion.begin(); it != sicVersion.end(); ++it )
    {
        if( it->revision_ == revision )
        {
            sicVersions_[ sic ] = it->name_;
            return;
        }
    }
}
