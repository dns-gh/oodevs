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
#include "3a/FunctionFactory.h"
#include "3a/Task.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include "xeumeuleu/xml.h"
#include "game_asn/asn.h"

using namespace mockpp;

namespace
{
    ASN1T_MsgsSimToClient OperationalState( unsigned opstate, unsigned long id )
    {
        static ASN1T_MsgUnitAttributes attributes;
        attributes.m.etat_operationnel_brutPresent = 1;
        attributes.etat_operationnel_brut = opstate;
        attributes.oid = id;
        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_unit_attributes;
        result.msg.u.msg_unit_attributes = &attributes;
        return result;
    }
    ASN1T_MsgsSimToClient BeginTick()
    {
        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_control_begin_tick;
        return result;
    }
    ASN1T_MsgsSimToClient EndTick()
    {
        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_control_end_tick;
        return result;
    }

    class MockPublisher : public dispatcher::ClientPublisher_ABC, public mockpp::ChainableMockObject
    {
    public:
        MockPublisher()
            : mockpp::ChainableMockObject( "MockPublisher", 0 )
            , Send_mocker( "Send", this ) {}

        virtual void Send( const ASN1T_MsgsSimToClient& ) { }
        virtual void Send( const ASN1T_MsgsAuthenticationToClient& ) { }
        virtual void Send( const ASN1T_MsgsReplayToClient& ) { }
        virtual void Send( const ASN1T_MsgsAarToClient& msg )
        {
            const ASN1T_MsgIndicatorResult& result = *msg.msg.u.msg_indicator_result;
            for( unsigned i = 0; i < result.values.n; ++i )
                Send_mocker.forward( result.values.elem[i] );
        };

        mockpp::ChainableMockMethod< void, double > Send_mocker;
    };

    template< typename Mocker, std::size_t N >
    void MakeExpectation( Mocker& mocker, double (&data)[N], double margin = 0 )
    {
        for( unsigned i = 0; i < N; ++i )
            mocker.expects( once() ).with( eq( data[i], margin ) );
    }
}
// -----------------------------------------------------------------------------
// Name: Facade_TestOperationalState
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestOperationalState )
{
    const std::string input =
    "<indicator>"
        "<extract function='operational-state' id='opstate'/>"
        "<reduce type='float' function='select' input='opstate' key='2' id='myopstate'/>"
        "<plot input='myopstate' type='float'/>"
    "</indicator>";
    xml::xistringstream xis( input );

    FunctionFactory facade;
    boost::shared_ptr< Task > task( facade.CreateTask( 42, xis ) );

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

    double expectedResult[] = { 0.25, 0.25, 0.75, 0.75 };
    MockPublisher publisher;
    MakeExpectation( publisher.Send_mocker, expectedResult );
    task->Commit( publisher );
    publisher.verify();
}

namespace
{
    ASN1T_MsgsSimToClient MakePosition( const char* position, unsigned long id )
    {
        static ASN1T_MsgUnitAttributes attributes;
        attributes.m.positionPresent = 1;
        attributes.position = position;
        attributes.oid = id;
        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_unit_attributes;
        result.msg.u.msg_unit_attributes = &attributes;
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
    const std::string input =
    "<indicator>"
        "<extract function='position' id='position'/>"
        "<reduce type='position' function='select' input='position' key='1' id='position1'/>"
        "<reduce type='position' function='select' input='position' key='2' id='position2'/>"
        "<transform function='distance' input='position1,position2' id='distance'/>"
        "<plot input='distance' type='float'/>"
    "</indicator>";
    xml::xistringstream xis( input );

    FunctionFactory facade;
    boost::shared_ptr< Task > task( facade.CreateTask( 42, xis ) );
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
    MockPublisher publisher;
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit( publisher );
    publisher.verify();
}


// -----------------------------------------------------------------------------
// Name: Facade_TestTypeInstanciationIsVerifiedAtRuntime
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestTypeInstanciationIsVerifiedAtRuntime )
{
    const std::string input =
    "<indicator>"
        "<extract function='position' id='position'/>"
        "<reduce type='position' function='sum' input='position' id='position2'/>" // summing positions
    "</indicator>";
    xml::xistringstream xis( input );
    FunctionFactory facade;
    BOOST_CHECK_THROW( facade.CreateTask( 42, xis ), std::invalid_argument );
}

namespace
{
    ASN1T_MsgsSimToClient CreateConsign( unsigned long id, unsigned long unit_id = 0  )
    {
        static ASN1T_MsgLogMaintenanceHandlingCreation creation;
        creation.oid_consigne = id;
        creation.oid_pion = unit_id;

        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_log_maintenance_handling_creation;
        result.msg.u.msg_log_maintenance_handling_creation = &creation;
        return result;
    }

