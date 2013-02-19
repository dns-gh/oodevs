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
#include "tools/ZipExtractor.h"
#include "clients_gui/Tools.h"
#pragma warning( push )
#pragma warning( disable: 4127 4244 4245 4996 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <zipstream/zipstream.h>
#pragma warning( pop )
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <QtCore/qstringlist.h>

namespace bfs = boost::filesystem;

namespace
{
    void DeleteDirectory( const bfs::path path, std::vector< std::string >& result )
    {
        if( bfs::exists( path ) && bfs::is_directory( path ) )
        {
            result.push_back( path.filename().string() );
            bfs::remove_all( path );
        }
    }
}

namespace frontend
{
    namespace commands
    {
        template< typename Validator >
        QStringList ListDirectoriesNoRecursive( const std::string& base, Validator v )
        {
            QStringList result;
            const bfs::path root = bfs::path( base );
            if( ! bfs::exists( root ) )
                return result;

            bfs::directory_iterator end;
            for( bfs::directory_iterator it( root ); it != end; ++it )
            {
                const bfs::path child = *it;
                if( v( child ) )
                    result.append( child.filename().string().c_str() );
            }
            return result;
        }

        bool IsValidTerrain( const bfs::path& dir )
        {
            return bfs::is_directory( dir )
                && bfs::exists( dir / "terrain.xml" );
        }

        QStringList ListTerrains( const tools::GeneralConfig& config )
        {
            return gui::ListDirectories( config.GetTerrainsDir(), &IsValidTerrain );
        }

        bool IsValidExercise( const bfs::path& dir )
        {
            return bfs::is_directory( dir )
                && bfs::exists( dir / "exercise.xml" );
        }

        QStringList ListExercises( const tools::GeneralConfig& config, const std::string& subDirs )
        {
            const std::string baseDirectory = config.GetExercisesDir();
            if( !baseDirectory.empty() )
                return gui::ListDirectories( config.GetExercisesDir() + "/" + subDirs, &IsValidExercise );
            return QStringList();
        }

        bool IsValidReplay( const bfs::path& session )
        {
            bfs::path record = session / "record";
            if( !bfs::is_directory( session ) || !bfs::exists( record ) || !bfs::is_directory( record ) )
                return false;
            bfs::directory_iterator end;
            for( bfs::directory_iterator it( record ); it != end; ++it )
            {
                const bfs::path child = *it;
                if( bfs::is_directory( child ) && bfs::exists( child / "info" ) && bfs::exists( child / "index" )
                    && bfs::exists( child / "keyindex" ) && bfs::exists( child / "key" ) && bfs::exists( child / "update" ) )
                    return true; // un seul dossier valide suffit
            }
            return false;
        }

        QStringList ListSessions( const tools::GeneralConfig& config, const std::string& exercise )
        {
            return gui::ListDirectories( config.GetSessionsDir( exercise ), &IsValidReplay );
        }

        bool HasCheckpoints( const bfs::path& session )
        {
            return bfs::is_directory( session )
                && bfs::exists      ( session / "checkpoints" );
        }

        QStringList ListSessionsWithCheckpoint( const tools::GeneralConfig& config, const std::string& exercise )
        {
            return gui::ListDirectories( config.GetSessionsDir( exercise ), &HasCheckpoints );
        }

        bool IsValidCheckpoint( const bfs::path& record )
        {
            return bfs::is_directory( record );
        }

        QStringList ListCheckpoints( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session )
        {
            return gui::ListDirectories( config.GetCheckpointsDir( exercise, session ), &IsValidCheckpoint );
        }

        void ReadSide( xml::xistream& xis, std::map< unsigned int, QString >& result )
        {
            result[ xis.attribute< unsigned int >( "id" ) ] = xis.attribute< std::string >( "name", "" ).c_str();
        }

        bfs::path GetOrbatFile( const tools::GeneralConfig& config, const std::string& exercise )
        {
            std::string fileName = config.GetExerciseDir( exercise );
            return bfs::path( bfs::path( fileName ) / "orbat.xml" );
        }

        std::map< unsigned int, QString > ListSides( const tools::GeneralConfig& config, const std::string& exercise )
        {
            std::map< unsigned int, QString > result;
            const bfs::path path( GetOrbatFile( config, exercise ) );
            if( bfs::exists( path ) )
            {
                xml::xifstream xis( path.string() );
                xis >> xml::start( "orbat" )
                        >> xml::start( "parties" )
                            >> xml::list( "party", boost::bind( &ReadSide, _1, boost::ref( result ) ) );
            }
            return result;
        }

        std::vector< std::string > RemoveCheckpoint( const tools::GeneralConfig& config, const std::string& exercise,
                                                     const std::string& session, const std::vector< std::string >& checkpoints )
        {
            std::vector< std::string > result;
            const bfs::path path( config.GetCheckpointsDir( exercise, session ) );
            if( !checkpoints.empty() )
                BOOST_FOREACH( const std::string& checkpoint, checkpoints )
                    DeleteDirectory( bfs::path( path / checkpoint ), result );
            else
                for( bfs::directory_iterator it( path ); it != bfs::directory_iterator(); ++it )
                    DeleteDirectory( bfs::path( *it ), result );
            return result;
        }

