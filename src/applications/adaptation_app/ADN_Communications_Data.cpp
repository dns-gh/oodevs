// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-14 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Communications_Data.cpp $
// $Author: Ape $
// $Modtime: 21/03/05 17:22 $
// $Revision: 2 $
// $Workfile: ADN_Communications_Data.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Communications_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_Tools.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"



// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::ADN_Communications_Data
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Communications_Data::ADN_Communications_Data()
: ADN_Data_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::~ADN_Communications_Data
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Communications_Data::~ADN_Communications_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Communications_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szCom_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::Reset
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Communications_Data::Reset()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Communications_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Communications" );
    input.Section( "ImpactCoupureCommunications" );
    input.ReadField( "CoefModificationVitesse", rSpeedModifier_ );
    input.ReadField( "CoefModificationTempsRechargement", rReloadModifier_ );
    input.EndSection(); // Communications
    input.EndSection(); // CoefModificationTempsRechargement
}


// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Communications_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Communications" );
    output.Section( "ImpactCoupureCommunications" );
    output.WriteField( "CoefModificationVitesse", rSpeedModifier_.GetData() );
    output.WriteField( "CoefModificationTempsRechargement", rReloadModifier_.GetData() );
    output.EndSection(); // Communications
    output.EndSection(); // CoefModificationTempsRechargement
}
