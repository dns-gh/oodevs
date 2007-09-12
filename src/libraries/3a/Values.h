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
#include "game_asn/asn.h"

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
    explicit ContinuousValue( const Type& value = Type() )
        : value_( value ) {}
    void Prepare()
    {
        // NOTHING
    };
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
/** @class  ConstantValue
    @brief  Constant value
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename T >
struct ConstantValue : public ContinuousValue< T >
{
    explicit ConstantValue( const T& value )
        : ContinuousValue( value ) {}
    void Receive( const ASN1T_MsgsSimToClient& )
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
struct InstantValue : public Value< T >
{
    InstantValue()
        : value_(), set_( false ) {}
    void Prepare()
    {
        set_ = false;
    };
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
void EvaluateValue( Value& value, const ASN1T_MsgsSimToClient& message, ValueHandler_ABC< typename Value::Type >& handler )
{
    value.Prepare();
    value.Receive( message );
    value.Push( handler );
}

#endif // __Values_h_
