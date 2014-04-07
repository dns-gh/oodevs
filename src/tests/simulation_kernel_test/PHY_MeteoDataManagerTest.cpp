// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/Meteo/PHY_Ephemeride.h"
#include "simulation_kernel/Meteo/PHY_MeteoDataManager.h"
#include "simulation_kernel/Meteo/RawVisionData/PHY_RawVisionData.h"
#include "simulation_kernel/MIL_Time_ABC.h"
#include "simulation_kernel/Network/NET_Publisher_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "MT_Tools/MT_Rect.h"
#include "StubTER_World.h"
#include <meteo/Meteo.h>
#include <tools/Helpers.h>
#include <xeumeuleu/xml.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace bpt = boost::posix_time;

namespace
{

class FakePublisher : public NET_Publisher_ABC
{
public:
    FakePublisher() {}
    virtual ~FakePublisher() {}

    virtual void Send( sword::SimToClient& ) {}
};

} // namespace

BOOST_AUTO_TEST_CASE( phy_ephemeride_test )
{
    const std::string invalid[] =
    {
        // Invalid day, night and moon lighting
        "<ephemerides day-lighting=\"invalid\" night-lighting=\"NuitPleineLune\" "
            "sunrise=\"7h30m0s\" sunset=\"21h30m0s\"/>",
        "<ephemerides day-lighting=\"JourSansNuage\" night-lighting=\"invalid\" "
            "sunrise=\"7h30m0s\" sunset=\"21h30m0s\"/>",
        "<ephemerides moon=\"invalid\" sunrise=\"7h30m0s\" sunset=\"21h30m0s\"/>",
        // Invalid sunrise and sunset
        "<ephemerides day-lighting=\"JourSansNuage\" night-lighting=\"NuitPleineLune\" "
            "sunrise=\"25h30m0s\" sunset=\"21h30m0s\"/>",
        "<ephemerides day-lighting=\"JourSansNuage\" night-lighting=\"NuitPleineLune\" "
            "sunrise=\"7h30m0s\" sunset=\"21h60m0s\"/>",
    };
    for( size_t i = 0; i != COUNT_OF( invalid ); ++i )
    {
        xml::xistringstream xis( invalid[i] );
        BOOST_CHECK_THROW( ReadEphemeride( xis, 0 ), tools::Exception );
    }

    // Valid day/night instance
    const std::string e1 = "<ephemerides day-lighting=\"JourSansNuage\" "
        "night-lighting=\"NuitPleineLune\" sunrise=\"7h00m\" sunset=\"20h00m0s\"/>";
    xml::xistringstream xis( e1 );
    auto eph = ReadEphemeride( xis, 0 );
    BOOST_REQUIRE( eph );
    BOOST_CHECK( eph->IsNight() );

    // Check night switches
    BOOST_CHECK( !eph->UpdateNight( 3600*7 - 1 ) );
    BOOST_CHECK( eph->IsNight() );
    BOOST_CHECK( eph->UpdateNight( 3600*7 ) );
    BOOST_CHECK( !eph->IsNight() );

    BOOST_CHECK( !eph->UpdateNight( 3600*20 - 1 ) );
    BOOST_CHECK( !eph->IsNight() );
    BOOST_CHECK( eph->UpdateNight( 3600*20 ) );
    BOOST_CHECK( eph->IsNight() );

    // Moon instance for the fun
    const std::string e2 = "<ephemerides moon=\"JourSansNuage\" "
        "sunrise=\"7h00m\" sunset=\"20h00m0s\"/>";
    xml::xistringstream xis2( e2 );
    auto moon = ReadEphemeride( xis, 0 );
    BOOST_REQUIRE( moon );
}

namespace
{

boost::shared_ptr< PHY_MeteoDataManager > CreateMeteoManager(
        const boost::shared_ptr< TER_World >& world )
{
    xml::xistringstream xis(
    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
    "<weather model-version=\"4.8.2\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:noNamespaceSchemaLocation=\"schemas/5.3.0/exercise/weather.xsd\">"
        "<ephemerides day-lighting=\"JourSansNuage\" night-lighting=\"NuitPleineLune\" "
            "sunrise=\"7h30m0s\" sunset=\"21h30m0s\"/>"
        "<theater>"
            "<wind direction=\"90\" speed=\"30\"/>"
            "<cloud-cover ceiling=\"9700\" density=\"4\" floor=\"800\"/>"
            "<temperature value=\"20\"/>"
            "<precipitation value=\"Crachin\"/>"
        "</theater>"
        "<local-weather/>"
    "</weather>"
    );
    tools::Path detectionFile = testOptions.GetDataPath(
            "../../data/terrains/Paris_Est/Detection/detection.dat" );
    return boost::make_shared< PHY_MeteoDataManager >(
            world, xis, detectionFile, 0, 10 );
}

std::string MakeSimTime( uint32_t time )
{
    return bpt::to_iso_string( bpt::from_time_t( time ) );
}

boost::shared_ptr< const weather::Meteo > AddWeather( const TER_World& world,
    PHY_MeteoDataManager& man, double top, double left, double bottom, double right,
    int start, int end )
{
    std::string bottomRight, topLeft;
    world.SimToMosMgrsCoord( MT_Vector2D( left, top ), topLeft );
    world.SimToMosMgrsCoord( MT_Vector2D( right, bottom ), bottomRight );
    const auto startTime = MakeSimTime( start );
    const auto endTime = MakeSimTime( end );

    xml::xistringstream xis(
        "<local bottom-right=\"" + bottomRight + "\" top-left=\"" + topLeft + "\" "
        "  start-time=\"" + startTime + "\" end-time=\"" + endTime + "\">"
        "  <wind direction=\"0\" speed=\"0\"/>"
        "  <cloud-cover ceiling=\"0\" density=\"0\" floor=\"0\"/>"
        "  <temperature value=\"20\"/>"
        "  <precipitation value=\"PasDePrecipitation\"/>"
        "</local>" );
    return man.AddLocalWeather( xis );
}

}  // namespace

