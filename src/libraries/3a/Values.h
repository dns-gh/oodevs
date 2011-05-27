// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Values_h_
#define __Values_h_

#include "ValueHandler_ABC.h"

// =============================================================================
/** @class  Value
    @brief  Value base
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename T >
struct Value
{
    typedef T Type;
};

// =============================================================================
/** @class  ContinuousValue
    @brief  Continuous value
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename T >
struct ContinuousValue : public Value< T >
{
    explicit ContinuousValue()
        : value_(), set_( false ) {}
    void Prepare()
    {
        // NOTHING
    };
    void Set( const Type& value )
    {
        set_ = true;
        value_ = value;
    }
    void Push( ValueHandler_ABC< Type >& handler )
    {
        if( set_ )
            handler.Handle( value_ );
    }
    Type value_;
    bool set_;
};

// =============================================================================
/** @class  ConstantValue
    @brief  Constant value
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename T >
struct ConstantValue : public ContinuousValue< T >
{
    explicit ConstantValue( const T& value )
    {
        Set( value );
    }
    void Receive( const sword::SimToClient& )
    {
        // NOTHING
    }
};

// =============================================================================
/** @class  InstantValue
    @brief  Instant value
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename T >
struct InstantValue : public ContinuousValue< T >
{
    void Prepare()
    {
        set_ = false;
    };
};

// =============================================================================
/** @class  PulsedValue
    @brief  "pulsed" value
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename T >
struct PulsedValue : public Value< T >
{
    PulsedValue()
        : value_(), flagForReset_( false ), set_( false ) {}
    void Prepare()
    {
        if( flagForReset_ )
        {
            set_ = false;
            flagForReset_ = false;
        }
    }
    void Push( ValueHandler_ABC< Type >& handler )
    {
        if( set_ )
            handler.Handle( value_ );
    }
    void Set( const Type& value )
    {
        value_ = value;
        set_ = true;
    }
    void Reset()
    {
        flagForReset_ = true;
    }
    Type value_;
    bool flagForReset_;
    bool set_;
};

// =============================================================================
/** @class  TickValue
@brief  "tick" value
*/
// Created: FPO 2011-05-19
// =============================================================================
template< typename T >
struct TickValue : public Value< T >
{
    TickValue()
        : value_(), set_( false ) {}
    void Prepare()
    {
        set_ = false;
        value_ = 0;
    }
    void Push( ValueHandler_ABC< Type >& handler )
    {
        if( set_ )
            handler.Handle( value_ );
    }
    void Set( const Type& value )
    {
        value_ += value;
        set_ = true;
    }
    Type value_;
    bool set_;
};

// =============================================================================
/** @function  EvaluateValue
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename Value >
inline
void EvaluateValue( Value& value, const sword::SimToClient& message, ValueHandler_ABC< typename Value::Type >& handler )
{
    value.Prepare();
    value.Receive( message );
    value.Push( handler );
}

#endif // __Values_h_
