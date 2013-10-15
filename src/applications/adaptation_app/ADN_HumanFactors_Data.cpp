// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_HumanFactors_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"

// =============================================================================
// ADN_HumanFactors_Data
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ModifiersInfo::ModifiersInfo
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_HumanFactors_Data::ModifiersInfo::ModifiersInfo( const std::string& strName )
    : strName_( strName )
    , rSpeedModifier_( 0.001 )
    , rPHModifier_( 0.001 )
    , rSensorsModifier_( 0.001 )
    , rReloadModifier_( 0.001 )
    , rStanceModifier_( 0.001 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModifiersInfo::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::ModifiersInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "max-speed", rSpeedModifier_ )
          >> xml::attribute( "ph", rPHModifier_ )
          >> xml::attribute( "sensor-distance", rSensorsModifier_ )
          >> xml::attribute( "loading-time", rReloadModifier_ )
          >> xml::attribute( "posture-setup-time", rStanceModifier_ );
}

// -----------------------------------------------------------------------------
// Name: ModifiersInfo::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::ModifiersInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "modifier" )
            << xml::attribute( "state", strName_ )
            << xml::attribute( "max-speed", rSpeedModifier_ )
            << xml::attribute( "ph", rPHModifier_ )
            << xml::attribute( "sensor-distance", rSensorsModifier_ )
            << xml::attribute( "loading-time", rReloadModifier_ )
            << xml::attribute( "posture-setup-time", rStanceModifier_ )
           << xml::end;
}

// =============================================================================
// ADN_HumanFactors_Data
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data::ThresholdsInfo
// Created: ABR 2011-12-08
// -----------------------------------------------------------------------------
ADN_HumanFactors_Data::ThresholdsInfo::ThresholdsInfo()
    : firstThreshold_( 1 )
    , secondThreshold_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ThresholdsInfo::ReadArchive
// Created: ABR 2011-12-08
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::ThresholdsInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "first-threshold", firstThreshold_ )
          >> xml::attribute( "second-threshold", secondThreshold_ );
}

// -----------------------------------------------------------------------------
// Name: ThresholdsInfo::WriteArchive
// Created: ABR 2011-12-08
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::ThresholdsInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::attribute( "first-threshold", firstThreshold_ )
           << xml::attribute( "second-threshold", secondThreshold_ );
}

// =============================================================================
// ADN_HumanFactors_Data
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_HumanFactors_Data::ADN_HumanFactors_Data()
    : ADN_Data_ABC( eHumanFactors )
    , veteranModifiers_      ( "Veteran" )
    , xpModifiers_           ( "Experimente" )
    , newbieModifiers_       ( "Conscrit" )
    , normalModifiers_       ( "Normal" )
    , tirednessModifiers_    ( "Fatigue" )
    , exhaustedModifiers_    ( "Epuise" )
    , calmModifiers_         ( "Calm" )
    , worriedModifiers_      ( "Worried" )
    , stressedModifiers_     ( "Stressed" )
    , tirednessThresholds_   ()
    , tirednessMoving_       ( 0 )
    , tirednessWorking_      ( 0 )
    , tirednessEngineStopped_( 0 )
    , tirednessEngineRunning_( 0 )
    , stressThresholds_      ()
    , stressIncPerShot_      ( 0 )
    , stressDecPerHour_      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_HumanFactors_Data::~ADN_HumanFactors_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szHumanFactors_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "humans-factors" )
            >> xml::start( "experience-factor" )
                >> xml::list( "modifier", *this, &ADN_HumanFactors_Data::ReadModifier )
            >> xml::end
            >> xml::start( "tiredness-factor" )
                >> xml::list( "modifier", *this, &ADN_HumanFactors_Data::ReadModifier )
            >> xml::end
            >> xml::start( "stress-factor" )
                >> xml::list( "modifier", *this, &ADN_HumanFactors_Data::ReadModifier )
            >> xml::end
            >> xml::start( "automatic-evolution" )
                >> xml::start( "tiredness-evolution" )
                    >> xml::attribute( "moving", tirednessMoving_ )
                    >> xml::attribute( "working", tirednessWorking_ )
                    >> xml::attribute( "engine-stopped", tirednessEngineStopped_ )
                    >> xml::attribute( "engine-running", tirednessEngineRunning_ );
    tirednessThresholds_.ReadArchive( input );
    input       >> xml::end // Tiredness-evolution
                >> xml::start( "stress-evolution" )
                    >> xml::attribute( "inc-per-shot", stressIncPerShot_ )
                    >> xml::attribute( "dec-per-hour", stressDecPerHour_ );
    stressThresholds_.ReadArchive( input );
    input       >> xml::end // Stress-evolution
            >> xml::end // Automatic-evolution
          >> xml::end; // humans-factors
}

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data::ReadModifier
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::ReadModifier( xml::xistream& input )
{
    const std::string state = input.attribute< std::string >( "state" );
    if( state == "Veteran" )
        veteranModifiers_.ReadArchive( input );
    else if( state == "Experimente" )
        xpModifiers_.ReadArchive( input );
    else if( state == "Conscrit" )
        newbieModifiers_.ReadArchive( input );
    else if( state == "Normal" )
        normalModifiers_.ReadArchive( input );
    else if( state == "Fatigue" )
        tirednessModifiers_.ReadArchive( input );
    else if( state == "Epuise" )
        exhaustedModifiers_.ReadArchive( input );
    else if( state == "Calm" )
        calmModifiers_.ReadArchive( input );
    else if( state == "Worried" )
        worriedModifiers_.ReadArchive( input );
    else if( state == "Stressed" )
        stressedModifiers_.ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_HumanFactors_Data::WriteArchive( xml::xostream& output ) const
{
    output  << xml::start( "humans-factors" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "HumanFactors" );
    output  << xml::start( "experience-factor" );
    veteranModifiers_.WriteArchive( output );
    xpModifiers_.WriteArchive( output );
    newbieModifiers_.WriteArchive( output );
    output  << xml::end // experience-factor
            << xml::start( "tiredness-factor" );
    normalModifiers_.WriteArchive( output );
    tirednessModifiers_.WriteArchive( output );
    exhaustedModifiers_.WriteArchive( output );
    output  << xml::end // tiredness-factor
            << xml::start( "stress-factor" );
    calmModifiers_.WriteArchive( output );
    worriedModifiers_.WriteArchive( output );
    stressedModifiers_.WriteArchive( output );
    output  << xml::end // Stress-factor
            << xml::start( "automatic-evolution" )
                << xml::start( "tiredness-evolution" )
                    << xml::attribute( "moving", tirednessMoving_ )
                    << xml::attribute( "working", tirednessWorking_ )
                    << xml::attribute( "engine-stopped", tirednessEngineStopped_ )
                    << xml::attribute( "engine-running", tirednessEngineRunning_ );
    tirednessThresholds_.WriteArchive( output );
    output      << xml::end // tireness-evolution
                << xml::start( "stress-evolution" )
                    << xml::attribute( "inc-per-shot", stressIncPerShot_ )
                    << xml::attribute( "dec-per-hour", stressDecPerHour_ );
    stressThresholds_.WriteArchive( output );
    output      << xml::end // stress-evolution
            << xml::end // Automatic-evolution
        << xml::end; // humans-factor
}
