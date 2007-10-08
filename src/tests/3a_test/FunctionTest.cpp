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
#include "3a/Functions.h"
#include "3a/KeyMarshaller.h"
#include "3a/Composer.h"

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: Function_TestMarshallingWithNoKeyJustForwards
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Function_TestMarshallingWithNoKeyJustForwards )
{
    MockFunction2< unsigned long, float, unsigned > function;
    KeyMarshaller< unsigned long, float, unsigned > marshaller( function );

    marshaller.FirstParameter().BeginTick();
    marshaller.FirstParameter().Apply( 42.f );
    marshaller.FirstParameter().EndTick();

    marshaller.SecondParameter().BeginTick();
    marshaller.SecondParameter().Apply( 12u );

    function.BeginTick_mocker.expects( once() );
    function.Apply_mocker.expects( once() ).with( eq( 42.f ), eq( 12u ) ); ;
    function.EndTick_mocker.expects( once() );

    marshaller.SecondParameter().EndTick();

    function.verify();
}

// -----------------------------------------------------------------------------
// Name: Function_TestMarshallingWithOneKeyForwardsKey
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Function_TestMarshallingWithOneKeyForwardsKey )
{
    MockFunction2< unsigned long, float, unsigned > function;
    KeyMarshaller< unsigned long, float, unsigned > marshaller( function );
    {
        marshaller.FirstParameter().BeginTick();
        marshaller.FirstParameter().SetKey( 1 );
        marshaller.FirstParameter().Apply( 42.f );
        marshaller.FirstParameter().SetKey( 2 );
        marshaller.FirstParameter().Apply( 43.f );
        marshaller.FirstParameter().SetKey( 3 );
        marshaller.FirstParameter().Apply( 44.f );
        marshaller.FirstParameter().EndTick();

        marshaller.SecondParameter().BeginTick();
        marshaller.SecondParameter().Apply( 12u );

        function.BeginTick_mocker.expects( once() );
        function.SetKey_mocker.expects( once() ).with( eq( 1ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 42.f ), eq( 12u ) );
        function.SetKey_mocker.expects( once() ).with( eq( 2ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 43.f ), eq( 12u ) );
        function.SetKey_mocker.expects( once() ).with( eq( 3ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 44.f ), eq( 12u ) );
        function.EndTick_mocker.expects( once() );

        marshaller.SecondParameter().EndTick();
        function.verify();
    }

    {
        marshaller.FirstParameter().BeginTick();
        marshaller.FirstParameter().Apply( 42.f );
        marshaller.FirstParameter().EndTick();

        marshaller.SecondParameter().BeginTick();
        marshaller.SecondParameter().SetKey( 1 );
        marshaller.SecondParameter().Apply( 12u );
        marshaller.SecondParameter().SetKey( 2 );
        marshaller.SecondParameter().Apply( 13u );
        marshaller.SecondParameter().SetKey( 3 );
        marshaller.SecondParameter().Apply( 14u );

        function.BeginTick_mocker.expects( once() );
        function.SetKey_mocker.expects( once() ).with( eq( 1ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 42.f ), eq( 12u ) );
        function.SetKey_mocker.expects( once() ).with( eq( 2ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 42.f ), eq( 13u ) );
        function.SetKey_mocker.expects( once() ).with( eq( 3ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 42.f ), eq( 14u ) );
        function.EndTick_mocker.expects( once() );

        marshaller.SecondParameter().EndTick();
        function.verify();
    }
}

// -----------------------------------------------------------------------------
// Name: Function_TestMarshallingWithTwoKeysSortsKeysBack
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Function_TestMarshallingWithTwoKeysSortsKeysBack )
{
    MockFunction2< unsigned long, float, unsigned > function;
    KeyMarshaller< unsigned long, float, unsigned > marshaller( function );
    {
        marshaller.FirstParameter().BeginTick();
        marshaller.FirstParameter().SetKey( 1 );
        marshaller.FirstParameter().Apply( 42.f );
        marshaller.FirstParameter().SetKey( 2 );
        marshaller.FirstParameter().Apply( 43.f );
        marshaller.FirstParameter().SetKey( 3 );
        marshaller.FirstParameter().Apply( 44.f );
        marshaller.FirstParameter().EndTick();

        marshaller.SecondParameter().SetKey( 3 );
        marshaller.SecondParameter().Apply( 14u );
        marshaller.SecondParameter().BeginTick();
        marshaller.SecondParameter().SetKey( 1 );
        marshaller.SecondParameter().Apply( 12u );
        marshaller.SecondParameter().SetKey( 2 );
        marshaller.SecondParameter().Apply( 13u );

        function.BeginTick_mocker.expects( once() );
        function.SetKey_mocker.expects( once() ).with( eq( 1ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 42.f ), eq( 12u ) );
        function.SetKey_mocker.expects( once() ).with( eq( 2ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 43.f ), eq( 13u ) );
        function.SetKey_mocker.expects( once() ).with( eq( 3ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 44.f ), eq( 14u ) );
        function.EndTick_mocker.expects( once() );

        marshaller.SecondParameter().EndTick();
        function.verify();
    }
}

