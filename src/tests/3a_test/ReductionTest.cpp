// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "3a_test_pch.h"
#include "MockValueHandler.h"
#include "3a/Adder.h"
#include "3a/Count.h"
#include "3a/Meaner.h"
#include "3a/Selector.h"
#include "3a/Functions.h"

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
    MockValueHandler< float > handler;
    std::auto_ptr< Function1_ABC< unsigned long, float > > adder( new Adder< unsigned long, float >( handler ) );
    {
        handler.Handle_mocker.expects( once() ).with( eq( 42.f ) );
        ReduceSimple( *adder );
    }
    {
        handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( 18.f ) );
        ReduceTwoTicks( *adder );
    }
}

// -----------------------------------------------------------------------------
// Name: Reduction_TestCount
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Reduction_TestCount )
{
    MockValueHandler< unsigned > handler;
    std::auto_ptr< Function1_ABC< unsigned long, float > > counter( new Count< unsigned long, float >( handler ) );
    {
        handler.Handle_mocker.expects( once() ).with( eq( 2u ) );
        ReduceSimple( *counter );
    }
    {
        handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( 1u ) );
        ReduceTwoTicks( *counter );
    }
}

// -----------------------------------------------------------------------------
// Name: Reduction_TestMeaner
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Reduction_TestMeaner )
{
    MockValueHandler< float > handler;
    std::auto_ptr< Function1_ABC< unsigned long, float > > meaner( new Meaner< unsigned long, float >( handler ) );
    {
        handler.Handle_mocker.expects( once() ).with( eq( 21.f ) );
        ReduceSimple( *meaner );
    }
    {
        handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( 18.f ) );
        ReduceTwoTicks( *meaner );
    }
}

// -----------------------------------------------------------------------------
// Name: Reduction_TestSelector
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Reduction_TestSelector )
{
    MockValueHandler< float > handler;
    std::auto_ptr< Function1_ABC< unsigned long, float > > selector( new Selector< unsigned long, float >( 2, handler ) );
    {
        handler.Handle_mocker.expects( once() ).with( eq( 24.f ) );
        ReduceSimple( *selector );
    }
    {
        handler.Handle_mocker.expects( exactly( 2 ) ).with( eq( 18.f ) );
        ReduceTwoTicks( *selector );
    }
}