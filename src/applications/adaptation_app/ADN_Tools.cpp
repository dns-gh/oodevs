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

#include <windows.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <qpopmenu.h>

#include "ENT/ENT_Tr.h"
#include "ADN_Connector_ABC.h"
#include "ADN_Xml_Exception.h"


//-----------------------------------------------------------------------------
// Name: ADN_Tools::Replace
// Created: AGN 03-07-30
//-----------------------------------------------------------------------------
std::string ADN_Tools::Replace( const std::string& str, char src, char dst )
{
    std::string szRes = str;
    int nPos = 0;
    do
    {
        int newPos = str.find_first_of( src, nPos );
        if( newPos == (int)str.npos )
            break;
        szRes[newPos] = dst;
        nPos = newPos + 1;
    } while( 1 );
    return szRes;
}


//-----------------------------------------------------------------------------
// Name: ADN_Tools::CreatePathToFile
// Created: AGN 03-07-30
//-----------------------------------------------------------------------------
void ADN_Tools::CreatePathToFile( const std::string& strFilePath )
{
    size_t nPos = 0;
    size_t nFind = 0;
    while( (nFind = strFilePath.find( '/', nPos ) ) != std::string::npos )
    {
        std::string strCurrentDir = strFilePath.substr( 0, nFind );
        _mkdir( strCurrentDir.c_str() );
        nPos = nFind + 1;
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Tools::CopyFileToFile
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
bool ADN_Tools::CopyFileToFile( const std::string& strSrc, const std::string& strDest )
{
    CreatePathToFile( strDest );
    return (bool)::CopyFile( strSrc.c_str(), strDest.c_str(), false );
}



// -----------------------------------------------------------------------------
// Name: ADN_Tools::CountElementsInList
// Created: NLD 2003-11-12
// -----------------------------------------------------------------------------
uint ADN_Tools::CountElementsInList( MT_InputArchive_ABC& input, const std::string& strListName, const std::string& strElementName )
{
    uint nNbElt = 0;
    if( ! input.BeginList( strListName ) )
        throw ADN_Xml_Exception( "", input.RetrieveLastError()->GetInfo() );

    while ( input.NextListElement() )
    {
        if( !input.Section( strElementName ) )
            throw ADN_Xml_Exception( "", input.RetrieveLastError()->GetInfo() );
        input.EndSection();
        ++nNbElt;
    }
        
    input.EndList();
    return nNbElt;
}


// -----------------------------------------------------------------------------
// Name: ADN_Tools::CaselessCompare
// Created: APE 2005-01-20
// -----------------------------------------------------------------------------
bool ADN_Tools::CaselessCompare( const std::string& str1, const std::string& str2 )
{
    std::string str1Lower = str1;
    std::transform( str1Lower.begin(), str1Lower.end(), str1Lower.begin(), std::tolower );
    std::string str2Lower = str2;
    std::transform( str2Lower.begin(), str2Lower.end(), str2Lower.begin(), std::tolower );

    return str1Lower == str2Lower;
}


// -----------------------------------------------------------------------------
// Name: ADN_Tools::Scriptify
// Created: APE 2005-02-21
// -----------------------------------------------------------------------------
std::string ADN_Tools::Scriptify( const std::string& strFieldName )
{
    std::string strResult;
    assert( strFieldName.size() >= 1 );
    strResult += std::toupper( strFieldName[0] );
    for( uint n = 1; n < strFieldName.length(); ++n )
    {
        if( strFieldName[n] == ' ' )
            continue;
        if( strFieldName[n-1] == ' ' )
            strResult += std::toupper( strFieldName[n] );
        else
            strResult += strFieldName[n];
    }
    return strResult;
}


// -----------------------------------------------------------------------------
// Name: ADN_Tools::ComputePostureScriptName
// Created: APE 2005-02-21
// -----------------------------------------------------------------------------
std::string ADN_Tools::ComputePostureScriptName( E_UnitPosture nPosture )
{
    std::string strName = ENT_Tr::ConvertFromUnitPosture( nPosture );
    assert( strName.length() > 8 );
    strName = strName.substr( 8 );

    return ADN_Tools::Scriptify( strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::CheckConnectorVector
// Created: APE 2005-03-01
// -----------------------------------------------------------------------------
void ADN_Tools::CheckConnectorVector( const T_ConnectorVector& vConnectors, uint nExpectedSize )
{
#ifndef _DEBUG
    return;
#endif

    assert( vConnectors.size() == nExpectedSize );
    for( uint n = 0; n < nExpectedSize; ++n )
    {
        assert( vConnectors[n] != 0 );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Tools::SecondToString
// Created: APE 2005-03-08
// -----------------------------------------------------------------------------
std::string ADN_Tools::SecondToString( double rSecond )
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
// Name: ADN_Tools::SortMenu
// Created: APE 2005-04-05
// -----------------------------------------------------------------------------
void ADN_Tools::SortMenu( QPopupMenu& menu )
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