// -----------------------------------------------------------------------------
// Name: Function_TestMarshallingWithTwoKeysOnlyForwardsWhenBothKeysArePresent
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Function_TestMarshallingWithTwoKeysOnlyForwardsWhenBothKeysArePresent )
{
    MockFunction2< unsigned long, float, unsigned > function;
    KeyMarshaller< unsigned long, float, unsigned > marshaller( function );
    {
        marshaller.FirstParameter().BeginTick();
        marshaller.FirstParameter().SetKey( 1 );
        marshaller.FirstParameter().Apply( 42.f );
        marshaller.FirstParameter().SetKey( 3 );
        marshaller.FirstParameter().Apply( 44.f );
        marshaller.FirstParameter().EndTick();

        marshaller.SecondParameter().SetKey( 3 );
        marshaller.SecondParameter().Apply( 14u );
        marshaller.SecondParameter().BeginTick();
        marshaller.SecondParameter().SetKey( 2 );
        marshaller.SecondParameter().Apply( 13u );

        function.BeginTick_mocker.expects( once() );
        function.SetKey_mocker.expects( once() ).with( eq( 3ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 44.f ), eq( 14u ) );
        function.EndTick_mocker.expects( once() );

        marshaller.SecondParameter().EndTick();
        function.verify();
    }
}

// -----------------------------------------------------------------------------
// Name: Model_TestComposerWithKeysComposes
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Model_TestComposerWithKeysComposes )
{
    MockFunction1< unsigned long, float > function;
    Composer< unsigned long, unsigned, float > composer( function );
    {
        composer.G().BeginTick();
        composer.G().SetKey( 1 );
        composer.G().Apply( 12 );
        composer.G().SetKey( 3 );
        composer.G().Apply( 14 );
        composer.G().EndTick();

        composer.F().BeginTick();
        composer.F().SetKey( 12 );
        composer.F().Apply( 42.f );
        composer.F().SetKey( 14  );
        composer.F().Apply( 44.f );

        function.BeginTick_mocker.expects( once() );
        function.SetKey_mocker.expects( once() ).with( eq( 1ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 42.f ) );
        function.SetKey_mocker.expects( once() ).with( eq( 3ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 44.f ) );
        function.EndTick_mocker.expects( once() );

        composer.F().EndTick();
    }
}

// -----------------------------------------------------------------------------
// Name: Model_TestComposerWithConstantFIsConstant
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Model_TestComposerWithConstantFIsConstant )
{
    MockFunction1< unsigned long, float > function;
    Composer< unsigned long, unsigned, float > composer( function );
    {
        composer.G().BeginTick();
        composer.G().SetKey( 1 );
        composer.G().Apply( 12 );
        composer.G().SetKey( 3 );
        composer.G().Apply( 14 );
        composer.G().EndTick();

        composer.F().BeginTick();
        composer.F().Apply( 42.f );

        function.BeginTick_mocker.expects( once() );
        function.SetKey_mocker.expects( once() ).with( eq( 1ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 42.f ) );
        function.SetKey_mocker.expects( once() ).with( eq( 3ul ) );
        function.Apply_mocker.expects( once() ).with( eq( 42.f ) );
        function.EndTick_mocker.expects( once() );

        composer.F().EndTick();
    }
}

// -----------------------------------------------------------------------------
// Name: Model_TestComposerWithConstantGIsConstant
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Model_TestComposerWithConstantGIsConstant )
{
    MockFunction1< unsigned long, float > function;
    Composer< unsigned long, unsigned, float > composer( function );
    {
        composer.G().BeginTick();
        composer.G().Apply( 14 );
        composer.G().EndTick();

        composer.F().BeginTick();
        composer.F().SetKey( 12 );
        composer.F().Apply( 42.f );
        composer.F().SetKey( 14  );
        composer.F().Apply( 44.f );

        function.BeginTick_mocker.expects( once() );
        function.Apply_mocker.expects( once() ).with( eq( 44.f ) );
        function.EndTick_mocker.expects( once() );

        composer.F().EndTick();
    }
}