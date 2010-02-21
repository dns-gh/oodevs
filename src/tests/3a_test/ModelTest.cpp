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

using namespace MsgsSimToClient;
using namespace mockpp;

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
        std::auto_ptr< ModelFunction_ABC > function( new ModelFunction< attributes::OperationalState >( valueHandler ) );
    }
    {
        DispatcherFactory< IdentifierValue, attributes::OperationalState > factory;
        boost::shared_ptr< ModelFunction_ABC > function( factory( keyHandler, valueHandler ) );
    }
}

namespace
{
    MsgSimToClient MakeMessage( MsgUnitAttributes& attributes, unsigned long id = 0 )
    {
        MsgSimToClient result;
        attributes.set_oid( id );
        *result.mutable_message()->mutable_unit_attributes() = attributes;
        return result;
    }

    void Evaluate( ModelFunction_ABC& function, const MsgSimToClient& message )
    {
        function.BeginTick();
        function.Receive( message );
        function.EndTick();
    }
}

// -----------------------------------------------------------------------------
// Name: Model_TestValueExtraction
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Model_TestValueExtraction )
{
    static const NumericValue epsilon( 0.001f );
    MockValueHandler< NumericValue > handler;

    std::auto_ptr< ModelFunction_ABC > function( new ModelFunction< attributes::OperationalState >( handler ) );
    handler.BeginTick_mocker.expects( exactly( 4 ) );
    handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( NumericValue( 0.25f ), epsilon ) );
    handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( NumericValue( 0.3f ), epsilon ) );
    handler.EndTick_mocker.expects( exactly( 4 ) );


    MsgUnitAttributes attributes;
    attributes.set_etat_operationnel_brut( 25 );
    {
        Evaluate( *function, MakeMessage( attributes ) );
        Evaluate( *function, MakeMessage( attributes ) );
   
        attributes.set_etat_operationnel_brut( 30 );
        Evaluate( *function, MakeMessage( attributes ) );
        Evaluate( *function, MakeMessage( attributes ) );
    }

    handler.verify();
}

// -----------------------------------------------------------------------------
// Name: Model_TestDispatchedValueExtraction
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Model_TestDispatchedValueExtraction )
{
    static const NumericValue epsilon( 0.001f );
    MockValueHandler< NumericValue > handler;
    MockValueHandler< NumericValue > keyHandler;

    DispatcherFactory< IdentifierValue, attributes::OperationalState > factory;
    boost::shared_ptr< ModelFunction_ABC > function( factory( keyHandler, handler ) );

    handler.BeginTick_mocker.expects( exactly( 4 ) );
    keyHandler.BeginTick_mocker.expects( exactly( 4 ) );
    handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( NumericValue( 0.25f ), epsilon ) );
    keyHandler.Handle_mocker.expects( once() ).with( eq( NumericValue( 1 ) ) );
    keyHandler.Handle_mocker.expects( once() ).with( eq( NumericValue( 2 ) ) );
    handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( NumericValue( 0.3f ), epsilon ) );
    keyHandler.Handle_mocker.expects( once() ).with( eq( NumericValue( 1 ) ) );
    keyHandler.Handle_mocker.expects( once() ).with( eq( NumericValue( 2 ) ) );
    handler.EndTick_mocker.expects( exactly( 4 ) );
    keyHandler.EndTick_mocker.expects( exactly( 4 ) );


    MsgUnitAttributes attributes;
    attributes.set_etat_operationnel_brut( 25 );
    {
        function->BeginTick();
        function->Receive( MakeMessage( attributes, 1 ) );
        function->Receive( MakeMessage( attributes, 2 ) );
        function->EndTick();
    
        attributes.set_etat_operationnel_brut( 30 );
        function->BeginTick();
        function->Receive( MakeMessage( attributes, 1 ) );
        function->Receive( MakeMessage( attributes, 2 ) );
        function->EndTick();
    }
    handler.verify();
    keyHandler.verify();
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
        BOOST_CHECK(   zone.Contains( Position( "31TBN7728449222" ) ) );
        BOOST_CHECK( ! zone.Contains( Position( "31TBN7728449225" ) ) );
    }
    {
        const std::string polygon = "polygon(31TBN7728449220,31TBN7728449216,31TBN7728049216,31TBN7728049220)";
        std::stringstream is( polygon );
        Zone zone;
        is >> zone;
        BOOST_CHECK(   zone.Contains( Position( "31TBN7728249218" ) ) );
        BOOST_CHECK( ! zone.Contains( Position( "31TBN7728249212" ) ) );
    }
}
