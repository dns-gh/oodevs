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
void ADN_IntValidator::fixup( QString& strInput ) const
{
    bool ok = true;
    int value = strInput.toInt( &ok );
    if( !ok || value < bottom() )
        strInput.setNum( bottom() );
    else if( value > top() )
        strInput.setNum( top() );
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
    if( input.contains( ',' ) || input.contains( '.' ) )
        return Invalid;

    bool ok = true;
    int entered = input.toInt( &ok );
    if( ok && entered > top )
    {
        input = QString::number( top );
        nPos = input.length();
        return Acceptable;
    }
    else if( ok && entered < bottom )
        return Intermediate;
    else
        return QIntValidator::validate( input, nPos );
}


// -----------------------------------------------------------------------------
// ADN_DoubleValidator
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::ADN_DoubleValidator
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
ADN_DoubleValidator::ADN_DoubleValidator( QObject* parent )
    : QDoubleValidator( parent )
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
void ADN_DoubleValidator::fixup( QString& strInput ) const
{
    double b = bottom();
    double t = top();
    bool bOk = true;
    double rValue = strInput.toDouble( &bOk );
    if( ! bOk )
        return;
    if( rValue > t )
        strInput = QString::number( t );
    else if( rValue < b )
        strInput = QString::number( b );
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
    int d = decimals();
    QRegExp empty( QString::fromLatin1( " *-?[.,]? *" ) );
    if( bottom >= 0 && input.stripWhiteSpace().startsWith( QString::fromLatin1( "-" ) ) )
        return Invalid;
    if( input.stripWhiteSpace() == "." || input.stripWhiteSpace() == "," )
        return Invalid;
    if( empty.exactMatch( input ) )
        return Intermediate;
    bool ok = TRUE;
    double entered = input.toDouble( &ok );
    bool nume = input.contains( 'e', FALSE ) != 0;
    if( !ok )
    {
        // explicit exponent regexp
        QRegExp expexpexp( QString::fromLatin1( "[Ee][+-]?\\d*$" ) );
        int eeePos = expexpexp.search( input );
        if( eeePos > 0 && nume == 1 )
        {
            QString mantissa = input.left( eeePos );
            entered = mantissa.toDouble( &ok );
            if( !ok )
                return Invalid;
        }
        else if( eeePos == 0 )
            return Intermediate;
        else
            return Invalid;
    }

    int i = input.find( '.' );
    if( i < 0 )
        i = input.find( ',' );
    if( i >= 0 )
    {
        if( d == 0 )
            return Invalid;
        else if( nume == 0 )
        {
            // has decimal point (but no E), now count digits after that
            ++i;
            int j = i;
            while( input[ j ].isDigit() )
                ++j;
            if( j - i > d )
                return Invalid;
        }
    }

    if( entered > top )
    {
        input = QString::number( top );
        nPos = input.length();
        return Acceptable;
    }
    else if( entered < bottom )
        return Intermediate;
    else
        return Acceptable;
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
    : ADN_DoubleValidator( 0, 100, 2, pParent )
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
// Name: ADN_PercentageValidator::fixup
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
template< typename BaseType, typename BaseValidator >
void ADN_PercentageValidator< BaseType, BaseValidator >::fixup( QString& strInput ) const
{
    BaseType rSum = 0;
    for( CIT_ValuesVector it = linkedValues_.begin(); it != linkedValues_.end(); ++it )
        rSum += ( *it )->GetData();
    if( rSum >= top() )
    {
        strInput = QString::number( 0.f );
        return;
    }
    bool bOk = true;
    BaseType rValue = static_cast< BaseType >( strInput.toDouble( &bOk ) );
    if( ! bOk )
        return;
    if( rSum + rValue > top() )
        strInput = QString::number( top() - rSum );
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
    BaseType rSum = 0;
    for( CIT_ValuesVector it = linkedValues_.begin(); it != linkedValues_.end(); ++it )
        rSum += ( *it )->GetData();
    return InternalValidate( top() - rSum, bottom(), input, nPos );
}
