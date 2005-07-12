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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Health_Data.cpp $
// $Author: Ape $
// $Modtime: 22/03/05 15:05 $
// $Revision: 1 $
// $Workfile: ADN_Health_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Health_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"


// -----------------------------------------------------------------------------
// Name: WoundInfo::WoundInfo
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Health_Data::WoundInfo::WoundInfo()
{
}


// -----------------------------------------------------------------------------
// Name: WoundInfo::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::WoundInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( ADN_Tr::ConvertFromDoctorSkills( nType_) );
    input.ReadTimeAttribute( "esperanceVie", rLifeExpectancy_ );
    input.ReadTimeAttribute( "tempsSoin", rTreatTime_ );
    input.ReadAttribute( "pourcentage", rPercentage_ );
    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: WoundInfo::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::WoundInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( ADN_Tr::ConvertFromDoctorSkills( nType_) );
    output.WriteAttribute( "esperanceVie", ADN_Tools::SecondToString( rLifeExpectancy_.GetData() ) );
    output.WriteAttribute( "tempsSoin", ADN_Tools::SecondToString( rTreatTime_.GetData() ) ) ;
    output.WriteAttribute( "pourcentage", rPercentage_.GetData() );
    output.EndSection();
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_Data constructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Health_Data::ADN_Health_Data()
: ADN_Data_ABC()
{
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        wounds[n].nType_ = (E_DoctorSkills)n;
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_Data destructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Health_Data::~ADN_Health_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::FilesNeeded
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szHealth_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::Reset
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::Reset()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Sante" );
    input.ReadTimeField( "TempsDiagnostique", rDiagnosticTime_ );
    input.ReadTimeField( "TempsTri", rSortingTime_ );

    input.Section( "CategoriesBlesse" );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        wounds[n].ReadArchive( input );
    input.EndSection(); // CategoriesBlesse

    input.Section( "ReacMental" );
    input.ReadTimeAttribute( "tempsSoin", rShockTreatTime_ );
    input.ReadAttribute( "pourcentage", rShockPercentage_ );
    input.EndSection(); // ReacMental 

    input.Section( "Contamines" );
    input.ReadTimeAttribute( "tempsSoin", rContaminationTreatTime_ );
    input.EndSection(); // Contamines 

    input.EndSection(); // Sante
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Sante" );
    output.WriteField( "TempsDiagnostique", ADN_Tools::SecondToString( rDiagnosticTime_.GetData() ) );
    output.WriteField( "TempsTri", ADN_Tools::SecondToString( rSortingTime_.GetData() ) );

    output.Section( "CategoriesBlesse" );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        wounds[n].WriteArchive( output );
    output.EndSection(); // CategoriesBlesse

    output.Section( "ReacMental" );
    output.WriteAttribute( "tempsSoin", ADN_Tools::SecondToString( rShockTreatTime_.GetData() ) );
    output.WriteAttribute( "pourcentage", rShockPercentage_.GetData() );
    output.EndSection(); // ReacMental 

    output.Section( "Contamines" );
    output.WriteAttribute( "tempsSoin", ADN_Tools::SecondToString( rContaminationTreatTime_.GetData() ) );
    output.EndSection(); // Contamines 

    output.EndSection(); // Sante
}



