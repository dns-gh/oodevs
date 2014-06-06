// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Health_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: WoundInfo::WoundInfo
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Health_Data::WoundInfo::WoundInfo()
    : lifeExpectancy_( "0s" )
    , treatTime_( "0s" )
    , restingTime_( "0s" )
    , rPercentage_( 0. )
    , goBackToWar_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WoundInfo::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::WoundInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "life-expectancy", lifeExpectancy_ )
          >> xml::attribute( "caring-time", treatTime_ )
          >> xml::attribute( "resting-time", restingTime_ )
          >> xml::attribute( "percentage", rPercentage_ )
          >> xml::optional >> xml::attribute( "back-to-war", goBackToWar_ );
}

// -----------------------------------------------------------------------------
// Name: WoundInfo::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::WoundInfo::WriteArchive( xml::xostream& output, int n ) const
{
    output << xml::start( "injury" )
             << xml::attribute( "category", ADN_Tr::ConvertFromDoctorSkills( static_cast< E_DoctorSkills >( n ) ) )
             << xml::attribute( "life-expectancy", lifeExpectancy_ )
             << xml::attribute( "caring-time", treatTime_ )
             << xml::attribute( "resting-time", restingTime_ )
             << xml::attribute( "percentage", rPercentage_ )
             << xml::attribute( "back-to-war", goBackToWar_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_Data constructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Health_Data::ADN_Health_Data()
    : ADN_Data_ABC             ( eLogistic, eHealth )
    , diagnosticTime_          ( "0s" )
    , sortingTime_             ( "0s" )
    , shockTreatTime_          ( "0s" )
    , shockRestingTime_        ( "0s" )
    , rShockPercentage_        ( 0 )
    , shockGoBackToWar_        ( true )
    , contaminationTreatTime_  ( "0s" )
    , contaminationRestingTime_( "0s" )
    , contaminatedGoBackToWar_ ( false )
    , diagnosisLifeExpectancyFactor_( 2.0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_Data destructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Health_Data::~ADN_Health_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::FilesNeeded
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szHealth_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::ReadInjury
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Health_Data::ReadInjury( xml::xistream& input )
{
    std::string category;
    input >> xml::attribute( "category", category );
    if( category == "mental" )
    {
        input >> xml::attribute( "caring-time", shockTreatTime_ )
              >> xml::attribute( "resting-time", shockRestingTime_ )
              >> xml::attribute( "percentage", rShockPercentage_ )
              >> xml::optional >> xml::attribute( "back-to-war", shockGoBackToWar_ );
    }
    else if( category == "contaminated" )
    {
        input >> xml::attribute( "caring-time", contaminationTreatTime_ )
              >> xml::attribute( "resting-time", contaminationRestingTime_ )
              >> xml::optional >> xml::attribute( "back-to-war", contaminatedGoBackToWar_ );
    }
    else
    {
        E_DoctorSkills skill = ADN_Tr::ConvertToDoctorSkills( category );
        if( skill == E_DoctorSkills( -1 ) )
            throw MASA_EXCEPTION( tools::translate( "Health_Data", "Logistic medical system - Invalid wound type '%1'" ).arg( category.c_str() ).toStdString() );
        wounds[skill].ReadArchive( input );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::ReadResourceAvailability
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Health_Data::ReadResourceAvailability( xml::xistream& input )
{
    std::unique_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
    pNew->ReadArchive( input );
    const std::string resource = input.attribute< std::string >( "resource" );
    if( resource == "relieve" )
        vChangeOverWarnings_.AddItem( pNew.release() );
    else if( resource == "collection" )
        vCollectingWarnings_.AddItem( pNew.release() );
    else if( resource == "doctor" )
        vDoctorsWarnings_.AddItem( pNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "health" )
            >> xml::start( "times" )
                >> xml::attribute( "diagnosis-time", diagnosticTime_ )
                >> xml::attribute( "sorting-time", sortingTime_ )
                >> xml::optional >> xml::attribute( "diagnosis-life-expectancy-factor", diagnosisLifeExpectancyFactor_ )
            >> xml::end
            >> xml::start( "injuries" )
                >> xml::list( "injury", *this, &ADN_Health_Data::ReadInjury )
            >> xml::end
            >> xml::start( "resource-availability-alerts" )
                >> xml::list( "resource-availability-alert", *this, &ADN_Health_Data::ReadResourceAvailability )
            >> xml::end
          >> xml::end;

    vChangeOverWarnings_.AddItem( 0 );
    vCollectingWarnings_.AddItem( 0 );
    vDoctorsWarnings_.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_Data::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Health_Data::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "health" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Medical" );
    output  << xml::start( "times" )
                << xml::attribute( "diagnosis-time", diagnosticTime_ )
                << xml::attribute( "sorting-time", sortingTime_ )
                << xml::attribute( "diagnosis-life-expectancy-factor", diagnosisLifeExpectancyFactor_ )
            << xml::end
            << xml::start( "injuries" );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        wounds[n].WriteArchive( output, n );
    output      << xml::start( "injury" )
                    << xml::attribute( "category", "mental" )
                    << xml::attribute( "caring-time", shockTreatTime_ )
                    << xml::attribute( "resting-time", shockRestingTime_ )
                    << xml::attribute( "percentage", rShockPercentage_ )
                    << xml::attribute( "back-to-war", shockGoBackToWar_ )
                << xml::end
                << xml::start( "injury" )
                    << xml::attribute( "category", "contaminated" )
                    << xml::attribute( "caring-time", contaminationTreatTime_ )
                    << xml::attribute( "resting-time", contaminationRestingTime_ )
                    << xml::attribute( "back-to-war", contaminatedGoBackToWar_ )
                << xml::end
            << xml::end
            << xml::start( "resource-availability-alerts" );
    for( auto it = vChangeOverWarnings_.begin(); it != vChangeOverWarnings_.end(); ++it )
        (*it)->WriteArchive( output, "resource", "relieve" );
    for( auto it = vCollectingWarnings_.begin(); it != vCollectingWarnings_.end(); ++it )
        (*it)->WriteArchive( output, "resource", "collection" );
    for( auto it = vDoctorsWarnings_.begin(); it != vDoctorsWarnings_.end(); ++it )
        (*it)->WriteArchive( output, "resource", "doctor" );
    output  << xml::end
          << xml::end;
}
