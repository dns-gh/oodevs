// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "propagation_module_test_pch.h"

BOOST_FIXTURE_TEST_CASE( simple_flood_propagation, sword::propagation::ModuleFixture )
{
    //  ***
    // *****
    // **c**
    // *****
    //  ***
    const std::size_t identifier = 42u;
    ExpectEvent( "flood event", sword::test::MakeModel( "identifier", identifier )
                                                      ( "deep-areas", sword::test::MakeModel()
                                                        [ sword::test::MakeModel()
                                                            [ sword::test::MakeModel( "x", -150 )( "y", -150 ) ]
                                                            [ sword::test::MakeModel( "x", -150 )( "y", -50 ) ]
                                                            [ sword::test::MakeModel( "x", -150 )( "y", 50 ) ]
                                                            [ sword::test::MakeModel( "x", -150 )( "y", 150 ) ]
                                                            [ sword::test::MakeModel( "x", 150 )( "y", 150 ) ]
                                                            [ sword::test::MakeModel( "x", 150 )( "y", 50 ) ]
                                                            [ sword::test::MakeModel( "x", 150 )( "y", -50 ) ]
                                                            [ sword::test::MakeModel( "x", 150 )( "y", -150 ) ]
                                                        ] )
                                                      ( "low-areas", mock::any ) );
    // flood elevation
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 0.f, 0.f ).returns( 0 );

    // step
    for( int i = -2; i <= 2; ++i )
        for( int j = -2; j <= 2; ++j )
            if( std::abs( i ) != 2 || std::abs( j ) != 2 )
                MOCK_EXPECT( GetModifiedAltitude ).once().with( i * 100.f, j * 100.f ).returns( 0 );

    PostCommand( "flood command", core::MakeModel( "center", core::MakeModel( "x", 0.f )
                                                                              ( "y", 0.f ) )
                                                   ( "depth", 50 )
                                                   ( "radius", 200 )
                                                   ( "identifier", identifier ) );
    ExecuteCommands();
}

BOOST_FIXTURE_TEST_CASE( flood_propagation, sword::propagation::ModuleFixture )
{
    // -***-
    // *****
    // 00c**
    // 00***
    // -***-
    const std::size_t identifier = 42u;
    ExpectEvent( "flood event", sword::test::MakeModel( "identifier", identifier )
                                                      ( "deep-areas", sword::test::MakeModel()
                                                        [ sword::test::MakeModel()
                                                            [ sword::test::MakeModel( "x", -50 )( "y", -150 ) ]
                                                            [ sword::test::MakeModel( "x", -50 )( "y", -50 ) ]
                                                            [ sword::test::MakeModel( "x", -50 )( "y", 50 ) ]
                                                            [ sword::test::MakeModel( "x", -150 )( "y", 50 ) ]
                                                            [ sword::test::MakeModel( "x", -150 )( "y", 150 ) ]
                                                            [ sword::test::MakeModel( "x", 150 )( "y", 150 ) ]
                                                            [ sword::test::MakeModel( "x", 150 )( "y", 50 ) ]
                                                            [ sword::test::MakeModel( "x", 150 )( "y", -50 ) ]
                                                            [ sword::test::MakeModel( "x", 150 )( "y", -150 ) ]
                                                        ] )
                                                      ( "low-areas", mock::any ) );
    // flood elevation
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 0.f, 0.f ).returns( 0 );

    // elevation
    MOCK_EXPECT( GetModifiedAltitude ).once().with( -100.f, -100.f ).returns( 100 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( -100.f, 0.f ).returns( 100 );
    // step
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 0.f, 0.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 100.f, 0.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 0.f, -100.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 0.f, 100.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 200.f, 0.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 100.f, -100.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 100.f, 100.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 0.f, -200.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( -100.f, 100.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 0, 200.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 200, -100.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 100, -200.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 200, 100.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( 100, 200.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( -200, 100.f ).returns( 0 );
    MOCK_EXPECT( GetModifiedAltitude ).once().with( -100, 200.f ).returns( 0 );

    PostCommand( "flood command", core::MakeModel( "center", core::MakeModel( "x", 0.f )
                                                                              ( "y", 0.f ) )
                                                   ( "depth", 50 )
                                                   ( "radius", 200 )
                                                   ( "identifier", identifier ) );
    ExecuteCommands();
}