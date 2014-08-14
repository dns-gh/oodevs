// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data constructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_Data::ADN_AiEngine_Data()
    : ADN_Data_ABC                 ( eAiEngine )
    , rPertinenceMaxDecrease_      ( 0.f )
    , rOperationalStateMaxDecrease_( 0.f )
    , rNeutralizedStateMaxDecrease_( 0.f )
    , rMinorEquipmentWeight_       ( 0.f )
    , rMajorEquipmentWeight_       ( 1.f )
    , rHumanWeight_                ( 0.f )
    , rDecisionalThreshold_        ( 0.f )
    , rUrbanCombatWeight_          ( 100.f )
    , rDefaultFeedbackTime_        ( "1s" )
    , rMinimumAffinity_            ( "1s" )
    , rNeutralAffinity_            ( "1s" )
    , rMaximumAffinity_            ( "1s" )
    , rAvailableModificator_       ( 100.f )
    , rUnavailableModificator_     ( 100.f )
    , rRepairableModificator_      ( 100.f )
    , rRepairingModificator_       ( 100.f )
    , rCapturedModificator_        ( 100.f )
    , bDetectDestroyedUnits_       ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data destructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_Data::~ADN_AiEngine_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data::FilesNeeded
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDecisional_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "decisional" )
            >> xml::start( "dangerosity-modifiers" )
                >> xml::attribute( "max-accuracy", rPertinenceMaxDecrease_ )
                >> xml::attribute( "max-operational-state", rOperationalStateMaxDecrease_ )
                >> xml::attribute( "max-neutralized-state", rNeutralizedStateMaxDecrease_ )
            >> xml::end;

    if( rPertinenceMaxDecrease_.GetData() < 0.0 || rPertinenceMaxDecrease_.GetData() > 100.0 )
        throw MASA_EXCEPTION( tools::translate( "AiEngine_Data", "Op. Indicators - Thread management - Invalid relevance effect value(must be >=0 and <= 100)" ).toStdString() );

    if( rOperationalStateMaxDecrease_.GetData() < 0.0 || rOperationalStateMaxDecrease_.GetData() > 100.0 )
        throw MASA_EXCEPTION( tools::translate( "AiEngine_Data", "Op. Indicators - Thread management - Invalid operational state effect value (must be >=0 and <= 100)" ).toStdString() );

    if( rNeutralizedStateMaxDecrease_.GetData() < 0.0 || rNeutralizedStateMaxDecrease_.GetData() > 100.0 )
        throw MASA_EXCEPTION( tools::translate( "AiEngine_Data", "Op. Indicators - Thread management - Invalid neutralization state effect value (must be >=0 and <= 100)" ).toStdString() );

    input >> xml::start( "operational-state-weights" )
            >> xml::attribute( "component", rMinorEquipmentWeight_ )
            >> xml::attribute( "major-component", rMajorEquipmentWeight_ )
            >> xml::attribute( "crew", rHumanWeight_ )
            >> xml::attribute( "threshold", rDecisionalThreshold_ )
          >> xml::end;

    if( rMinorEquipmentWeight_.GetData() < 0.f || rMinorEquipmentWeight_.GetData() > 1.f )
        throw MASA_EXCEPTION( tools::translate( "AiEngine_Data", "Op. Indicators - Op. state computation - Invalid equipment weight sum (must be >=0 and <= 1)" ).toStdString() );

    if( rMajorEquipmentWeight_.GetData() < 0.f || rMajorEquipmentWeight_.GetData() > 1.f )
        throw MASA_EXCEPTION( tools::translate( "AiEngine_Data", "Op. Indicators - Op. state computation - Invalid equipment weight sum (must be >=0 and <= 1)" ).toStdString() );

    if( rMinorEquipmentWeight_.GetData() + rMajorEquipmentWeight_.GetData() != 1.f )
        throw MASA_EXCEPTION( tools::translate( "AiEngine_Data", "Op. Indicators - Op. state computation - Invalid equipment weight sum (must be >=0 and <= 1)" ).toStdString() );

    if( rDecisionalThreshold_.GetData() < 0.f || rDecisionalThreshold_.GetData() > 1.f )
        throw MASA_EXCEPTION( tools::translate( "AiEngine_Data", "Op. Indicators - Op. state computation - Invalid threshold (must be >=0 and <= 1)" ).toStdString() );

    if( rHumanWeight_.GetData() < 0.f || rHumanWeight_.GetData() > 1.f )
        throw MASA_EXCEPTION( tools::translate( "AiEngine_Data", "Op. Indicators - Op. state computation - Invalid crew state weight (must be >=0 and <= 1)" ).toStdString() );

    input >> xml::start( "force-ratio" )
            >> xml::attribute( "default-feedback-time", rDefaultFeedbackTime_ )
          >> xml::end;

    input >> xml::optional >> xml::start( "urban-combat" )
            >> xml::attribute( "hit-factor", rUrbanCombatWeight_ )
          >> xml::end;

    input >> xml::optional >> xml::start( "critical-intelligence-delay" )
            >> xml::attribute( "minimum-affinity", rMinimumAffinity_ )
            >> xml::attribute( "neutral-affinity", rNeutralAffinity_ )
            >> xml::attribute( "maximum-affinity", rMaximumAffinity_ )
          >> xml::end;

    input >> xml::start( "wound-effects-modificators" )
            >> xml::attribute( "available", rAvailableModificator_ )
            >> xml::attribute( "unavailable", rUnavailableModificator_ )
            >> xml::attribute( "repairable", rRepairableModificator_ )
            >> xml::attribute( "repairing", rRepairingModificator_ )
            >> xml::attribute( "captured", rCapturedModificator_ )
        >> xml::end;

    input >> xml::optional >> xml::start( "perception" )
            >> xml::attribute( "detect-destroyed-units", bDetectDestroyedUnits_ )
          >> xml::end;

    rAvailableModificator_ = rAvailableModificator_.GetData() * 100.f;
    rUnavailableModificator_ = rUnavailableModificator_.GetData() * 100.f;
    rRepairableModificator_ = rRepairableModificator_.GetData() * 100.f;
    rRepairingModificator_ = rRepairingModificator_.GetData() * 100.f;
    rCapturedModificator_ = rCapturedModificator_.GetData() * 100.f;

    input >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "decisional" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Decisional" );
    output  << xml::start( "dangerosity-modifiers" )
                << xml::attribute( "max-accuracy", rPertinenceMaxDecrease_ )
                << xml::attribute( "max-operational-state", rOperationalStateMaxDecrease_ )
                << xml::attribute( "max-neutralized-state", rNeutralizedStateMaxDecrease_ )
            << xml::end
            << xml::start( "operational-state-weights" )
                << xml::attribute( "component", rMinorEquipmentWeight_ )
                << xml::attribute( "major-component", rMajorEquipmentWeight_ )
                << xml::attribute( "crew", rHumanWeight_ )
                << xml::attribute( "threshold", rDecisionalThreshold_ )
            << xml::end
            << xml::start( "force-ratio" )
                << xml::attribute( "default-feedback-time", rDefaultFeedbackTime_ )
            << xml::end
            << xml::start( "urban-combat" )
                << xml::attribute( "hit-factor", rUrbanCombatWeight_ )
            << xml::end
            << xml::start( "critical-intelligence-delay" )
                << xml::attribute( "minimum-affinity", rMinimumAffinity_ )
                << xml::attribute( "neutral-affinity", rNeutralAffinity_ )
                << xml::attribute( "maximum-affinity", rMaximumAffinity_ )
            << xml::end
            << xml::start( "wound-effects-modificators" )
                << xml::attribute( "available", rAvailableModificator_.GetData() / 100.f )
                << xml::attribute( "unavailable", rUnavailableModificator_.GetData() / 100.f )
                << xml::attribute( "repairable", rRepairableModificator_.GetData() / 100.f )
                << xml::attribute( "repairing", rRepairingModificator_.GetData() / 100.f )
                << xml::attribute( "captured", rCapturedModificator_.GetData() / 100.f )
            << xml::end
            << xml::start( "perception" )
                << xml::attribute( "detect-destroyed-units", bDetectDestroyedUnits_ )
            << xml::end
          << xml::end;
}
