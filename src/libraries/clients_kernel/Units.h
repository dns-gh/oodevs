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
#include <QtGui/qapplication.h>
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
    explicit Unit( const QString& symbol, const QString& dummy, double minValue, double maxValue, unsigned short decimal );
    ~Unit() {}
    const QString& AsString() const
    {
        if( qApp && translated_.isEmpty() )
            translated_ = qApp->translate( "Units", symbol_.toUtf8().constData() );
        return translated_;
    }
    const double GetMinValue() const { return minValue_; }
    const double GetMaxValue() const { return maxValue_; }
    const unsigned short GetDecimal() const { return decimal_; }

    bool operator==( const Unit& other ) const { return symbol_ == other.symbol_; }
private:
    QString         symbol_;
    mutable QString translated_;
    double          minValue_;
    double          maxValue_;
    unsigned short  decimal_;
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
    static const Unit percentageGTzero;
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
