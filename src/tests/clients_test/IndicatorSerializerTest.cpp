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
#include "preparation/IndicatorParser.h"
#include "preparation/IndicatorSerializer.h"
#include "preparation/IndicatorElement_ABC.h"
#include "preparation/IndicatorElementFactory.h"
#include "preparation/IndicatorPrimitives.h"
#include "preparation/IndicatorVariable.h"
#include "preparation/IndicatorVariables.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

namespace
{
    class SerializerFixture
    {
    public:
        SerializerFixture() : factory_( primitives_, variables_ ) {}

        void ParseAndCheck( const std::string& text, const std::string& expected )
        {
            xml::xostringstream xos;
            {
                IndicatorSerializer handler( factory_, xos );
                IndicatorParser parser( handler );
                parser.Parse( text );
            }
            BOOST_CHECK_XML_EQUAL( expected, xos.str() );
        }

        void RegisterVariable( const std::string& name, const std::string& type )
        {
            variables_.Register( name, boost::shared_ptr< IndicatorElement_ABC >( new IndicatorVariable( name, type ) ) );
        }

    private:
        IndicatorPrimitives primitives_;
        IndicatorVariables variables_;
        IndicatorElementFactory factory_;
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
// Name: IndicatorSerializer_TestExtractor
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestExtractor )
{
    const std::string expected = 
        "<indicator>"
            "<extract function='operational-state' id='1'/>"
        "</indicator>";
    ParseAndCheck( "operational-state()", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestExtractorCaseInsensitive
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestExtractorCaseInsensitive )
{
    const std::string expected = 
        "<indicator>"
            "<extract function='operational-state' id='1'/>"
        "</indicator>";
    ParseAndCheck( "OpEraTionAl-StaTe()", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestExtractorWithParameters
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestExtractorWithParameters )
{
    const std::string expected = 
        "<indicator>"
            "<extract equipments='$Equipments' function='equipments' id='1' states='$States'/>"
        "</indicator>";
    RegisterVariable( "Equipments", "list(equipment-type)" );
    RegisterVariable( "States", "list(equipment-state)" );
    ParseAndCheck( "equipments( $Equipments, $States )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestFunctionWithTemplateReturnType
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestFunctionWithTemplateReturnType )
{
    const std::string expected = 
        "<indicator>"
            "<reduce function='sum' id='1' input='$values' type='float'/>"
        "</indicator>";
    RegisterVariable( "values", "list(float)" );
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
            "<extract dotations='$MyResources' function='resources' id='1'/>"
            "<transform function='domain' id='2' input='1' select='$MyUnits' type='int'/>"
            "<reduce function='sum' id='3' input='2' type='int'/>"
        "</indicator>";
    RegisterVariable( "MyResources", "list(resource-type)" );
    RegisterVariable( "MyUnits", "list(key)" );
    ParseAndCheck( "sum( domain( resources( $MyResources ), $MyUnits ) )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestMultipleVariableReferences
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestMultipleVariableReferences )
{
    const std::string expected = 
        "<indicator>"
            "<transform function='distance' id='1' input='$loc,$loc' type='float'/>"
        "</indicator>";
    RegisterVariable( "loc", "position" );
    ParseAndCheck( "distance( $loc, $loc )", expected );
}

// -----------------------------------------------------------------------------
// Name: IndicatorSerializer_TestMultipleInputOrder
// Created: SBO 2009-03-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( IndicatorSerializer_TestMultipleInputOrder )
{
    const std::string expected = 
        "<indicator>"
            "<transform function='distance' id='1' input='$loc1,$loc2' type='float'/>"
        "</indicator>";
    RegisterVariable( "loc1", "position" );
    RegisterVariable( "loc2", "position" );
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
            "<reduce function='select' id='2' input='1' key='$Unit' type='float'/>"
        "</indicator>";
    RegisterVariable( "Unit", "key" );
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
            "<transform function='domain' id='2' input='1' select='$Units' type='float'/>"
            "<reduce function='mean' id='3' input='2' type='float'/>"
        "</indicator>";
    RegisterVariable( "Units", "list(key)" );
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
            "<reduce function='select' id='2' input='1' key='$Unit1' type='position'/>"
            "<extract function='position' id='3'/>"
            "<reduce function='select' id='4' input='3' key='$Unit2' type='position'/>"
            "<transform function='distance' id='5' input='2,4' type='float'/>"
        "</indicator>";
    RegisterVariable( "Unit1", "key" );
    RegisterVariable( "Unit2", "key" );
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
            "<transform function='is-one-of' id='2' input='1' select='$Units' type='bool'/>"
            "<extract function='maintenance-handling-unit' id='3'/>"
            "<transform function='filter' id='4' input='2,3' type='unsigned long'/>"
            "<reduce function='count' id='5' input='4' type='unsigned'/>"
        "</indicator>";
    RegisterVariable( "Units", "list(key)" );
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
            "<extract components='$Components' function='fire-component-damage' id='1'/>"
            "<reduce function='sum' id='2' input='1' type='float'/>"
        "</indicator>";
    RegisterVariable( "Components", "list(key)" );
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
            "<extract function='position' id='1'/>"
            "<extract function='direct-fire-unit' id='2'/>"
            "<transform function='compose' id='3' input='1,2' type='position'/>"
            "<transform function='contains' id='4' input='$Zone,3' type='bool'/>"
            "<extract components='$Components' function='fire-component-damage' id='5'/>"
            "<transform function='filter' id='6' input='4,5' type='float'/>"
            "<reduce function='sum' id='7' input='6' type='float'/>"
        "</indicator>";
    RegisterVariable( "Components", "list(key)" );
    RegisterVariable( "Zone", "zone" );
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
            "<extract dotations='$Dotations' function='resources' id='1'/>"
            "<transform function='domain' id='2' input='1' select='$Units' type='int'/>"
            "<reduce function='sum' id='3' input='2' type='int'/>"
        "</indicator>";
    RegisterVariable( "Dotations", "list(key)" );
    RegisterVariable( "Units", "list(key)" );
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
            "<extract dotations='$Resources' function='resources' id='1'/>"
            "<transform function='derivate' id='2' input='1' type='int'/>"
            "<constant id='3' type='float' value='0'/>"
            "<transform function='compare' id='4' input='2,3' operator='less' type='bool'/>"
            "<extract dotations='$Resources' function='resources' id='5'/>"
            "<transform function='derivate' id='6' input='5' type='int'/>"
            "<transform function='filter' id='7' input='4,6' type='int'/>"
            "<transform function='domain' id='8' input='7' select='$Units' type='int'/>"
            "<reduce function='sum' id='9' input='8' type='int'/>"
        "</indicator>";
    RegisterVariable( "Resources", "list(key)" );
    RegisterVariable( "Units", "list(key)" );
    ParseAndCheck( "Sum( Domain( Filter( Compare( 'less', Derivate( resources( $Resources ) ), 0 ), Derivate( resources( $Resources ) ) ), $Units ) )", expected );
}

BOOST_AUTO_TEST_SUITE_END()
