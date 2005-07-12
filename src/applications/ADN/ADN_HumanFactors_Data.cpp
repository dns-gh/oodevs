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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HumanFactors_Data.cpp $
// $Author: Ape $
// $Modtime: 21/03/05 17:22 $
// $Revision: 2 $
// $Workfile: ADN_HumanFactors_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_HumanFactors_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_Tools.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"


// -----------------------------------------------------------------------------
// Name: ModifiersInfo::ModifiersInfo
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_HumanFactors_Data::ModifiersInfo::ModifiersInfo( const std::string& strName )
: strName_ ( strName )
{
}


// -----------------------------------------------------------------------------
// Name: ModifiersInfo::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::ModifiersInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( strName_ );
    input.ReadField( "CoefModificationVitesseMax", rSpeedModifier_ );
    input.ReadField( "CoefModificationPH", rPHModifier_ );
    input.ReadField( "CoefModificationDistanceCapteurs", rSensorsModifier_ );
    input.ReadField( "CoefModificationTempsRechargement", rReloadModifier_ );
    input.ReadField( "CoefModificationTempsMiseEnPosture", rStanceModifier_ );
    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: ModifiersInfo::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::ModifiersInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( strName_ );
    output.WriteField( "CoefModificationVitesseMax", rSpeedModifier_.GetData() );
    output.WriteField( "CoefModificationPH", rPHModifier_.GetData() );
    output.WriteField( "CoefModificationDistanceCapteurs", rSensorsModifier_.GetData() );
    output.WriteField( "CoefModificationTempsRechargement", rReloadModifier_.GetData() );
    output.WriteField( "CoefModificationTempsMiseEnPosture", rStanceModifier_.GetData() );
    output.EndSection();
}


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_HumanFactors_Data::ADN_HumanFactors_Data()
: ADN_Data_ABC()
, veteranModifiers_     ( "Veteran" )
, xpModifiers_          ( "Experimente" )
, newbieModifiers_      ( "Conscrit" )
, normalModifiers_      ( "Normal" )
, tiredModifiers_       ( "Fatigue" )
, exhaustedModifiers_   ( "Epuise" )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_HumanFactors_Data::~ADN_HumanFactors_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szHumanFactors_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data::Reset
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::Reset()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "FacteursHumains" );
    
    input.Section( "Experience" );
    veteranModifiers_.ReadArchive( input );
    xpModifiers_.ReadArchive( input );
    newbieModifiers_.ReadArchive( input );
    input.EndSection(); // Experience
    
    input.Section( "Fatigue" );
    normalModifiers_.ReadArchive( input );
    tiredModifiers_.ReadArchive( input );
    exhaustedModifiers_.ReadArchive( input );
    input.EndSection(); // Fatigue
    
    input.EndSection(); // FacteursHumains
}


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "FacteursHumains" );

    output.Section( "Experience" );
    veteranModifiers_.WriteArchive( output );
    xpModifiers_.WriteArchive( output );
    newbieModifiers_.WriteArchive( output );
    output.EndSection(); // Experience

    output.Section( "Fatigue" );
    normalModifiers_.WriteArchive( output );
    tiredModifiers_.WriteArchive( output );
    exhaustedModifiers_.WriteArchive( output );
    output.EndSection(); // Fatigue

    output.EndSection(); // FacteursHumains
}
