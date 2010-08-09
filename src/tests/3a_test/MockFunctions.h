// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MockFunctions_h_
#define __MockFunctions_h_

#include "3a/Functions.h"

// =============================================================================
/** @class  MockFunction1
    @brief  Mock Function 1
*/
// Created: LGY 2010-08-09
// =============================================================================
#ifndef TYPE
#define TYPE Function1_ABC< K, A >
template< typename K, typename A >
MOCK_BASE_CLASS( MockFunction1, TYPE )
{
    MOCK_METHOD_EXT_TPL( BeginTick, 0, void(), BeginTick );
    MOCK_METHOD_EXT_TPL( SetKey, 1, void( const K& ), SetKey );
    MOCK_METHOD_EXT_TPL( Apply, 1, void( const A& ), Apply );
    MOCK_METHOD_EXT_TPL( EndTick, 0, void(), EndTick );
};
#endif

// =============================================================================
/** @class  MockFunction1
    @brief  MockFunction1
*/
// Created: AGE 2007-09-10
// =============================================================================
template< typename K, typename A1, typename A2 >
class MockFunction2 : public Function2_ABC< K, A1, A2 >, public mockpp::ChainableMockObject
{
public:
    //! @name Constructors/Destructor
    //@{
    MockFunction2()
        : mockpp::ChainableMockObject( "MockFunction2", 0 )
        , BeginTick_mocker( "BeginTick", this )
        , SetKey_mocker   ( "SetKey", this )
        , Apply_mocker    ( "Apply", this )
        , EndTick_mocker  ( "EndTick", this )
        {}
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        BeginTick_mocker.forward();
    }
    virtual void SetKey( const K& key )
    {
        SetKey_mocker.forward( key );
    }
    virtual void Apply( const A1& arg1, const A2& arg2 )
    {
        Apply_mocker.forward( arg1, arg2 );
    }
    virtual void EndTick()
    {
        EndTick_mocker.forward();
    }
    //@}

    //! @name Member data
    //@{
    mockpp::ChainableMockMethod< void         > BeginTick_mocker;
    mockpp::ChainableMockMethod< void, K      > SetKey_mocker;
    mockpp::ChainableMockMethod< void, A1, A2 > Apply_mocker;
    mockpp::ChainableMockMethod< void         > EndTick_mocker;
    //@}
};


#endif // __MockFunctions_h_
