// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "commands.h"
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <xeumeuleu/xml.hpp>

namespace fcmd = frontend::commands;

namespace
{

void DeleteDirectory( const tools::Path path, tools::Path::T_Paths& result )
{
    if( path.Exists() && path.IsDirectory() )
    {
        result.push_back( path );
        path.RemoveAll();
    }
}

void CheckForDirectories( tools::Path::T_Paths& result, const tools::Path::T_Paths& directories, const tools::Path& root, const tools::Path::T_Functor& validator )
{
    for( auto it = directories.begin(); it != directories.end(); ++it )
    {
        if( validator( *it ) )
            result.push_back( it->Relative( root ) );
        else if ( it->IsDirectory() )
            CheckForDirectories( result, it->ListDirectories( false, false ), root, validator );
    }
}

// Invokes callback on each subdirectory of rootDir, recursively. If callback,
// returns boost::none, keep iterating. Otherwise, stop recursing in current
// directory children and if it returned true, add it to the result entries
// set.
tools::Path::T_Paths ListDirectories( const tools::Path& rootDir,
        const std::function< boost::optional< bool >( const tools::Path& ) >& callback )
{
    tools::Path::T_Paths kept;
    rootDir.Walk( [&]( const tools::Path& p ) -> tools::WalkStatus
    {
        if( !p.IsDirectory() )
            return tools::WalkContinue;
        const auto res = callback( p );
        if( !res )
            return tools::WalkContinue;
        if( *res )
            kept.push_back( p.Relative( rootDir ) );
        return tools::WalkSkipDir;
    });
    return kept;
}

}  // namespace

tools::Path::T_Paths fcmd::ListTerrains( const tools::GeneralConfig& config )
{
    return ListDirectories( config.GetTerrainsDir(), []( const tools::Path& p )
        -> boost::optional< bool >
    {
        if( ( p / "terrain.xml" ).Exists() )
            return true;
        return boost::none;
    });
}

tools::Path::T_Paths fcmd::ListExercises( const tools::GeneralConfig& config,
        const tools::Path& subDir )
{
    const tools::Path baseDir = config.GetExercisesDir();
    if( baseDir.IsEmpty() )
        return tools::Path::T_Paths();
    return ListDirectories( baseDir / subDir, []( const tools::Path& dir )
        -> boost::optional< bool >
    {
        if( ( dir / "exercise.xml" ).Exists() )
            return true;
        return boost::none;
    });
}

tools::Path::T_Paths fcmd::ListSessions( const tools::GeneralConfig& config,
        const tools::Path& exercise, bool useValidator )
{
    return ListDirectories( config.GetSessionsDir( exercise ), [&]( const tools::Path& dir )
            -> boost::optional< bool >
    {
        if( !useValidator )
            return true;
        const auto record = dir / "record";
        if( !record.Exists() || !record.IsDirectory() )
            return false;

        for( auto it = record.begin(); it != record.end(); ++it )
        {
            const tools::Path child = *it;
            if( child.IsDirectory() &&
                ( child / "info" ).Exists() &&
                ( child / "index" ).Exists() &&
                ( child / "keyindex" ).Exists() &&
                ( child / "key" ).Exists() && 
                ( child / "update" ).Exists() )
                return true;
        }
        return false;
    });
}

namespace
{

bool HasCheckpoints( const tools::Path& session )
{
    return session.IsDirectory() && ( session / "checkpoints" ).Exists();
}

}  // namespace

tools::Path::T_Paths fcmd::ListSessionsWithCheckpoint( const tools::GeneralConfig& config, const tools::Path& exercise )
{
    return config.GetSessionsDir( exercise ).ListElements( boost::bind( &HasCheckpoints, _1 ) );
}

tools::Path::T_Paths fcmd::ListCheckpoints( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session )
{
    return config.GetCheckpointsDir( exercise, session ).ListDirectories( true );
}

namespace
{

void ReadSide( xml::xistream& xis, std::map< unsigned int, QString >& result )
{
    result[ xis.attribute< unsigned int >( "id" ) ] = xis.attribute< std::string >( "name", "" ).c_str();
}

} // namespace

