//*****************************************************************************
//
// $Created: JDY 03-06-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Tools.cpp $
// $Author: Nld $
// $Modtime: 21/07/05 11:41 $
// $Revision: 11 $
// $Workfile: ADN_Tools.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Tools.h"
#include "ADN_Types.h"
#include "ADN_Tr.h"

#include "clients_kernel/Tools.h"
#include "ENT/ENT_Tr.h"
#include "tools/VersionHelper.h"

#include <xeumeuleu/xml.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>

namespace ADN_Tools
{

// -----------------------------------------------------------------------------
// Name: Scriptify
// Created: APE 2005-02-21
// -----------------------------------------------------------------------------
std::string Scriptify( const std::string& strFieldName )
{
    std::string strResult;
    assert( strFieldName.size() >= 1 );
    strResult += static_cast< char >( std::toupper( strFieldName[0] ) );
    for( uint n = 1; n < strFieldName.length(); ++n )
    {
        if( strFieldName[n] == ' ' )
            continue;
        if( strFieldName[n-1] == ' ' )
            strResult += static_cast< char>( std::toupper( strFieldName[n] ) );
        else
            strResult += strFieldName[n];
    }
    return strResult;
}

// -----------------------------------------------------------------------------
// Name: ComputePostureScriptName
// Created: APE 2005-02-21
// -----------------------------------------------------------------------------
std::string ComputePostureScriptName( E_UnitPosture nPosture )
{
    std::string strName = ENT_Tr::ConvertFromUnitPosture( nPosture );
    assert( strName.length() > 8 );
    strName = strName.substr( 8 );

    return Scriptify( strName );
}

// -----------------------------------------------------------------------------
// Name: CheckConnectorVector
// Created: APE 2005-03-01
// -----------------------------------------------------------------------------
void CheckConnectorVector( const T_ConnectorVector& vConnectors, uint nExpectedSize )
{
#ifdef _DEBUG
    assert( vConnectors.size() == nExpectedSize );
    for( uint n = 0; n < nExpectedSize; ++n )
        assert( vConnectors[n] != 0 );
#else
    ( void )vConnectors;
    ( void )nExpectedSize;
#endif
    return;
}

// -----------------------------------------------------------------------------
// Name: SecondToString
// Created: APE 2005-03-08
// -----------------------------------------------------------------------------
std::string SecondToString( double rSecond )
{
    std::stringstream strText;
    strText << rSecond << "s";
    return strText.str();
}

// -----------------------------------------------------------------------------
// Name: Local defs for the function below
// Created: APE 2005-04-05
// -----------------------------------------------------------------------------
typedef std::pair< QString, int >   T_MenuItem;
typedef std::vector< T_MenuItem >   T_MenuItemVector;
typedef T_MenuItemVector::iterator IT_MenuItemVector;

struct ItemSort
{
    bool operator()( T_MenuItem& lhs, T_MenuItem& rhs )
    {
        return lhs.first.lower() < rhs.first.lower();
    }
};

// -----------------------------------------------------------------------------
// Name: SortMenu
// Created: APE 2005-04-05
// -----------------------------------------------------------------------------
void SortMenu( Q3PopupMenu& menu )
{
    T_MenuItemVector vItems;
    while( menu.count() > 0 )
    {
        T_MenuItem item;
        item.first = menu.text( menu.idAt( 0 ) );
        item.second = menu.idAt( 0 );
        menu.removeItemAt( 0 );
        vItems.push_back( item );
    }

    std::sort( vItems.begin(), vItems.end(), ItemSort() );

    for( IT_MenuItemVector it = vItems.begin(); it != vItems.end(); ++it )
        menu.insertItem( (*it).first, (*it).second );
}

// -----------------------------------------------------------------------------

E_EntityType ConvertMissionToEntityType( E_MissionType type )
{
    switch( type )
    {
    case eMissionType_Pawn:
        return eEntityType_Pawn;
    case eMissionType_Automat:
        return eEntityType_Automat;
    case eMissionType_Population:
        return eEntityType_Population;
    default:
        throw MASA_EXCEPTION( "Invalid conversion from E_EntityType to E_MissionType" );
    }
}

E_MissionType ConvertEntityTypeToMissionType( E_EntityType type )
{
    switch( type )
    {
    case eEntityType_Pawn:
        return eMissionType_Pawn;
    case eEntityType_Automat:
        return eMissionType_Automat;
    case eEntityType_Population:
        return eMissionType_Population;
    default:
        throw MASA_EXCEPTION( "Invalid conversion from E_EntityType to E_MissionType" );
    }
}

namespace
{
    int ComputeValue( const QStringList& buffer )
    {
        if( qApp->isLeftToRight() )
        {
            const QStringList seconds = buffer.at( 2 ).split( QLocale().decimalPoint() );
            return buffer.at( 0 ).toInt() * 360000 + buffer.at( 1 ).toInt() * 6000 + seconds.at( 0 ).toInt() * 100 + seconds.at( 1 ).toInt();
        }
        const QStringList seconds = buffer.at( 0 ).split( QLocale().decimalPoint() );
        return buffer.at( 2 ).toInt() * 360000 + buffer.at( 1 ).toInt() * 6000 + seconds.at( 0 ).toInt() * 100 + seconds.at( 1 ).toInt();
    }
    std::string Fill( int number )
    {
        return ( number  < 10 ? "0" : "" ) + boost::lexical_cast< std::string >( number );
    }
    const QString xmlFormat = "hh:mm";
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::GetLocalFormatWithoutSeconds
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
QString ADN_Tools::GetLocalFormatWithoutSeconds()
{
    return QLocale().timeFormat().remove( QRegExp( "\\:ss|\\:s" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::ConvertLocalTimeToXmlTime
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
QString ADN_Tools::ConvertLocalTimeToXmlTime( const QString& localTime )
{
    return QTime::fromString( localTime, GetLocalFormatWithoutSeconds() ).toString( xmlFormat );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::ConvertXmlTimeToLocalTime
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
QString ADN_Tools::ConvertXmlTimeToLocalTime( const QString& xmlTime )
{
    return QTime::fromString( xmlTime, xmlFormat ).toString( GetLocalFormatWithoutSeconds() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::BuildLongString
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
QString BuildLongString( const std::string& hours, const std::string& minutes,
    const std::string& seconds, const std::string& microseconds,
    const std::string& decimalPoint )
{
    return qApp->isLeftToRight()
        ? ( hours + ":" + minutes + ":" + seconds + decimalPoint + microseconds ).c_str()
        : ( seconds + decimalPoint + microseconds + ":" + minutes + ":" + hours ).c_str();
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::ConvertDelayToLongString
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
QString ADN_Tools::ConvertDelayToLongString( const QString& delay )
{
    return ConvertCentisecondsToLongString( ConvertDelayToCentiseconds( delay ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::ConvertLongStringToDelay
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
QString ADN_Tools::ConvertLongStringToDelay( const QString& text )
{
    return ConvertCentisecondsToDelay( ConvertLongStringToCentiseconds( text ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::ConvertDelayToCentiseconds
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
int ADN_Tools::ConvertDelayToCentiseconds( const QString& delay )
{
    if( delay.isEmpty() )
        throw MASA_EXCEPTION( "Invalid use of ConvertDelayToCentiseconds, empty string." );
    const QString strUnit( delay.right( 1 ) );
    const QString strValue( delay.left( delay.size() - 1 ) );
    const int modifier = strUnit == "s" ? 100
                       : strUnit == "m" ? 6000
                       : strUnit == "h" ? 360000
                       :                  0;
    return boost::numeric_cast< int >( boost::lexical_cast< double >( strValue.toStdString() ) * modifier );
}

bool ADN_Tools::IsNullDelay( const ADN_Type_Time& delay )
{
    const std::string s = delay.GetData();
    if( s.empty() )
        return true;
    return ADN_Tools::ConvertDelayToCentiseconds( QString::fromStdString( s ) ) == 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::ConvertCentisecondsToDelay
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
QString ADN_Tools::ConvertCentisecondsToDelay( int value )
{
    const int seconds = value / 100;
    const int centiSeconds = value % 100;
    return ( boost::lexical_cast< std::string >( seconds ) + "." // $$$$ _RC_ SLI 2013-09-18: we always want to save decimal time with US format '.'
           + boost::lexical_cast< std::string >( centiSeconds ) + "s" ).c_str();
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::ConvertCentisecondsToLongString
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
QString ADN_Tools::ConvertCentisecondsToLongString( int value )
{
    const int minutes = ( value / 6000 ) % 60;
    const int hours = value / 360000;
    const int seconds = value / 100 - hours * 3600 - minutes * 60;
    const int centisec = value % 100;
    return BuildLongString( boost::lexical_cast< std::string >( hours ), Fill( minutes ),
                                  Fill( seconds ), Fill( centisec ), QString( QLocale().decimalPoint() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::ConvertLongStringToCentiseconds
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
int ADN_Tools::ConvertLongStringToCentiseconds( const QString& text )
{
    QStringList buffer = text.split( ":" );
    if( buffer.size() < 3 )
        throw MASA_EXCEPTION( tools::translate( "ADN_Tools", "Invalid time duration '%1'" ).arg( text ).toStdString() );
    return ComputeValue( buffer );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::SetAutoClear
// Created: ABR 2013-10-04
// -----------------------------------------------------------------------------
void ADN_Tools::SetAutoClear( T_ConnectorVector& v, bool b )
{
    for( auto itConnector = v.begin(); itConnector != v.end(); ++itConnector )
        if( *itConnector != 0 )
            ( *itConnector )->SetAutoClear( b );
}

std::string ADN_Tools::MakePluralFromEntityType( E_EntityType type )
{
    std::string result = ADN_Tr::ConvertFromEntityType( type, ENT_Tr::eToSim );
    if( type != eEntityType_Population )
        result += 's';
    return result;
}

} //! namespace ADN_Tools
