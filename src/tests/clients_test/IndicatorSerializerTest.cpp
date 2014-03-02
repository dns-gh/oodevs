// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "XmlChecks.h"
#include "indicators/DataTypeFactory.h"
#include "indicators/Element_ABC.h"
#include "indicators/ElementFactory.h"
#include "indicators/FormulaParser.h"
#include "indicators/Primitives.h"
#include "indicators/Serializer.h"
#include "indicators/Variable.h"
#include "indicators/Variables.h"
#include "tools/ExerciseConfig.h"
#include "tools/NullFileLoaderObserver.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    class SerializerFixture
    {
    public:
        SerializerFixture()
            : factory_( primitives_, variables_ )
        {
            tools::ExerciseConfig config( tools::CreateNullFileLoaderObserver() );
            xml::xifstream xis( BOOST_RESOLVE( "../../app-data/resources/IndicatorPrimitives.xml" ));
            primitives_.Load( xis );
        }

        void ParseAndCheck( const std::string& text, const std::string& expected )
        {
            xml::xostringstream xos;
            indicators::Serializer handler( factory_ );
            indicators::FormulaParser parser( handler );
            parser.Parse( text );
            handler.Serialize( xos );
            BOOST_CHECK_XML_EQUAL( expected, xos.str() );
        }

        void RegisterVariable( const std::string& name, const std::string& type, const std::string& value )
        {
            variables_.Register( name, boost::shared_ptr< indicators::Element_ABC >( new indicators::Variable( name, types_.Instanciate( type ), value ) ) );
        }

    private:
        indicators::Primitives primitives_;
        indicators::Variables variables_;
        indicators::ElementFactory factory_;
        indicators::DataTypeFactory types_;
    };
}

