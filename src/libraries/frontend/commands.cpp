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
#include <xeumeuleu/xml.hpp>

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
}

namespace frontend
{
    namespace commands
    {
        bool IsValidTerrain( const tools::Path& dir )
        {
            return dir.IsDirectory() && ( dir / "terrain.xml" ).Exists();
        }

        tools::Path::T_Paths ListTerrains( const tools::GeneralConfig& config )
        {
            return config.GetTerrainsDir().ListElements( boost::bind( &IsValidTerrain, _1 ) );
        }

        bool IsValidExercise( const tools::Path& dir )
        {
            return dir.IsDirectory() && ( dir / "exercise.xml" ).Exists();
        }

        tools::Path::T_Paths ListExercises( const tools::GeneralConfig& config, const tools::Path& subDirs )
        {
            const tools::Path baseDirectory = config.GetExercisesDir();
            tools::Path::T_Paths result;
            if( !baseDirectory.IsEmpty() )
            {
                tools::Path::T_Paths directories = ( baseDirectory / subDirs ).ListDirectories( false, false );
                CheckForDirectories( result, directories, ( baseDirectory / subDirs ), boost::bind( &IsValidExercise, _1 ) );
            }
            return result;
        }

        bool IsValidReplay( const tools::Path& session )
        {
            tools::Path record = session / "record";
            if( !session.IsDirectory() || !record.Exists() || !record.IsDirectory() )
                return false;

            for( auto it = record.begin(); it != record.end(); ++it )
            {
                const tools::Path child = *it;
                if( child.IsDirectory() && ( child / "info" ).Exists() && ( child / "index" ).Exists() &&
                    ( child / "keyindex" ).Exists() && ( child / "key" ).Exists() && ( child / "update" ).Exists() )
                    return true; // un seul dossier valide suffit
            }
            return false;
        }

        tools::Path::T_Paths ListSessions( const tools::GeneralConfig& config, const tools::Path& exercise, bool useValidator )
        {
            if( useValidator )
                return config.GetSessionsDir( exercise ).ListElements( boost::bind( &IsValidReplay, _1 ) );
            return config.GetSessionsDir( exercise ).ListElements( 0, false );
        }

        bool HasCheckpoints( const tools::Path& session )
        {
            return session.IsDirectory() && ( session / "checkpoints" ).Exists();
        }

        tools::Path::T_Paths ListSessionsWithCheckpoint( const tools::GeneralConfig& config, const tools::Path& exercise )
        {
            return config.GetSessionsDir( exercise ).ListElements( boost::bind( &HasCheckpoints, _1 ) );
        }

        tools::Path::T_Paths ListCheckpoints( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session )
        {
            return config.GetCheckpointsDir( exercise, session ).ListDirectories( true );
        }

        void ReadSide( xml::xistream& xis, std::map< unsigned int, QString >& result )
        {
            result[ xis.attribute< unsigned int >( "id" ) ] = xis.attribute< std::string >( "name", "" ).c_str();
        }

        std::map< unsigned int, QString > ListSides( const tools::GeneralConfig& config, const tools::Path& exercise )
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

        tools::Path::T_Paths RemoveCheckpoint( const tools::GeneralConfig& config, const tools::Path& exercise,
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

        bool IsValidModel( const tools::Path& record )
        {
            return record.IsDirectory() &&
                   ( record / "decisional" ).Exists() && ( record / "decisional" ).IsDirectory() &&
                   ( record / "decisional" / "decisional.xml" ).Exists() &&
                   ( record / "physical" ).Exists();
        }

        tools::Path::T_Paths ListModels( const tools::GeneralConfig& config )
        {
            return config.GetModelsDir().ListElements( boost::bind( &IsValidModel, _1 ), false );
        }

        bool IsValidPhysicalModel( const tools::Path& record )
        {
            return record.IsDirectory() && ( record / "physical.xml" ).Exists();
        }

        tools::Path::T_Paths ListPhysicalModels( const tools::GeneralConfig& config, const tools::Path& model )
        {
            return config.GetPhysicalsDir( model ).ListElements( boost::bind( &IsValidPhysicalModel, _1 ), false );
        }

        bool IsValidScript( const tools::Path& child )
        {
            return child.IsRegularFile() && child.Extension() == ".lua";
        }

        tools::Path::T_Paths ListScripts( const tools::GeneralConfig& config, const tools::Path& exercise )
        {
            return ( config.GetExerciseDir( exercise ) / "scripts" ).ListElements( boost::bind( &IsValidScript, _1 ) );
        }

        bool IsValidOrder( const tools::Path& child )
        {
            return child.IsRegularFile() && child.Extension() == ".ord";
        }

        tools::Path::T_Paths ListOrders( const tools::GeneralConfig& config, const tools::Path& exercise )
        {
            return ( config.GetExerciseDir( exercise ) / "orders" ).ListElements( boost::bind( &IsValidOrder, _1 ) );
        }

        bool IsOther( const tools::Path& child )
        {
            return child.IsDirectory() && child.FileName() != "sessions";
        }

        tools::Path::T_Paths ListOtherDirectories( const tools::GeneralConfig& config, const tools::Path& exercise )
        {
            return config.GetExerciseDir( exercise ).ListElements( boost::bind( &IsOther, _1 ), false );
        }

        bool ExerciseExists( const tools::GeneralConfig& config, const tools::Path& exercise )
        {
            const tools::Path::T_Paths exercises = ListExercises( config );
            return std::find( exercises.begin(), exercises.end(), exercise ) != exercises.end();
        }

        bool SessionExists( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session )
        {
            const tools::Path::T_Paths sessions = ListSessions( config, exercise, true );
            return std::find( sessions.begin(), sessions.end(), session ) != sessions.end();
        }

        bool CheckpointExists( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, const tools::Path& checkpoint )
        {
            const tools::Path::T_Paths checkpoints = ListCheckpoints( config, exercise, session );
            return std::find( checkpoints.begin(), checkpoints.end(), checkpoint ) != checkpoints.end();
        }

        bool TerrainExists( const tools::GeneralConfig& config, const tools::Path& terrain )
        {
            const tools::Path::T_Paths terrains = ListTerrains( config );
            return std::find( terrains.begin(), terrains.end(), terrain ) != terrains.end();
        }

        QStringList PathListToQStringList( const tools::Path::T_Paths& paths )
        {
            QStringList result;
            for( auto it = paths.begin(); it != paths.end(); ++it )
                result += it->ToUTF8().c_str();
            return result;
        }
    }
}
