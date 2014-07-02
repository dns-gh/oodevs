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
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: UrbanSpeedsInfos: Constructor
// Created: JSR 2014-07-09
// -----------------------------------------------------------------------------
ADN_AiEngine_Data::UrbanSpeedsInfos::UrbanSpeedsInfos( ADN_Urban_Data::UrbanMaterialInfos* ptr )
    : ptrMaterial_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), ptr )
    , searchSpeed_( 3 )
    , reconSpeed_( 3 )
{
    BindExistenceTo( &ptrMaterial_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanSpeedsInfos::GetItemName
// Created: JSR 2014-07-09
// -----------------------------------------------------------------------------
std::string ADN_AiEngine_Data::UrbanSpeedsInfos::GetItemName()
{
    return ptrMaterial_.GetData()->strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: UrbanSpeedsInfos::ReadArchive
// Created: JSR 2014-07-09
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::UrbanSpeedsInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "recon-speed", reconSpeed_ )
          >> xml::attribute( "search-speed", searchSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanSpeedsInfos::WriteArchive
// Created: JSR 2014-07-09
// -----------------------------------------------------------------------------
void ADN_AiEngine_Data::UrbanSpeedsInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "urban-speeds" )
                << xml::attribute( "type", ptrMaterial_.GetData()->strName_ )
                << xml::attribute( "recon-speed", reconSpeed_ )
                << xml::attribute( "search-speed", searchSpeed_ )
        << xml::end;
}

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
    , rUrbanViewFactor_            ( 100.f )
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
    , reconSpeed_                  ( 12 )
    , searchSpeed_                 ( 6 )
    , nMaxPerceptionLevel_         ( ePerceptionLevel_Identification )
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

    input >> xml::optional >> xml::start( "urban-perception" )
            >> xml::attribute( "view-factor", rUrbanViewFactor_ )
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

    std::string maxPerceptionLevel;
    input >> xml::optional >> xml::start( "perception" )
            >> xml::attribute( "max-level", maxPerceptionLevel )
            >> xml::attribute( "detect-destroyed-units", bDetectDestroyedUnits_ )
          >> xml::end;
    if( !maxPerceptionLevel.empty() )
        nMaxPerceptionLevel_ = ENT_Tr::ConvertToPerceptionLevel( maxPerceptionLevel );

    urbanSearchSpeeds_.SetFixedVector( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() );
    input >> xml::optional >> xml::start( "recon-and-search-speeds" )
            >> xml::attribute( "recon-speed", reconSpeed_ )
            >> xml::attribute( "search-speed", searchSpeed_ )
            >> xml::list( "urban-speeds", [&]( xml::xistream& xis )
            {
                const std::string type = xis.attribute< std::string >( "type" );
                auto it = std::find_if( urbanSearchSpeeds_.begin(), urbanSearchSpeeds_.end(), ADN_AiEngine_Data::UrbanSpeedsInfos::Cmp( type ) );
                if( it == urbanSearchSpeeds_.end() )
                    throw MASA_EXCEPTION( tools::translate( "ADN_AiEngine_Data", "Invalid data : Op. indicators - Invalid urban material '%1'" ).arg( type.c_str() ).toStdString() );
                ( *it )->ReadArchive( xis );
            })
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
            << xml::start( "urban-perception" )
                << xml::attribute( "view-factor", rUrbanViewFactor_ )
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
                << xml::attribute( "max-level", nMaxPerceptionLevel_.Convert() )
                << xml::attribute( "detect-destroyed-units", bDetectDestroyedUnits_ )
            << xml::end
            << xml::start( "recon-and-search-speeds" )
            << xml::attribute( "recon-speed", reconSpeed_ )
            << xml::attribute( "search-speed", searchSpeed_ );
    for( auto it = urbanSearchSpeeds_.begin(); it != urbanSearchSpeeds_.end(); ++it )
        ( *it )->WriteArchive( output );
    output  << xml::end
          << xml::end;
}
