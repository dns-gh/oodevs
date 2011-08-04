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
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szCom_.GetData() );
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
void ADN_Communications_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "communications" )
            >> xml::start( "communication-breakdown" )
                >> xml::attribute( "speed-modifier", rSpeedModifier_ )
                >> xml::attribute( "reloading-time-modifier", rReloadModifier_ )
            >> xml::end
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Communications_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "communications" );
    ADN_Tools::AddSchema( output, "Communications" );
    output  << xml::start( "communication-breakdown" )
                << xml::attribute( "speed-modifier", rSpeedModifier_ )
                << xml::attribute( "reloading-time-modifier", rReloadModifier_ )
            << xml::end
          << xml::end;
}
