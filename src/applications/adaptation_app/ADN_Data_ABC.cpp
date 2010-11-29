// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Data_ABC.h"
#include "moc_ADN_Data_ABC.cpp"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Tools.h"
#include <tools/XmlCrc32Signature.h>

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC constructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Data_ABC::ADN_Data_ABC()
    : QObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC destructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Data_ABC::~ADN_Data_ABC()
{
    // NOTHING
}

namespace
{
    class SchemaReader
    {
    public:
    void ReadSchema( const std::string&, xml::xistream& xis )
    {
        schema_ = xis.attribute< std::string >( "xsi:noNamespaceSchemaLocation", "" );
    }
    const std::string& GetSchema()
    {
        return schema_;
    }
    private:
        std::string schema_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Load
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Load( std::string& invalidSignedFiles )
{
    T_StringList fileList;
    FilesNeeded( fileList );
    if( ! fileList.empty() )
    {
        const std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + fileList.front();
        tools::EXmlCrc32SignatureError error = tools::CheckXmlCrc32Signature( strFile );
        if( error == tools::eXmlCrc32SignatureError_Invalid || error == tools::eXmlCrc32SignatureError_NotSigned )
            invalidSignedFiles.append( "\n" + fileList.front() );
        xml::xifstream xis( strFile );
        SchemaReader reader;
        xis >> xml::list( reader, &SchemaReader::ReadSchema );
        const std::string& schema = reader.GetSchema();
        if( schema.empty() )
        {
            xml::xifstream input( strFile );
            ReadArchive( input );
        }
        else
        {
            xml::xifstream input( strFile, xml::external_grammar( std::string( "resources/" ) + schema ) );
            ReadArchive( input );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Save
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Save()
{
    T_StringList fileList;
    FilesNeeded( fileList );
    if( ! fileList.empty() )
    {
        std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() + fileList.front();
        ADN_Tools::CreatePathToFile( strFile );
        {
            xml::xofstream output( strFile );
            WriteArchive( output );
        }
        tools::WriteXmlCrc32Signature( strFile );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::ReadArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::ReadArchive( xml::xistream& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::WriteArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::WriteArchive( xml::xostream& )
{
    // NOTHING
}
