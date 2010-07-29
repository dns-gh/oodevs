// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "3a_test_pch.h"
#include "3a/DispatchedFunctionHelper.h"
#include "3a/Attributes.h"
#include "3a/IdentifierValue.h"
#include "3a/AarFacade.h"
#include "3a/Task.h"
#include "protocol/ClientPublisher_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <geocoord/geodetic.h>
#include <geocoord/mgrs.h>
#include <boost/lexical_cast.hpp>
#include "protocol/protocol.h"

using namespace Common;
using namespace MsgsSimToClient;
using namespace MsgsMessengerToClient;
using namespace MsgsDispatcherToClient;
using namespace MsgsAuthenticationToClient;
using namespace MsgsReplayToClient;
using namespace MsgsAarToClient;

using namespace mockpp;

namespace
{
    MsgSimToClient OperationalState( unsigned opstate, unsigned long id )
    {
        MsgSimToClient result;
        MsgUnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
        attributes.set_oid( id );
        attributes.set_etat_operationnel_brut( opstate );
        return result;
    }
    MsgSimToClient BeginTick()
    {
        MsgSimToClient result;
        MsgControlBeginTick beginTick;
        *result.mutable_message()->mutable_control_begin_tick() = beginTick ;
        return result;
    }
    MsgSimToClient EndTick()
    {
        MsgSimToClient result;
        MsgControlEndTick endTick;
        *result.mutable_message()->mutable_control_end_tick() = endTick ;
        return result;

    }

    MsgSimToClient MakeUnitCreation( unsigned long id )
    {
        MsgSimToClient result;
        MsgUnitCreation& message = *result.mutable_message()->mutable_unit_creation();
        message.set_oid( id );
        message.set_type_pion( 42 );
        message.set_nom( "test" );
        message.set_oid_automate( 12 );
        message.set_pc( false );
        return result;
    }

    class MockPublisher : public dispatcher::ClientPublisher_ABC, public mockpp::ChainableMockObject
    {
    public:
        MockPublisher()
            : mockpp::ChainableMockObject( "MockPublisher", 0 )
            , Send_mocker( "Send", this ) {}

        virtual void Send( const MsgSimToClient& ) { }
        virtual void Send( const MsgAuthenticationToClient& ) { }
        virtual void Send( const MsgReplayToClient& ) { }
        virtual void Send( const MsgAarToClient& wrapper )
        {
            const MsgPlotResult& result = wrapper.message().plot_result();
            for( unsigned i = 0; i < result.values_size(); ++i )
                Send_mocker.forward( result.values(i) );
        };
        virtual void Send( const MsgMessengerToClient& msg ) { }
        virtual void Send( const MsgDispatcherToClient& msg ) { }
        virtual std::string GetEndpoint() const { return "";}

        mockpp::ChainableMockMethod< void, double > Send_mocker;
    };

    template< typename Mocker, std::size_t N >
    void MakeExpectation( Mocker& mocker, double (&data)[N], double margin = 0 )
    {
        for( unsigned i = 0; i < N; ++i )
            mocker.expects( once() ).with( eq( data[i], margin ) );
    }

    xml::xistream& UnWrap( xml::xistream& xis )
    {
        xis >> xml::start( "indicator" );
        return xis;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade_TestOperationalState
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestOperationalState )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='operational-state' id='opstate'/>"
                             "    <reduce type='float' function='select' input='opstate' key='2' id='myopstate'/>"
                             "    <result function='plot' input='myopstate' type='float'/>"
                             " </indicator>" );
    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( OperationalState( 50, 1 ) );
    task->Receive( OperationalState( 25, 2 ) );
    task->Receive( OperationalState( 75, 3 ) );
    task->Receive( EndTick() );

    task->Receive( BeginTick() );
    task->Receive( MakeUnitCreation( 51 ) ); //testing unit creation in a tick
    task->Receive( OperationalState( 75, 1 ) );
    task->Receive( OperationalState( 85, 3 ) );
    task->Receive( EndTick() );

    task->Receive( MakeUnitCreation( 52 ) ); //testing unit creation between two ticks

    task->Receive( BeginTick() );
    task->Receive( OperationalState( 75, 2 ) );
    task->Receive( EndTick() );

    task->Receive( BeginTick() );
    task->Receive( EndTick() );

    double expectedResult[] = { 0.25, 0.25, 0.75, 0.75 };
    MakeExpectation( publisher.Send_mocker, expectedResult );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestOperationalStateNormalized
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestOperationalStateNormalized )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='operational-state' id='opstate'/>"
                             "    <reduce type='float' function='select' input='opstate' key='2' id='myopstate'/>"
                             "    <reduce type='float' function='threshold' input='myopstate' thresholds='0.5' values='0,1' id='mynormalizedopstate'/>"
                             "    <result function='plot' input='mynormalizedopstate' type='float'/>"
                             "</indicator>" );
    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( OperationalState( 50, 1 ) );
    task->Receive( OperationalState( 25, 2 ) );
    task->Receive( OperationalState( 75, 3 ) );
    task->Receive( EndTick() );

    task->Receive( BeginTick() );
    task->Receive( OperationalState( 75, 1 ) );
    task->Receive( OperationalState( 85, 3 ) );
    task->Receive( EndTick() );

    task->Receive( BeginTick() );
    task->Receive( OperationalState( 75, 2 ) );
    task->Receive( EndTick() );

    task->Receive( BeginTick() );
    task->Receive( EndTick() );

    double expectedResult[] = { 0, 0, 1, 1 };
    MakeExpectation( publisher.Send_mocker, expectedResult );
    task->Commit();
    publisher.verify();
}

