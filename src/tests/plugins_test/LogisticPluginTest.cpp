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
#include "logistic_plugin/ConsignResolver_ABC.h"
#include "protocol/Protocol.h"
#include <tools/StdFileWrapper.h>
#include <tools/TemporaryDirectory.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
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

boost::shared_ptr<LogisticPlugin> CreateLogisticPlugin( const tools::Path& tempDir )
{
    boost::shared_ptr< SimpleNameResolver > nameResolver( new SimpleNameResolver() );
    return boost::shared_ptr< LogisticPlugin >( new LogisticPlugin( nameResolver,
        tempDir / "maintenance",
        tempDir / "supply",
        tempDir / "funeral",
        tempDir / "medical" ) );
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

void CheckRegexps( const std::vector< LogFile >& logFiles, const tools::Path::T_Paths& files )
{
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
    boost::shared_ptr<LogisticPlugin> plugin = CreateLogisticPlugin( tempDir.Path() );

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
            maint->mutable_equipement()->set_id( 10 );
            maint->mutable_breakdown()->set_id( 11 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 1 );

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
            maint->mutable_equipement()->set_id( 20 );
            maint->mutable_breakdown()->set_id( 21 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 2 );

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
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 2 );

        {
            sword::SimToClient m;
            sword::LogMaintenanceHandlingDestruction* maint = m.mutable_message()->mutable_log_maintenance_handling_destruction();
            maint->mutable_request()->set_id( 7 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 1 );

        {
            sword::SimToClient m;
            sword::LogMaintenanceHandlingDestruction* maint = m.mutable_message()->mutable_log_maintenance_handling_destruction();
            maint->mutable_request()->set_id( 17 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Maintenance ), 0 );
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
            medic->set_rank( static_cast< sword::EnumHumanRank >( 0 ) );
            medic->set_wound( static_cast< sword::EnumHumanWound >( 0 ) );
            medic->set_nbc_contaminated( true );
            medic->set_mental_wound( false );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Medical ), 1 );

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
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Medical ), 1 );

        {
            sword::SimToClient m;
            sword::LogMedicalHandlingUpdate* medic = m.mutable_message()->mutable_log_medical_handling_update();
            medic->mutable_request()->set_id( 7 );
            medic->set_state( static_cast< sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus >( 1 ) );
            medic->set_current_state_end_tick( 400 );
            plugin->Receive( m, day2 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Medical ), 1 );

        {
            sword::SimToClient m;
            sword::LogMedicalHandlingUpdate* medic = m.mutable_message()->mutable_log_medical_handling_update();
            medic->mutable_request()->set_id( 7 );
            medic->set_state( static_cast< sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus >( 2 ) );
            medic->set_current_state_end_tick( 500 );
            plugin->Receive( m, day3 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Medical ), 1 );

        {
            sword::SimToClient m;
            sword::LogMedicalHandlingDestruction* medic = m.mutable_message()->mutable_log_medical_handling_destruction();
            medic->mutable_request()->set_id( 7 );
            plugin->Receive( m, day3 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Medical ), 0 );
    }

    {
        bg::date day1( bg::from_string( "2005/02/20" ) );
        plugin->SetMaxLinesInFile( 3 );

        {
            sword::SimToClient m;
            sword::LogFuneralHandlingCreation* funeral = m.mutable_message()->mutable_log_funeral_handling_creation();
            funeral->mutable_request()->set_id( 7 );
            funeral->mutable_unit()->set_id( 8 );
            funeral->set_rank( static_cast< sword::EnumHumanRank >( 0 ) );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Funeral ), 1 );

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
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Funeral ), 1 );

        {
            sword::SimToClient m;
            sword::LogFuneralHandlingDestruction* funeral = m.mutable_message()->mutable_log_funeral_handling_destruction();
            funeral->mutable_request()->set_id( 7 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Funeral ), 0 );
    }

    {
        bg::date day1( bg::from_string( "2008/09/21" ) );
        bg::date day2( bg::from_string( "2008/09/22" ) );
        plugin->SetMaxLinesInFile( 3 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingCreation* supply = m.mutable_message()->mutable_log_supply_handling_creation();
            supply->mutable_request()->set_id( 7 );
            supply->mutable_supplier()->mutable_automat()->set_id( 8 );
            supply->mutable_transporters_provider()->mutable_automat()->set_id( 9 );
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Supply ), 1 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingUpdate* supply = m.mutable_message()->mutable_log_supply_handling_update();
            supply->mutable_request()->set_id( 7 );
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
                    res->set_convoyed( 50 );
                    res->set_granted( 75 );
                    res->set_requested( 120 );
                }
            }
            plugin->Receive( m, day1 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Supply ), 1 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingUpdate* supply = m.mutable_message()->mutable_log_supply_handling_update();
            supply->mutable_request()->set_id( 8 );
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
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Supply ), 2 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingUpdate* supply = m.mutable_message()->mutable_log_supply_handling_update();
            supply->mutable_request()->set_id( 7 );
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
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Supply ), 2 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingUpdate* supply = m.mutable_message()->mutable_log_supply_handling_update();
            supply->mutable_request()->set_id( 8 );
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
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Supply ), 2 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingDestruction* supply = m.mutable_message()->mutable_log_supply_handling_destruction();
            supply->mutable_request()->set_id( 7 );
            plugin->Receive( m, day2 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Supply ), 1 );

        {
            sword::SimToClient m;
            sword::LogSupplyHandlingDestruction* supply = m.mutable_message()->mutable_log_supply_handling_destruction();
            supply->mutable_request()->set_id( 8 );
            plugin->Receive( m, day2 );
        }
        BOOST_CHECK_EQUAL( plugin->GetConsignCount( LogisticPlugin::eLogisticType_Supply ), 0 );
    }

    tools::Path::T_Paths files = tempDir.Path().ListElements( tools::Path::T_Functor(), true, false, true );
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
        expectedLines.push_back( "7 ; 300 ; GDH2 ;  ; automat_8 ; automat_9 ;  ;  ; " );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; automat_11 ; automat_8 ; automat_9 ; agent_10 ; supply_0 ; 1000 ; resource_0 ; 300 ; 200 ; 100 ; resource_1 ; 310 ; 210 ; 110" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; automat_12 ; automat_8 ; automat_9 ; agent_10 ; supply_0 ; 1000 ; resource_3 ; 120 ; 75 ; 50" );
        expecteds.push_back( LogFile( L"^.*/supply\\.20080921\\.0\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; recipient ; provider ; transport provider ; conveyor ; state ; state end tick ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed" );
        expectedLines.push_back( "8 ; 300 ; GDH2 ; automat_13 ;  ;  ; agent_10 ; supply_0 ; 1000 ; resource_0 ; 300 ; 200 ; 100" );
        expecteds.push_back( LogFile( L"^.*/supply\\.20080921\\.1\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; recipient ; provider ; transport provider ; conveyor ; state ; state end tick ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; automat_11 ; automat_8 ; automat_9 ; agent_10 ; supply_1 ; 2000 ; resource_0 ; 300 ; 200 ; 100 ; resource_1 ; 310 ; 210 ; 110" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; automat_12 ; automat_8 ; automat_9 ; agent_10 ; supply_1 ; 2000" );
        expectedLines.push_back( "7 ; 300 ; GDH2 ; automat_14 ; automat_8 ; automat_9 ; agent_10 ; supply_1 ; 2000 ; resource_3 ; 120 ; 75 ; 50" );
        expecteds.push_back( LogFile( L"^.*/supply\\.20080922\\.0\\.csv$", expectedLines ) );
    }

    {
        T_Lines expectedLines;
        expectedLines.push_back( "request id ; tick ; GDH ; recipient ; provider ; transport provider ; conveyor ; state ; state end tick ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed ; resource type ; requested ; granted ; conveyed" );
        expectedLines.push_back( "8 ; 300 ; GDH2 ; automat_13 ;  ;  ; agent_10 ; supply_1 ; 2000 ; resource_0 ; 300 ; 200 ; 100" );
        expectedLines.push_back( "8 ; 300 ; GDH2 ; automat_17 ;  ;  ; agent_10 ; supply_1 ; 2000 ; resource_3 ; 180 ; 85 ; 80" );
        expectedLines.push_back( "8 ; 300 ; GDH2 ; automat_18 ;  ;  ; agent_10 ; supply_1 ; 2000 ; resource_4 ; 24 ; 23 ; 21" );
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
    funeral->set_rank( static_cast< sword::EnumHumanRank >( 0 ) );
    plugin->Receive( m,  bg::from_string( "2005/02/20" ) );
}

} // namespace

BOOST_AUTO_TEST_CASE( TestLogisticPluginRestart )
{
    tools::TemporaryDirectory tempDir( "testlogisticplugin-", testOptions.GetTempDir() );
    boost::shared_ptr<LogisticPlugin> plugin = CreateLogisticPlugin( tempDir.Path() );

    plugin->SetMaxLinesInFile( 1 );
    for( int i = 0; i < 2; ++i )
        PushFuneralMessage( plugin.get() );

    // Recreate it on multiple files
    plugin = CreateLogisticPlugin( tempDir.Path() );
    PushFuneralMessage( plugin.get() );

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
