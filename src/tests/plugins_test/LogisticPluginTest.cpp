// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include "logistic_plugin/NameResolver_ABC.h"
#include "logistic_plugin/LogisticPlugin.h"
#include "protocol/Protocol.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/regex.hpp>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace plugins::logistic;
namespace bfs = boost::filesystem;

namespace
{

std::string NormalizePath( std::string path )
{
    for( size_t i = 0; i != path.size(); ++i )
    {
        if( path[i] == '\\')
            path[i] = '/';
    }
    return path;
}

class TemporaryDirectory: private boost::noncopyable
{
public:
    TemporaryDirectory(const std::string& namePrefix, const std::string& basePath)
    {
        const std::string suffix = boost::lexical_cast<std::string>( rand() );
        const std::string name = namePrefix + suffix;
        const bfs::path path = bfs::path( basePath ) / bfs::path( name );
        if( bfs::exists(path) )
            bfs::remove_all( path );
        bfs::create_directories( path );
        path_ = bfs::path( NormalizePath( path.string() ));
    }

    ~TemporaryDirectory()
    {
        try
        {
            bfs::remove_all( path_ );
        }
        catch( const bfs::filesystem_error& )
        {
        }
    }

    const bfs::path& path() const
    {
        return path_;
    }

private:
    bfs::path path_;
};

class SimpleNameResolver : public NameResolver_ABC
{
public:
            SimpleNameResolver() {}

    virtual void GetAgentName( int id, std::string& name ) const
    {
        name = "agent_" + boost::lexical_cast<std::string>( id );
    }

    virtual void GetAutomatName( int id, std::string& name ) const
    {
        name = "automat_" + boost::lexical_cast<std::string>( id );
    }

    virtual void GetFormationName( int id, std::string& name ) const
    {
        name = "formation_" + boost::lexical_cast<std::string>( id );
    }

    virtual void GetSupplykName( const sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus& eSupply, std::string& name ) const
    {
        name = "supply_" + boost::lexical_cast<std::string>( eSupply );
    }

    virtual void GetRankName( const sword::EnumHumanRank& eRank, std::string& name ) const
    {
        name = "rank_" + boost::lexical_cast<std::string>( eRank );
    }

    virtual void GetWoundName( const sword::EnumHumanWound& eWound, std::string& name ) const
    {
        name = "wound_" + boost::lexical_cast<std::string>( eWound );
    }

    virtual void GetMedicalName( const sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus& eMedical, std::string& name ) const
    {
        name = "medical_" + boost::lexical_cast<std::string>( eMedical );
    }

    virtual void GetMaintenanceName( const sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus& eMaintenance, std::string& name ) const
    {
        name = "medical_" + boost::lexical_cast<std::string>( eMaintenance );
    }

    virtual void GetFuneralName( const sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus& eFuneral, std::string& name ) const
    {
        name = "funeral_" + boost::lexical_cast<std::string>( eFuneral );
    }

    virtual void GetEquipmentName( const sword::EquipmentType& equipmentType, std::string& name ) const
    {
        name = "equipment_" + boost::lexical_cast<std::string>( equipmentType.id() );
    }

    virtual void GetBreakdownName( const sword::BreakdownType& breakdownType, std::string& name ) const
    {
        name = "breakdown_" + boost::lexical_cast<std::string>( breakdownType.id() );
    }

    virtual void GetResourceName( const sword::ResourceType& resourceType, std::string& name ) const
    {
        name = "resource_" + boost::lexical_cast<std::string>( resourceType.id() );
    }
};

boost::shared_ptr<LogisticPlugin> CreateLogisticPlugin( const bfs::path& tempDir )
{
    boost::shared_ptr< SimpleNameResolver > nameResolver( new SimpleNameResolver() );
    return boost::shared_ptr< LogisticPlugin >( new LogisticPlugin( nameResolver,
        (tempDir / "maintenance" ).string(),
        (tempDir / "supply" ).string(),
        (tempDir / "funeral" ).string(),
        (tempDir / "medical" ).string(),
        "en" ));
}

typedef std::vector< std::string > T_FileList;
void ListDir( const bfs::path& path, T_FileList& files )
{
    files.clear();
    bfs::recursive_directory_iterator end;
    for( bfs::recursive_directory_iterator entry(path) ; entry != end ; ++entry)
    {
        if( bfs::is_regular_file( entry->status() ))
            files.push_back( NormalizePath( entry->path().string() ) );
    }
    std::sort( files.begin(), files.end() );
}

void CheckRegexps( const T_FileList& regexps, const T_FileList& files )
{
    BOOST_CHECK_EQUAL( regexps.size(), files.size() );
    for( size_t i = 0; i != files.size(); ++i )
    {
        boost::regex re( regexps[i], boost::regex::perl );
        BOOST_CHECK( boost::regex_match( files[i], re ));
    }
}

typedef std::vector< std::string > T_Lines;
void CheckFileContent( T_Lines expected, const std::string& path )
{
    T_Lines lines;
    std::fstream fp( path.c_str(), std::ios::in );
    std::string line;
    while( std::getline( fp, line ))
        lines.push_back(line);

    BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), lines.begin(), lines.end() );
}

}  // namespace

BOOST_AUTO_TEST_CASE( TestLogisticPlugin )
{
    TemporaryDirectory tempDir( "testlogisticplugin-", ::GetTestTempDirectory() );
    boost::shared_ptr<LogisticPlugin> plugin = CreateLogisticPlugin( tempDir.path() );

    sword::SimToClient m1;
    sword::LogMaintenanceHandlingCreation* maint1
        = m1.mutable_message()->mutable_log_maintenance_handling_creation();
    maint1->mutable_request()->set_id( 7 );
    maint1->mutable_unit()->set_id( 8 );
    maint1->set_tick( 9 );
    maint1->mutable_equipement()->set_id( 10 );
    maint1->mutable_breakdown()->set_id( 11 );

    plugin->Receive( m1 );

    T_FileList files;
    ListDir( tempDir.path(), files );

    T_FileList expecteds;
    expecteds.push_back( "^.*/maintenance\\.\\d{8}\\.\\d\\.csv$" );
    CheckRegexps( expecteds, files );

    T_Lines expectedLines;
    expectedLines.push_back( "request id ; tick ; GDH ; unit ; provider ; equipment ; breakdown ; state ; state end tick" );
    expectedLines.push_back( "7 ; 0 ;  ; agent_8 ;  ; equipment_10 ; breakdown_11 ;  ; " );
    CheckFileContent( expectedLines, files[0] );
}