    ASN1T_MsgsSimToClient DestroyConsign( unsigned long id )
    {
        static ASN1T_MsgLogMaintenanceHandlingDestruction destruction;
        destruction.oid_consigne = id;

        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_log_maintenance_handling_destruction;
        result.msg.u.msg_log_maintenance_handling_destruction = &destruction;
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
    const std::string input =
    "<indicator>"
        "<extract function='maintenance-handling-unit' id='consigns'/>"
        "<reduce type='unsigned long' function='count' input='consigns' id='count'/>"
        "<plot input='count' type='unsigned'/>"
    "</indicator>";
    xml::xistringstream xis( input );

    FunctionFactory facade;
    boost::shared_ptr< Task > task( facade.CreateTask( 42, xis ) );

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

    MockPublisher publisher;
    double expectedResult[] = { 1., 2., 2., 1., 1. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit( publisher );
    publisher.verify();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestNumberOfBreakdownsWithUnitFilter
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestNumberOfBreakdownsWithUnitFilter )
{
    const std::string input =
    "<indicator>"
        "<extract function='maintenance-handling-unit' id='consigns'/>"
        "<transform function='is-one-of' type='unsigned long' select='12,42' input='consigns' id='selected-consigns'/>"
        "<transform function='filter' type='unsigned long' input='selected-consigns,consigns' id='the-consigns'/>"
        "<reduce type='unsigned long' function='count' input='the-consigns' id='count'/>"
        "<plot input='count' type='unsigned'/>"
    "</indicator>";
    xml::xistringstream xis( input );

    FunctionFactory facade;
    boost::shared_ptr< Task > task( facade.CreateTask( 42, xis ) );

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

    MockPublisher publisher;
    double expectedResult[] = { 2., 3., 3., 2., 2. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit( publisher );
    publisher.verify();
}

namespace
{
    ASN1T_MsgsSimToClient CreateDirectFire( unsigned fire_id, unsigned long firer )
    {
        static ASN1T_MsgStartUnitFire fire;
        fire.fire_oid  = fire_id;
        fire.firer_oid = firer;

        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_start_unit_fire;
        result.msg.u.msg_start_unit_fire = &fire;
        return result;
    }

    ASN1T_MsgsSimToClient StopFire( unsigned fire_id )
    {
        static ASN1T_MsgStopUnitFire fire;
        fire.fire_oid  = fire_id;

        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_stop_unit_fire;
        result.msg.u.msg_stop_unit_fire = &fire;
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
    const std::string input =
    "<indicator>"
        "<extract function='direct-fire-unit' id='fires'/>"
        "<transform function='is-one-of' type='unsigned long' select='12,42' input='fires' id='selected-fires'/>"
        "<transform function='filter' type='unsigned long' input='selected-fires,fires' id='the-fires'/>"
        "<reduce type='unsigned long' function='count' input='the-fires' id='count'/>"
        "<plot input='count' type='unsigned'/>"
    "</indicator>";
    xml::xistringstream xis( input );

    FunctionFactory facade;
    boost::shared_ptr< Task > task( facade.CreateTask( 42, xis ) );

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

    MockPublisher publisher;
    double expectedResult[] = { 1., 0., 0., 1., 1. };
    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
    task->Commit( publisher );
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
//BOOST_AUTO_TEST_CASE( Facade_TestInflictedComponentDamagesFromDirectFire )
//{
//    const std::string input =
//    "<indicator>"
//        "<extract function='fire-component-damage' id='damages'/>"
//        "<extract function='direct-fire-unit' id='units'/>"
//        "<transform function='is-one-of' type='unsigned long' select='12,42' input='units' id='selected-fires'/>"
//        "<transform function='filter' type='float' input='selected-fires,damages' id='the-damages'/>"
//        "<reduce type='float' function='sum' input='the-damages' id='sum'/>"
//        "<plot input='sum' type='float'/>"
//    "</indicator>";
//    xml::xistringstream xis( input );
//
//    FunctionFactory facade;
//    boost::shared_ptr< Task > task( facade.CreateTask( 42, xis ) );
//
//    MockPublisher publisher;
//    double expectedResult[] = { 5., 0., 0., 3., 0. };
//    MakeExpectation( publisher.Send_mocker, expectedResult, 0.01 );
//    task->Commit( publisher );
//    publisher.verify();
//}

// $$$$ AGE 2007-09-10: ressources consommées => variation <0
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_RESSOURCES_CONSOMMEES_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Quantites)]
//(
//  @TDebut DATETIME,           -- Date Debut
//  @TFin DATETIME,             -- Date Fin
//  @Unites unit_id_tablename,      -- Unites concernees
//  @Ressources ressource_id_tablename, -- Ressource … comptabiliser
//  @Resultat tablename OUTPUT      -- Table de resultats
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

// $$$$ AGE 2007-09-10: pourcentage efficacité => nombre de tirs avec degat / nombre de tirs
//CREATE PROCEDURE dbo.[AAAT_MELEE-APPUI_POURCENTAGE_EFFICACITE_DES_TIRS_INDIRECTS_D_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2]
//(
//  @TDebut DATETIME,           -- DATE DEBUT
//  @TFin DATETIME,         -- DATE FIN
//  @Unites  unit_id_tablename,     -- UNITES CONCERNEES
//  @Resultat tablename OUTPUT      -- TABLE DES RESULTATS
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
