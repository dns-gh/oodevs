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
#include "3a/Zone.h"
#include "3a/IdentifierValue.h"
#include "3a/Types.h"
#include "MockValueHandler.h"
#include "protocol/Protocol.h"

namespace
{
    template< typename T >
    struct NullValueHandler : public ValueHandler_ABC< T >
    {
        virtual void BeginTick() {}
        virtual void Handle( const T& ) {}
        virtual void EndTick() {}
    };
}

// -----------------------------------------------------------------------------
// Name: Model_TestInstanciation
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Model_TestInstanciation )
{
    NullValueHandler< NumericValue > valueHandler;
    NullValueHandler< NumericValue > keyHandler;
    {
        std::unique_ptr< ModelFunction_ABC > function( new ModelFunction< attributes::OperationalState >( valueHandler ) );
    }
    {
        DispatcherFactory< IdentifierValue, attributes::OperationalState > factory;
        boost::shared_ptr< ModelFunction_ABC > function( factory( keyHandler, valueHandler ) );
    }
}

namespace
{
    sword::SimToClient MakeMessage( sword::UnitAttributes& attributes, unsigned long id = 0 )
    {
        sword::SimToClient result;
        attributes.mutable_unit()->set_id( id );
        *result.mutable_message()->mutable_unit_attributes() = attributes;
        return result;
    }
    void Evaluate( ModelFunction_ABC& function, const sword::SimToClient& message )
    {
        function.BeginTick();
        function.Receive( message );
        function.EndTick();
    }
    bool CheckClose( const NumericValue& expected, const NumericValue& actual, const NumericValue& epsilon )
    {
        if( expected == actual )
            return true;
        const NumericValue relativeError = std::abs( ( expected - actual ) / actual );
        if( relativeError <= epsilon )
            return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: Model_TestValueExtraction
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Model_TestValueExtraction )
{
    const NumericValue epsilon( 0.001f );
    MockValueHandler< NumericValue > handler;
    std::unique_ptr< ModelFunction_ABC > function( new ModelFunction< attributes::OperationalState >( handler ) );
    MOCK_EXPECT( handler.BeginTick ).exactly( 4 );
    MOCK_EXPECT( handler.Handle ).exactly( 2 ).with( boost::bind( &CheckClose, NumericValue( 0.25f ), _1, epsilon ) );
    MOCK_EXPECT( handler.Handle ).exactly( 2 ).with( boost::bind( &CheckClose, NumericValue( 0.3f ), _1, epsilon ) );
    MOCK_EXPECT( handler.EndTick ).exactly( 4 );
    sword::UnitAttributes attributes;
    attributes.set_raw_operational_state( 25 );
    {
        Evaluate( *function, MakeMessage( attributes ) );
        Evaluate( *function, MakeMessage( attributes ) );
        attributes.set_raw_operational_state( 30 );
        Evaluate( *function, MakeMessage( attributes ) );
        Evaluate( *function, MakeMessage( attributes ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Model_TestDispatchedValueExtraction
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Model_TestDispatchedValueExtraction )
{
    const NumericValue epsilon( 0.001f );
    MockValueHandler< NumericValue > handler;
    MockValueHandler< NumericValue > keyHandler;
    DispatcherFactory< IdentifierValue, attributes::OperationalState > factory;
    boost::shared_ptr< ModelFunction_ABC > function( factory( keyHandler, handler ) );
    MOCK_EXPECT( handler.BeginTick ).exactly( 4 );
    MOCK_EXPECT( keyHandler.BeginTick ).exactly( 4 );
    MOCK_EXPECT( handler.Handle ).exactly( 2 ).with( boost::bind( &CheckClose, NumericValue( 0.25f ), _1, epsilon ) );
    MOCK_EXPECT( keyHandler.Handle ).once().with( NumericValue( 1 ) );
    MOCK_EXPECT( keyHandler.Handle ).once().with( NumericValue( 2 ) );
    MOCK_EXPECT( handler.Handle ).exactly( 2 ).with( boost::bind( &CheckClose, NumericValue( 0.3f ), _1, epsilon ) );
    MOCK_EXPECT( keyHandler.Handle ).once().with( NumericValue( 1 ) );
    MOCK_EXPECT( keyHandler.Handle ).once().with( NumericValue( 2 ) );
    MOCK_EXPECT( handler.EndTick ).exactly( 4 );
    MOCK_EXPECT( keyHandler.EndTick ).exactly( 4 );
    sword::UnitAttributes attributes;
    attributes.set_raw_operational_state( 25 );
    {
        function->BeginTick();
        function->Receive( MakeMessage( attributes, 1 ) );
        function->Receive( MakeMessage( attributes, 2 ) );
        function->EndTick();
        attributes.set_raw_operational_state( 30 );
        function->BeginTick();
        function->Receive( MakeMessage( attributes, 1 ) );
        function->Receive( MakeMessage( attributes, 2 ) );
        function->EndTick();
    }
}

// -----------------------------------------------------------------------------
// Name: Model_TestZone
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Model_TestZone )
{
    {
        const std::string circle = "circle(31TBN7728449220,31TBN7728449216)";
        std::stringstream is( circle );
        Zone zone;
        is >> zone;
        BOOST_CHECK( zone.Contains( Position( "31TBN7728449222" ) ) );
        BOOST_CHECK( ! zone.Contains( Position( "31TBN7728449225" ) ) );
    }
    {
        const std::string polygon = "polygon(31TBN7728449220,31TBN7728449216,31TBN7728049216,31TBN7728049220)";
        std::stringstream is( polygon );
        Zone zone;
        is >> zone;
        BOOST_CHECK( zone.Contains( Position( "31TBN7728249218" ) ) );
        BOOST_CHECK( ! zone.Contains( Position( "31TBN7728249212" ) ) );
    }
}
