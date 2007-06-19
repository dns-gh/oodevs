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
        MT_XXmlInputArchive input;
        input.Open( "./agr_tmp/" + strFile );
        input.EnableExceptions( true );
        Read( input, strModuleName + ":" );
    }
    catch( MT_Exception& e )
    {
        std::stringstream strMsg;
        strMsg << e.GetWholeMessage();

        MessageBox( 0, strMsg.str().c_str(), "ASN Class Generator - Error while parsing files", MB_ICONERROR | MB_OK );
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::Read
/** @param  input 
*/
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
void AGR_Workspace::Read( MT_XXmlInputArchive& input, const std::string& strModuleName )
{
    input.BeginList( "xsd:schema" );

    while( input.NextListElement() )
    {
        std::string strSection = input.GetCurrentElementName();

        if( strSection == "xsd:import" )
            ReadImport( input, strModuleName );
        else if( strSection == "xsd:simpleType" )
            ReadSimpleType( input, strModuleName );
        else
        {
            // Force the list pointer to advance.
            input.Section( strSection );
            input.EndSection();
        }
    }    
    input.EndList(); // xsd:schema
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadImport
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadImport( MT_XXmlInputArchive& input, const std::string& /*strModuleName*/ )
{
    input.Section( "xsd:import" );
    // read an other file
    std::string strFileName;
    input.ReadAttribute( "schemaLocation", strFileName );

    if( parsedFileSet_.find( strFileName ) == parsedFileSet_.end() )
    {
        Read( strFileName );
        parsedFileSet_.insert( strFileName );
    }

    input.EndSection(); // xsd:import
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadSimpleType
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadSimpleType( MT_XXmlInputArchive& input, const std::string& strModuleName )
{
    input.BeginList( "xsd:simpleType" );
    std::string strTypeName;
    input.ReadAttribute( "name", strTypeName );

    // POSSIBLY AN ENUM
    std::string strSubSection = input.GetCurrentElementName();
    if( strSubSection == "xsd:restriction" )
    {
        input.Section( "xsd:restriction" );
        std::string strTypeBase;
        input.ReadAttribute( "base", strTypeBase );

        // AN ENUM
        if( strTypeBase == "xsd:token" )
        {
            // Some items can be read multiple times because of file inclusions. Ensure that we read them only once.
            bool bFound = false;
            for( IT_Enumeration_Vector it = enumerationList_.begin(); it != enumerationList_.end(); ++it )
            {
                if( (*it)->GetName() == strTypeName )
                    bFound = true;
            }

            if( ! bFound )
            {
                // we define an enumeration. Just mark it as enum
                AGR_Enumeration* pEnum = new AGR_Enumeration( strTypeName );
                pEnum->Read( input );
                enumerationList_.push_back( pEnum );
            }
        }

        input.EndSection(); // "xsd:restriction"
    }

    input.EndList(); // "xsd:simpleType"
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
