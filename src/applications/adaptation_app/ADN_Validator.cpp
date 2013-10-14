// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Validator.h"

// -----------------------------------------------------------------------------
// ADN_IntValidator
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_IntValidator constructor
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
ADN_IntValidator::ADN_IntValidator( QObject* parent )
    : QIntValidator( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_IntValidator constructor
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
ADN_IntValidator::ADN_IntValidator( int bottom, int top, QObject * parent )
    : QIntValidator( bottom, top, parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_IntValidator destructor
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
ADN_IntValidator::~ADN_IntValidator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_IntValidator::fixup
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
void ADN_IntValidator::fixup( QString& input ) const
{
    QLocale locale;
    input = input.remove( locale.groupSeparator() );
    bool ok = true;
    int value = input.toInt( &ok );
    if( !ok || value < bottom() )
        value = bottom();
    else if( value > top() )
        value = top();
    input = locale.toString( value );
}

// -----------------------------------------------------------------------------
// Name: ADN_IntValidator::validate
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
QValidator::State ADN_IntValidator::validate( QString& input, int& nPos ) const
{
    return InternalValidate( top(), bottom(), input, nPos );
}

// -----------------------------------------------------------------------------
// Name: ADN_IntValidator::InternalValidate
// Created: ABR 2012-01-25
// -----------------------------------------------------------------------------
QValidator::State ADN_IntValidator::InternalValidate( int top, int bottom, QString& input, int& nPos ) const
{
    QLocale locale;
    int sizeBefore = input.size();
    input = input.remove( locale.groupSeparator() );
    QValidator::State result;

    if( bottom >= 0 && input.stripWhiteSpace().startsWith( '-' ) )
        result = Invalid;
    else if( input.contains( ',' ) || input.contains( '.' ) )
        result = Invalid;
    else
    {
        bool ok = true;
        long long entered = input.toLongLong( &ok );
        if( ok && entered > top )
        {
            input = QString::number( top );
            result = Acceptable;
        }
        else if( ok && entered < bottom )
            result = Intermediate;
        else
            result = QIntValidator::validate( input, nPos );
    }

    if( result == Acceptable )
    {
        int value = input.toInt();
        input = locale.toString( value );
        int sizeAfter = input.size();
        nPos += ( sizeAfter - sizeBefore );
        nPos = ( nPos > input.length() ) ? input.length() : ( nPos < 0 ) ? 0 : nPos;
    }
    return result;
}

// -----------------------------------------------------------------------------
// ADN_DoubleValidator
// -----------------------------------------------------------------------------

namespace
{
    QString& removeTrailingZero( QString& input, QChar decimalPoint )
    {
        int i = input.find( decimalPoint );
        if( i >= 0 )
        {
            int j = input.size() - 1;
            while( j > i && input[ j ] == '0' ){ --j; }
            input.truncate( ( j == i ) ? j : j + 1 );
        }
        return input;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::ADN_DoubleValidator
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
ADN_DoubleValidator::ADN_DoubleValidator( QObject* parent )
    : QDoubleValidator( 0, std::numeric_limits< double >::max(), 2, parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::ADN_DoubleValidator
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
ADN_DoubleValidator::ADN_DoubleValidator( double bottom, double top, int decimals, QObject * parent )
    : QDoubleValidator( bottom, top, decimals, parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::~ADN_DoubleValidator
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
ADN_DoubleValidator::~ADN_DoubleValidator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::fixup
// Created: JDY 03-09-04
// -----------------------------------------------------------------------------
void ADN_DoubleValidator::fixup( QString& input ) const
{
    QLocale locale;
    input = input.remove( locale.groupSeparator() );
    input = input.replace( ',', locale.decimalPoint() );
    input = input.replace( '.', locale.decimalPoint() );
    bool hasDecimal = input.find( locale.decimalPoint() ) != -1 && decimals() != 0;
    bool bOk = true;
    double value = input.toDouble( &bOk );
    if( ! bOk )
        return;
    if( value > top() )
        value = top();
    else if( value < bottom() )
        value = bottom();
    input = locale.toString( value, 'f', decimals() );
    if( !hasDecimal )
        input = removeTrailingZero( input, locale.decimalPoint() );
}

//-----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::validate
/** Copied from Qt.
*/
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
QValidator::State ADN_DoubleValidator::validate( QString& input, int& nPos ) const
{
    return InternalValidate( top(), bottom(), input, nPos );
}

// -----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::InternalValidate
// Created: ABR 2012-01-25
// -----------------------------------------------------------------------------
QValidator::State ADN_DoubleValidator::InternalValidate( double top, double bottom, QString& input, int& nPos ) const
{
    QLocale locale;
    int sizeBefore = input.size();
    input = input.remove( locale.groupSeparator() );
    input = input.replace( ',', locale.decimalPoint() );
    input = input.replace( '.', locale.decimalPoint() );
    QValidator::State result = Acceptable;
    int d = decimals();
    int decimalPointIndex = input.find( locale.decimalPoint() );
    int trailingChars = ( decimalPointIndex == -1 ) ? d + 1 : d;

    QRegExp empty( QString( "\\s*-?\\" ) + locale.decimalPoint() + "?\\s*" );
    if( bottom >= 0 && input.stripWhiteSpace().startsWith( '-' ) )
        result = Invalid;
    else if( input.stripWhiteSpace() == locale.decimalPoint() )
        result = Invalid;
    else if( empty.exactMatch( input ) )
        result = Intermediate;
    else
    {
        bool ok = false;
        double entered = locale.toDouble( input, &ok );
        if( !ok )
            result = Invalid;
        else if( decimalPointIndex >= 0 )
        {
            if( d == 0 )
                result = Invalid;
            else    // has decimal point, now count digits after that
            {
                int j = decimalPointIndex + 1;
                while( input[ j ].isDigit() )
                {
                    ++j;
                    --trailingChars;
                }
                if( j - decimalPointIndex - 1 > d )
                    result = Invalid;
            }
        }
        if( result != Invalid )
        {
            if( entered > top )
            {
                input = QString::number( top, 'f', d );
                result = Acceptable;
            }
            else
                result = ( entered < bottom ) ? Intermediate : Acceptable;
        }
    }

    if( result == Acceptable )
    {
        double value = input.toDouble();
        input = locale.toString( value, 'f', d );
        if( value == top )
            input = removeTrailingZero( input, locale.decimalPoint() );
        else
            input.truncate( input.size() - trailingChars );
        int sizeAfter = input.size();
        nPos += ( sizeAfter - sizeBefore );
        nPos = ( nPos > input.length() ) ? input.length() : ( nPos < 0 ) ? 0 : nPos;
    }
    return result;
}

// -----------------------------------------------------------------------------
// ADN_PercentageValidator
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
template<>
ADN_PercentageValidator< int, ADN_IntValidator >::ADN_PercentageValidator( QObject* pParent )
    : ADN_IntValidator( 0, 100, pParent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
template<>
ADN_PercentageValidator< double, ADN_DoubleValidator >::ADN_PercentageValidator( QObject* pParent )
    : ADN_DoubleValidator( 0, 100, 10, pParent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::~ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
template< typename BaseType, typename BaseValidator >
ADN_PercentageValidator< BaseType, BaseValidator >::~ADN_PercentageValidator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Validator::ComputeLinkedSum
// Created: ABR 2012-04-11
// -----------------------------------------------------------------------------
template< typename BaseType, typename BaseValidator >
BaseType ADN_PercentageValidator< BaseType, BaseValidator >::ComputeLinkedSum() const
{
    BaseType rSum = 0;
    for( auto it = linkedValues_.begin(); it != linkedValues_.end(); ++it )
        rSum += ( *it )->GetData();
    return rSum;
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
template<>
void ADN_PercentageValidator< int, ADN_IntValidator >::fixup( QString& input ) const
{
    QLocale locale;
    input = input.remove( locale.groupSeparator() );

    int value = 0;
    int rSum = ComputeLinkedSum();
    if( rSum >= top() )
        value = 0;
    else
    {
        bool bOk = true;
        value = input.toInt( &bOk );
        if( !bOk )
            return;
        if( rSum + value > top() )
            value = top() - rSum;
    }

    input = locale.toString( value );
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
template<>
void ADN_PercentageValidator< double, ADN_DoubleValidator >::fixup( QString& input ) const
{
    QLocale locale;
    input = input.remove( locale.groupSeparator() );
    input = input.replace( ',', locale.decimalPoint() );
    input = input.replace( '.', locale.decimalPoint() );
    bool hasDecimal = input.find( locale.decimalPoint() ) != -1 && decimals() != 0;

    double value = 0;
    double rSum = ComputeLinkedSum();
    if( rSum >= top() )
        value = 0;
    else
    {
        bool bOk = true;
        value = input.toDouble( &bOk );
        if( !bOk )
            return;
        if( rSum + value > top() )
            value = top() - rSum;
    }

    input = locale.toString( value, 'f', decimals() );
    if( !hasDecimal )
        input = removeTrailingZero( input, locale.decimalPoint() );
}

//-----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::validate
/** Copied from Qt.
*/
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
template< typename BaseType, typename BaseValidator >
QValidator::State ADN_PercentageValidator< BaseType, BaseValidator >::validate( QString& input, int& nPos ) const
{
    return InternalValidate( top() - ComputeLinkedSum(), bottom(), input, nPos );
}
