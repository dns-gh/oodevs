// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Funeral_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Enums.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Funeral_Data::ADN_Funeral_Data()
: ADN_Data_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Funeral_Data::~ADN_Funeral_Data()
{
    funeralPackagingResources_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szFuneral_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::Reset
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::Reset()
{
    funeralPackagingResources_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::ReadFuneralPackagingResource
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::ReadFuneralPackagingResource( xml::xistream& input )
{
    std::auto_ptr< ADN_FuneralPackagingResource > pNew( new ADN_FuneralPackagingResource() );
    pNew->ReadArchive( input );
    funeralPackagingResources_.AddItem( pNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "funeral" )
            >> xml::start( "fake-transporter" )
                >> xml::attribute( "speed", fakeTransporterSpeed_ )
            >> xml::end()
            >> xml::start( "packagings" )
                >> xml::list( "packaging", *this, &ADN_Funeral_Data::ReadFuneralPackagingResource )
            >> xml::end
          >> xml::end;
    funeralPackagingResources_.AddItem( 0 );
}   

// -----------------------------------------------------------------------------
// Name: ADN_Funeral_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Funeral_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "funeral" );
    ADN_Tools::AddSchema( output, "Funeral" );
    output  << xml::start( "fake-transporter" )
                << xml::attribute( "speed", fakeTransporterSpeed_ )
            << xml::end()
            << xml::start( "packagings" );
    for( IT_FuneralPackagingResource_Vector it = funeralPackagingResources_.begin(); it != funeralPackagingResources_.end(); ++it )
        (*it)->WriteArchive( output );
    output  << xml::end
        << xml::end;
}
