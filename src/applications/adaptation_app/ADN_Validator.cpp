//*****************************************************************************
//
// $Created: JDY 03-09-04 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Validator.cpp $
// $Author: Ape $
// $Modtime: 19/04/05 14:39 $
// $Revision: 6 $
// $Workfile: ADN_Validator.cpp $
//
//*****************************************************************************
#include "ADN_pch.h"
#include "ADN_Validator.h"

#include "ADN_EditLine.h"


//-----------------------------------------------------------------------------
// Name: ADN_IntValidator constructor
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
ADN_IntValidator::ADN_IntValidator( QObject* pParent, const char* szName )
: QIntValidator( pParent, szName )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_IntValidator constructor
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
ADN_IntValidator::ADN_IntValidator( int nBottom, int nTop, QObject* pParent, const char* szName )
: QIntValidator( nBottom, nTop, pParent, szName )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_IntValidator destructor
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
ADN_IntValidator::~ADN_IntValidator()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_IntValidator::validate
/** Copied from Qt.
*/
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
QValidator::State ADN_IntValidator::validate( QString& input, int& nPos ) const
{
    int b = bottom();
    int t = top();

    QString stripped = input.stripWhiteSpace();
    if( stripped.isEmpty() || (b < 0 && stripped == "-") )
	    return Intermediate;
    bool ok;
    long entered = input.toLong( &ok );
    if ( !ok || (entered < 0 && b >= 0) || entered > t)
    {
        this->fixup( input );
        nPos = input.length();
        return Acceptable;
    }
    else if ( entered < b )
	    return Intermediate;
    else
	    return Acceptable;
}


// -----------------------------------------------------------------------------
// Name: ADN_IntValidator::fixup
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
void ADN_IntValidator::fixup( QString& strInput ) const
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
// Name: ADN_DoubleValidator constructor
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
ADN_DoubleValidator::ADN_DoubleValidator( QObject* pParent, const char* szName )
: QDoubleValidator( pParent, szName )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_DoubleValidator constructor
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
ADN_DoubleValidator::ADN_DoubleValidator( double rBottom, double rTop, int nDecimals, QObject* pParent, const char* szName )
: QDoubleValidator( rBottom, rTop, nDecimals, pParent, szName )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_DoubleValidator destructor
// Created: JDY 03-09-04
//-----------------------------------------------------------------------------
ADN_DoubleValidator::~ADN_DoubleValidator()
{
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

    QRegExp empty( QString::fromLatin1(" *-?\\.? *") );
    if ( b >= 0 &&
	 input.stripWhiteSpace().startsWith(QString::fromLatin1("-")) )
	    return Invalid;
    if ( empty.exactMatch(input) )
    	return Intermediate;
    bool ok = TRUE;
    double entered = input.toDouble( &ok );
    int nume = input.contains( 'e', FALSE );
    if ( !ok ) 
    {
	    // explicit exponent regexp
	    QRegExp expexpexp( QString::fromLatin1("[Ee][+-]?\\d*$") );
	    int eeePos = expexpexp.search( input );
	    if ( eeePos > 0 && nume == 1 ) 
        {
	        QString mantissa = input.left( eeePos );
	        entered = mantissa.toDouble( &ok );
	        if ( !ok )
		    return Invalid;
	    } 
        else if ( eeePos == 0 ) 
            return Intermediate;
	    else 
            return Invalid;
    }

    int i = input.find( '.' );
    if ( i >= 0 )
        if ( d==0 )
            return Invalid;
        else if ( nume == 0 ) 
        {
	        // has decimal point (but no E), now count digits after that
	        i++;
	        int j = i;
	        while( input[j].isDigit() )
	            j++;
	        if ( j - i > d )
	            return Invalid;
        }

    if ( entered > t )
    {
        this->fixup( input );
        nPos = input.length();
        return Acceptable;
    }
    else if( entered < b )
        return Intermediate;
    else
	    return Acceptable;
}


// -----------------------------------------------------------------------------
// Name: ADN_Validator::fixup
// Created: APE 2005-04-13
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





// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
ADN_PercentageValidator::ADN_PercentageValidator( QObject* pParent, const char* szName )
: ADN_DoubleValidator( pParent, szName )
, doubleValidator_   ( *new ADN_DoubleValidator( 0, 100, 2, this ) )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::~ADN_PercentageValidator
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
ADN_PercentageValidator::~ADN_PercentageValidator()
{
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
// Name: ADN_PercentageValidator::validate
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
QValidator::State ADN_PercentageValidator::validate( QString& strInput, int& nPos ) const
{
    doubleValidator_.validate( strInput, nPos );
    this->fixup( strInput );
    if( doubleValidator_.validate( strInput, nPos ) == QValidator::Invalid )
        return QValidator::Invalid;

    nPos = strInput.length();
    return QValidator::Acceptable;
}


// -----------------------------------------------------------------------------
// Name: ADN_PercentageValidator::fixup
// Created: APE 2005-04-15
// -----------------------------------------------------------------------------
void ADN_PercentageValidator::fixup( QString& strInput ) const
{
    double rSum = 0;
    for( CIT_ValuesVector it = linkedValues_.begin(); it != linkedValues_.end(); ++it )
    {
        rSum += (*it)->GetData();
    }

    if( rSum >= 100.0 )
    {
        strInput = QString::number( 0.0 );
        return;
    }

    bool bOk = true;
    double rValue = strInput.toDouble( &bOk );
    if( ! bOk )
        return;

    if( rSum + rValue > 100.0 )
        strInput = QString::number( 100.0 - rSum );
}
