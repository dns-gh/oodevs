// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "StaticModel.h"
#include "IntelligenceKarmas.h"
#include "SuccessFactorActionTypes.h"
#include "TeamKarmas.h"
#include "FormationLogisticLevel.h"
#include "clients_gui/DrawingTypes.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/ObjectTypes.h"
#include "indicators/GaugeTypes.h"
#include "indicators/Primitives.h"
#include "tools/ExerciseConfig.h"
#include <tools/XmlCrc32Signature.h>
#include <boost/filesystem/path.hpp>

namespace bfs = boost::filesystem;

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StaticModel constructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::StaticModel( Controllers& controllers )
    : controllers_             ( controllers )
    , coordinateSystems_       ( *new CoordinateSystems() )
    , coordinateConverter_     ( *new CoordinateConverter( coordinateSystems_ ) )
    , detection_               ( *new DetectionMap() )
    , types_                   ( *new AgentTypes() )
    , objectTypes_             ( *new ObjectTypes() )
    , levels_                  ( *new FormationLevels() )
    , extensions_              ( *new ExtensionTypes() )
    , teamKarmas_              ( *new TeamKarmas() )
    , intelligenceKarmas_      ( *new IntelligenceKarmas() )
    , drawings_                ( *new gui::DrawingTypes( controllers.controller_ ) )
    , indicators_              ( *new indicators::Primitives() )
    , gaugeTypes_              ( *new indicators::GaugeTypes() )
    , successFactorActionTypes_( *new SuccessFactorActionTypes() )
    , logisticLevels_          ( *new FormationLogisticLevel() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StaticModel destructor
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
StaticModel::~StaticModel()
{
    delete &successFactorActionTypes_;
    delete &gaugeTypes_;
    delete &indicators_;
    delete &drawings_;
    delete &intelligenceKarmas_;
    delete &teamKarmas_;
    delete &extensions_;
    delete &levels_;
    delete &objectTypes_;
    delete &types_;
    delete &detection_;
    delete &coordinateConverter_;
    delete &coordinateSystems_;
    delete &logisticLevels_;
}

namespace
{
    const std::string& XmlSignatureHelper( const std::string& path, std::string& invalidSignatureFiles )
    {
        tools::EXmlCrc32SignatureError error = tools::CheckXmlCrc32Signature( path );
        if( error == tools::eXmlCrc32SignatureError_Invalid || error == tools::eXmlCrc32SignatureError_NotSigned )
            invalidSignatureFiles.append( "\n" + bfs::path( path, bfs::native ).leaf() );
        return path;
    }
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Load
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Load( const tools::ExerciseConfig& config, std::string& invalidSignatureFiles )
{
    Purge();
    types_.Load( config, invalidSignatureFiles );
    objectTypes_.Load( config, invalidSignatureFiles );
    static_cast< CoordinateConverter& >( coordinateConverter_ ).Load( config );
    detection_.Load( config );
    drawings_.Load( XmlSignatureHelper( config.BuildPhysicalChildFile( "DrawingTemplates.xml" ), invalidSignatureFiles ) );
    indicators_.Load( XmlSignatureHelper( tools::GeneralConfig::BuildResourceChildFile( "IndicatorPrimitives.xml" ), invalidSignatureFiles ) );
    gaugeTypes_.Load( XmlSignatureHelper( tools::GeneralConfig::BuildResourceChildFile( "IndicatorGaugeTemplates.xml" ), invalidSignatureFiles ) );
    successFactorActionTypes_.Load( XmlSignatureHelper( tools::GeneralConfig::BuildResourceChildFile( "SuccessFactorActions.xml" ), invalidSignatureFiles ) );
    extensions_.Load( XmlSignatureHelper( tools::GeneralConfig::BuildResourceChildFile( "Extensions.xml" ), invalidSignatureFiles ) );
    controllers_.controller_.Update( ModelLoaded( config ) );
}

// -----------------------------------------------------------------------------
// Name: StaticModel::Purge
// Created: AGE 2006-08-01
// -----------------------------------------------------------------------------
void StaticModel::Purge()
{
    successFactorActionTypes_.Purge();
    gaugeTypes_.Purge();
    indicators_.Purge();
    drawings_.Purge();
    types_.Purge();
    objectTypes_.Purge();
    extensions_.Purge();
    controllers_.controller_.Update( ModelUnLoaded() );
}