BOOST_FIXTURE_TEST_SUITE( IndicatorSerializerTestSuite, SerializerFixture )

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestEmpty
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestEmpty )
{
    BOOST_CHECK_THROW( ParseAndCheck( "", "" ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestNonExistingFunction
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestNonExistingFunction )
{
    BOOST_CHECK_THROW( ParseAndCheck( "Dummy()", "" ), std::exception );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestExtractorTypeIsImplicit
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestExtractorTypeIsImplicit )
{
    const std::string expected =
        "<indicator>"
            "<extract function='operational-state' id='1'/>"
        "</indicator>";
    ParseAndCheck( "operational-state()", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestFunctionNameIsCaseInsensitive
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestFunctionNameIsCaseInsensitive )
{
    const std::string expected =
        "<indicator>"
            "<extract function='operational-state' id='1'/>"
        "</indicator>";
    ParseAndCheck( "OpEraTionAl-StaTe()", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestResultFunctionsHaveNoId
// Created: SBO 2009-04-27
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestResultFunctionsHaveNoId )
{
    const std::string expected =
        "<indicator>"
            "<extract function='operational-state' id='1'/>"
            "<result function='indicator' input='1' name='test' type='float'/>"
        "</indicator>";
    ParseAndCheck( "indicator( 'test', operational-state() )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestFunctionWithReturnTypeDeducedFromParameterType
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestFunctionWithReturnTypeDeducedFromParameterType )
{
    const std::string expected =
        "<indicator>"
            "<constant id='$values' type='list(string)' value='va,lu,es'/>"
            "<reduce function='mean' id='1' input='$values' type='string'/>"
        "</indicator>";
    RegisterVariable( "values", "list(string)", "va,lu,es" );
    ParseAndCheck( "mean( $values )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestVariableReferenceIsReplacedByValue
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestVariableReferenceIsReplacedByValue )
{
    const std::string expected =
        "<indicator>"
            "<extract equipments='eq1,eq2,eq3' function='equipments' id='1' states='state1,state2,state3'/>"
        "</indicator>";
    RegisterVariable( "Equipments", "list(equipment-type)", "eq1,eq2,eq3" );
    RegisterVariable( "States", "list(equipment-state)", "state1,state2,state3" );
    ParseAndCheck( "equipments( $Equipments, $States )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestUnreferencedVariableIsNotDeclared
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestUnreferencedVariableIsNotDeclared )
{
    const std::string expected =
        "<indicator>"
            "<extract equipments='eq1,eq2,eq3' function='equipments' id='1' states='state1,state2,state3'/>"
        "</indicator>";
    RegisterVariable( "Equipments", "list(equipment-type)", "eq1,eq2,eq3" );
    RegisterVariable( "States", "list(equipment-state)", "state1,state2,state3" );
    RegisterVariable( "Unreferenced", "float", "1.0" );
    ParseAndCheck( "equipments( $Equipments, $States )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestVariableReferencedInInputNeedsDeclaration
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestVariableReferencedInInputNeedsDeclaration )
{
    const std::string expected =
        "<indicator>"
            "<constant id='$values' type='list(float)' value='va,lu,es'/>"
            "<reduce function='sum' id='1' input='$values' type='float'/>"
        "</indicator>";
    RegisterVariable( "values", "list(float)", "va,lu,es" );
    ParseAndCheck( "sum( $values )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestNestedFunctions
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestNestedFunctions )
{
    const std::string expected =
        "<indicator>"
            "<extract function='resources' id='1' resources='res,our,ces'/>"
            "<transform function='domain' id='2' input='1' select='un,its' type='int'/>"
            "<reduce function='sum' id='3' input='2' type='int'/>"
        "</indicator>";
    RegisterVariable( "MyResources", "list(resource-type)", "res,our,ces" );
    RegisterVariable( "MyUnits", "list(key)", "un,its" );
    ParseAndCheck( "sum( domain( resources( $MyResources ), $MyUnits ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestMultipleReferencedVariableIsDeclaredOnlyOnce
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestMultipleReferencedVariableIsDeclaredOnlyOnce )
{
    const std::string expected =
        "<indicator>"
            "<constant id='$loc' type='position' value='00UTM0000000000'/>"
            "<transform function='distance' id='1' input='$loc,$loc' type='float'/>"
        "</indicator>";
    RegisterVariable( "loc", "position", "00UTM0000000000" );
    ParseAndCheck( "distance( $loc, $loc )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestMultipleReferencedVariableIsDeclaredBeforeFirstReference
// Created: SBO 2009-04-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestMultipleReferencedVariableIsDeclaredBeforeFirstReference )
{
    const std::string expected =
        "<indicator>"
            "<constant id='$Zone' type='zone' value='circle(00UTM0000000000,00UTM0000000000)'/>"
            "<extract function='position' id='1'/>"
            "<extract function='direct-fire-unit' id='2'/>"
            "<transform function='compose' id='3' input='1,2' type='position'/>"
            "<transform function='contains' id='4' input='$Zone,3' type='bool'/>"
            "<extract function='position' id='5'/>"
            "<extract function='maintenance-handling-unit' id='6'/>"
            "<transform function='compose' id='7' input='5,6' type='position'/>"
            "<transform function='contains' id='8' input='$Zone,7' type='bool'/>"
            "<transform function='compare' id='9' input='4,8' operator='less' type='bool'/>"
        "</indicator>";
    RegisterVariable( "Zone", "zone", "circle(00UTM0000000000,00UTM0000000000)" );
    ParseAndCheck( "compare( 'less', contains( $Zone, Compose( position(), direct-fire-unit() ) ), contains( $Zone, Compose( position(), maintenance-handling-unit() ) ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestMultipleInputOrder
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestMultipleInputOrder )
{
    const std::string expected =
        "<indicator>"
            "<constant id='$loc1' type='position' value='00UTM0000000000'/>"
            "<constant id='$loc2' type='position' value='00UTM0000000001'/>"
            "<transform function='distance' id='1' input='$loc1,$loc2' type='float'/>"
        "</indicator>";
    RegisterVariable( "loc1", "position", "00UTM0000000000" );
    RegisterVariable( "loc2", "position", "00UTM0000000001" );
    ParseAndCheck( "distance( $loc1, $loc2 )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestOperationalState
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestOperationalState )
{
    const std::string expected =
        "<indicator>"
            "<extract function='operational-state' id='1'/>"
            "<reduce function='select' id='2' input='1' key='42' type='float'/>"
        "</indicator>";
    RegisterVariable( "Unit", "unit", "42" );
    ParseAndCheck( "Select( operational-state(), $Unit )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestMeanOperationalState
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestMeanOperationalState )
{
    const std::string expected =
        "<indicator>"
            "<extract function='operational-state' id='1'/>"
            "<transform function='domain' id='2' input='1' select='un,its' type='float'/>"
            "<reduce function='mean' id='3' input='2' type='float'/>"
        "</indicator>";
    RegisterVariable( "Units", "unit list", "un,its" );
    ParseAndCheck( "Mean( Domain( operational-state(), $Units ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestDistanceBetweenUnits
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestDistanceBetweenUnits )
{
    const std::string expected =
        "<indicator>"
            "<extract function='position' id='1'/>"
            "<reduce function='select' id='2' input='1' key='42' type='position'/>"
            "<extract function='position' id='3'/>"
            "<reduce function='select' id='4' input='3' key='51' type='position'/>"
            "<transform function='distance' id='5' input='2,4' type='float'/>"
        "</indicator>";
    RegisterVariable( "Unit1", "unit", "42" );
    RegisterVariable( "Unit2", "unit", "51" );
    ParseAndCheck( "Distance( Select( position(), $Unit1 ), Select( position(), $Unit2 ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestNumberOfBreakdown
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestNumberOfBreakdown )
{
    const std::string expected =
        "<indicator>"
            "<extract function='maintenance-handling-unit' id='1'/>"
            "<transform function='is-one-of' id='2' input='1' select='un,its' type='bool'/>"
            "<extract function='maintenance-handling-unit' id='3'/>"
            "<transform function='filter' id='4' input='2,3' type='unsigned long'/>"
            "<reduce function='count' id='5' input='4' type='unsigned long'/>"
        "</indicator>";
    RegisterVariable( "Units", "unit list", "un,its" );
    ParseAndCheck( "Count( Filter( Is-One-Of( maintenance-handling-unit(), $Units ), maintenance-handling-unit() ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestNumberOfDirectFire
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestNumberOfDirectFire )
{
    const std::string expected =
        "<indicator>"
            "<extract function='direct-fire-unit' id='1'/>"
            "<reduce function='count' id='2' input='1' type='unsigned long'/>"
        "</indicator>";
    ParseAndCheck( "Count( direct-fire-unit() )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestComponentDamages
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestComponentDamages )
{
    const std::string expected =
        "<indicator>"
            "<extract components='com,pon,ents' fire-types='firetypes' fratricide='friendlyfire' function='fire-component-damage' id='1' states='states'/>"
            "<reduce function='sum' id='2' input='1' type='float'/>"
        "</indicator>";
    RegisterVariable( "Components", "equipment list", "com,pon,ents" );
    RegisterVariable( "Equipmentstates", "equipment states", "states" );
    RegisterVariable( "Firetypes", "fire types", "firetypes" );
    RegisterVariable( "Fratricidefires", "fratricide", "friendlyfire" );
    ParseAndCheck( "Sum( fire-component-damage( $Components, $Equipmentstates, $Firetypes, $Fratricidefires ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestInflictedDamageFromDirectFiresFromZone
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestInflictedDamageFromDirectFiresFromZone )
{
    const std::string expected =
        "<indicator>"
            "<constant id='$Zone' type='zone' value='circle(00UTM0000000000,00UTM0000000000)'/>"
            "<extract function='position' id='1'/>"
            "<extract function='direct-fire-unit' id='2'/>"
            "<transform function='compose' id='3' input='1,2' type='position'/>"
            "<transform function='contains' id='4' input='$Zone,3' type='bool'/>"
            "<extract components='com,pon,ents' fire-types='firetypes' fratricide='friendlyfire' function='fire-component-damage' id='5' states='states'/>"
            "<transform function='filter' id='6' input='4,5' type='float'/>"
            "<reduce function='sum' id='7' input='6' type='float'/>"
        "</indicator>";
    RegisterVariable( "Components", "equipment list", "com,pon,ents" );
    RegisterVariable( "Equipmentstates", "equipment states", "states" );
    RegisterVariable( "Firetypes", "fire types", "firetypes" );
    RegisterVariable( "Fratricidefires", "fratricide", "friendlyfire" );
    RegisterVariable( "Zone", "zone", "circle(00UTM0000000000,00UTM0000000000)" );
    ParseAndCheck( "Sum( Filter( Contains( $Zone, Compose( position(), direct-fire-unit() ) ), fire-component-damage( $Components, $Equipmentstates, $Firetypes, $Fratricidefires ) ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestTotalResourcesForUnits
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestTotalResourcesForUnits )
{
    const std::string expected =
        "<indicator>"
            "<extract function='resources' id='1' resources='dot,ati,ons'/>"
            "<transform function='domain' id='2' input='1' select='un,its' type='int'/>"
            "<reduce function='sum' id='3' input='2' type='int'/>"
        "</indicator>";
    RegisterVariable( "Dotations", "dotation list", "dot,ati,ons" );
    RegisterVariable( "Units", "unit list", "un,its" );
    ParseAndCheck( "Sum( Domain( resources( $Dotations ), $Units ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestResourceConsumptionsForUnits
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestResourceConsumptionsForUnits )
{
    const std::string expected =
        "<indicator>"
            "<constant id='4' type='float' value='0'/>"
            "<extract function='resources' id='1' resources='res,our,ces'/>"
            "<transform function='derivate' id='3' input='1' period='1' type='int'/>"
            "<transform function='compare' id='5' input='3,4' operator='less' type='bool'/>"
            "<extract function='resources' id='6' resources='res,our,ces'/>"
            "<transform function='derivate' id='8' input='6' period='1' type='int'/>"
            "<transform function='filter' id='9' input='5,8' type='int'/>"
            "<transform function='domain' id='10' input='9' select='un,its' type='int'/>"
            "<reduce function='sum' id='11' input='10' type='int'/>"
        "</indicator>";
    RegisterVariable( "Resources", "dotation list", "res,our,ces" );
    RegisterVariable( "Units", "unit list", "un,its" );
    ParseAndCheck( "Sum( Domain( Filter( Compare( 'less', Derivate( resources( $Resources ), 1 ), 0 ), Derivate( resources( $Resources ), 1 ) ), $Units ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestEquipmentStatusForUnits
// Created: SBO 2009-12-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestEquipmentStatusForUnits )
{
    const std::string expected =
        "<indicator>"
            "<extract equipments='11' function='equipments' id='1' states='prisoner,repairable,repairing,unavailable'/>"
            "<transform function='domain' id='2' input='1' select='14,22,23,24,15,16,17,18,25,26,20,21' type='unsigned long'/>"
            "<reduce function='sum' id='3' input='2' type='unsigned long'/>"
        "</indicator>";

    RegisterVariable( "Units", "unit list", "14,22,23,24,15,16,17,18,25,26,20,21" );
    RegisterVariable( "MBT", "equipment list", "11" );
    RegisterVariable( "states", "equipment states", "prisoner,repairable,repairing,unavailable" );
    ParseAndCheck( "Sum( Domain( Equipments( $MBT, $states ), $Units ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestHumansStatusForUnits
// Created: SBO 2009-12-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestHumansStatusForUnits )
{
    const std::string expected =
        "<indicator>"
            "<extract function='humans' id='1' ranks='officer,sub-officer,troopers' states='dead'/>"
            "<transform function='domain' id='2' input='1' select='14,22,23,24,15,16,17,18,25,26,20,21' type='unsigned long'/>"
            "<reduce function='sum' id='3' input='2' type='unsigned long'/>"
        "</indicator>";

    RegisterVariable( "Units", "unit list", "14,22,23,24,15,16,17,18,25,26,20,21" );
    RegisterVariable( "ranks", "human ranks", "officer,sub-officer,troopers" );
    RegisterVariable( "states", "human states", "dead" );
    ParseAndCheck( "Sum( Domain( Humans( $ranks, $states ), $Units ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestUnitDetection
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestUnitDetection )
{
    const std::string expected =
        "<indicator>"
            "<extract detected='69' function='detecting-unit' id='1' visibility='detected,recognized,identified'/>"
            "<transform function='domain' id='2' input='1' select='42,51' type='unsigned long'/>"
            "<reduce function='sum' id='3' input='2' type='unsigned long'/>"
        "</indicator>";

    RegisterVariable( "units", "unit list", "42,51" );
    RegisterVariable( "unit", "unit", "69" );
    RegisterVariable( "visibility", "perception levels", "detected,recognized,identified" );
    ParseAndCheck( "Sum( Domain( Detecting-unit( $unit, $visibility), $units ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestUnitMounting
// Created: SBO 2010-07-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestUnitMounting )
{
    const std::string expected =
        "<indicator>"
            "<extract function='mounted' id='1'/>"
            "<reduce function='select' id='2' input='1' key='42' type='float'/>"
        "</indicator>";

    RegisterVariable( "unit", "unit", "42" );
    ParseAndCheck( "Select( mounted(), $unit )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestTimeBetweenDetectionAndDestruction
// Created: SBO 2010-07-12
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestTimeBetweenDetectionAndDestruction )
{
    const std::string expected =
         "<indicator>"
         "    <extract detected='69' function='detecting-unit' id='1' visibility='detected,recognized,identified'/>"
         "    <reduce function='select' id='2' input='1' key='42' type='unsigned long'/>"
         "    <extract function='operational-state' id='3'/>"
         "    <reduce function='select' id='4' input='3' key='69' type='float'/>"
         "    <reduce function='threshold' id='5' input='4' thresholds='0' type='float' values='0,1'/>"
         "    <reduce function='product' id='6' input='2,5' type='unsigned long'/>"
         "    <transform function='integrate' id='7' input='6' type='unsigned long'/>"
         "</indicator>";

    RegisterVariable( "unitDetecting", "unit", "42" );
    RegisterVariable( "unitDetected", "unit", "69" );
    RegisterVariable( "visibility", "perception levels", "detected,recognized,identified" );
    ParseAndCheck( "Integrate( Product( Select( detecting-unit( $unitDetected, $visibility ), $unitDetecting ), Threshold( '0', '0,1', Select( operational-state(), $unitDetected ) ) ) )", expected );
}

BOOST_AUTO_TEST_SUITE_END()
