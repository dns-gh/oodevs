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
#include "logistic_plugin/ConsignArchive.h"
#include "logistic_plugin/ConsignCsvLogger.h"
#include "logistic_plugin/ConsignRecorder.h"
#include "logistic_plugin/ConsignResolver_ABC.h"
#include "protocol/Protocol.h"
#include <tools/Helpers.h>
#include <tools/TemporaryDirectory.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/regex.hpp>
#include <algorithm>
#include <cstdlib>
#include <string>

using namespace plugins::logistic;
namespace bpt = boost::posix_time;
namespace bg = boost::gregorian;

namespace
{

class SimpleNameResolver : public NameResolver_ABC
{
public:
    virtual void GetAgentName( int id, std::string& name ) const
    {
        name = "agent_" + boost::lexical_cast< std::string >( id );
    }

    virtual void GetAutomatName( int id, std::string& name ) const
    {
        name = "automat_" + boost::lexical_cast< std::string >( id );
    }

    virtual void GetFormationName( int id, std::string& name ) const
    {
        name = "formation_" + boost::lexical_cast< std::string >( id );
    }

    virtual void GetSupplykName( const sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus& eSupply, std::string& name ) const
    {
        name = "supply_" + boost::lexical_cast< std::string >( eSupply );
    }

    virtual void GetRankName( const sword::EnumHumanRank& eRank, std::string& name ) const
    {
        name = "rank_" + boost::lexical_cast< std::string >( eRank );
    }

    virtual void GetWoundName( const sword::EnumHumanWound& eWound, std::string& name ) const
    {
        name = "wound_" + boost::lexical_cast< std::string >( eWound );
    }

    virtual void GetMedicalName( const sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus& eMedical, std::string& name ) const
    {
        name = "medical_" + boost::lexical_cast< std::string >( eMedical );
    }

    virtual void GetMaintenanceName( const sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus& eMaintenance, std::string& name ) const
    {
        name = "maintenance_" + boost::lexical_cast< std::string >( eMaintenance );
    }

    virtual void GetFuneralName( const sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus& eFuneral, std::string& name ) const
    {
        name = "funeral_" + boost::lexical_cast< std::string >( eFuneral );
    }

    virtual void GetEquipmentName( const sword::Id& equipmentType, std::string& name ) const
    {
        name = "equipment_" + boost::lexical_cast< std::string >( equipmentType.id() );
    }

    virtual void GetBreakdownName( const sword::Id& breakdownType, std::string& name ) const
    {
        name = "breakdown_" + boost::lexical_cast< std::string >( breakdownType.id() );
    }

    virtual void GetResourceName( const sword::Id& resourceType, std::string& name ) const
    {
        name = "resource_" + boost::lexical_cast< std::string >( resourceType.id() );
    }
};

boost::shared_ptr< ConsignCsvLogger > CreateCsvLogger( const tools::Path& tempDir )
{
    boost::shared_ptr< SimpleNameResolver > nameResolver( new SimpleNameResolver() );
    return boost::make_shared< ConsignCsvLogger >( nameResolver,
        tempDir / "maintenance",
        tempDir / "supply",
        tempDir / "funeral",
        tempDir / "medical" );
}

boost::shared_ptr< LogisticPlugin > CreateLogisticPlugin(
    const boost::shared_ptr< ConsignCsvLogger >& logger, const tools::Path& tempDir )
{
    return boost::make_shared< LogisticPlugin >( logger, tempDir / "logistics", false );
}

typedef std::vector< std::string > T_Lines;
void CheckFileContent( T_Lines expected, const tools::Path& path )
{
    T_Lines lines;
    tools::Fstream fp( path, std::ios::in );
    std::string line;
    while( std::getline( fp, line ))
        lines.push_back(line);

    BOOST_CHECK_EQUAL_COLLECTIONS( expected.begin(), expected.end(), lines.begin(), lines.end() );
}

struct LogFile
{
    LogFile( const std::wstring& r, const T_Lines& l ): regexp( r ), lines( l ) {}

    std::wstring regexp;
    T_Lines lines;
};

void CheckRegexps( const std::vector< LogFile >& logFiles, tools::Path::T_Paths files )
{
    boost::remove_erase_if( files, []( const tools::Path& path ) -> bool
        {
            return path.ToUTF8().find( "logistics" ) != std::string::npos;
        });
    BOOST_CHECK_EQUAL( logFiles.size(), files.size() );
    for( size_t i = 0; i != files.size(); ++i )
    {
        boost::wregex re( logFiles[i].regexp, boost::wregex::perl );
        BOOST_CHECK( boost::regex_match( files[i].ToUnicode(), re ) );

        CheckFileContent( logFiles[i].lines, files[i] );
    }
}

}  // namespace

