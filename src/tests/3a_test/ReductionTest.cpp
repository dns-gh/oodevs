// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "3a_test_pch.h"
#include "MockFunctions.h"
#include "3a/Adder.h"
#include "3a/Count.h"
#include "3a/Meaner.h"
#include "3a/Selector.h"
#include "3a/Functions.h"
#include <xeumeuleu/xml.h>

using namespace mockpp;

namespace
{
    template< typename K, typename T >
    void ReduceSimple( Function1_ABC< K, T >& function )
    {
        function.BeginTick();
        function.SetKey( 1 );
        function.Apply( 18.f );
        function.SetKey( 2 );
        function.Apply( 24.f );
        function.EndTick();
    }

    template< typename K, typename T >
    void ReduceTwoTicks( Function1_ABC< K, T >& function )
    {
        function.BeginTick();
        function.SetKey( 1 );
        function.Apply( 18.f );
        function.EndTick();
        function.BeginTick();
        function.SetKey( 2 );
        function.Apply( 18.f );
        function.EndTick();
    }
}

// -----------------------------------------------------------------------------
// Name: Reduction_TestAdder
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Reduction_TestAdder )
{
    MockFunction1< unsigned long, float > handler;
    std::auto_ptr< Function1_ABC< unsigned long, float > > adder( new Adder< unsigned long, float >( handler ) );
    {
        handler.BeginTick_mocker.expects( once() );
        handler.Apply_mocker.expects( once() ).with( eq( 42.f ) );
        handler.EndTick_mocker.expects( once() );
        ReduceSimple( *adder );
    }
    {
        handler.BeginTick_mocker.expects( exactly( 2 ) );
        handler.Apply_mocker.expects( exactly( 2 ) ).with( eq( 18.f ) );
        handler.EndTick_mocker.expects( exactly( 2 ) );
        ReduceTwoTicks( *adder );
    }
}

// -----------------------------------------------------------------------------
// Name: Reduction_TestCount
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Reduction_TestCount )
{
    MockFunction1< unsigned long, unsigned > handler;
    std::auto_ptr< Function1_ABC< unsigned long, float > > counter( new Count< unsigned long, float >( handler ) );
    {
        handler.BeginTick_mocker.expects( once() );
        handler.Apply_mocker.expects( once() ).with( eq( 2u ) );
        handler.EndTick_mocker.expects( once() );
        ReduceSimple( *counter );
    }
    {
        handler.BeginTick_mocker.expects( exactly( 2 ) );
        handler.Apply_mocker.expects( exactly( 2 ) ).with( eq( 1u ) );
        handler.EndTick_mocker.expects( exactly( 2 ) );
        ReduceTwoTicks( *counter );
    }
}

// -----------------------------------------------------------------------------
// Name: Reduction_TestMeaner
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Reduction_TestMeaner )
{
    MockFunction1< unsigned long, float > handler;
    std::auto_ptr< Function1_ABC< unsigned long, float > > meaner( new Meaner< unsigned long, float >( handler ) );
    {
        handler.BeginTick_mocker.expects( once() );
        handler.Apply_mocker.expects( once() ).with( eq( 21.f ) );
        handler.EndTick_mocker.expects( once() );
        ReduceSimple( *meaner );
    }
    {
        handler.BeginTick_mocker.expects( exactly( 2 ) );
        handler.Apply_mocker.expects( exactly( 2 ) ).with( eq( 18.f ) );
        handler.EndTick_mocker.expects( exactly( 2 ) );
        ReduceTwoTicks( *meaner );
    }
}

// -----------------------------------------------------------------------------
// Name: Reduction_TestSelector
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Reduction_TestSelector )
{
    MockFunction1< unsigned long, float > handler;
    std::auto_ptr< Function1_ABC< unsigned long, float > > selector( new Selector< unsigned long, float >( 2, handler ) );
    {
        handler.BeginTick_mocker.expects( once() );
        handler.Apply_mocker.expects( once() ).with( eq( 24.f ) );
        handler.EndTick_mocker.expects( once() );
        ReduceSimple( *selector );
    }
    {
        handler.BeginTick_mocker.expects( exactly( 2 ) );
        handler.Apply_mocker.expects( exactly( 2 ) ).with( eq( 18.f ) );
        handler.EndTick_mocker.expects( exactly( 2 ) );
        ReduceTwoTicks( *selector );
    }
}