namespace
{
    MsgSimToClient MakePosition( const char* position, unsigned long id )
    {
        MsgSimToClient result;
        MsgUnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
        attributes.set_oid(id);
        geocoord::MGRS mgrs( position );
        geocoord::Geodetic geodetic( mgrs );
        attributes.mutable_position()->set_latitude( geodetic.GetLatitude() * 180 / std::acos( -1. ) );
        attributes.mutable_position()->set_longitude( geodetic.GetLongitude() * 180 / std::acos( -1. ) );
        return result;
    }
}

//CREATE PROCEDURE dbo.[AAAT_GENERAL_DISTANCE_ENTRE_DEUX_UNITES_A_LA_DATE_T]
//(
//  @TDebut DATETIME,       -- Date du calcul
//  @Unite1  unit_id,       -- Premiere d'unites
//  @Unite2  unit_id,       -- Seconde d'unites
//  @Resultat tablename OUTPUT  -- Distance entre les deux unites
//)
// -----------------------------------------------------------------------------
// Name: Facade_TestDistanceBetweenTwoUnits
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestDistanceBetweenTwoUnits )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='position' id='position'/>"
                             "    <reduce type='position' function='select' input='position' key='1' id='position1'/>"
                             "    <reduce type='position' function='select' input='position' key='2' id='position2'/>"
                             "    <transform function='distance' input='position1,position2' id='distance'/>"
                             "    <result function='plot' input='distance' type='float'/>"
                             "</indicator>" ) ;
    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( MakePosition( "31TBN7728449218", 1 ) );
    task->Receive( MakePosition( "31TBN7728449218", 2 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakePosition( "31TBN7728449217", 1 ) );
    task->Receive( MakePosition( "31TBN7728449219", 2 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakePosition( "31TBN7728449216", 1 ) );
    task->Receive( MakePosition( "31TBN7728449220", 2 ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 0., 2., 4. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

namespace
{
    MsgSimToClient MakeMounted( bool mounted, unsigned long id )
    {
        MsgSimToClient result;
        MsgUnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
        attributes.set_oid( id );
        attributes.set_embarque( mounted );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade_TestMountedUnit
// Created: SBO 2010-07-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestMountedUnit )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='mounted' id='m'/>"
                             "    <reduce type='int' function='select' input='m' key='1' id='m1'/>"
                             "    <result function='plot' input='m1' type='float'/>"
                             "</indicator>" ) ;
    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( MakeMounted( true, 1 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeMounted( false, 1 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeMounted( true, 1 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( EndTick() );

    double expectedResult[] = { 0., 1., 0., 0. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestTypeInstanciationIsVerifiedAtRuntime
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestTypeInstanciationIsVerifiedAtRuntime )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='position' id='position'/>"
                             "    <reduce type='position' function='sum' input='position' id='position2'/>" // summing positions
                             "</indicator>" );
    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    BOOST_CHECK_THROW( facade.CreateTask( UnWrap( xis ) ), std::invalid_argument );
}

namespace
{
    MsgSimToClient CreateConsign( unsigned long id, unsigned long unit_id = 0  )
    {
        MsgSimToClient result;
        MsgLogMaintenanceHandlingCreation& creation = *result.mutable_message()->mutable_log_maintenance_handling_creation();
        creation.set_oid_consigne( id );
        creation.set_oid_pion( unit_id ) ;
        return result;
    }

    MsgSimToClient DestroyConsign( unsigned long id )
    {
        MsgSimToClient result;
        MsgLogMaintenanceHandlingDestruction& destruction = *result.mutable_message()->mutable_log_maintenance_handling_destruction();
        destruction.set_oid_consigne( id );
        return result;
    }
}

// $$$$ AGE 2007-09-13: AU_NTI1 => liste de pannes
//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_MATERIELS_AU_NTI1_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,               -- Date
//  @TFin DATETIME,                 -- Date
//  @Unites unit_id_tablename,          -- Unites concernees
//  @Materiels equipment_id_tablename,      -- Materiels a prendre en compte
//  @Resultat tablename OUT             -- Tableau de resultat
//)
// -----------------------------------------------------------------------------
// Name: Facade_TestNumberOfBreakdowns
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestNumberOfBreakdowns )
{
     xml::xistringstream xis( "<indicator>"
                              "    <extract function='maintenance-handling-unit' id='consigns'/>"
                              "    <reduce type='unsigned long' function='count' input='consigns' id='count'/>"
                              "    <result function='plot' input='count' type='unsigned'/>"
                              "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( CreateConsign( 12 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateConsign( 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( DestroyConsign( 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( EndTick() );

    double expectedResult[] = { 1., 2., 2., 2., 1. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestNumberOfBreakdownsWithUnitFilter
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestNumberOfBreakdownsWithUnitFilter )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='maintenance-handling-unit' id='consigns'/>"
                             "    <transform function='is-one-of' type='unsigned long' select='12,42' input='consigns' id='selected-consigns'/>"
                             "    <transform function='filter' type='unsigned long' input='selected-consigns,consigns' id='the-consigns'/>"
                             "    <reduce type='unsigned long' function='count' input='the-consigns' id='count'/>"
                             "    <result function='plot' input='count' type='unsigned'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( CreateConsign( 12, 12 ) );
    task->Receive( CreateConsign( 13, 13 ) );
    task->Receive( CreateConsign( 14, 12 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateConsign( 42, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( DestroyConsign( 13 ) );
    task->Receive( CreateConsign( 15, 15 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( DestroyConsign( 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( EndTick() );

    double expectedResult[] = { 2., 3., 3., 3., 2. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

namespace
{
    MsgSimToClient CreateDirectFire( unsigned fire_id, unsigned long firer )
    {
        MsgSimToClient result;
        MsgStartUnitFire& fire = *result.mutable_message()->mutable_start_unit_fire();
        fire.set_fire_oid( fire_id );
        fire.set_firer_oid( firer );
        return result;
    }

    MsgSimToClient StopFire( unsigned fire_id, unsigned long damage_count = 0 )
    {
        MsgSimToClient result;
        MsgStopUnitFire& fire = *result.mutable_message()->mutable_stop_unit_fire();
        fire.set_fire_oid( fire_id );
        MsgUnitFireDamages& damage = *fire.mutable_units_damages()->add_elem();
        damage.mutable_equipments()->add_elem()->set_unavailable_nbr( damage_count );
        return result;
    }
}

//CREATE PROCEDURE dbo.[AAAT_MELEE-APPUI_NOMBRE_DE_COUPS_DIRECTS_TIRES_PAR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2]
//(
//  @TDebut DATETIME,           -- Date de debut de l'intervalle
//  @TFin DATETIME,         -- Date de fin de l'intervalle
//  @Unites  unit_id_tablename,     -- Groupe d'unites
//  @Resultat tablename OUTPUT      -- Table contenant le résultat
//)

// -----------------------------------------------------------------------------
// Name: Facade_TestNumberOfDirectFiresWithUnitFilter
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestNumberOfDirectFiresWithUnitFilter )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='direct-fire-unit' id='fires'/>"
                             "    <transform function='is-one-of' type='unsigned long' select='12,42' input='fires' id='selected-fires'/>"
                             "    <transform function='filter' type='unsigned long' input='selected-fires,fires' id='the-fires'/>"
                             "    <reduce type='unsigned long' function='count' input='the-fires' id='count'/>"
                             "    <result function='plot' input='count' type='unsigned'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( CreateDirectFire( 12, 12 ) );
    task->Receive( CreateDirectFire( 13, 13 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 12 ) );
    task->Receive( StopFire( 13 ) );
    task->Receive( CreateDirectFire( 14, 14 ) );
    task->Receive( CreateDirectFire( 15, 15 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 13 ) );
    task->Receive( StopFire( 14 ) );
    task->Receive( StopFire( 15 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateDirectFire( 16, 15 ) );
    task->Receive( CreateDirectFire( 17, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 16 ) );
    task->Receive( CreateDirectFire( 18, 13 ) );
    task->Receive( CreateDirectFire( 19, 14 ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 1., 1., 0., 1., 1. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

//CREATE PROCEDURE DBO.[AAAT_MELEE-APPUI_PERTES_EN_MATERIEL_INFLIGEES_PAR_UNE_UNITE_PAR_TIR_DIRECT_ENTRE_T1_ET_T2]
//(
//  @TDebut DATETIME,               -- Date debut
//  @TFin DATETIME,                 -- Date Fin
//  @Unites unit_id_tablename,      -- Groupe d unites
//  @Resultat tablename OUT         -- Table de resultats
//)

// -----------------------------------------------------------------------------
// Name: Facade_TestInflictedComponentDamagesFromDirectFire
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestInflictedComponentDamagesFromDirectFire )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='fire-component-damage' id='damages'/>"
                             "    <extract function='direct-fire-unit' id='units'/>"
                             "    <transform function='is-one-of' type='unsigned long' select='12,42' input='units' id='selected-fires'/>"
                             "    <transform function='filter' type='float' input='selected-fires,damages' id='the-damages'/>"
                             "    <reduce type='float' function='sum' input='the-damages' id='sum'/>"
                             "    <result function='plot' input='sum' type='float'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( CreateDirectFire( 12, 12 ) );
    task->Receive( CreateDirectFire( 13, 13 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 12, 5 ) );
    task->Receive( StopFire( 13 ) );
    task->Receive( CreateDirectFire( 14, 14 ) );
    task->Receive( CreateDirectFire( 15, 15 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 13 ) );
    task->Receive( StopFire( 14 ) );
    task->Receive( StopFire( 15 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateDirectFire( 16, 15 ) );
    task->Receive( CreateDirectFire( 17, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 16 ) );
    task->Receive( CreateDirectFire( 18, 13 ) );
    task->Receive( CreateDirectFire( 19, 14 ) );
    task->Receive( StopFire( 17, 3. ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 0., 5., 0., 0., 3. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestInflictedComponentDamagesFromDirectFireWithComposedFilterOfHell
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestInflictedComponentDamagesFromDirectFireWithComposedFilterOfHell )
{

    xml::xistringstream xis( "<indicator>"
                             "    <extract function='fire-component-damage' id='damages'/>"
                             "    <extract function='direct-fire-unit' id='units'/>"
                             "    <extract function='position' id='positions'/>"
                             "    <constant type='zone' value='circle(31TBN7728449218,31TBN7728449222)' id='circle'/>"
                             "    <transform function='compose' type='position' input='positions,units' id='fire-positions'/>"
                             "    <transform function='contains' input='circle,fire-positions' id='selected-fires'/>"
                             "    <transform function='filter' type='float' input='selected-fires,damages' id='the-damages'/>"
                             "    <reduce type='float' function='sum' input='the-damages' id='sum'/>"
                             "    <result function='plot' input='sum' type='float'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( MakePosition( "31TBN7728449218", 12 ) );
    task->Receive( MakePosition( "31TBN7728449242", 13 ) );
    task->Receive( MakePosition( "31TBN7728449212", 14 ) );
    task->Receive( MakePosition( "31TBN7728449242", 15 ) );
    task->Receive( MakePosition( "31TBN7728449220", 42 ) );
    task->Receive( CreateDirectFire( 12, 12 ) );
    task->Receive( CreateDirectFire( 13, 13 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 12, 5 ) );
    task->Receive( StopFire( 13 ) );
    task->Receive( CreateDirectFire( 14, 14 ) );
    task->Receive( CreateDirectFire( 15, 15 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 13 ) );
    task->Receive( StopFire( 14 ) );
    task->Receive( StopFire( 15 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateDirectFire( 16, 15 ) );
    task->Receive( CreateDirectFire( 17, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 16 ) );
    task->Receive( CreateDirectFire( 18, 13 ) );
    task->Receive( CreateDirectFire( 19, 14 ) );
    task->Receive( StopFire( 17, 3. ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 0., 5., 0., 0., 3. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

namespace
{
    MsgSimToClient MakeResourceVariation( int variation, unsigned long id, unsigned long resourceId = 42 )
    {
        MsgSimToClient result;
        MsgUnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
        attributes.set_oid( id );
        ResourceDotations_ResourceDotation& resource = *attributes.mutable_dotation_eff_ressource()->add_elem();
        resource.set_ressource_id( resourceId );
        resource.set_quantite_disponible( variation );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade_TestAllResources
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestAllResources )
{

    xml::xistringstream xis( "<indicator>"
                             "    <extract function='resources' id='resources'/>"
                             "    <reduce type='int' function='sum' input='resources' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 4212, 12 ) );
    task->Receive( MakeResourceVariation( 1242, 13 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 4200, 12 ) );
    task->Receive( MakeResourceVariation( 1200, 13 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 1000, 14 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 4100, 12 ) );
    task->Receive( EndTick() );


    double expectedResult[] = { 5454, 5400, 5400, 6400, 6300 };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestInstantaneousResourceConsumptionsWithResourceFilter
// $$$$ SBO 2010-06-14: triggers a runtime_error if "Compare" is created with < K, T > instead of < K, NumericValue >
//                      (see TranformationFactory), for now, compare can only be used to compare numeric values (not positions for instance)
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestInstantaneousResourceConsumptionsWithResourceFilter )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='resources' id='resources' dotations='42'/>"
                             "    <transform type='int' function='derivate' input='resources' id='resources-var' period='1'/>"
                             "    <constant type='int' value='0' id='zero'/>"
                             "    <transform function='compare' type='bool' operator='less' input='resources-var,zero' id='test'/>"
                             "    <transform function='filter' type='int' input='test,resources-var' id='consumptions'/>"
                             "    <reduce type='int' function='sum' input='consumptions' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 4212, 12, 42 ) );
    task->Receive( MakeResourceVariation( 1242, 13, 42 ) );
    task->Receive( MakeResourceVariation( 1000, 15, 12 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 4200, 12, 42 ) );
    task->Receive( MakeResourceVariation( 1200, 13, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 5200, 12, 42 ) );
    task->Receive( MakeResourceVariation( 2200, 13, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 1000, 14, 15 ) );
    task->Receive( MakeResourceVariation(  500, 15, 12 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 5100, 12, 42 ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 0, -54, 0, 0, -100 };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestResourceConsumptionsWithResourceFilter
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestResourceConsumptionsWithResourceFilter )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='resources' id='resources' dotations='42'/>"
                             "    <transform type='int' function='derivate' input='resources' id='resources-var' period='1'/>"
                             "    <constant type='int' value='0' id='zero'/>"
                             "    <transform function='compare' type='bool' operator='less' input='resources-var,zero' id='test'/>"
                             "    <transform function='filter' type='int' input='test,resources-var' id='consumptions'/>"
                             "    <reduce type='int' function='sum' input='consumptions' id='sum'/>"
                             "    <transform function='integrate' id='total' input='sum' type='int'/>"
                             "    <result function='plot' input='total' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 4212, 12, 42 ) );
    task->Receive( MakeResourceVariation( 1242, 13, 42 ) );
    task->Receive( MakeResourceVariation( 1000, 15, 12 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 4200, 12, 42 ) );
    task->Receive( MakeResourceVariation( 1200, 13, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 5200, 12, 42 ) );
    task->Receive( MakeResourceVariation( 2200, 13, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 1000, 14, 15 ) );
    task->Receive( MakeResourceVariation(  500, 15, 12 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeResourceVariation( 5100, 12, 42 ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 0, -54, -54, -54, -154 };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

namespace
{
    MsgSimToClient MakeEquipementVariation( int variation[5], unsigned long id, unsigned long equipmentId = 42 )
    {
        MsgSimToClient result;
        MsgUnitAttributes& attributes = *result.mutable_message()->mutable_unit_attributes();
        attributes.set_oid( id );
        EquipmentDotations_EquipmentDotation& equipment = *attributes.mutable_dotation_eff_materiel()->add_elem();
        equipment.set_type_equipement( equipmentId );
        equipment.set_nb_disponibles            ( variation[0] );
        equipment.set_nb_indisponibles          ( variation[1] );
        equipment.set_nb_reparables             ( variation[2] );
        equipment.set_nb_dans_chaine_maintenance( variation[3] );
        equipment.set_nb_prisonniers            ( variation[4] );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade_TestEquipments
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestEquipments )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='equipments' states='available,prisoner' equipments='12,42' id='equipments'/>"
                             "    <reduce type='int' function='sum' input='equipments' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    int base  [5] = { 5, 0, 0, 0, 0 }; //  0
    int prison[5] = { 2, 1, 0, 1, 1 }; // -2
    int casse [5] = { 2, 1, 1, 1, 0 }; // -3

    task->Receive( BeginTick() );
    task->Receive( MakeEquipementVariation( base, 12, 42 ) );
    task->Receive( MakeEquipementVariation( base, 13, 42 ) );
    task->Receive( MakeEquipementVariation( base, 15, 12 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeEquipementVariation( prison, 12, 42 ) );
    task->Receive( MakeEquipementVariation( casse, 13, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeEquipementVariation( casse, 12, 42 ) );
    task->Receive( MakeEquipementVariation( prison, 13, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeEquipementVariation( base, 14, 15 ) );
    task->Receive( MakeEquipementVariation( base, 16, 12 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( MakeEquipementVariation( prison, 12, 42 ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 15, 10, 10, 15, 16 };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestHumans
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestHumans )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='humans' states='operational,nbc' ranks='troopers' id='humans'/>"
                             "    <reduce type='int' function='sum' input='humans' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

// $$$$ _RC_ LGY 2010-07-07: TODO
 
//    MockPublisher publisher;
//    double expectedResult[] = { 0, -54, 0, 0, -100 };
//    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
//    task->Commit( publisher );
//    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestTypeAdaptation
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestTypeAdaptation )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='operational-state' id='opstate'/>"
                             "    <reduce type='int' function='select' input='opstate' key='2' id='myopstate'/>"
                             "    <result function='plot' input='myopstate' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );
}

// -----------------------------------------------------------------------------
// Name: Facade_TestTypeBadLexicalCast
// $$$$ SBO 2010-06-14: triggers bad_lexical_cast if "IsOneOf" is created with < K, T > instead of < K, NumericValue >
//                      (see TranformationFactory), for now, IsOneOf can only be used with numeric values (not positions for instance)
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestBadLexicalCast )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='maintenance-handling-unit' id='1'/>"
                             "    <transform function='is-one-of' id='2' input='1' select='13,67,68,69,71,74,75,76,78,79,80,55,81,82,197,198,199,90,91,92,93,147,56,148,149,150,151,173,174,175,176,177,178,58,59,60,61,63,66' type='bool'/>"
                             "    <extract function='maintenance-handling-unit' id='3'/>"
                             "    <transform function='filter' id='4' input='2,3' type='unsigned long'/>"
                             "    <reduce function='count' id='5' input='4' type='unsigned'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );
}

// -----------------------------------------------------------------------------
// Name: Facade_TestConflicts
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestConflicts )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='direct-fire-unit' id='1'/>"
                             "    <transform function='is-one-of' id='2' input='1' select='55,56' type='unsigned long'/>"
                             "    <extract function='direct-fire-unit' id='3'/>"
                             "    <transform function='filter' id='4' input='2,3' type='unsigned long'/>"
                             "    <reduce function='count' id='5' input='4' type='unsigned long'/>"
                             "    <transform function='integrate' id='6' input='5' type='unsigned long'/>"
                             "    <result function='plot' input='6' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( CreateDirectFire( 12, 55 ) );
    task->Receive( CreateDirectFire( 13, 56 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 12 ) );
    task->Receive( StopFire( 13 ) );
    task->Receive( CreateDirectFire( 14, 55 ) );
    task->Receive( CreateDirectFire( 15, 56 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( StopFire( 14 ) );
    task->Receive( StopFire( 15 ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 2, 4, 8, 10 };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

namespace
{
    MsgSimToClient CreateUnitDetection( unsigned int detector, unsigned int detected, Common::EnumUnitVisibility visibility )
    {
        MsgSimToClient result;
        MsgsSimToClient::MsgUnitDetection& message = *result.mutable_message()->mutable_unit_detection();
        message.set_oid( detector );
        message.set_detected_unit_oid( detected );
        message.set_current_visibility( visibility );
        message.set_max_visibility( visibility );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Facade_TestUnitDetection
// Created: SBO 2010-06-01
// $$$$ SBO 2010-07-28: 1 when detection level (for specified detecting unit) matches specified visibility, otherwise 0
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestUnitDetection )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='detecting-unit' id='1' detected='69' visibility='detected,recognized,identified'/>"
                             "    <reduce type='int' function='select' input='1' key='42' id='2'/>"
                             "    <result function='plot' input='2' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( CreateUnitDetection( 42, 69, Common::identified ) ); // ok
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateUnitDetection( 42, 51, Common::recognized ) ); // irrelevant detected unit
    task->Receive( CreateUnitDetection( 51, 69, Common::detected ) ); // irrelevant detecting unit
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateUnitDetection( 42, 69, Common::invisible ) ); // irrelevant detection level
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateUnitDetection( 42, 69, Common::recognized ) ); // ok
    task->Receive( EndTick() );

    double expectedResult[] = { 1, 1, 0, 1 };//, 1, 1, 1, 1 };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestUnitDetectionWithThreshold
// Created: SBO 2010-06-01
// $$$$ SBO 2010-07-28: 1 when detection level (for any specified detecting units) matches specified visibility, otherwise 0
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestUnitDetectionWithThreshold )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='detecting-unit' id='1' detected='69' visibility='detected,recognized,identified'/>"
                             "    <transform function='domain' id='2' input='1' select='42,51' type='unsigned long'/>"
                             "    <reduce type='int' function='sum' input='2' id='3'/>"
                             "    <reduce type='float' function='threshold' input='3' thresholds='0,1' values='0,1' id='4'/>"
                             "    <result function='plot' input='4' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( CreateUnitDetection( 12, 69, Common::detected ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateUnitDetection( 42, 69, Common::identified ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateUnitDetection( 42, 51, Common::recognized ) );
    task->Receive( CreateUnitDetection( 51, 69, Common::detected ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateUnitDetection( 42, 69, Common::invisible ) );
    task->Receive( CreateUnitDetection( 51, 69, Common::recorded ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( CreateUnitDetection( 42, 69, Common::recognized ) );
    task->Receive( CreateUnitDetection( 51, 69, Common::recognized ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 0, 1, 1, 0, 1 };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestProduct
// Created: SBO 2010-07-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestProduct )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='operational-state' id='opstate'/>"
                             "    <reduce type='float' function='select' input='opstate' key='42' id='myopstate'/>"
                             "    <constant type='float' value='10' id='ten'/>"
                             "    <reduce type='float' function='product' input='myopstate,ten' id='prod'/>"
                             "    <result function='plot' input='prod' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( OperationalState( 100, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( OperationalState( 50, 42 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( OperationalState( 0, 42 ) );
    task->Receive( EndTick() );

    double expectedResult[] = { 10, 5, 0 };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestTimeElapsedBetweenDetectionAndDestruction
// Created: SBO 2010-07-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestTimeElapsedBetweenDetectionAndDestruction )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='detecting-unit' id='detection' detected='69' visibility='detected,recognized,identified'/>"
                             "    <reduce type='int' function='select' input='detection' key='42' id='mydetection'/>"
                             "    <extract function='operational-state' id='opstate'/>"
                             "    <reduce type='float' function='select' input='opstate' key='69' id='myopstate'/>"
                             "    <reduce type='float' function='threshold' input='myopstate' thresholds='0' values='0,1' id='threshopstate'/>"
                             "    <reduce type='float' function='product' input='mydetection,threshopstate' id='prod'/>"
                             "    <transform function='integrate' id='sum' input='prod' type='int'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );

    MockPublisher publisher;
    AarFacade facade( publisher, 42 );
    boost::shared_ptr< Task > task( facade.CreateTask( UnWrap( xis ) ) );

    task->Receive( BeginTick() );
    task->Receive( OperationalState( 100, 69 ) );
    task->Receive( CreateUnitDetection( 42, 69, Common::detected ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( OperationalState( 0, 69 ) );
    task->Receive( EndTick() );
    task->Receive( BeginTick() );
    task->Receive( EndTick() );
    
    double expectedResult[] = { 1, 2, 3, 3, 3 };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit();
    publisher.verify();
}

// $$$$ AGE 2007-09-10: ressources consommées => variation <0
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_RESSOURCES_CONSOMMEES_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Quantites)]
//(
//  @TDebut DATETIME,           -- Date Debut
//  @TFin DATETIME,             -- Date Fin
//  @Unites unit_id_tablename,      -- Unites concernees
//  @Ressources ressource_id_tablename, -- Ressource … comptabiliser
//  @Resultat tablename OUTPUT      -- Table de resultats
//)

// $$$$ AGE 2007-09-10: pourcentage efficacité => nombre de tirs avec degat / nombre de tirs
//CREATE PROCEDURE dbo.[AAAT_MELEE-APPUI_POURCENTAGE_EFFICACITE_DES_TIRS_INDIRECTS_D_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2]
//(
//  @TDebut DATETIME,           -- DATE DEBUT
//  @TFin DATETIME,         -- DATE FIN
//  @Unites  unit_id_tablename,     -- UNITES CONCERNEES
//  @Resultat tablename OUTPUT      -- TABLE DES RESULTATS
//)

//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_RESSOURCES_DISPONIBLES_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,           -- Date Debut
//  @Unites unit_id_tablename,      -- Unites concernees
//  @Ressources ressource_id_tablename, -- Ressource à comptabiliser
//  @Resultat tablename OUTPUT      -- Table de resultats
//)

// $$$$ AGE 2007-09-10: UE-UF-JV => consommation normalisée
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_STOCKS_DISPONIBLES_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(UE-UF-JV)]
//(
//  @TDebut DATETIME,           -- Date Debut
//  @Unites unit_id_tablename,      -- Unites concernees
//  @Ressources ressource_id_tablename, -- Ressource à comptabiliser
//  @Resultat tablename OUTPUT      -- Table de resultats
//)

//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_MATERIELS_AU_NTI2_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,               -- Date
//  @TFin DATETIME,                 -- Date
//  @Unites unit_id_tablename,          -- Unites concernees
//  @Materiels equipment_id_tablename,      -- Materiels a prendre en compte
//  @Resultat tablename OUT             -- Tableau de resultat
//)

//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_MATERIELS_AU_NTI3_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Quantites)]
//(
//  @TDebut DATETIME,           -- Date
//  @TFin DATETIME,             -- Date
//  @Unites unit_id_tablename,      -- Unites concernees
//  @Materiels equipment_id_tablename,  -- Materiels a prendre en compte
//  @Resultat tablename OUT         -- Tableau de resultat
//)

//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_MATERIELS_DISPONIBLES_POUR_UNE_OU_PLUSIEURS_UNITES_DANS_ZONE_ENTRE_T1_ET_T2(Pourcentages)]
//(
//  @TDebut DATETIME,                   -- Date
//  @Unites unit_id_tablename,              -- Unites concernees
//  @Materiels equipment_id_tablename,          -- Materiels a prendre en compte
//  @Zone geometry,                 -- Zone
//  @Resultat tablename OUT             -- Tableau de resultat
//)

// $$$$ AGE 2007-09-10: decedé au combat
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_PERSONNEL_DECEDE_AU_COMBAT_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,               -- Date
//  @TFin DATETIME,                 -- Date
//  @Unites unit_id_tablename,          -- Unites concernees
//  @Resultat tablename OUT             -- Tableau de resultat
//)

// $$$$ AGE 2007-09-10: decedé faute de soins
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_PERSONNEL_DECEDE_FAUTE_DE_SOIN_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Quantite)]
//(
//  @TDebut DATETIME,                   -- Date
//  @TFin DATETIME,                     -- Date
//  @Unites unit_id_tablename,              -- Unites concernees
//  @Resultat tablename OUT                 -- Tableau de resultat
//)

//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_PERSONNEL_DISPONIBLE_POUR_UNE_OU_PLUSIEURS_UNITES_DANS_ZONE_ENTRE_T1_ET_T2(Pourcentage)]
//(
//  @TDebut DATETIME,               -- Date
//  @Unites unit_id_tablename,          -- Unites concernees
//  @Zone geometry,                 -- Zone
//  @Resultat tablename OUT             -- Tableau de resultat
//)

//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_PERSONNEL_TOTAL_BLESSES_POUR_UNE_OU_PLUSIEURS_POPULATIONS_ENTRE_T1_ET_T2_(Quantite)]
//(
//  @TDebut DATETIME,                   -- Date
//  @Populations population_id_tablename,               -- Populations concernees
//  @Resultat tablename OUT                 -- Tableau de resultat
//)

//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_PERTES_MATERIELS_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,                   -- Date
//  @Unites unit_id_tablename,              -- Unites concernees
//  @Materiels equipment_id_tablename,          -- Materiels a prendre en compte
//  @Resultat tablename OUT             -- Tableau de resultat
//)
