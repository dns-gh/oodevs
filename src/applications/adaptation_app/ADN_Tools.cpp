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
#include "ENT/ENT_Tr.h"
#include "tools/VersionHelper.h"
#include <windows.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>

namespace ADN_Tools
{

// -----------------------------------------------------------------------------
// Name: CaselessCompare
// Created: APE 2005-01-20
// -----------------------------------------------------------------------------
bool CaselessCompare( const std::string& str1, const std::string& str2 )
{
    return boost::iequals( str1, str2 );
}

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
// Name: AddSchema
// Created: SBO 2009-08-25
// -----------------------------------------------------------------------------
void AddSchema( xml::xostream& xos, const std::string& name )
{
    xos << xml::prefix( "http://www.w3.org/2001/XMLSchema-instance", "xsi" )
            << xml::attribute( "model-version", tools::AppModelVersion() )
            << xml::ns( "http://www.w3.org/2001/XMLSchema-instance" )
            << xml::attribute( "noNamespaceSchemaLocation", "schemas/" + std::string( tools::AppMajorVersion() ) + "/physical/" + name + ".xsd" );
}

// -----------------------------------------------------------------------------
// Name: AddVersion
// Created: RPD 2010-09-22
// -----------------------------------------------------------------------------
void AddVersion( xml::xostream& xos, const std::string& version )
{
    xos << xml::attribute( "model-version", version );
}

}
