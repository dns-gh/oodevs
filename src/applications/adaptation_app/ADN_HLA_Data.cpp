// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HLA_Data.cpp $
// $Author: Ape $
// $Modtime: 23/03/05 11:45 $
// $Revision: 1 $
// $Workfile: ADN_HLA_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_HLA_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"


// -----------------------------------------------------------------------------
// Name: ADN_HLA_Data constructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_HLA_Data::ADN_HLA_Data()
: ADN_Data_ABC  ()
, bUseHLA_      ( false )
, strFederation_( "MyFederation" )
, strFederate_  ( "Scipio" )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HLA_Data destructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_HLA_Data::~ADN_HLA_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HLA_Data::FilesNeeded
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_HLA_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szHLA_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_HLA_Data::Reset
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_HLA_Data::Reset()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HLA_Data::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_HLA_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "HLA" );
    input.ReadField( "UseHLA", bUseHLA_ );
    input.ReadField( "Federation", strFederation_ );
    input.ReadField( "Federate", strFederate_ );
    input.EndSection(); //HLA
}


// -----------------------------------------------------------------------------
// Name: ADN_HLA_Data::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_HLA_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "HLA" );
    output.WriteField( "UseHLA", bUseHLA_.GetData() );
    output.WriteField( "Federation", strFederation_.GetData() );
    output.WriteField( "Federate", strFederate_.GetData() );
    output.EndSection(); //HLA
}
