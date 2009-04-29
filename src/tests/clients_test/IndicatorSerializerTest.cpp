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
#include "indicators/FormulaParser.h"
#include "indicators/Serializer.h"
#include "indicators/Element_ABC.h"
#include "indicators/ElementFactory.h"
#include "indicators/Primitives.h"
#include "indicators/Variable.h"
#include "indicators/Variables.h"
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

namespace
{
    class SerializerFixture
    {
    public:
        SerializerFixture()
            : factory_( primitives_, variables_ )
        {
            primitives_.Load( tools::GeneralConfig::BuildResourceChildFile( "IndicatorPrimitives.xml" ) );
        }

        void ParseAndCheck( const std::string& text, const std::string& expected )
        {
            xml::xostringstream xos;
            indicators::Serializer handler( factory_, variables_ );
            indicators::FormulaParser parser( handler );
            parser.Parse( text );
            handler.Serialize( xos );
            BOOST_CHECK_XML_EQUAL( expected, xos.str() );
        }

        void RegisterVariable( const std::string& name, const std::string& type, const std::string& value )
        {
            variables_.Register( name, boost::shared_ptr< indicators::Element_ABC >( new indicators::Variable( name, type, value ) ) );
        }

    private:
        indicators::Primitives primitives_;
        indicators::Variables variables_;
        indicators::ElementFactory factory_;
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
            "<extract dotations='res,our,ces' function='resources' id='1'/>"
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
    RegisterVariable( "Unit", "key", "42" );
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
    RegisterVariable( "Units", "list(key)", "un,its" );
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
    RegisterVariable( "Unit1", "key", "42" );
    RegisterVariable( "Unit2", "key", "51" );
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
            "<reduce function='count' id='5' input='4' type='unsigned'/>"
        "</indicator>";
    RegisterVariable( "Units", "list(key)", "un,its" );
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
            "<reduce function='count' id='2' input='1' type='unsigned'/>"
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
            "<extract components='com,pon,ents' function='fire-component-damage' id='1'/>"
            "<reduce function='sum' id='2' input='1' type='float'/>"
        "</indicator>";
    RegisterVariable( "Components", "list(key)", "com,pon,ents" );
    ParseAndCheck( "Sum( fire-component-damage( $Components ) )", expected );
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
            "<extract components='com,pon,ents' function='fire-component-damage' id='5'/>"
            "<transform function='filter' id='6' input='4,5' type='float'/>"
            "<reduce function='sum' id='7' input='6' type='float'/>"
        "</indicator>";
    RegisterVariable( "Components", "list(key)", "com,pon,ents" );
    RegisterVariable( "Zone", "zone", "circle(00UTM0000000000,00UTM0000000000)" );
    ParseAndCheck( "Sum( Filter( Contains( $Zone, Compose( position(), direct-fire-unit() ) ), fire-component-damage( $Components ) ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestTotalResourcesForUnits
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestTotalResourcesForUnits )
{
    const std::string expected = 
        "<indicator>"
            "<extract dotations='dot,ati,ons' function='resources' id='1'/>"
            "<transform function='domain' id='2' input='1' select='un,its' type='int'/>"
            "<reduce function='sum' id='3' input='2' type='int'/>"
        "</indicator>";
    RegisterVariable( "Dotations", "list(key)", "dot,ati,ons" );
    RegisterVariable( "Units", "list(key)", "un,its" );
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
            "<constant id='3' type='float' value='0'/>"
            "<extract dotations='res,our,ces' function='resources' id='1'/>"
            "<transform function='derivate' id='2' input='1' type='int'/>"
            "<transform function='compare' id='4' input='2,3' operator='less' type='bool'/>"
            "<extract dotations='res,our,ces' function='resources' id='5'/>"
            "<transform function='derivate' id='6' input='5' type='int'/>"
            "<transform function='filter' id='7' input='4,6' type='int'/>"
            "<transform function='domain' id='8' input='7' select='un,its' type='int'/>"
            "<reduce function='sum' id='9' input='8' type='int'/>"
        "</indicator>";
    RegisterVariable( "Resources", "list(key)", "res,our,ces" );
    RegisterVariable( "Units", "list(key)", "un,its" );
    ParseAndCheck( "Sum( Domain( Filter( Compare( 'less', Derivate( resources( $Resources ) ), 0 ), Derivate( resources( $Resources ) ) ), $Units ) )", expected );
}

BOOST_AUTO_TEST_SUITE_END()