BOOST_AUTO_TEST_CASE( phy_meteodatamanager_weather_creation_destruction )
{
    const auto world = CreateWorld( "worldwide/tests/EmptyParis-ML" );
    FakePublisher publisher;
    const auto ext = world->GetExtent();
    BOOST_CHECK_EQUAL( MT_Vector2D( 0, 0 ), ext.GetPointUpLeft() );
    BOOST_CHECK_EQUAL( MT_Vector2D( 103265, 134046 ), ext.GetPointDownRight() );

    auto man = CreateMeteoManager( world );

    const auto global = man->GetGlobalWeather();
    BOOST_REQUIRE( global );

    // Cannot remove globale weather
    BOOST_CHECK( !man->RemoveLocalWeather( global->GetId() ) );

    // Remove imaginary weather
    BOOST_CHECK( !man->RemoveLocalWeather( 1234 ) );

    const PHY_RawVisionData& vision = man->GetRawVisionData();

    // 3 active weather in partially overlapping square areas, in ascending
    // start time order. They are on a top-left -> bottom-right diagonal to
    // avoid symmetry bugs.
    const auto w1 = AddWeather( *world, *man, 50000,     0, 20000, 30000, 100, 1100 );
    BOOST_CHECK( w1 );
    const auto w2 = AddWeather( *world, *man, 30000, 20000,     0, 50000, 200, 1200 );
    BOOST_CHECK( w2 );
    const auto w3 = AddWeather( *world, *man, 40000, 10000, 10000, 40000, 300, 1300 );
    BOOST_CHECK( w3 );

    // Quick IsInside() check
    BOOST_CHECK( w1->IsInside( geometry::Point2f( 10000, 30000 ) ));
    BOOST_CHECK( !w1->IsInside( geometry::Point2f( 10000,     0 ) ));

    // Returns a string "wx wy wz ... " of N geographic locations along a line
    // crossing the weather set, each wx being the currently active weather
    // at this location.
    auto check = [&]() -> std::string
    {
        const auto p1 = MT_Vector2D( 5000, 45000 );
        const auto p13 = MT_Vector2D( 15000, 35000 );
        const auto p123 = MT_Vector2D( 25000, 25000 );
        const auto p23 = MT_Vector2D( 35000, 15000 );
        const auto p2 = MT_Vector2D( 45000, 5000 );
        const auto p0 = MT_Vector2D( 55000, 5000 );  // out of weather set
        const MT_Vector2D points[] = { p1, p13, p123, p23, p2, p0 };

        std::string res  = "";
        for( size_t i = 0; i != COUNT_OF( points ); ++i )
        {
            if( i > 0 )
                res += " ";
            const auto& w = vision.GetWeather( points[i] );
            if( &w == w1.get() )
                res += "w1";
            else if( &w == w2.get() )
                res += "w2";
            else if( &w == w3.get() )
                res += "w3";
            else if( &w == global.get() )
                res += "g0";
            else
                res += "??";
        }
        return res;
    };

    // Too early, none is activated
    man->Update( 0 );
    BOOST_CHECK_EQUAL( "g0 g0 g0 g0 g0 g0", check() );

    // Activate w1
    man->Update( 150 );
    BOOST_CHECK_EQUAL( "w1 w1 w1 g0 g0 g0", check() );

    // Activate w2
    man->Update( 250 );
    BOOST_CHECK_EQUAL( "w1 w1 w2 w2 w2 g0", check() );

    // Activate w3
    man->Update( 350 );
    BOOST_CHECK_EQUAL( "w1 w3 w3 w3 w2 g0", check() );

    // Disable w1
    man->Update( 1150 );
    BOOST_CHECK_EQUAL( "g0 w3 w3 w3 w2 g0", check() );

    // Disable w2
    man->Update( 1250 );
    BOOST_CHECK_EQUAL( "g0 w3 w3 w3 g0 g0", check() );

    // Disable w3
    man->Update( 1350 );
    BOOST_CHECK_EQUAL( "g0 g0 g0 g0 g0 g0", check() );

    // Test manual removal
    man->Update( 350 );
    BOOST_CHECK_EQUAL( "w1 w3 w3 w3 w2 g0", check() );
    BOOST_CHECK( man->RemoveLocalWeather( w1->GetId() ) );
    // Nothing changed, need to call Update() first
    BOOST_CHECK_EQUAL( "w1 w3 w3 w3 w2 g0", check() );
    man->Update( 350 );
    BOOST_CHECK_EQUAL( "g0 w3 w3 w3 w2 g0", check() );

    BOOST_CHECK( man->RemoveLocalWeather( w2->GetId() ) );
    man->Update( 350 );
    BOOST_CHECK_EQUAL( "g0 w3 w3 w3 g0 g0", check() );

    BOOST_CHECK( man->RemoveLocalWeather( w3->GetId() ) );
    man->Update( 350 );
    BOOST_CHECK_EQUAL( "g0 g0 g0 g0 g0 g0", check() );
}
