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
#include "ADN_GuiTools.h"
#include "tools/Loader_ABC.h"
#include <tools/XmlCrc32Signature.h>
#include <boost/bind.hpp>

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

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Load
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Load( const tools::Loader_ABC& fileLoader )
{
    T_StringList fileList;
    FilesNeeded( fileList );
    if( !fileList.empty() )
    {
        const std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + fileList.front();
        fileLoader.LoadFile( strFile, boost::bind( &ADN_Data_ABC::ReadArchive, this, _1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Initialize()
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::IsValidDatabase
// Created: PHC 2011-01-19
// -----------------------------------------------------------------------------
bool ADN_Data_ABC::IsValidDatabase()
{
    return true;
}