BOOST_AUTO_TEST_CASE( TestLogisticPlugin )
{
    tools::TemporaryDirectory tempDir( "testlogisticplugin-", testOptions.GetTempDir() );
    auto logger = CreateCsvLogger( tempDir.Path() );
    auto plugin = CreateLogisticPlugin( logger, tempDir.Path() );

    {
        bg::date day1( bg::from_string( "2001/05/17" ) );

        {
            sword::SimToClient m;
            sword::ControlBeginTick* tickMsg = m.mutable_message()->mutable_control_begin_tick();
            tickMsg->set_current_tick( 200 );
            tickMsg->mutable_date_time()->set_data( "GDH1" );
            plugin->Receive( m, day1 );
        }

        {
            sword::SimToClient m;
            sword::LogMaintenanceHandlingCreation* maint = m.mutable_message()->mutable_log_maintenance_handling_creation();
            maint->mutable_request()->set_id( 7 );
            maint->mutable_unit()->set_id( 8 );
            maint->set_tick( 200 );
            maint->mutable_equipement()->set_id( 10 );
            maint->mutable_breakdown()->set_id( 11 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 1 );

        {
            sword::SimToClient m;
            sword::ControlBeginTick* tickMsg = m.mutable_message()->mutable_control_begin_tick();
            tickMsg->set_current_tick( 300 );
            tickMsg->mutable_date_time()->set_data( "GDH2" );
            plugin->Receive( m, day1 );
        }

        {
            sword::SimToClient m;
            sword::LogMaintenanceHandlingCreation* maint = m.mutable_message()->mutable_log_maintenance_handling_creation();
            maint->mutable_request()->set_id( 17 );
            maint->mutable_unit()->set_id( 18 );
            maint->set_tick( 300 );
            maint->mutable_equipement()->set_id( 20 );
            maint->mutable_breakdown()->set_id( 21 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 2 );

        {
            sword::SimToClient m;
            sword::LogMaintenanceHandlingUpdate* maint = m.mutable_message()->mutable_log_maintenance_handling_update();
            maint->mutable_request()->set_id( 7 );
            maint->mutable_unit()->set_id( 8 );
            maint->mutable_provider()->set_id( 12 );
            maint->set_state( static_cast< sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus >( 0 ) );
            maint->set_current_state_end_tick( 400 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 2 );

        {
            sword::SimToClient m;
            sword::LogMaintenanceHandlingDestruction* maint = m.mutable_message()->mutable_log_maintenance_handling_destruction();
            maint->mutable_request()->set_id( 7 );
            maint->mutable_unit()->set_id( 8 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 1 );

        {
            sword::SimToClient m;
            sword::LogMaintenanceHandlingDestruction* maint = m.mutable_message()->mutable_log_maintenance_handling_destruction();
            maint->mutable_request()->set_id( 17 );
            maint->mutable_unit()->set_id( 8 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 0 );
    }

    {
        bg::date day1( bg::from_string( "2002/01/25" ) );
        bg::date day2( bg::from_string( "2002/01/26" ) );
        bg::date day3( bg::from_string( "2002/01/27" ) );

        {
            sword::SimToClient m;
            sword::LogMedicalHandlingCreation* medic = m.mutable_message()->mutable_log_medical_handling_creation();
            medic->mutable_request()->set_id( 7 );
            medic->mutable_unit()->set_id( 8 );
            medic->set_tick( 400 );
            medic->set_rank( static_cast< sword::EnumHumanRank >( 0 ) );
            medic->set_wound( static_cast< sword::EnumHumanWound >( 0 ) );
            medic->set_nbc_contaminated( true );
            medic->set_mental_wound( false );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Medical ), 1 );

        {
            sword::SimToClient m;
            sword::LogMedicalHandlingUpdate* medic = m.mutable_message()->mutable_log_medical_handling_update();
            medic->mutable_request()->set_id( 7 );
            medic->mutable_unit()->set_id( 8 );
            medic->mutable_provider()->set_id( 12 );
            medic->set_state( static_cast< sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus >( 0 ) );
            medic->set_current_state_end_tick( 30 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Medical ), 1 );

        {
            sword::SimToClient m;
            sword::LogMedicalHandlingUpdate* medic = m.mutable_message()->mutable_log_medical_handling_update();
            medic->mutable_request()->set_id( 7 );
            medic->mutable_unit()->set_id( 8 );
            medic->mutable_provider()->set_id( 12 );
            medic->set_state( static_cast< sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus >( 1 ) );
            medic->set_current_state_end_tick( 400 );
            plugin->Receive( m, day2 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Medical ), 1 );

        {
            sword::SimToClient m;
            sword::LogMedicalHandlingUpdate* medic = m.mutable_message()->mutable_log_medical_handling_update();
            medic->mutable_request()->set_id( 7 );
            medic->mutable_unit()->set_id( 8 );
            medic->mutable_provider()->set_id( 12 );
            medic->set_state( static_cast< sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus >( 2 ) );
            medic->set_current_state_end_tick( 500 );
            plugin->Receive( m, day3 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Medical ), 1 );

        {
            sword::SimToClient m;
            sword::LogMedicalHandlingDestruction* medic = m.mutable_message()->mutable_log_medical_handling_destruction();
            medic->mutable_request()->set_id( 7 );
            medic->mutable_unit()->set_id( 8 );
            plugin->Receive( m, day3 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Medical ), 0 );
    }

    {
        bg::date day1( bg::from_string( "2005/02/20" ) );
        logger->SetMaxLinesInFile( 3 );

        {
            sword::SimToClient m;
            sword::LogFuneralHandlingCreation* funeral = m.mutable_message()->mutable_log_funeral_handling_creation();
            funeral->mutable_request()->set_id( 7 );
            funeral->mutable_unit()->set_id( 8 );
            funeral->set_tick( 300 );
            funeral->set_rank( static_cast< sword::EnumHumanRank >( 0 ) );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Funeral ), 1 );

        for( int i = 0; i < 5; ++i )
        {
            sword::SimToClient m;
            sword::LogFuneralHandlingUpdate* funeral = m.mutable_message()->mutable_log_funeral_handling_update();
            funeral->mutable_request()->set_id( 7 );
            funeral->mutable_handling_unit()->mutable_automat()->set_id( 8 );
            funeral->mutable_convoying_unit()->set_id( 9 );
            funeral->mutable_packaging_resource()->set_id( 10 );
            funeral->set_state( static_cast< sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus >( i ) );
            funeral->set_current_state_end_tick( 300 + i * 100 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Funeral ), 1 );

        {
            sword::SimToClient m;
            sword::LogFuneralHandlingDestruction* funeral = m.mutable_message()->mutable_log_funeral_handling_destruction();
            funeral->mutable_request()->set_id( 7 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Funeral ), 0 );
    }

    {
        bg::date day1( bg::from_string( "2008/09/21" ) );
        bg::date day2( bg::from_string( "2008/09/22" ) );
        logger->SetMaxLinesInFile( 3 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingCreation* supply = m.mutable_message()->mutable_log_supply_handling_creation();
            supply->mutable_request()->set_id( 107 );
            supply->set_tick( 200 );
            supply->mutable_supplier()->mutable_automat()->set_id( 8 );
            supply->mutable_transporters_provider()->mutable_automat()->set_id( 9 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Supply ), 1 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingUpdate* supply = m.mutable_message()->mutable_log_supply_handling_update();
            supply->mutable_request()->set_id( 107 );
            supply->mutable_convoyer()->set_id( 10 );
            supply->set_state( static_cast< sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus >( 0 ) );
            supply->set_current_state_end_tick( 1000 );
            {
                sword::SupplyRecipientResourcesRequest* req = supply->mutable_requests()->add_requests();
                req->mutable_recipient()->set_id( 11 );
                {
                    sword::SupplyResourceRequest* res = req->add_resources();
                    res->mutable_resource()->set_id( 0 );
                    res->set_convoyed( 100 );
                    res->set_granted( 200 );
                    res->set_requested( 300 );
                }
                {
                    sword::SupplyResourceRequest* res = req->add_resources();
                    res->mutable_resource()->set_id( 1 );
                    res->set_convoyed( 110 );
                    res->set_granted( 210 );
                    res->set_requested( 310 );
                }
            }
            {
                sword::SupplyRecipientResourcesRequest* req = supply->mutable_requests()->add_requests();
                req->mutable_recipient()->set_id( 12 );
                {
                    sword::SupplyResourceRequest* res = req->add_resources();
                    res->mutable_resource()->set_id( 3 );
                    res->set_convoyed( 55 );
                    res->set_granted( 75 );
                    res->set_requested( 120 );
                }
            }
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Supply ), 1 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingUpdate* supply = m.mutable_message()->mutable_log_supply_handling_update();
            supply->mutable_request()->set_id( 108 );
            supply->mutable_convoyer()->set_id( 10 );
            supply->set_state( static_cast< sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus >( 0 ) );
            supply->set_current_state_end_tick( 1000 );
            {
                sword::SupplyRecipientResourcesRequest* req = supply->mutable_requests()->add_requests();
                req->mutable_recipient()->set_id( 13 );
                {
                    sword::SupplyResourceRequest* res = req->add_resources();
                    res->mutable_resource()->set_id( 0 );
                    res->set_convoyed( 100 );
                    res->set_granted( 200 );
                    res->set_requested( 300 );
                }
            }
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Supply ), 2 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingUpdate* supply = m.mutable_message()->mutable_log_supply_handling_update();
            supply->mutable_request()->set_id( 107 );
            supply->mutable_convoyer()->set_id( 10 );
            supply->set_state( static_cast< sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus >( 1 ) );
            supply->set_current_state_end_tick( 2000 );
            {
                sword::SupplyRecipientResourcesRequest* req = supply->mutable_requests()->add_requests();
                req->mutable_recipient()->set_id( 14 );
                {
                    sword::SupplyResourceRequest* res = req->add_resources();
                    res->mutable_resource()->set_id( 3 );
                    res->set_convoyed( 50 );
                    res->set_granted( 75 );
                    res->set_requested( 120 );
                }
            }
            plugin->Receive( m, day2 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Supply ), 2 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingUpdate* supply = m.mutable_message()->mutable_log_supply_handling_update();
            supply->mutable_request()->set_id( 108 );
            supply->mutable_convoyer()->set_id( 10 );
            supply->set_state( static_cast< sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus >( 1 ) );
            supply->set_current_state_end_tick( 2000 );
            {
                sword::SupplyRecipientResourcesRequest* req = supply->mutable_requests()->add_requests();
                req->mutable_recipient()->set_id( 17 );
                {
                    sword::SupplyResourceRequest* res = req->add_resources();
                    res->mutable_resource()->set_id( 3 );
                    res->set_convoyed( 80 );
                    res->set_granted( 85 );
                    res->set_requested( 180 );
                }
            }
            {
                sword::SupplyRecipientResourcesRequest* req = supply->mutable_requests()->add_requests();
                req->mutable_recipient()->set_id( 18 );
                {
                    sword::SupplyResourceRequest* res = req->add_resources();
                    res->mutable_resource()->set_id( 4 );
                    res->set_convoyed( 21 );
                    res->set_granted( 23 );
                    res->set_requested( 24 );
                }
            }
            plugin->Receive( m, day2 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Supply ), 2 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingDestruction* supply = m.mutable_message()->mutable_log_supply_handling_destruction();
            supply->mutable_request()->set_id( 107 );
            plugin->Receive( m, day2 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Supply ), 1 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingDestruction* supply = m.mutable_message()->mutable_log_supply_handling_destruction();
            supply->mutable_request()->set_id( 108 );
            plugin->Receive( m, day2 );
        }
        BOOST_CHECK_EQUAL( logger->DebugGetConsignCount( LogisticPlugin::eLogisticType_Supply ), 0 );
    }
    logger->Flush();

    auto files = tempDir.Path().ListFiles( true, false, true );
    std::vector< LogFile > expecteds;
    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; unit ; handling unit ; conveying unit ; rank ; packaging resource ; state ; state end tick" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ;  ;  ; rank_0 ;  ;  ; " );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ; automat_8 ; agent_9 ; rank_0 ; resource_10 ; funeral_0 ; " );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ; automat_8 ; agent_9 ; rank_0 ; resource_10 ; funeral_1 ; 400" );
        expecteds.push_back( LogFile( L"^.*/funeral\\.20050220\\.0\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; unit ; handling unit ; conveying unit ; rank ; packaging resource ; state ; state end tick" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ; automat_8 ; agent_9 ; rank_0 ; resource_10 ; funeral_2 ; 500" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ; automat_8 ; agent_9 ; rank_0 ; resource_10 ; funeral_3 ; 600" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ; automat_8 ; agent_9 ; rank_0 ; resource_10 ; funeral_4 ; 700" );
        expecteds.push_back( LogFile( L"^.*/funeral\\.20050220\\.1\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; unit ; provider ; equipment ; breakdown ; state ; state end tick" );
        expectedLines.push_back( "7 ; 200 ; GDH1 ; agent_8 ;  ; equipment_10 ; breakdown_11 ;  ; " );
        expectedLines.push_back( "17 ; 300 ; GDH2 ; agent_18 ;  ; equipment_20 ; breakdown_21 ;  ; " );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ; agent_12 ; equipment_10 ; breakdown_11 ; maintenance_0 ; 400" );
        expecteds.push_back( LogFile( L"^.*/maintenance\\.20010517\\.0\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; unit ; provider ; rank ; wound ; nbc ; mental ; state ; state end tick" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ; agent_12 ; rank_0 ; wound_0 ; yes ; no ; medical_1 ; 400" );
        expecteds.push_back( LogFile( L"^.*/medical\\.20020126\\.0\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; unit ; provider ; rank ; wound ; nbc ; mental ; state ; state end tick" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ; agent_12 ; rank_0 ; wound_0 ; yes ; no ; medical_2 ; 500" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; agent_8 ; agent_12 ; rank_0 ; wound_0 ; yes ; no ; instruction finished ; 500" );
        expecteds.push_back( LogFile( L"^.*/medical\\.20020127\\.0\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; recipient ; provider ; transport provider ; conveyor ; state ; state end tick ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed" );
        expectedLines.push_back( "107 ; 300 ; GDH2 ;  ; automat_8 ; automat_9 ;  ;  ; " );
        expectedLines.push_back( "107 ; 300 ; GDH2 ; automat_11 ; automat_8 ; automat_9 ; agent_10 ; supply_0 ; 1000 ; resource_0 ; 300 ; 200 ; 100 ; resource_1 ; 310 ; 210 ; 110" );
        expectedLines.push_back( "107 ; 300 ; GDH2 ; automat_12 ; automat_8 ; automat_9 ; agent_10 ; supply_0 ; 1000 ; resource_3 ; 120 ; 75 ; 55" );
        expecteds.push_back( LogFile( L"^.*/supply\\.20080921\\.0\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; recipient ; provider ; transport provider ; conveyor ; state ; state end tick ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed" );
        expectedLines.push_back( "108 ; 300 ; GDH2 ; automat_13 ;  ;  ; agent_10 ; supply_0 ; 1000 ; resource_0 ; 300 ; 200 ; 100" );
        expecteds.push_back( LogFile( L"^.*/supply\\.20080921\\.1\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; recipient ; provider ; transport provider ; conveyor ; state ; state end tick ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed" );
        expectedLines.push_back( "107 ; 300 ; GDH2 ; automat_11 ; automat_8 ; automat_9 ; agent_10 ; supply_1 ; 2000 ; resource_0 ; 300 ; 200 ; 100 ; resource_1 ; 310 ; 210 ; 110" );
        expectedLines.push_back( "107 ; 300 ; GDH2 ; automat_12 ; automat_8 ; automat_9 ; agent_10 ; supply_1 ; 2000 ; resource_3 ; 120 ; 75 ; 55" );
        expectedLines.push_back( "107 ; 300 ; GDH2 ; automat_14 ; automat_8 ; automat_9 ; agent_10 ; supply_1 ; 2000 ; resource_3 ; 120 ; 75 ; 50" );
        expecteds.push_back( LogFile( L"^.*/supply\\.20080922\\.0\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; recipient ; provider ; transport provider ; conveyor ; state ; state end tick ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed" );
        expectedLines.push_back( "108 ; 300 ; GDH2 ; automat_13 ;  ;  ; agent_10 ; supply_1 ; 2000 ; resource_0 ; 300 ; 200 ; 100" );
        expectedLines.push_back( "108 ; 300 ; GDH2 ; automat_17 ;  ;  ; agent_10 ; supply_1 ; 2000 ; resource_3 ; 180 ; 85 ; 80" );
        expectedLines.push_back( "108 ; 300 ; GDH2 ; automat_18 ;  ;  ; agent_10 ; supply_1 ; 2000 ; resource_4 ; 24 ; 23 ; 21" );
        expecteds.push_back( LogFile( L"^.*/supply\\.20080922\\.1\\.csv$", expectedLines ) );
    }

    CheckRegexps( expecteds, files );
}

namespace
{

void PushFuneralMessage( LogisticPlugin* plugin )
{
    sword::SimToClient m;
    sword::LogFuneralHandlingCreation* funeral = m.mutable_message()->mutable_log_funeral_handling_creation();
    funeral->mutable_request()->set_id( 7 );
    funeral->mutable_unit()->set_id( 8 );
    funeral->set_tick( 100 );
    funeral->set_rank( static_cast< sword::EnumHumanRank >( 0 ) );
    plugin->Receive( m,  bg::from_string( "2005/02/20" ) );
}

} // namespace

BOOST_AUTO_TEST_CASE( TestLogisticPluginRestart )
{
    tools::TemporaryDirectory tempDir( "testlogisticplugin-", testOptions.GetTempDir() );
    auto logger = CreateCsvLogger( tempDir.Path() );
    logger->SetMaxLinesInFile( 1 );
    auto plugin = CreateLogisticPlugin( logger, tempDir.Path() );

    for( int i = 0; i < 2; ++i )
        PushFuneralMessage( plugin.get() );

    // Recreate it on multiple files
    logger = CreateCsvLogger( tempDir.Path() );
    plugin = CreateLogisticPlugin( logger, tempDir.Path() );
    PushFuneralMessage( plugin.get() );
    logger->Flush();

    tools::Path::T_Paths files = tempDir.Path().ListElements( tools::Path::T_Functor(), true, false, true );
    std::vector< LogFile > expected;
    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; unit ; handling unit ; conveying unit ; rank ; packaging resource ; state ; state end tick" );
        expectedLines.push_back( "7 ; 0 ;  ; agent_8 ;  ;  ; rank_0 ;  ;  ; " );
        expected.push_back( LogFile( L"^.*/funeral\\.20050220\\.0\\.csv$", expectedLines ) );
        expected.push_back( LogFile( L"^.*/funeral\\.20050220\\.1\\.csv$", expectedLines ) );
        expected.push_back( LogFile( L"^.*/funeral\\.20050220\\.2\\.csv$", expectedLines ) );
    }
    CheckRegexps( expected, files );
}

BOOST_AUTO_TEST_CASE( TestEscapeRegex )
{
    BOOST_CHECK_EQUAL( "", "" );
    BOOST_CHECK_EQUAL( tools::Path::FromUnicode( L"abC123" ), tools::Path::FromUnicode( EscapeRegex( L"abC123" ) ) );
    BOOST_CHECK_EQUAL( tools::Path::FromUnicode( L"a\\^bc\\{12\\}3\\*\\\\" ), tools::Path::FromUnicode( EscapeRegex( L"a^bc{12}3*\\" ) ) );
}

BOOST_AUTO_TEST_CASE( TestConsignArchive )
{
    const std::string strings[] =
    {
        "abcd",
        "efgh",
        "ijkl",
    };

    tools::TemporaryDirectory tempDir( "testlogistic-plugin-", testOptions.GetTempDir() );
    const auto baseDir = tempDir.Path() / "logistics";
    ConsignArchive ar( baseDir, 10 );
    for( size_t i = 0; i != COUNT_OF( strings ); ++i )
        ar.Write( &strings[i][0], static_cast< uint32_t >( strings[i].size() ));
    ar.Flush();

    auto paths = baseDir.ListFiles( false, true, true );
    BOOST_REQUIRE_EQUAL( 3u, paths.size() );
    BOOST_CHECK_EQUAL( "data.2", paths[2].ToUTF8() );

    size_t seen = 0;
    ar.ReadAll( [&]( ConsignOffset, std::vector< uint8_t >& output )
    {
        BOOST_REQUIRE_EQUAL( 4u, output.size() );
        std::string s( reinterpret_cast< const char* >( &output[0] ), output.size() );
        BOOST_REQUIRE( seen < COUNT_OF( strings ));
        BOOST_CHECK_EQUAL( s, strings[ seen ] );
        ++seen;
    });
    BOOST_CHECK_EQUAL( seen, COUNT_OF( strings ));

    uint32_t file, index;
    BOOST_REQUIRE( ReadOffsetFile( ar.GetOffsetFilePath(), file, index ) );
    BOOST_CHECK_EQUAL( 2u, file );
    BOOST_CHECK_EQUAL( 9u, index );
}

namespace
{

std::set< uint32_t > MakeSet( uint32_t u1 = 0, uint32_t u2 = 0, uint32_t u3 = 0 )
{
    std::set< uint32_t > ids;
    if( u1 )
        ids.insert( u1 );
    if( u2 )
        ids.insert( u2 );
    if( u3 )
        ids.insert( u3 );
    return ids;
}

void AddAndFlush( ConsignRecorder& rec, uint32_t requestId, uint32_t tick,
        bool destroyed, const std::set< uint32_t >& entitiesSet )
{
    sword::LogHistoryEntry entry;
    entry.set_tick( tick );

    // For the WriteEntry test we could generate any kind of message, the
    // metadata can be passed to WriteEntry directly. But for a loading test
    // we have to craft a structure from which the entities and tick can
    // be retrieved.
    auto creation = entry.mutable_supply()->mutable_creation();
    creation->mutable_request()->set_id( requestId );
    creation->set_tick( tick );
    creation->mutable_supplier()->mutable_automat()->set_id( 12345 );
    creation->mutable_transporters_provider()->mutable_automat()->set_id( 12345 );

    auto update = entry.mutable_supply()->mutable_update();
    update->mutable_request()->set_id( requestId );
    update->mutable_convoyer()->set_id( 12345 );
    update->set_state(
            static_cast< sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus >( 0 ) );
    update->set_current_state_end_tick( 1000 );
    {
        for( auto it = entitiesSet.cbegin(); it != entitiesSet.cend(); ++it )
        {
            auto* req = update->mutable_requests()->add_requests();
            req->mutable_recipient()->set_id( *it );
        }
    }

    if( destroyed )
    {
        auto destruction = entry.mutable_supply()->mutable_destruction();
        destruction->mutable_request()->set_id( requestId );
    }

    std::vector< uint32_t > entities( entitiesSet.begin(), entitiesSet.end() );
    rec.WriteEntry( requestId, destroyed, entry, entities );
    rec.Flush();
}

void AddAndFlush( ConsignRecorder& rec, uint32_t requestId, uint32_t tick,
        bool destroyed )
{
    AddAndFlush( rec, requestId, tick, destroyed, MakeSet() );
}

// Turn recorded LogHistoryEntry into something easily checked.
std::string GetHistoryTrace( const boost::ptr_vector< sword::LogHistoryEntry >& entries )
{
    std::stringstream ss;
    bool first = true;
    for( auto it = entries.cbegin(); it != entries.end(); ++it )
    {
        if( !it->has_supply() || !it->supply().has_creation() )
            continue;
        if( first )
            first = false;
        else
            ss << ", ";
        auto& creation = it->supply().creation();
        ss << creation.request().id() << "." << creation.tick();
    }
    return ss.str();
}

}  // namespace

BOOST_AUTO_TEST_CASE( TestConsignRecorderLRU )
{
    tools::TemporaryDirectory tempDir( "testlogisticplugin-", testOptions.GetTempDir() );
    const tools::Path path = tempDir.Path() / "logistics";
    boost::ptr_vector< sword::LogHistoryEntry > entries;

    ConsignRecorder rec( path, 1024*1024, 2, 1000 );
    BOOST_CHECK_EQUAL( 0u, rec.GetHistorySize() );

    // Same consign
    AddAndFlush( rec, 1, 1, false );
    BOOST_CHECK_EQUAL( 1u, rec.GetHistorySize() );
    AddAndFlush( rec, 1, 2, false );
    BOOST_CHECK_EQUAL( 1u, rec.GetHistorySize() );
    rec.GetHistory( 1, entries );
    BOOST_CHECK_EQUAL( "1.1, 1.2", GetHistoryTrace( entries ) );

    // One destroyed
    AddAndFlush( rec, 2, 3, true );
    BOOST_CHECK_EQUAL( 2u, rec.GetHistorySize() );
    rec.GetHistory( 2, entries );
    BOOST_CHECK_EQUAL( "2.3", GetHistoryTrace( entries ) );

    // Another valid -> evict the destroyed [2]
    AddAndFlush( rec, 3, 4, false );
    BOOST_CHECK_EQUAL( 2u, rec.GetHistorySize() );
    rec.GetHistory( 3, entries );
    BOOST_CHECK_EQUAL( "3.4", GetHistoryTrace( entries ) );
    rec.GetHistory( 2, entries );
    BOOST_CHECK_EQUAL( "", GetHistoryTrace( entries ) );

    // Another destroyed -> removed immediately
    AddAndFlush( rec, 4, 5, true );
    BOOST_CHECK_EQUAL( 2u, rec.GetHistorySize() );
    rec.GetHistory( 4, entries );
    BOOST_CHECK_EQUAL( "", GetHistoryTrace( entries ) );

    // Update [1]
    AddAndFlush( rec, 1, 6, false );
    BOOST_CHECK_EQUAL( 2u, rec.GetHistorySize() );

    // A new valid one -> evict [3]
    AddAndFlush( rec, 5, 6, false );
    BOOST_CHECK_EQUAL( 2u, rec.GetHistorySize() );
    rec.GetHistory( 1, entries );
    BOOST_CHECK_EQUAL( "1.1, 1.2, 1.6", GetHistoryTrace( entries ) );
    rec.GetHistory( 3, entries );
    BOOST_CHECK_EQUAL( "", GetHistoryTrace( entries ) );
    rec.GetHistory( 5, entries );
    BOOST_CHECK_EQUAL( "5.6", GetHistoryTrace( entries ) );
}

namespace
{

std::string TraceEntitiesRequests( const ConsignRecorder& rec, int32_t startTick,
        const std::set< uint32_t >& entities )
{
    boost::ptr_vector< sword::LogHistoryEntry > entries;
    GetRequestsFromEntities( rec, entities, startTick, 100, entries );
    return GetHistoryTrace( entries );
}

}  // namespace

BOOST_AUTO_TEST_CASE( TestConsignRecorderEntitiesIndex )
{
    tools::TemporaryDirectory tempDir( "testlogisticplugin-", testOptions.GetTempDir() );
    const tools::Path path = tempDir.Path() / "logistics";

    // request [2] should be discarded
    ConsignRecorder rec( path, 1024*1024, 1000, 4 );
    AddAndFlush( rec, 2, 1, false, MakeSet( 10 ));
    AddAndFlush( rec, 2, 1, true, MakeSet( 30 ));
    AddAndFlush( rec, 1, 1, false, MakeSet( 30 ));
    AddAndFlush( rec, 3, 2, false, MakeSet( 10, 20 ));
    AddAndFlush( rec, 1, 3, true, MakeSet( 20 ));
    AddAndFlush( rec, 4, 3, false, MakeSet( 40 ));

    auto check = []( const ConsignRecorder& rec )
    {
        // Check history for single unit
        BOOST_CHECK_EQUAL( "3.2", TraceEntitiesRequests( rec, -1, MakeSet( 10 )));
        BOOST_CHECK_EQUAL( "1.3, 3.2", TraceEntitiesRequests( rec, -1, MakeSet( 20 )));
        BOOST_CHECK_EQUAL( "1.3", TraceEntitiesRequests( rec, -1, MakeSet( 30 )));
        // Missing unit
        BOOST_CHECK_EQUAL( "", TraceEntitiesRequests( rec, -1, MakeSet( 1000 )));
        // Multiple units with one missing
        BOOST_CHECK_EQUAL( "1.3, 3.2", TraceEntitiesRequests( rec, -1, MakeSet( 20, 30, 1000 )));

        // Check history from ticks
        BOOST_CHECK_EQUAL( "", TraceEntitiesRequests( rec, 0, MakeSet( 20, 30, 40 )));
        BOOST_CHECK_EQUAL( "1.1", TraceEntitiesRequests( rec, 1, MakeSet( 20, 30, 40 )));
        BOOST_CHECK_EQUAL( "3.2, 1.1", TraceEntitiesRequests( rec, 2, MakeSet( 20, 30, 40 )));
        BOOST_CHECK_EQUAL( "4.3, 1.3, 3.2", TraceEntitiesRequests( rec, 3, MakeSet( 20, 30, 40 )));
        BOOST_CHECK_EQUAL( "4.3, 1.3, 3.2", TraceEntitiesRequests( rec, 4, MakeSet( 20, 30, 40 )));
    };

    // Check from initial recorder
    check( rec );

    // Reload and check again
    ConsignRecorder reloaded( path, 1000, 4 );
    check( reloaded );

    // Cannot write to a read-only recorder (sic)
    BOOST_CHECK_THROW( AddAndFlush( reloaded, 4, 3, false, MakeSet( 40 )), tools::Exception );

    // Add new records, refresh the read-only store and check again
    AddAndFlush( rec, 5, 4, false, MakeSet( 20 ));
    AddAndFlush( rec, 4, 4, true, MakeSet( 20 ));
    reloaded.ReadNewEntries();
    BOOST_CHECK_EQUAL( "4.3, 1.3", TraceEntitiesRequests( reloaded, 3, MakeSet( 20 )));
    BOOST_CHECK_EQUAL( "4.4, 5.4, 1.3", TraceEntitiesRequests( reloaded, -1, MakeSet( 20 )));

    // Reload with nothing new
    reloaded.ReadNewEntries();
    BOOST_CHECK_EQUAL( "4.3, 1.3", TraceEntitiesRequests( reloaded, 3, MakeSet( 20 )));
    BOOST_CHECK_EQUAL( "4.4, 5.4, 1.3", TraceEntitiesRequests( reloaded, -1, MakeSet( 20 )));
}

BOOST_AUTO_TEST_CASE( TestConsignOffsetFile )
{
    tools::TemporaryDirectory tempDir( "testlogisticplugin-", testOptions.GetTempDir() );
    const tools::Path path = tempDir.Path() / "offsets";

    uint32_t file = 0;
    uint32_t index = 0;

    // File does not exist
    BOOST_CHECK( WriteOffsetFile( path, 10, 42 ) );
    BOOST_CHECK( ReadOffsetFile( path, file, index ) );
    BOOST_CHECK_EQUAL( 10u, file );
    BOOST_CHECK_EQUAL( 42u, index );

    // File already exists
    BOOST_CHECK( WriteOffsetFile( path, 11, 43 ) );
    BOOST_CHECK( ReadOffsetFile( path, file, index ) );
    BOOST_CHECK_EQUAL( 11u, file );
    BOOST_CHECK_EQUAL( 43u, index );

    // Failure, target file is a directory
    path.Remove();
    path.CreateDirectories();
    BOOST_CHECK( !WriteOffsetFile( path, 12, 44 ) );
    BOOST_CHECK( !ReadOffsetFile( path, file, index ) );
    BOOST_CHECK_EQUAL( 0u, file );
    BOOST_CHECK_EQUAL( 0u, index );
}