std::map< unsigned int, QString > fcmd::ListSides( const tools::GeneralConfig& config, const tools::Path& exercise )
{
    std::map< unsigned int, QString > result;
    const tools::Path orbatFile = config.GetExerciseDir( exercise ) / "orbat.xml";
    if( orbatFile.Exists() )
    {
        tools::Xifstream xis( orbatFile );
        xis >> xml::start( "orbat" )
                >> xml::start( "parties" )
                    >> xml::list( "party", boost::bind( &ReadSide, _1, boost::ref( result ) ) );
    }
    return result;
}

tools::Path::T_Paths fcmd::RemoveCheckpoint( const tools::GeneralConfig& config, const tools::Path& exercise,
                                             const tools::Path& session, const tools::Path::T_Paths& checkpoints )
{
    tools::Path::T_Paths result;
    const tools::Path path( config.GetCheckpointsDir( exercise, session ) );
    if( !checkpoints.empty() )
        BOOST_FOREACH( const tools::Path& checkpoint, checkpoints )
            DeleteDirectory( path / checkpoint, result );
    else
        for( auto it = path.begin(); it != path.end(); ++it )
            DeleteDirectory( *it, result );
    return result;
}

namespace
{

bool IsValidModel( const tools::Path& record )
{
    return record.IsDirectory() &&
           ( record / "decisional" ).Exists() && ( record / "decisional" ).IsDirectory() &&
           ( record / "decisional" / "decisional.xml" ).Exists() &&
           ( record / "physical" ).Exists();
}

} // namespace

tools::Path::T_Paths fcmd::ListModels( const tools::GeneralConfig& config )
{
    return config.GetModelsDir().ListElements( boost::bind( &IsValidModel, _1 ), false );
}

namespace
{

bool IsValidPhysicalModel( const tools::Path& record )
{
    return record.IsDirectory() && ( record / "physical.xml" ).Exists();
}

} // namespace

tools::Path::T_Paths fcmd::ListPhysicalModels( const tools::GeneralConfig& config, const tools::Path& model )
{
    return config.GetPhysicalsDir( model ).ListElements( boost::bind( &IsValidPhysicalModel, _1 ), false );
}

namespace
{

bool IsValidScript( const tools::Path& child )
{
    return child.IsRegularFile() && child.Extension() == ".lua";
}

} // namespace

tools::Path::T_Paths fcmd::ListScripts( const tools::GeneralConfig& config, const tools::Path& exercise )
{
    return ( config.GetExerciseDir( exercise ) / "scripts" ).ListElements( boost::bind( &IsValidScript, _1 ) );
}

namespace
{

bool IsValidOrder( const tools::Path& child )
{
    return child.IsRegularFile() && child.Extension() == ".ord";
}

} // namespace

tools::Path::T_Paths fcmd::ListOrders( const tools::GeneralConfig& config, const tools::Path& exercise )
{
    return ( config.GetExerciseDir( exercise ) / "orders" ).ListElements( boost::bind( &IsValidOrder, _1 ) );
}

namespace
{

bool IsOther( const tools::Path& child )
{
    return child.IsDirectory() && child.FileName() != "sessions";
}

} // namespace

tools::Path::T_Paths fcmd::ListOtherDirectories( const tools::GeneralConfig& config, const tools::Path& exercise )
{
    return config.GetExerciseDir( exercise ).ListElements( boost::bind( &IsOther, _1 ), false );
}

bool fcmd::ExerciseExists( const tools::GeneralConfig& config, const tools::Path& exercise )
{
    const tools::Path::T_Paths exercises = ListExercises( config );
    return std::find( exercises.begin(), exercises.end(), exercise ) != exercises.end();
}

bool fcmd::SessionExists( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session )
{
    const tools::Path::T_Paths sessions = ListSessions( config, exercise, true );
    return std::find( sessions.begin(), sessions.end(), session ) != sessions.end();
}

bool fcmd::CheckpointExists( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, const tools::Path& checkpoint )
{
    const tools::Path::T_Paths checkpoints = ListCheckpoints( config, exercise, session );
    return std::find( checkpoints.begin(), checkpoints.end(), checkpoint ) != checkpoints.end();
}

bool fcmd::TerrainExists( const tools::GeneralConfig& config, const tools::Path& terrain )
{
    const tools::Path::T_Paths terrains = ListTerrains( config );
    return std::find( terrains.begin(), terrains.end(), terrain ) != terrains.end();
}

QStringList fcmd::PathListToQStringList( const tools::Path::T_Paths& paths )
{
    QStringList result;
    for( auto it = paths.begin(); it != paths.end(); ++it )
        result += it->ToUTF8().c_str();
    return result;
}
