// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Data_ABC.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 16:08 $
// $Revision: 3 $
// $Workfile: ADN_Data_ABC.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Data_ABC.h"
#include "moc_ADN_Data_ABC.cpp"

#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Tools.h"


// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC constructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Data_ABC::ADN_Data_ABC()
: QObject()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC destructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Data_ABC::~ADN_Data_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Load
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Load()
{
    ADN_XmlInput_Helper input;
    
    T_StringList fileList;
    this->FilesNeeded( fileList );
    // This function is only valid is there is one file to write.
    assert( fileList.size() == 1 );

    std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + fileList.front();
    if ( !input.Open( strFile ) )
        throw ADN_OpenFile_Exception( strFile );

    try
    {
        this->ReadArchive( input );
    }
    catch( ADN_Xml_Exception& xmlException )
    {
        throw ADN_Xml_Exception( strFile, xmlException.GetContext(), xmlException.GetErrorMessage() );
    }
    catch( MT_ArchiveLogger_Exception& xmlException )
    {
        throw ADN_DataException( "", xmlException.what() );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Save
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Save()
{
    MT_XXmlOutputArchive output;

    T_StringList fileList;
    this->FilesNeeded( fileList );
    // This function is only valid is there is one file to write.
    assert( fileList.size() == 1 );

    this->WriteArchive( output );

    std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() + fileList.front();

    ADN_Tools::CreatePathToFile( strFile );
    if( ! output.WriteToFile( strFile ) )
        throw ADN_SaveFile_Exception( strFile );
}


// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::ReadArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::ReadArchive( ADN_XmlInput_Helper& /*input*/ )
{
    assert( 0 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::WriteArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::WriteArchive( MT_OutputArchive_ABC& /*output*/ )
{
    assert( 0 );
}
