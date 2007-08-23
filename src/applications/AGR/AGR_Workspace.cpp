// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_Workspace.h"
#include "AGR_Enumeration.h"

#include <windows.h>
#include <assert.h>

// -----------------------------------------------------------------------------
// Name: AGR_Workspace constructor
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
AGR_Workspace::AGR_Workspace()
{
    parsedFileSet_.insert( std::string( "osxsdlib.xsd" ) );
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace destructor
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
AGR_Workspace::~AGR_Workspace()
{
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::Read
/** @param  strFile
*/
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
void AGR_Workspace::Read( const std::string& strFile )
{
    std::string strModuleName;
    MT_ExtractBaseName( strFile, strModuleName );
    try
    {
        xml::xifstream xis( "./agr_tmp/" + strFile );
        Read( xis );
    }
    catch( std::exception& e )
    {
        MessageBox( 0, e.what(), "ASN Class Generator - Error while parsing files", MB_ICONERROR | MB_OK );
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::Read
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
void AGR_Workspace::Read( xml::xistream& xis )
{
    xis >> xml::start( "xsd:schema" )
            >> xml::list( *this, &AGR_Workspace::ReadElement )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadElement
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadElement( const std::string& type, xml::xistream& xis )
{
    if( type == "xsd:import" )
        ReadImport( xis );
    else if( type == "xsd:simpleType" )
        ReadSimpleType( xis );
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadImport
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadImport( xml::xistream& xis )
{
    const std::string strFileName = xml::attribute< std::string >( xis, "schemaLocation" );
    if( parsedFileSet_.find( strFileName ) == parsedFileSet_.end() )
    {
        Read( strFileName );
        parsedFileSet_.insert( strFileName );
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadSimpleTypeElement
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadSimpleTypeElement( const std::string& type, xml::xistream& xis, const std::string& strTypeName )
{
    if( type == "xsd:restriction" )
    {
        if( xml::attribute( xis, "base", std::string() ) == "xsd:token" )
        {
            bool bFound = false;
            for( IT_Enumeration_Vector it = enumerationList_.begin(); it != enumerationList_.end(); ++it )
                if( (*it)->GetName() == strTypeName )
                    bFound = true;

            if( ! bFound )
            {
                AGR_Enumeration* pEnum = new AGR_Enumeration( strTypeName );
                pEnum->Read( xis );
                enumerationList_.push_back( pEnum );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadSimpleType
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadSimpleType( xml::xistream& xis )
{
    const std::string strTypeName = xml::attribute< std::string >( xis, "name" );
    xis >> xml::list( *this, &AGR_Workspace::ReadSimpleTypeElement, strTypeName );
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::Enums
// Created: APE 2005-02-08
// -----------------------------------------------------------------------------
const T_Enumeration_Vector& AGR_Workspace::Enums() const
{
    return enumerationList_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReplaceInString
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
void AGR_Workspace::ReplaceInString( std::string& strToProcess, const std::string& strToSearch, const std::string& strReplace ) const
{
    const std::string::size_type nSize = strToSearch.size();
    std::string::size_type nPos = 0;

    std::string::size_type newPos = strToProcess.find( strToSearch, nPos );
    while( newPos != std::string::npos )
    {
        strToProcess.replace( newPos, nSize, strReplace );
        nPos = newPos + nSize;
        newPos = strToProcess.find( strToSearch, nPos );
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadStringFile
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadStringFile( const std::string& strFileName, std::string& strResult ) const
{
    std::ifstream ifs( strFileName.c_str() );
    if( !ifs )
        throw std::exception( std::string( "Error opening " + strFileName + " for reading." ).c_str() );
    strResult = std::string( std::istreambuf_iterator<char>( ifs ), std::istreambuf_iterator<char>() );
}

#include <direct.h>

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::WriteStringInFile
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
void AGR_Workspace::WriteStringInFile( const std::string& strToWrite, const std::string& strFileName ) const
{
    std::ofstream ofs( strFileName.c_str() );
    if( ! ofs.is_open() )
        throw std::exception( std::string( "Error opening " + strFileName + " for writing." ).c_str() );
    ofs << strToWrite;
}
