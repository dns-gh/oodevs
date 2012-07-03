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

// -----------------------------------------------------------------------------
// Name: Function_TestMarshallingWithNoKeyJustForwards
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Function_TestMarshallingWithNoKeyJustForwards )
{
    MockFunction2< unsigned long, float, unsigned int > function;
    KeyMarshaller< unsigned long, float, unsigned int > marshaller( function );
    marshaller.FirstParameter().BeginTick();
    marshaller.FirstParameter().Apply( 42.f );
    marshaller.FirstParameter().EndTick();
    marshaller.SecondParameter().BeginTick();
    marshaller.SecondParameter().Apply( 12u );
    MOCK_EXPECT( function.BeginTick ).once();
    MOCK_EXPECT( function.Apply ).once().with( 42.f, 12u );
    MOCK_EXPECT( function.EndTick ).once();
    marshaller.SecondParameter().EndTick();
}

// -----------------------------------------------------------------------------
// Name: Function_TestMarshallingWithOneKeyForwardsKey
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Function_TestMarshallingWithOneKeyForwardsKey )
{
    MockFunction2< unsigned long, float, unsigned int > function;
    KeyMarshaller< unsigned long, float, unsigned int > marshaller( function );
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
        MOCK_EXPECT( function.BeginTick ).once();
        MOCK_EXPECT( function.SetKey ).once().with( 1ul );
        MOCK_EXPECT( function.Apply ).once().with( 42.f, 12u );
        MOCK_EXPECT( function.SetKey ).once().with( 2ul );
        MOCK_EXPECT( function.Apply ).once().with( 43.f, 12u );
        MOCK_EXPECT( function.SetKey ).once().with( 3ul );
        MOCK_EXPECT( function.Apply ).once().with( 44.f, 12u );
        MOCK_EXPECT( function.EndTick ).once();
        marshaller.SecondParameter().EndTick();
        mock::verify( function );
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
        MOCK_EXPECT( function.BeginTick ).once();
        MOCK_EXPECT( function.SetKey ).once().with( 1ul );
        MOCK_EXPECT( function.Apply ).once().with( 42.f, 12u );
        MOCK_EXPECT( function.SetKey ).once().with( 2ul );
        MOCK_EXPECT( function.Apply ).once().with( 42.f, 13u );
        MOCK_EXPECT( function.SetKey ).once().with( 3ul );
        MOCK_EXPECT( function.Apply ).once().with( 42.f, 14u );
        MOCK_EXPECT( function.EndTick ).once();
        marshaller.SecondParameter().EndTick();
        mock::verify( function );
    }
}

// -----------------------------------------------------------------------------
// Name: Function_TestMarshallingWithTwoKeysSortsKeysBack
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Function_TestMarshallingWithTwoKeysSortsKeysBack )
{
    MockFunction2< unsigned long, float, unsigned int > function;
    KeyMarshaller< unsigned long, float, unsigned int > marshaller( function );
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
        MOCK_EXPECT( function.BeginTick ).once();
        MOCK_EXPECT( function.SetKey ).once().with( 1ul );
        MOCK_EXPECT( function.Apply ).once().with( 42.f, 12u );
        MOCK_EXPECT( function.SetKey ).once().with( 2ul );
        MOCK_EXPECT( function.Apply ).once().with( 43.f, 13u );
        MOCK_EXPECT( function.SetKey ).once().with( 3ul );
        MOCK_EXPECT( function.Apply ).once().with( 44.f, 14u );
        MOCK_EXPECT( function.EndTick ).once();
        marshaller.SecondParameter().EndTick();
        mock::verify( function );
    }
}

// -----------------------------------------------------------------------------
// Name: Function_TestMarshallingWithTwoKeysOnlyForwardsWhenBothKeysArePresent
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Function_TestMarshallingWithTwoKeysOnlyForwardsWhenBothKeysArePresent )
{
    MockFunction2< unsigned long, float, unsigned int > function;
    KeyMarshaller< unsigned long, float, unsigned int > marshaller( function );
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
        MOCK_EXPECT( function.BeginTick ).once();
        MOCK_EXPECT( function.SetKey ).once().with( 3ul );
        MOCK_EXPECT( function.Apply ).once().with( 44.f, 14u );
        MOCK_EXPECT( function.EndTick ).once();
        marshaller.SecondParameter().EndTick();
        mock::verify( function );
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
        MOCK_EXPECT( function.BeginTick ).once();
        MOCK_EXPECT( function.SetKey ).once().with( 1ul );
        MOCK_EXPECT( function.Apply ).once().with( 42.f );
        MOCK_EXPECT( function.SetKey ).once().with( 3ul );
        MOCK_EXPECT( function.Apply ).once().with( 44.f );
        MOCK_EXPECT( function.EndTick ).once();
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
    Composer< unsigned long, unsigned int, float > composer( function );
    {
        composer.G().BeginTick();
        composer.G().SetKey( 1 );
        composer.G().Apply( 12 );
        composer.G().SetKey( 3 );
        composer.G().Apply( 14 );
        composer.G().EndTick();
        composer.F().BeginTick();
        composer.F().Apply( 42.f );
        MOCK_EXPECT( function.BeginTick ).once();
        MOCK_EXPECT( function.SetKey ).once().with( 1ul );
        MOCK_EXPECT( function.Apply ).once().with( 42.f );
        MOCK_EXPECT( function.SetKey ).once().with( 3ul );
        MOCK_EXPECT( function.Apply ).once().with( 42.f );
        MOCK_EXPECT( function.EndTick ).once();
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
    Composer< unsigned long, unsigned int, float > composer( function );
    {
        composer.G().BeginTick();
        composer.G().Apply( 14 );
        composer.G().EndTick();
        composer.F().BeginTick();
        composer.F().SetKey( 12 );
        composer.F().Apply( 42.f );
        composer.F().SetKey( 14  );
        composer.F().Apply( 44.f );
        MOCK_EXPECT( function.BeginTick ).once();
        MOCK_EXPECT( function.Apply ).once().with( 44.f );
        MOCK_EXPECT( function.EndTick ).once();
        composer.F().EndTick();
    }
}