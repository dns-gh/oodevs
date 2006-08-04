// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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
    input.ReadAttribute( "esperanceVie", lifeExpectancy_ );
    input.ReadAttribute( "tempsSoin", treatTime_ );
    input.ReadAttribute( "tempsRepos", restingTime_ );
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
    output.WriteAttribute( "esperanceVie", lifeExpectancy_.GetData() ) ;
    output.WriteAttribute( "tempsSoin", treatTime_.GetData() ) ;
    output.WriteAttribute( "tempsRepos", restingTime_.GetData() ) ;
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
    input.ReadField( "TempsDiagnostique", diagnosticTime_ );
    input.ReadField( "TempsTri", sortingTime_ );

    input.Section( "CategoriesBlesse" );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        wounds[n].ReadArchive( input );
    input.EndSection(); // CategoriesBlesse

    input.Section( "ReacMental" );
    input.ReadAttribute( "tempsSoin", shockTreatTime_ );
    input.ReadAttribute( "tempsRepos", shockRestingTime_ );
    input.ReadAttribute( "pourcentage", rShockPercentage_ );
    input.EndSection(); // ReacMental 

    input.Section( "Contamines" );
    input.ReadAttribute( "tempsSoin", contaminationTreatTime_ );
    input.ReadAttribute( "tempsRepos", contaminationRestingTime_ );
    input.EndSection(); // Contamines 

    input.Section( "AlertesDisponibiliteMoyens" );
        input.BeginList( "AlertesDisponibiliteMoyensReleve" );
        while( input.NextListElement() )
        {
            std::auto_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
            pNew->ReadArchive( input, "AlerteDisponibiliteMoyensReleve" );
            vChangeOverWarnings_.AddItem( pNew.release() );
        }
        vChangeOverWarnings_.AddItem( 0 );
        input.EndList();

        input.BeginList( "AlertesDisponibiliteMoyensRamassage" );
        while( input.NextListElement() )
        {
            std::auto_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
            pNew->ReadArchive( input, "AlerteDisponibiliteMoyensRamassage" );
            vCollectingWarnings_.AddItem( pNew.release() );
        }
        vCollectingWarnings_.AddItem( 0 );
        input.EndList();

        input.BeginList( "AlertesDisponibiliteMedecins" );
        while( input.NextListElement() )
        {
            std::auto_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
            pNew->ReadArchive( input, "AlerteDisponibiliteMedecins" );
            vDoctorsWarnings_.AddItem( pNew.release() );
        }
        vDoctorsWarnings_.AddItem( 0 );
        input.EndList();

    input.EndSection();

    input.EndSection(); // Sante
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Sante" );
    output.WriteField( "TempsDiagnostique", diagnosticTime_.GetData() );
    output.WriteField( "TempsTri", sortingTime_.GetData() );

    output.Section( "CategoriesBlesse" );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        wounds[n].WriteArchive( output );
    output.EndSection(); // CategoriesBlesse

    output.Section( "ReacMental" );
    output.WriteAttribute( "tempsSoin", shockTreatTime_.GetData() );
    output.WriteAttribute( "tempsRepos", shockRestingTime_.GetData() );
    output.WriteAttribute( "pourcentage", rShockPercentage_.GetData() );
    output.EndSection(); // ReacMental 

    output.Section( "Contamines" );
    output.WriteAttribute( "tempsSoin", contaminationTreatTime_.GetData() );
    output.WriteAttribute( "tempsRepos", contaminationRestingTime_.GetData() );
    output.EndSection(); // Contamines 

    output.Section( "AlertesDisponibiliteMoyens" );
        output.BeginList( "AlertesDisponibiliteMoyensReleve", vChangeOverWarnings_.size() );
        for( IT_AvailabilityWarning_Vector it = vChangeOverWarnings_.begin(); it != vChangeOverWarnings_.end(); ++it )
            (*it)->WriteArchive( output, "AlerteDisponibiliteMoyensReleve" );
        output.EndList();

        output.BeginList( "AlertesDisponibiliteMoyensRamassage", vCollectingWarnings_.size() );
        for( IT_AvailabilityWarning_Vector it = vCollectingWarnings_.begin(); it != vCollectingWarnings_.end(); ++it )
            (*it)->WriteArchive( output, "AlerteDisponibiliteMoyensRamassage" );
        output.EndList();

        output.BeginList( "AlertesDisponibiliteMedecins", vDoctorsWarnings_.size() );
        for( IT_AvailabilityWarning_Vector it = vDoctorsWarnings_.begin(); it != vDoctorsWarnings_.end(); ++it )
            (*it)->WriteArchive( output, "AlerteDisponibiliteMedecins" );
        output.EndList();
    output.EndSection();

    output.EndSection(); // Sante
}
