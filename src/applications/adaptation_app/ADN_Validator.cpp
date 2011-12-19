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
// Name: ADN_PercentageValidator::ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
ADN_PercentageValidator::ADN_PercentageValidator( QObject* pParent, const char* szName )
    : QDoubleValidator( 0, 100, 2, pParent, szName )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::~ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
ADN_PercentageValidator::~ADN_PercentageValidator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::AddLinkedValue
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
void ADN_PercentageValidator::AddLinkedValue( ADN_Type_Double& value )
{
    linkedValues_.push_back( &value );
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::ClearLinkedValues
// Created: MMC 2011-03-29
// -----------------------------------------------------------------------------
void ADN_PercentageValidator::ClearLinkedValues()
{
    linkedValues_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::fixup
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
void ADN_PercentageValidator::fixup( QString& strInput ) const
{
    double rSum = 0;
     for( CIT_ValuesVector it = linkedValues_.begin(); it != linkedValues_.end(); ++it )
        rSum += ( *it )->GetData();
    if( rSum >= 100.0 )
    {
        strInput = QString::number( 0.f );
        return;
    }
    bool bOk = true;
    double rValue = strInput.toDouble( &bOk );
    if( ! bOk )
        return;
    if( rSum + rValue > 100.f )
        strInput = QString::number( 100.f - rSum );
}

//-----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::validate
/** Copied from Qt.
*/
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
QValidator::State ADN_PercentageValidator::validate( QString& input, int& nPos ) const
{
    double rSum = 0;
    for( CIT_ValuesVector it = linkedValues_.begin(); it != linkedValues_.end(); ++it )
        rSum += ( *it )->GetData();

    double b = bottom();
    double t = 100.f - rSum;
    int d = decimals();
    QRegExp empty( QString::fromLatin1( " *-?\\.? *" ) );
    if( b >= 0 && input.stripWhiteSpace().startsWith( QString::fromLatin1( "-" ) ) )
        return Invalid;
    if( input.stripWhiteSpace() == "." )
        return Invalid;
    if( empty.exactMatch( input ) )
        return Intermediate;
    bool ok = TRUE;
    double entered = input.toDouble( &ok );
    bool nume = input.contains( 'e', FALSE );
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

    if( entered > t )
    {
        input = QString::number( t );
        nPos = input.length();
        return Acceptable;
    }
    else if( entered < b )
        return Intermediate;
    else
        return Acceptable;
}

// -----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::ADN_DoubleValidator
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
ADN_DoubleValidator::ADN_DoubleValidator( QObject* parent, const char* name /*= 0*/ )
    : QDoubleValidator( parent, name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_DoubleValidator::ADN_DoubleValidator
// Created: ABR 2011-03-17
// -----------------------------------------------------------------------------
ADN_DoubleValidator::ADN_DoubleValidator( double bottom, double top, int decimals, QObject * parent, const char *name /*= 0*/ )
    : QDoubleValidator( bottom, top, decimals, parent, name )
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
    double b = bottom();
    double t = top();
    int d = decimals();
    QRegExp empty( QString::fromLatin1( " *-?\\.? *" ) );
    if( b >= 0 && input.stripWhiteSpace().startsWith( QString::fromLatin1( "-" ) ) )
        return Invalid;
    if( input.stripWhiteSpace() == "." )
        return Invalid;
    if( empty.exactMatch( input ) )
        return Intermediate;
    bool ok = TRUE;
    double entered = input.toDouble( &ok );
    bool nume = input.contains( 'e', FALSE );
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

    if( entered > t )
    {
        input = QString::number( t );
        nPos = input.length();
        return Acceptable;
    }
    else if( entered < b )
        return Intermediate;
    else
        return Acceptable;
}

ADN_IntValidator::ADN_IntValidator( QObject* parent ) : QIntValidator( parent )
{}

ADN_IntValidator::ADN_IntValidator( int bottom, int top, QObject * parent ) : QIntValidator( bottom, top, parent )
{}
  
ADN_IntValidator::~ADN_IntValidator()
{}

QValidator::State ADN_IntValidator::validate( QString& input, int& nPos ) const
{
    if( input.contains( ',' ) )
        return Invalid;
    return QIntValidator::validate( input, nPos );
}

void ADN_IntValidator::fixup( QString& strInput ) const
{
    bool ok = true;
    int value = strInput.toInt( &ok );
    if( !ok || value < bottom() )
        strInput.setNum( bottom() );
    else if( value > top() )
        strInput.setNum( top() );
}
