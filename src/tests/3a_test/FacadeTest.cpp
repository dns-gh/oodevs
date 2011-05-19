// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "3a_test_pch.h"
#include "3aTestTools.h"
#include "MockStaticModel.h"
#include "MockClientPublisher.h"
#include "3a/DispatchedFunctionHelper.h"
#include "3a/Attributes.h"
#include "3a/IdentifierValue.h"
#include "3a/AarFacade.h"
#include "3a/Task.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>
#include <geocoord/geodetic.h>
#include <geocoord/mgrs.h>

using namespace sword;

namespace
{
    static unsigned int currentTick_ = 1;

    class Fixture
    {
    public:
        Fixture()
            : facade( publisher, 42, model )
        {
            currentTick_ = 1;
        }
        ~Fixture()
        {
        }
        MockClientPublisher publisher;
        MockStaticModel model;
        AarFacade facade;
    };
}

// -----------------------------------------------------------------------------
// Name: Facade_TestOperationalState
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestOperationalState, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='operational-state' id='opstate'/>"
                             "    <reduce type='float' function='select' input='opstate' key='2' id='myopstate'/>"
                             "    <result function='plot' input='myopstate' type='float'/>"
                             " </indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 50, 1 ) );
    task->Receive( TestTools::OperationalState( 25, 2 ) );
    task->Receive( TestTools::OperationalState( 75, 3 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeUnitCreation( 51, 42 ) ); //testing unit creation in a tick
    task->Receive( TestTools::OperationalState( 75, 1 ) );
    task->Receive( TestTools::OperationalState( 85, 3 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::MakeUnitCreation( 52, 42 ) ); //testing unit creation between two ticks
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 75, 2 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 0.25, 0.25, 0.75, 0.75 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestOperationalStateNormalized
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestOperationalStateNormalized, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='operational-state' id='opstate'/>"
                             "    <reduce type='float' function='select' input='opstate' key='2' id='myopstate'/>"
                             "    <reduce type='float' function='threshold' input='myopstate' thresholds='0.5' values='0,1' id='mynormalizedopstate'/>"
                             "    <result function='plot' input='mynormalizedopstate' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 50, 1 ) );
    task->Receive( TestTools::OperationalState( 25, 2 ) );
    task->Receive( TestTools::OperationalState( 75, 3 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 75, 1 ) );
    task->Receive( TestTools::OperationalState( 85, 3 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 75, 2 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 0, 0, 1, 1 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
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
BOOST_FIXTURE_TEST_CASE( Facade_TestDistanceBetweenTwoUnits, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='position' id='position'/>"
                             "    <reduce type='position' function='select' input='position' key='1' id='position1'/>"
                             "    <reduce type='position' function='select' input='position' key='2' id='position2'/>"
                             "    <transform function='distance' input='position1,position2' id='distance'/>"
                             "    <result function='plot' input='distance' type='float'/>"
                             "</indicator>" ) ;
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TBN7728449218", 1 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449218", 2 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TBN7728449217", 1 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449219", 2 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TBN7728449216", 1 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449220", 2 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 0., 2.00212, 4.00424 }; // $$$$ _RC_ LGY 2010-08-10: margin?
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestMountedUnit
// Created: SBO 2010-07-12
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestMountedUnit, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='mounted' id='m'/>"
                             "    <reduce type='int' function='select' input='m' key='1' id='m1'/>"
                             "    <result function='plot' input='m1' type='float'/>"
                             "</indicator>" ) ;
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMounted( true, 1 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMounted( false, 1 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMounted( true, 1 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 0., 1., 0., 0. };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestTypeInstanciationIsVerifiedAtRuntime
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestTypeInstanciationIsVerifiedAtRuntime, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='position' id='position'/>"
                             "    <reduce type='position' function='sum' input='position' id='position2'/>" // summing positions
                             "</indicator>" );
    BOOST_CHECK_THROW( facade.CreateTask( xis >> xml::start( "indicator" ) ), std::invalid_argument );
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
BOOST_FIXTURE_TEST_CASE( Facade_TestNumberOfBreakdowns, Fixture )
{
     xml::xistringstream xis( "<indicator>"
                              "    <extract function='maintenance-handling-unit' id='consigns'/>"
                              "    <reduce type='unsigned long' function='count' input='consigns' id='count'/>"
                              "    <result function='plot' input='count' type='unsigned'/>"
                              "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateConsign( 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateConsign( 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::DestroyConsign( 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 1., 2., 2., 2., 1. };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestNumberOfBreakdownsWithUnitFilter
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestNumberOfBreakdownsWithUnitFilter, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='maintenance-handling-unit' id='consigns'/>"
                             "    <transform function='is-one-of' type='unsigned long' select='12,42' input='consigns' id='selected-consigns'/>"
                             "    <transform function='filter' type='unsigned long' input='selected-consigns,consigns' id='the-consigns'/>"
                             "    <reduce type='unsigned long' function='count' input='the-consigns' id='count'/>"
                             "    <result function='plot' input='count' type='unsigned'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateConsign( 12, 12 ) );
    task->Receive( TestTools::CreateConsign( 13, 13 ) );
    task->Receive( TestTools::CreateConsign( 14, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateConsign( 42, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::DestroyConsign( 13 ) );
    task->Receive( TestTools::CreateConsign( 15, 15 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::DestroyConsign( 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 2., 3., 3., 3., 2. };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

//CREATE PROCEDURE dbo.[AAAT_MELEE-APPUI_NOMBRE_DE_COUPS_DIRECTS_TIRES_PAR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2]
//(
//  @TDebut DATETIME,               -- Date de debut de l'intervalle
//  @TFin DATETIME,                 -- Date de fin de l'intervalle
//  @Unites  unit_id_tablename,     -- Groupe d'unites
//  @Resultat tablename OUTPUT      -- Table contenant le r�sultat
//)

// -----------------------------------------------------------------------------
// Name: Facade_TestNumberOfDirectFiresWithUnitFilter
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestNumberOfDirectFiresWithUnitFilter, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='direct-fire-unit' id='fires'/>"
                             "    <transform function='is-one-of' type='unsigned int' select='12,42' input='fires' id='selected-fires'/>"
                             "    <transform function='filter' type='unsigned int' input='selected-fires,fires' id='the-fires'/>"
                             "    <reduce type='unsigned int' function='count' input='the-fires' id='count'/>"
                             "    <result function='plot' input='count' type='unsigned'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateDirectFire( 12, 12 ) );
    task->Receive( TestTools::CreateDirectFire( 13, 13 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 12, 16 ) );
    task->Receive( TestTools::StopFire( 13, 16 ) );
    task->Receive( TestTools::CreateDirectFire( 14, 14 ) );
    task->Receive( TestTools::CreateDirectFire( 15, 15 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 13, 16 ) );
    task->Receive( TestTools::StopFire( 14, 18 ) );
    task->Receive( TestTools::StopFire( 15, 19 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateDirectFire( 16, 15 ) );
    task->Receive( TestTools::CreateDirectFire( 17, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 16, 16 ) );
    task->Receive( TestTools::CreateDirectFire( 18, 13 ) );
    task->Receive( TestTools::CreateDirectFire( 19, 14 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 1., 1., 0., 1., 1. };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestNumberOfIndirectFiresWithUnitFilter
// Created: FPO 2011-05-05
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestNumberOfIndirectFiresWithUnitFilter, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='indirect-fire-unit' id='fires'/>"
                             "    <transform function='is-one-of' type='unsigned long' select='12,42' input='fires' id='selected-fires'/>"
                             "    <transform function='filter' type='unsigned long' input='selected-fires,fires' id='the-fires'/>"
                             "    <reduce type='unsigned long' function='count' input='the-fires' id='count'/>"
                             "    <result function='plot' input='count' type='unsigned'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateIndirectFire( 12, 12 ) );
    task->Receive( TestTools::CreateIndirectFire( 13, 13 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 12, 16 ) );
    task->Receive( TestTools::StopFire( 13, 16 ) );
    task->Receive( TestTools::CreateIndirectFire( 14, 14 ) );
    task->Receive( TestTools::CreateIndirectFire( 15, 15 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 13, 16 ) );
    task->Receive( TestTools::StopFire( 14, 18 ) );
    task->Receive( TestTools::StopFire( 15, 19 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateIndirectFire( 16, 15 ) );
    task->Receive( TestTools::CreateIndirectFire( 17, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 16, 16 ) );
    task->Receive( TestTools::CreateIndirectFire( 18, 13 ) );
    task->Receive( TestTools::CreateIndirectFire( 19, 14 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 1., 1., 0., 1., 1. };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
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
BOOST_FIXTURE_TEST_CASE( Facade_TestInflictedComponentDamagesFromDirectFire, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='fire-component-damage' id='damages'/>"
                             "    <extract function='direct-fire-unit' id='units'/>"
                             "    <transform function='is-one-of' type='unsigned long' select='12,42' input='units' id='selected-fires'/>"
                             "    <transform function='filter' type='float' input='selected-fires,damages' id='the-damages'/>"
                             "    <reduce type='float' function='sum' input='the-damages' id='sum'/>"
                             "    <result function='plot' input='sum' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateDirectFire( 12, 12 ) );
    task->Receive( TestTools::CreateDirectFire( 13, 13 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 12, 16, 5 ) );
    task->Receive( TestTools::StopFire( 13, 16 ) );
    task->Receive( TestTools::CreateDirectFire( 14, 14 ) );
    task->Receive( TestTools::CreateDirectFire( 15, 15 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 13, 16 ) );
    task->Receive( TestTools::StopFire( 14, 18 ) );
    task->Receive( TestTools::StopFire( 15, 19 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateDirectFire( 16, 15 ) );
    task->Receive( TestTools::CreateDirectFire( 17, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 16, 18 ) );
    task->Receive( TestTools::CreateDirectFire( 18, 13 ) );
    task->Receive( TestTools::CreateDirectFire( 19, 14 ) );
    task->Receive( TestTools::StopFire( 17, 16, 3 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 0., 5., 0., 0., 3. };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestInflictedComponentDamagesFromDirectFireWithComposedFilterOfHell
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestInflictedComponentDamagesFromDirectFireWithComposedFilterOfHell, Fixture )
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
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TBN7728449218", 12 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449242", 13 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449212", 14 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449242", 15 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449220", 42 ) );
    task->Receive( TestTools::CreateDirectFire( 12, 12 ) );
    task->Receive( TestTools::CreateDirectFire( 13, 13 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 12, 18, 5 ) );
    task->Receive( TestTools::StopFire( 13, 19 ) );
    task->Receive( TestTools::CreateDirectFire( 14, 14 ) );
    task->Receive( TestTools::CreateDirectFire( 15, 15 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 13, 14 ) );
    task->Receive( TestTools::StopFire( 14, 13 ) );
    task->Receive( TestTools::StopFire( 15, 18 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateDirectFire( 16, 15 ) );
    task->Receive( TestTools::CreateDirectFire( 17, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 16, 13 ) );
    task->Receive( TestTools::CreateDirectFire( 18, 13 ) );
    task->Receive( TestTools::CreateDirectFire( 19, 14 ) );
    task->Receive( TestTools::StopFire( 17, 17, 3 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 0., 5., 0., 0., 3. };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestAllResources
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestAllResources, Fixture )
{

    xml::xistringstream xis( "<indicator>"
                             "    <extract function='resources' id='resources'/>"
                             "    <reduce type='int' function='sum' input='resources' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 4212, 12, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 1242, 13, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 4200, 12, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 1200, 13, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 1000, 14 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 4100, 12 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 5454, 5400, 5400, 6400, 6300 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestInstantaneousResourceConsumptionsWithResourceFilter
// $$$$ SBO 2010-06-14: triggers a runtime_error if "Compare" is created with < K, T > instead of < K, NumericValue >
//                      (see TranformationFactory), for now, compare can only be used to compare numeric values (not positions for instance)
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestInstantaneousResourceConsumptionsWithResourceFilter, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='resources' id='resources' resources='42'/>"
                             "    <transform type='int' function='derivate' input='resources' id='resources-var' period='1'/>"
                             "    <constant type='int' value='0' id='zero'/>"
                             "    <transform function='compare' type='bool' operator='less' input='resources-var,zero' id='test'/>"
                             "    <transform function='filter' type='int' input='test,resources-var' id='consumptions'/>"
                             "    <reduce type='int' function='sum' input='consumptions' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 4212, 12, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 1242, 13, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 1000, 15, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 4200, 12, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 1200, 13, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 5200, 12, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 2200, 13, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 1000, 14, 15 ) );
    task->Receive( TestTools::MakeResourceVariation(  500, 15, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 5100, 12, 42 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 0, -54, 0, 0, -100 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestResourceConsumptionsWithResourceFilter
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestResourceConsumptionsWithResourceFilter, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='resources' id='resources' resources='42'/>"
                             "    <transform type='int' function='derivate' input='resources' id='resources-var' period='1'/>"
                             "    <constant type='int' value='0' id='zero'/>"
                             "    <transform function='compare' type='bool' operator='less' input='resources-var,zero' id='test'/>"
                             "    <transform function='filter' type='int' input='test,resources-var' id='consumptions'/>"
                             "    <reduce type='int' function='sum' input='consumptions' id='sum'/>"
                             "    <transform function='integrate' id='total' input='sum' type='int'/>"
                             "    <result function='plot' input='total' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 4212, 12, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 1242, 13, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 1000, 15, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 4200, 12, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 1200, 13, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 5200, 12, 42 ) );
    task->Receive( TestTools::MakeResourceVariation( 2200, 13, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 1000, 14, 15 ) );
    task->Receive( TestTools::MakeResourceVariation(  500, 15, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 5100, 12, 42 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 0, -54, -54, -54, -154 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestEquipments
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestEquipments, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='equipments' states='available,captured' equipments='12,42' id='equipments'/>"
                             "    <reduce type='int' function='sum' input='equipments' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int base  [5] = { 5, 0, 0, 0, 0 }; //  0
    int prison[5] = { 2, 1, 0, 1, 1 }; // -2
    int casse [5] = { 2, 1, 1, 1, 0 }; // -3
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( base, 12, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( base, 13, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( base, 15, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( prison, 12, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( casse, 13, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( casse, 12, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( prison, 13, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( base, 14, 15 ) );
    task->Receive( TestTools::MakeEquipementVariation( base, 16, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( prison, 12, 42 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 15, 10, 10, 15, 16 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestHumans
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestHumans, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='humans' states='dead' ranks='officer,sub-officer,troopers' id='humans'/>"
                             "    <reduce type='int' function='sum' input='humans' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int state[8] = { 0, 0, 1, 0, 0, 0, 0, 0 };
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeHumanVariation( state, 42 ) );
    task->Receive( TestTools::MakeHumanVariation( state, 43 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeHumanVariation( state, 42 ) );
    task->Receive( TestTools::EndTick() );
    {
        double expectedResult[] = { 2, 2 };
        TestTools::MakeExpectation( publisher, expectedResult );
    }
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestTypeAdaptation
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestTypeAdaptation, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='operational-state' id='opstate'/>"
                             "    <reduce type='int' function='select' input='opstate' key='2' id='myopstate'/>"
                             "    <result function='plot' input='myopstate' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Facade_TestTypeBadLexicalCast
// $$$$ SBO 2010-06-14: triggers bad_lexical_cast if "IsOneOf" is created with < K, T > instead of < K, NumericValue >
//                      (see TranformationFactory), for now, IsOneOf can only be used with numeric values (not positions for instance)
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestBadLexicalCast, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='maintenance-handling-unit' id='1'/>"
                             "    <transform function='is-one-of' id='2' input='1' select='13,67,68,69,71,74,75,76,78,79,80,55,81,82,197,198,199,90,91,92,93,147,56,148,149,150,151,173,174,175,176,177,178,58,59,60,61,63,66' type='bool'/>"
                             "    <extract function='maintenance-handling-unit' id='3'/>"
                             "    <transform function='filter' id='4' input='2,3' type='unsigned long'/>"
                             "    <reduce function='count' id='5' input='4' type='unsigned'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Facade_TestConflicts
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestConflicts, Fixture )
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
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateDirectFire( 12, 55 ) );
    task->Receive( TestTools::CreateDirectFire( 13, 56 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 12, 18 ) );
    task->Receive( TestTools::StopFire( 13, 19 ) );
    task->Receive( TestTools::CreateDirectFire( 14, 55 ) );
    task->Receive( TestTools::CreateDirectFire( 15, 56 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 14, 16 ) );
    task->Receive( TestTools::StopFire( 15, 13 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 2, 4, 8, 10 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestUnitDetection
// Created: SBO 2010-06-01
// $$$$ SBO 2010-07-28: 1 when detection level (for specified detecting unit) matches specified visibility, otherwise 0
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestUnitDetection, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='detecting-unit' id='1' detected='69' visibility='detected,recognized,identified'/>"
                             "    <reduce type='int' function='select' input='1' key='42' id='2'/>"
                             "    <result function='plot' input='2' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateUnitDetection( 42, 69, sword::UnitVisibility::identified ) ); // ok
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateUnitDetection( 42, 51, sword::UnitVisibility::recognized ) ); // irrelevant detected unit
    task->Receive( TestTools::CreateUnitDetection( 51, 69, sword::UnitVisibility::detected ) ); // irrelevant detecting unit
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateUnitDetection( 42, 69, sword::UnitVisibility::invisible ) ); // irrelevant detection level
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateUnitDetection( 42, 69, sword::UnitVisibility::recognized ) ); // ok
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 1, 1, 0, 1 };//, 1, 1, 1, 1 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

//------------------------------------------------------------------------------
// Name: Facade_TestUnitDetectionWithThreshold
// Created: SBO 2010-06-01
// $$$$ SBO 2010-07-28: 1 when detection level (for any specified detecting units) matches specified visibility, otherwise 0
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestUnitDetectionWithThreshold, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='detecting-unit' id='1' detected='69' visibility='detected,recognized,identified'/>"
                             "    <transform function='domain' id='2' input='1' select='42,51' type='unsigned long'/>"
                             "    <reduce type='int' function='sum' input='2' id='3'/>"
                             "    <reduce type='float' function='threshold' input='3' thresholds='0,1' values='0,1' id='4'/>"
                             "    <result function='plot' input='4' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateUnitDetection( 12, 69, sword::UnitVisibility::detected ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateUnitDetection( 42, 69, sword::UnitVisibility::identified ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateUnitDetection( 42, 51, sword::UnitVisibility::recognized ) );
    task->Receive( TestTools::CreateUnitDetection( 51, 69, sword::UnitVisibility::detected ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateUnitDetection( 42, 69, sword::UnitVisibility::invisible ) );
    task->Receive( TestTools::CreateUnitDetection( 51, 69, sword::UnitVisibility::recorded ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateUnitDetection( 42, 69, sword::UnitVisibility::recognized ) );
    task->Receive( TestTools::CreateUnitDetection( 51, 69, sword::UnitVisibility::recognized ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 0, 1, 1, 0, 1 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestProduct
// Created: SBO 2010-07-28
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestProduct, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='operational-state' id='opstate'/>"
                             "    <reduce type='float' function='select' input='opstate' key='42' id='myopstate'/>"
                             "    <constant type='float' value='10' id='ten'/>"
                             "    <reduce type='float' function='product' input='myopstate,ten' id='prod'/>"
                             "    <result function='plot' input='prod' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 100, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 50, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 0, 42 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 10, 5, 0 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestTimeElapsedBetweenDetectionAndDestruction
// Created: SBO 2010-07-28
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestTimeElapsedBetweenDetectionAndDestruction, Fixture )
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
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 100, 69 ) );
    task->Receive( TestTools::CreateUnitDetection( 42, 69, sword::UnitVisibility::detected ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::OperationalState( 0, 69 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 1, 2, 3, 3, 3 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestCloseCombatPower
// Created: ABR 2011-02-14
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestCloseCombatPower, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "   <extract function='close-combat-power' id='closecombat'/>"
                             "   <reduce  function='select' id='myselect' input='closecombat' key='42' type='float'/>"
                             "   <result  function='plot' input='myselect' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int variation[5] = { 1, 1, 1, 1, 0 };
    sword::SimToClient message = TestTools::MakeEquipementVariation( variation, 42u );
    MOCK_EXPECT( model, ComputePower ).once().with( mock::any, &TestTools::IsCloseCombatPower ).returns( 10.f );
    task->Receive( TestTools::BeginTick() );
    task->Receive( message );
    task->Receive( TestTools::EndTick() );
    MOCK_EXPECT( model, ComputePower ).once().with( mock::any, &TestTools::IsCloseCombatPower ).returns( 11.f );
    task->Receive( TestTools::BeginTick() );
    task->Receive( message );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 10, 11 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestProductOnTwoExtractors
// Created: ABR 2011-02-14
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestProductOnTwoExtractors, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "   <extract function='operational-state' id='opstate'/>"
                             "   <extract function='close-combat-power' id='closecombat'/>"
                             "   <reduce  function='product' id='myproduct' input='opstate,closecombat' type='float'/>"
                             "   <reduce  function='select' id='myselect' input='myproduct' key='42' type='float'/>"
                             "   <result  function='plot' input='myselect' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int variation[5] = { 1, 1, 1, 1, 0 };
    sword::SimToClient  message1 = TestTools::MakeEquipementVariation( variation, 42u );
    sword::SimToClient  message2 = TestTools::MakeEquipementVariation( variation, 123u );
    MOCK_EXPECT( model, ComputePower ).once().with( mock::any, &TestTools::IsCloseCombatPower ).returns( 10.f );
    MOCK_EXPECT( model, ComputePower ).once().with( mock::any, &TestTools::IsCloseCombatPower ).returns( 1000.f );
    task->Receive( TestTools::BeginTick() );
    task->Receive( message1 );
    task->Receive( message2 );
    task->Receive( TestTools::OperationalState( 50, 42 ) );
    task->Receive( TestTools::EndTick() ); // expect 10 * 0.5
    mock::verify();
    MOCK_EXPECT( model, ComputePower ).once().with( mock::any, &TestTools::IsCloseCombatPower ).returns( 20.f );
    task->Receive( TestTools::BeginTick() );
    task->Receive( message1 );
    task->Receive( TestTools::OperationalState( 25, 42 ) );
    task->Receive( TestTools::EndTick() ); // expect 20 * 0.25
    MOCK_EXPECT( model, ComputePower ).once().with( mock::any, &TestTools::IsCloseCombatPower ).returns( 30.f );
    task->Receive( TestTools::BeginTick() );
    task->Receive( message1 );
    task->Receive( TestTools::OperationalState( 100, 42 ) );
    task->Receive( TestTools::EndTick() ); // expect 30 * 1
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() ); // expect same as previous
    double expectedResult[] = { 5, 5, 30, 30 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestAvailableEquipmentsForUnitList
// Created: FPO 2011-04-26
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestAvailableEquipmentsForUnitList, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "<extract function='equipments' states='available' id='1' equipments='12,42'/>"
                             "<transform function='domain' type='int' select='17,18' input='1' id='2'/>"
                             "<reduce type='int' function='sum' input='2' id='3'/>"
                             "<result function='plot' input='3' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int variation[5] = { 5, 0, 0, 0, 0 };
    int variation2[5] = { 1, 0, 0, 0, 0 };
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 15, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 51 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 17, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 17, 12 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 18, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 15, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 17, 42 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 5, 3, 3 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestUnavailableEquipmentsForUnitList
// Created: FPO 2011-04-26
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestUnavailableEquipmentsForUnitList, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "<extract function='equipments' states='unavailable,repairable,repairing,captured' id='1' equipments='12,42'/>"
                             "<transform function='domain' type='int' select='17,18' input='1' id='2'/>"
                             "<reduce type='int' function='sum' input='2' id='3'/>"
                             "<result function='plot' input='3' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int variation[5] = { 0, 1, 1, 1, 1 };
    int variation2[5] = { 0, 1, 0, 0, 0 };
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 15, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 51 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 17, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 17, 12 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 18, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 15, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 17, 42 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 4, 3, 3 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestAvailableEquipmentsInSpecifiedZone
// Created: FPO 2011-04-26
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestAvailableEquipmentsInSpecifiedZone, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "<extract function='equipments' states='available' id='equipments' equipments='12,42'/>"
                             "<extract function='position' id='positions'/>"
                             "<constant type='zone' value='circle(31TCM1508386208,31TCM1410587214)' id='circle'/>"
                             "<transform function='contains' input='circle,positions' id='selected-units'/>"
                             "<transform function='filter' id='filtered-positions' input='selected-units,equipments' type='int'/>"
                             "<reduce type='int' function='sum' input='filtered-positions' id='3'/>"
                             "<result function='plot' input='3' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int variation[5] = { 5, 0, 0, 0, 0 };
    int variation2[5] = { 1, 0, 0, 0, 0 };
    sword::SimToClient  message = TestTools::MakeEquipementVariation( variation, 17, 42 );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TCM1508386208", 17 ) );
    task->Receive( TestTools::MakePosition( "31TCM1543486826", 15 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 15, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TCM0960387104", 18 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 18, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 17, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 12 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 10, 11, 15 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestUnavailableEquipmentsInSpecifiedZone
// Created: FPO 2011-04-28
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestUnavailableEquipmentsInSpecifiedZone, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "<extract function='equipments' states='unavailable,repairable,repairing,captured' id='equipments' equipments='12,42'/>"
                             "<extract function='position' id='positions'/>"
                             "<constant type='zone' value='circle(31TCM1508386208,31TCM1410587214)' id='circle'/>"
                             "<transform function='contains' input='circle,positions' id='selected-units'/>"
                             "<transform function='filter' id='filtered-positions' input='selected-units,equipments' type='int'/>"
                             "<reduce type='int' function='sum' input='filtered-positions' id='3'/>"
                             "<result function='plot' input='3' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int variation[5] = { 10, 1, 1, 1, 2 };
    int variation2[5] = { 0, 0, 1, 0, 0 };
    sword::SimToClient  message = TestTools::MakeEquipementVariation( variation, 17, 42 );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TCM1508386208", 17 ) );
    task->Receive( TestTools::MakePosition( "31TCM1543486826", 15 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 15, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TCM0960387104", 18 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 18, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 17, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 12 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 10, 11, 15 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestDestroyedEquipmentsInSpecifiedZone
// Created: FPO 2011-04-28
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestDestroyedEquipmentsInSpecifiedZone, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "<extract function='equipments' states='unavailable' id='equipments' equipments='12,42'/>"
                             "<extract function='position' id='positions'/>"
                             "<constant type='zone' value='circle(31TCM1508386208,31TCM1410587214)' id='circle'/>"
                             "<transform function='contains' input='circle,positions' id='selected-units'/>"
                             "<transform function='filter' id='filtered-positions' input='selected-units,equipments' type='int'/>"
                             "<reduce type='int' function='sum' input='filtered-positions' id='3'/>"
                             "<result function='plot' input='3' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int variation[5] = { 10, 5, 1, 1, 2 };
    int variation2[5] = { 0, 1, 1, 0, 0 };
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TCM1508386208", 17 ) );
    task->Receive( TestTools::MakePosition( "31TCM1543486826", 15 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation, 15, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TCM0960387104", 18 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 18, 42 ) );
    task->Receive( TestTools::MakeEquipementVariation( variation2, 17, 12 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 12 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 10, 11, 15 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestDestroyedEquipmentsInSpecifiedZoneWithPolygon
// Created: FPO 2011-04-28
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestDestroyedEquipmentsInSpecifiedZoneWithPolygon, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "<extract function='equipments' states='unavailable' id='equipments' equipments='12,42'/>"
                             "<extract function='position' id='positions'/>"
                             "<constant type='zone' value='polygon(31TBN9260307999,31TBN9768407716,31TBN9516711524)' id='defined-polygon'/>"
                             "<transform function='contains' input='defined-polygon,positions' id='selected-units'/>"
                             "<transform function='filter' id='filtered-positions' input='selected-units,equipments' type='int'/>"
                             "<reduce type='int' function='sum' input='filtered-positions' id='3'/>"
                             "<result function='plot' input='3' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int variation[5] = { 10, 5, 1, 1, 2 };
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation, 17, 42 ) );
    task->Receive( TestTools::MakePosition( "31TBN9525308404", 17 ) );
    task->Receive( TestTools::MakePosition( "31TCM1543486826", 15 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeEquipementVariation( variation, 15, 42 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 5, 5 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestDeadHumansForUnitList
// Created: FPO 2011-04-26
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestDeadHumansForUnitList, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "<extract function='humans' states='dead' ranks='officer,sub-officer,troopers' id='humans'/>"
                             "<transform function='domain' type='int' select='42,17' input='humans' id='domained'/>"
                             "<reduce type='int' function='sum' input='domained' id='sum'/>"
                             "<result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int state[8] = { 1, 0, 1, 0, 0, 0, 0, 0 };
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeHumanVariation( state, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeHumanVariation( state, 17 ) );
    task->Receive( TestTools::MakeHumanVariation( state, 18 ) );
    task->Receive( TestTools::EndTick() );
    {
        double expectedResult[] = { 1, 2 };
        TestTools::MakeExpectation( publisher, expectedResult );
    }
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestOperationalHumansForUnitList
// Created: FPO 2011-04-26
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestOperationalHumansForUnitList, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "<extract function='humans' states='operational' ranks='officer,sub-officer,troopers' id='humans'/>"
                             "<transform function='domain' type='int' select='42,17' input='humans' id='domained'/>"
                             "<reduce type='int' function='sum' input='domained' id='sum'/>"
                             "<result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int state[8] = { 2, 1, 0, 0, 0, 1, 0, 0 };
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeHumanVariation( state, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeHumanVariation( state, 17 ) );
    task->Receive( TestTools::MakeHumanVariation( state, 18 ) );
    task->Receive( TestTools::EndTick() );
    {
        double expectedResult[] = { 1, 2 };
        TestTools::MakeExpectation( publisher, expectedResult );
    }
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestOperationalHumansForSpecifiedZone
// Created: FPO 2011-04-29
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestOperationalHumansForSpecifiedZone, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "<extract function='humans' states='operational' ranks='officer,sub-officer,troopers' id='humans'/>"
                             "<extract function='position' id='positions'/>"
                             "<constant type='zone' value='circle(31TCM1508386208,31TCM1410587214)' id='circle'/>"
                             "<transform function='contains' input='circle,positions' id='selected-units'/>"
                             "<transform function='filter' id='filtered-positions' input='selected-units,humans' type='int'/>"
                             "<reduce type='int' function='sum' input='filtered-positions' id='sum'/>"
                             "<result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int state[8] = { 2, 1, 0, 0, 0, 0, 1, 0 };
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TCM1508386208", 17 ) );
    task->Receive( TestTools::MakePosition( "31TCM1543486826", 15 ) );
    task->Receive( TestTools::MakePosition( "31TCM0960387104", 18 ) );
    task->Receive( TestTools::MakeHumanVariation( state, 15 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeHumanVariation( state, 17 ) );
    task->Receive( TestTools::MakeHumanVariation( state, 18 ) );
    task->Receive( TestTools::EndTick() );
    {
        double expectedResult[] = { 1, 2 };
        TestTools::MakeExpectation( publisher, expectedResult );
    }
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestWoundedHumansForUnitList
// Created: FPO 2011-04-26
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestWoundedHumansForUnitList, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='humans' states='wounded' ranks='officer,sub-officer,troopers' id='humans'/>"
                             "    <transform function='domain' type='int' select='42,17' input='humans' id='domained'/>"
                             "    <reduce type='int' function='sum' input='domained' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    int state[8] = { 3, 0, 0, 3, 1, 0, 0, 0 };
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeHumanVariation( state, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeHumanVariation( state, 17 ) );
    task->Receive( TestTools::MakeHumanVariation( state, 18 ) );
    task->Receive( TestTools::EndTick() );
    {
        double expectedResult[] = { 3, 6 };
        TestTools::MakeExpectation( publisher, expectedResult );
    }
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestAvailableResourcesForUnitList
// Created: FPO 2011-04-26
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestAvailableResourcesForUnitList, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='resources' id='1' resources='47,51'/>"
                             "    <transform function='domain' type='int' select='12,17' input='1' id='2'/>"
                             "    <reduce type='int' function='sum' input='2' id='3'/>"
                             "    <result function='plot' input='3' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 4212, 12, 47 ) );
    task->Receive( TestTools::MakeResourceVariation( 1242, 23, 47 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeResourceVariation( 4200, 17, 47 ) );
    task->Receive( TestTools::MakeResourceVariation( 1200, 12, 51 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    {
        double expectedResult[] = { 4212, 9612, 9612 };
        TestTools::MakeExpectation( publisher, expectedResult );
    }
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestNumberOfDirectFires
// Created: FPO 2011-04-29
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestNumberOfDirectFires, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='direct-fire-unit' id='fires'/>"
                             "    <reduce type='unsigned long' function='count' input='fires' id='count'/>"
                             "    <result function='plot' input='count' type='unsigned'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateDirectFire( 12, 12 ) );
    task->Receive( TestTools::CreateDirectFire( 13, 13 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 12, 16 ) );
    task->Receive( TestTools::StopFire( 13, 18 ) );
    task->Receive( TestTools::CreateDirectFire( 14, 14 ) );
    task->Receive( TestTools::CreateDirectFire( 15, 15 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::StopFire( 13, 19 ) );
    task->Receive( TestTools::StopFire( 14, 23 ) );
    task->Receive( TestTools::StopFire( 15, 26 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateDirectFire( 16, 15 ) );
    task->Receive( TestTools::CreateDirectFire( 17, 42 ) );
    task->Receive( TestTools::StopFire( 16, 28 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateDirectFire( 18, 13 ) );
    task->Receive( TestTools::CreateDirectFire( 19, 14 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 2., 4., 2., 2., 3. };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestDeadHumansFromDirectFire
// Created: FPO 2011-04-29
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestDeadHumansFromDirectFire, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='fire-human-damages' id='damages' ranks='officer,sub-officer,troopers' states='dead'/>"
                             "    <transform function='domain' type='int' select='12,17' input='damages' id='domained-damages'/>"
                             "    <reduce type='float' function='sum' input='domained-damages' id='sum'/>"
                             "    <result function='plot' input='sum' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeUnitDamages( 12, 13, 0, 3 ) );
    task->Receive( TestTools::MakeUnitDamages( 12, 23, 0, 5 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeUnitDamages( 17, 23, 0, 1 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 8, 0, 1, 0 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestHumansLossFromDirectFire
// Created: FPO 2011-05-19
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestHumansLossFromDirectFire, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='fire-human-loss' id='damages' ranks='officer,sub-officer,troopers' states='dead'/>"
                             "    <transform function='domain' type='int' select='13,23' input='damages' id='domained-damages'/>"
                             "    <reduce type='float' function='sum' input='domained-damages' id='sum'/>"
                             "    <result function='plot' input='sum' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeUnitDamages( 12, 13, 0, 3 ) );
    task->Receive( TestTools::MakeUnitDamages( 12, 23, 0, 5 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeUnitDamages( 17, 23, 0, 1 ) );
    task->Receive( TestTools::MakeUnitDamages( 12, 23, 0, 5 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 8, 0, 6, 0 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestDeadHumansFromDirectFireInZone
// Created: FPO 2011-05-03
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestDeadHumansFromDirectFireInZone, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='fire-human-damages' id='damages' ranks='officer,sub-officer,troopers' states='dead'/>"
                             "    <extract function='position' id='positions'/>"
                             "    <constant type='zone' value='circle(31TBN7728449218,31TBN7728449222)' id='circle'/>"
                             "    <transform function='contains' input='circle,positions' id='selected-units'/>"
                             "    <transform function='filter' type='float' input='selected-units,damages' id='the-damages'/>"
                             "    <reduce type='float' function='sum' input='the-damages' id='sum'/>"
                             "    <result function='plot' input='sum' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakePosition( "31TBN7728449218", 12 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449242", 13 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449212", 14 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449242", 15 ) );
    task->Receive( TestTools::MakePosition( "31TBN7728449220", 42 ) );
    task->Receive( TestTools::MakePosition( "31TCM1543486826", 16 ) );
    task->Receive( TestTools::MakeUnitDamages( 12, 13, 0, 1 ) );
    task->Receive( TestTools::MakeUnitDamages( 16, 21, 0, 2 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeUnitDamages( 12, 13, 0, 11 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 1, 0, 11 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestLogEvacuation
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestLogEvacuation, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='ambulances' types='releve' id='ambulances'/>"
                             "    <transform function='domain' type='int' select='10' input='ambulances' id='domained-ambulances'/>"
                             "    <reduce type='int' function='sum' input='domained-ambulances' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeAmbulances( 4, 2, 10 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 4, 4 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestLogCollection
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestLogCollection, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='ambulances' types='ramassage' id='ambulances'/>"
                             "    <transform function='domain' type='int' select='10' input='ambulances' id='domained-ambulances'/>"
                             "    <reduce type='int' function='sum' input='domained-ambulances' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeAmbulances( 4, 2, 10 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 2, 2 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestLogHaulers
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestLogHaulers, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='maintenance' types='haulers' id='maintenance'/>"
                             "    <transform function='domain' type='int' select='10' input='maintenance' id='domained-maintenance'/>"
                             "    <reduce type='int' function='sum' input='domained-maintenance' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMaintenance( 4, 2, 10 ) );
    task->Receive( TestTools::MakeMaintenance( 12, 5, 11 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMaintenance( 12, 15, 11 ) );
    task->Receive( TestTools::MakeMaintenance( 4, 5, 10 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 2, 2, 5 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestLogRepairers
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestLogRepairers, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='maintenance' types='repairers' id='maintenance'/>"
                             "    <transform function='domain' type='int' select='10,13' input='maintenance' id='domained-maintenance'/>"
                             "    <reduce type='int' function='sum' input='domained-maintenance' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMaintenance( 4, 2, 10 ) );
    task->Receive( TestTools::MakeMaintenance( 12, 5, 11 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMaintenance( 12, 5, 13 ) );
    task->Receive( TestTools::MakeMaintenance( 4, 2, 10 ) );
    task->Receive( TestTools::MakeMaintenance( 12, 5, 11 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 4, 16, 16 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestLogRepairersAndHaulers
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestLogRepairersAndHaulers, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='maintenance' id='maintenance'/>"
                             "    <transform function='domain' type='int' select='10,13' input='maintenance' id='domained-maintenance'/>"
                             "    <reduce type='int' function='sum' input='domained-maintenance' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMaintenance( 4, 2, 10 ) );
    task->Receive( TestTools::MakeMaintenance( 12, 5, 13 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMaintenance( 5, 5, 13 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeMaintenance( 5, 10, 13 ) );
    task->Receive( TestTools::MakeMaintenance( 5, 17, 17 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 23, 16, 21 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestStocks
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestStocks, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='stocks' types='42,13' id='stocks'/>"
                             "    <transform function='domain' type='int' select='10' input='stocks' id='domained-stocks'/>"
                             "    <reduce type='int' function='sum' input='domained-stocks' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeStockVariation( 3, 10, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeStockVariation( 5, 10, 42 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::MakeStockVariation( 7, 10, 13 ) );
    task->Receive( TestTools::MakeStockVariation( 7, 10, 11 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 3, 5, 12 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestPopulationStates
// Created: FPO 2011-05-05
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestPopulationStates, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='population-states' states='dead' id='populations'/>"
                             "    <transform function='domain' type='int' select='40' input='populations' id='domained-populations'/>"
                             "    <reduce type='int' function='sum' input='domained-populations' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdatePopulationStates( 1, 2, 3, 40 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdatePopulationStatesbis( 0, 2, 40 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdatePopulationStates( 10, 22, 55, 41 ) );
    task->Receive( TestTools::UpdatePopulationStates( 0, 2, 5, 40 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 3, 3, 5 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestPopulationSatisfaction
// Created: FPO 2011-05-05
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestPopulationSatisfaction, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='satisfactions' type='health' id='satisfactions'/>"
                             "    <reduce type='int' function='mean' input='satisfactions' id='mean'/>"
                             "    <result function='plot' input='mean' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdatePopulationSatisfaction( 1.5f, 2.3f, 3.4f, 40 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdatePopulationSatisfaction( 5.6f, 5.5f, 10.4f, 99 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdatePopulationSatisfaction( 3.5f, 2.3f, 13.4f, 99 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 2.3, 2.3, 3.9, 2.3 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestPopulationInBlocks
// Created: FPO 2011-05-12
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestPopulationInBlocks, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='population-in-selected-blocks' blocks='12,17,14' id='popuinblocks'/>"
                             "    <reduce type='int' function='sum' input='popuinblocks' id='sum'/>"
                             "    <result function='plot' input='sum' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    std::map< unsigned long, int > blocks;
    std::map< unsigned long, int > blocks2;
    blocks[ 12 ] = 15;
    blocks[ 17 ] = 5;
    blocks2[ 12 ] = 13;
    blocks2[ 46 ] = 103;
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdatePopulationInBlocks( 10, blocks ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdatePopulationInBlocks( 10, blocks2 ) );
    task->Receive( TestTools::UpdatePopulationInBlocks( 11, blocks ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 20, 20, 33 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestCrowdStates
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestCrowdStates, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='crowd-states' states='dead' id='crowds'/>"
                             "    <transform function='domain' type='int' select='10' input='crowds' id='domained-crowds'/>"
                             "    <reduce type='int' function='sum' input='domained-crowds' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdateCrowdDeadState( 10, 11 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdateCrowdDeadState( 10, 15 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 11, 11, 15 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestWaitingForMedicalAttention
// Created: FPO 2011-05-09
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestWaitingForMedicalAttention, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='waiting-for-medical' id='wounded'/>"
                             "    <transform function='domain' type='int' select='12,15' input='wounded' id='domained-wounded'/>"
                             "    <reduce type='int' function='sum' input='domained-wounded' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateMedicalConsign( 12, 13, true ) );
    task->Receive( TestTools::CreateMedicalConsign( 12, 15, true ) );
    task->Receive( TestTools::CreateMedicalConsign( 12, 18, true ) );
    task->Receive( TestTools::CreateMedicalConsign( 15, 18, true ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateMedicalConsign( 15, 31, true ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateMedicalConsign( 15, 31, false ) );

    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 2, 2, 2, 1 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestWaitingForMedicalAttentionInZone
// Created: FPO 2011-05-09
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestWaitingForMedicalAttentionInZone, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='waiting-for-medical' id='wounded'/>"
                             "    <transform function='domain' type='int' select='12,15' input='wounded' id='domained-wounded'/>"
                             "    <reduce type='int' function='sum' input='domained-wounded' id='sum'/>"
                             "    <result function='plot' input='sum' type='int'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateMedicalConsign( 12, 13, true ) );
    task->Receive( TestTools::CreateMedicalConsign( 12, 15, true ) );
    task->Receive( TestTools::CreateMedicalConsign( 12, 18, true ) );
    task->Receive( TestTools::CreateMedicalConsign( 15, 18, true ) );
    task->Receive( TestTools::CreateMedicalConsign( 16, 19, true ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateMedicalConsign( 15, 31, true ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateMedicalConsign( 15, 31, false ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 2, 2, 2, 1 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// -----------------------------------------------------------------------------
// Name: Facade_TestStructuralStates
// Created: FPO 2011-05-16
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( Facade_TestStructuralStates, Fixture )
{
    xml::xistringstream xis( "<indicator>"
                             "    <extract function='infrastructures-functional-states' id='structural-states'/>"
                             "    <transform function='domain' type='int' select='12,15' input='structural-states' id='domained-structural-states'/>"
                             "    <reduce type='float' function='mean' input='domained-structural-states' id='mean'/>"
                             "    <result function='plot' input='mean' type='float'/>"
                             "</indicator>" );
    boost::shared_ptr< Task > task( facade.CreateTask( xis >> xml::start( "indicator" ) ) );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateFunctionalState( 12,75 ) );
    task->Receive( TestTools::CreateFunctionalState( 17,33 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::UpdateFunctionalState( 12,50 ) );
    task->Receive( TestTools::UpdateFunctionalState( 15,80 ) );
    task->Receive( TestTools::EndTick() );
    task->Receive( TestTools::BeginTick() );
    task->Receive( TestTools::CreateFunctionalState( 15,100 ) );
    task->Receive( TestTools::EndTick() );
    double expectedResult[] = { 75, 75, 50, 75 };
    TestTools::MakeExpectation( publisher, expectedResult );
    task->Commit();
}

// $$$$ AGE 2007-09-10: ressources consomm�es => variation <0
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_RESSOURCES_CONSOMMEES_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Quantites)]
//(
//  @TDebut DATETIME,           -- Date Debut
//  @TFin DATETIME,             -- Date Fin
//  @Unites unit_id_tablename,      -- Unites concernees
//  @Ressources ressource_id_tablename, -- Ressource � comptabiliser
//  @Resultat tablename OUTPUT      -- Table de resultats
//)

// $$$$ AGE 2007-09-10: pourcentage efficacit� => nombre de tirs avec degat / nombre de tirs
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
//  @Ressources ressource_id_tablename, -- Ressource � comptabiliser
//  @Resultat tablename OUTPUT      -- Table de resultats
//)

// $$$$ AGE 2007-09-10: UE-UF-JV => consommation normalis�e
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_STOCKS_DISPONIBLES_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(UE-UF-JV)]
//(
//  @TDebut DATETIME,           -- Date Debut
//  @Unites unit_id_tablename,      -- Unites concernees
//  @Ressources ressource_id_tablename, -- Ressource � comptabiliser
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

// $$$$ AGE 2007-09-10: deced� au combat
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_PERSONNEL_DECEDE_AU_COMBAT_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,               -- Date
//  @TFin DATETIME,                 -- Date
//  @Unites unit_id_tablename,          -- Unites concernees
//  @Resultat tablename OUT             -- Tableau de resultat
//)

// $$$$ AGE 2007-09-10: deced� faute de soins
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
