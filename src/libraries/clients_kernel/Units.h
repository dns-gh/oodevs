// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Units_h_
#define __Units_h_

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace kernel
{
// =============================================================================
/** @class  Unit
    @brief  Unit
*/
// Created: AGE 2006-02-21
// =============================================================================
class Unit
{
public:
    explicit Unit( const QString& symbol );
    ~Unit() {}
    const QString& AsString() const { return symbol_; }
private:
    QString symbol_;
};

// =============================================================================
/** @class  Units
    @brief  Units
*/
// Created: AGE 2006-02-21
// =============================================================================
class Units
{
public:
    static const Unit meters;
    static const Unit degrees;
    static const Unit percentage;
    static const Unit metersPerSecond;
    static const Unit vehiclesPerHour;
    static const Unit tons;
    static const Unit kilometersPerHour;
    static const Unit hours;
    static const Unit minesPerMeter;
};

template< typename T >
class UnitedValue
{
public:
    UnitedValue( const T& value, const Unit& unit )
        : value_( value )
        , unit_( unit )
    {};
    virtual ~UnitedValue() {}
public:
    T value_;
    Unit unit_;
};

template< typename T >
UnitedValue< T > operator*( const T& value, const Unit& unit )
{
    return UnitedValue< T >( value, unit );
};

}

#endif // __Units_h_