        bool IsValidModel( const bfs::path& record )
        {
            return bfs::is_directory( record )
                && bfs::exists( record / "decisional" ) && bfs::is_directory( record / "decisional" )
                && bfs::exists( record / "decisional" / "decisional.xml" )
                && bfs::exists( record / "physical" );
        }

        QStringList ListModels( const tools::GeneralConfig& config )
        {
            return gui::ListDirectories( config.GetModelsDir(), &IsValidModel );
        }

        bool IsValidPhysicalModel( const bfs::path& record )
        {
            return bfs::is_directory( record )
                && bfs::exists( record / "physical.xml" );
        }

        QStringList ListPhysicalModels( const tools::GeneralConfig& config, const std::string& model )
        {
            return gui::ListDirectories( config.GetPhysicalsDir( model ), &IsValidPhysicalModel );
        }

        bool IsValidScript( const bfs::path& child )
        {
            // std::string file( child.filename().c_str() );
            return bfs::is_regular_file( child ) && bfs::extension( child ).compare( ".lua" ) == 0;
        }

        QStringList ListScripts( const tools::GeneralConfig& config, const std::string& exercise )
        {
            std::string  dir( ( bfs::path( config.GetExerciseDir( exercise ) ) / "scripts" ).string() );
            return gui::ListDirectories( dir, &IsValidScript );
        }

        bool IsValidOrder( const bfs::path& child )
        {
            return bfs::is_regular_file( child ) && bfs::extension( child ).compare( ".ord" ) == 0;
            // std::string file( child.filename().c_str() );
            // return std::string( file, file.find_last_of( '.' ) ).compare( ".ord" ) == 0;
        }

        QStringList ListOrders( const tools::GeneralConfig& config, const std::string& exercise )
        {
            std::string  dir( ( bfs::path( config.GetExerciseDir( exercise ) ) / "orders" ).string() );
            return gui::ListDirectories( dir, &IsValidOrder );
        }

        bool IsValidPropagation( const bfs::path& child )
        {
            return bfs::is_directory( child )
                   && bfs::exists( child / "propagation" ) && bfs::is_directory( child / "propagation" )
                   && bfs::exists( child / "propagation.xml" );
        }

        QStringList ListPropagations( const tools::GeneralConfig& config )
        {
            std::string  dir( ( bfs::path( config.GetRootDir() ) / "data/propagations" ).string() );
            return gui::ListDirectories( dir, &IsValidPropagation );
        }

        bool IsOther( const bfs::path& child )
        {
            return bfs::is_directory( child ) && child.filename() != "sessions" && child.filename() != ".svn";
        }

        QStringList ListOtherDirectories( const tools::GeneralConfig& config, const std::string& exercise )
        {
            std::string dir( ( bfs::path( config.GetExerciseDir( exercise ) ) ).string() );
            return ListDirectoriesNoRecursive( dir, &IsOther );
        }

        QStringList ListPackageFiles( const std::string& filename )
        {
            QStringList list;
            zip::izipfile archive( filename );
            if( archive.isOk() )
                while( archive.browse() )
                {
                    const QString name = archive.getCurrentFileName().c_str();
                    if( name != "content.xml" ) // $$$$ SBO 2008-03-17: hard coded!
                        list.append( name );
                }
            archive.close();
            return list;
        }

        void InstallPackageFile( zip::izipfile& archive, const std::string& filename, const std::string& destination )
        {
            tools::zipextractor::ExtractFile( archive, filename.c_str(), filename, destination );
        }

        void InstallPackageFile( zip::izipfile& archive, const std::string& destination, boost::function0< void > callback )
        {
            while( archive.isOk() && archive.browse() )
            {
                const std::string name = archive.getCurrentFileName();
                if( name != "content.xml" ) // $$$$ SBO 2008-03-17: hard coded!
                {
                    tools::zipextractor::ExtractFile( archive, 0, name, destination );
                    callback();
                }
            }
        }

        bool ExerciseExists( const tools::GeneralConfig& config, const std::string& exercise )
        {
            const QStringList exercises = ListExercises( config );
            return exercises.indexOf( exercise.c_str() ) != -1;
        }

        bool SessionExists( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session )
        {
            const QStringList sessions = ListSessions( config, exercise );
            return sessions.indexOf( session.c_str() ) != -1;
        }

        bool CheckpointExists( const tools::GeneralConfig& config, const std::string& exercise, const std::string& session, const std::string& checkpoint )
        {
            const QStringList checkpoints = ListCheckpoints( config, exercise, session );
            return checkpoints.indexOf( checkpoint.c_str() ) != -1;
        }

        bool TerrainExists( const tools::GeneralConfig& config, const std::string& terrain )
        {
            const QStringList terrains = ListTerrains( config );
            return terrains.indexOf( terrain.c_str() ) != -1;
        }
    }
}
