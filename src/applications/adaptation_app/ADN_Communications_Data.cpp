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

// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::ADN_Communications_Data
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Communications_Data::ADN_Communications_Data()
    : ADN_Data_ABC( eCommunications )
    , rSpeedModifier_( 0 )
    , rReloadModifier_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::~ADN_Communications_Data
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Communications_Data::~ADN_Communications_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Communications_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Communications_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szCom_ );
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
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Communications" );
    output  << xml::start( "communication-breakdown" )
                << xml::attribute( "speed-modifier", rSpeedModifier_ )
                << xml::attribute( "reloading-time-modifier", rReloadModifier_ )
            << xml::end
          << xml::end;
}
