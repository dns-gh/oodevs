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
#include "MockValueHandler.h"

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
    NullValueHandler< float >         valueHandler;
    NullValueHandler< unsigned long > keyHandler;
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
    ASN1T_MsgsSimToClient MakeMessage( ASN1T_MsgUnitAttributes& attributes, unsigned long id = 0 )
    {
        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_unit_attributes;
        result.msg.u.msg_unit_attributes = &attributes;
        attributes.oid = id;
        return result;
    }

    void Evaluate( ModelFunction_ABC& function, const ASN1T_MsgsSimToClient& message )
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
    MockValueHandler< float > handler;

    std::auto_ptr< ModelFunction_ABC > function( new ModelFunction< attributes::OperationalState >( handler ) );
    handler.BeginTick_mocker.expects( exactly( 4 ) );
    handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( 0.25f ) );
    handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( unsigned( 30 ) * 0.01f ) );
    handler.EndTick_mocker.expects( exactly( 4 ) );


    ASN1T_MsgUnitAttributes attributes;
    attributes.etat_operationnel_brut = 25;
    {
        attributes.m.etat_operationnel_brutPresent = 1;
        Evaluate( *function, MakeMessage( attributes ) );

        attributes.m.etat_operationnel_brutPresent = 0;
        Evaluate( *function, MakeMessage( attributes ) );
    
        attributes.etat_operationnel_brut = 30;
        attributes.m.etat_operationnel_brutPresent = 1;
        Evaluate( *function, MakeMessage( attributes ) );

        attributes.m.etat_operationnel_brutPresent = 0;
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
    MockValueHandler< float >         handler;
    MockValueHandler< unsigned long > keyHandler;

    DispatcherFactory< IdentifierValue, attributes::OperationalState > factory;
    boost::shared_ptr< ModelFunction_ABC > function( factory( keyHandler, handler ) );

    handler.BeginTick_mocker.expects( exactly( 4 ) );
    keyHandler.BeginTick_mocker.expects( exactly( 4 ) );
    handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( 0.25f ) );
    keyHandler.Handle_mocker.expects( once() ).with( eq( unsigned long( 1 ) ) );
    keyHandler.Handle_mocker.expects( once() ).with( eq( unsigned long( 2 ) ) );
    handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( unsigned( 30 ) * 0.01f ) );
    keyHandler.Handle_mocker.expects( once() ).with( eq( unsigned long( 1 ) ) );
    keyHandler.Handle_mocker.expects( once() ).with( eq( unsigned long( 2 ) ) );
    handler.EndTick_mocker.expects( exactly( 4 ) );
    keyHandler.EndTick_mocker.expects( exactly( 4 ) );


    ASN1T_MsgUnitAttributes attributes;
    attributes.etat_operationnel_brut = 25;
    attributes.m.etat_operationnel_brutPresent = 1;
    {
        function->BeginTick();
        function->Receive( MakeMessage( attributes, 1 ) );
        function->Receive( MakeMessage( attributes, 2 ) );
        function->EndTick();
    
        attributes.etat_operationnel_brut = 30;
        function->BeginTick();
        function->Receive( MakeMessage( attributes, 1 ) );
        function->Receive( MakeMessage( attributes, 2 ) );
        function->EndTick();
    }
    handler.verify();
    keyHandler.verify();
}
