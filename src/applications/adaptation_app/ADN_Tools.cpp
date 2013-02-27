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
#include "tools/Version.h"
#include <windows.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

namespace ADN_Tools
{

//-----------------------------------------------------------------------------
// Name: CreatePathToFile
// Created: AGN 03-07-30
//-----------------------------------------------------------------------------
void CreatePathToFile( const std::string& strFilePath )
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
// Name: CopyFileToFile
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
bool CopyFileToFile( const std::string& strSrc, const std::string& strDest )
{
    CreatePathToFile( strDest );
    return ::CopyFile( strSrc.c_str(), strDest.c_str(), false ) != 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::CopyDirToDir
// Created: NPT 2013-02-26
// -----------------------------------------------------------------------------
void CopyDirToDir( const bfs::path& pathFrom, const bfs::path& pathTo, bool recursive, bool forceCopy )
{
    if( !bfs::exists( pathTo ) )
        bfs::create_directories( pathTo );
    bfs::directory_iterator end;
    for( bfs::directory_iterator it( pathFrom ); it != end; ++it )
    {
        if( recursive && bfs::is_directory( *it ) )
        {
            bfs::path dest( pathTo / it->path().filename() );
            CopyDirToDir( *it, dest, recursive, forceCopy );
        }
        else
        {
            if( forceCopy || !bfs::exists( pathTo / it->path().filename() ) )
                bfs::copy_file( *it, pathTo / it->path().filename(), bfs::copy_option::overwrite_if_exists );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Tools::CleanDirContent
// Created: NPT 2013-02-26
// -----------------------------------------------------------------------------
void CleanDirectoryContent( const boost::filesystem::path& dir, bool recursive )
{
    if( !bfs::exists( dir ) )
        return;
    bfs::directory_iterator end;
    for( bfs::directory_iterator it( dir ); it != end; ++it )
    {
        if( recursive && bfs::is_directory( *it ) )
        {
             CleanDirectoryContent( *it, recursive );
             bfs::remove( *it );
        }
        else
        {
            if( bfs::exists( it->path().filename() ) )
                bfs::remove( *it );
        }
    }
}

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
