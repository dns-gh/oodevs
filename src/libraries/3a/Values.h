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
    @brief  Pushes the value that has been set continuously (it is changed after each Set)
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename T >
struct ContinuousValue : public Value< T >
{
    ContinuousValue()
        : value_()
        , set_( false )
    {
        // NOTHING
    }
    void Prepare()
    {
        // NOTHING
    }
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
    @brief  Always pushes the same value that has been defined in the constructor
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
    @brief  Pushes the value only once when it is set.
*/
// Created: AGE 2007-08-30
// =============================================================================
// $$$$ _RC_ JSR 2012-12-11: NOT USED
template< typename T >
struct InstantValue : public ContinuousValue< T >
{
    void Prepare()
    {
        set_ = false;
    }
};

// =============================================================================
/** @class  InstantValueIdentifier
    @brief  Always pushes a value, with default one if not set
*/
// Created: FPO 2011-06-27
// =============================================================================
// $$$$ _RC_ JSR 2012-12-11: to rename
template< typename T >
struct InstantValueIdentifier : public Value< T >
{
    explicit InstantValueIdentifier()
        : value_()
    {
        // NOTHING
    }
    void Prepare()
    {
        // NOTHING
    }
    void Set( const Type& value )
    {
        value_ = value;
    }
    void Push( ValueHandler_ABC< Type >& handler )
    {
        handler.Handle( value_ );
    }
    Type value_;
};

// =============================================================================
/** @class  PulsedValue
    @brief  Same as ContinuousValue, except that it can be reset to its initial state, i.e. not set.
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename T >
struct PulsedValue : public Value< T >
{
    PulsedValue()
        : value_()
        , flagForReset_( false )
        , set_( false )
    {
        // NOTHING
    }
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
    @brief  Sums all the values that are "set" in one tick, push this sum, then reset.
*/
// Created: FPO 2011-05-19
// =============================================================================
// $$$$ _RC_ JSR 2012-12-11: to rename
template< typename T >
struct TickValue : public Value< T >
{
    TickValue()
        : value_()
        , set_( false )
    {
        // NOTHING
    }
    void Prepare()
    {
        set_ = false;
        value_ = Type();
    }
    void Push( ValueHandler_ABC< Type >& handler )
    {
        if( set_ )
            handler.Handle( value_ );
    }
    // $$$$ _RC_ JSR 2012-12-11: to rename (Sum?)
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
