//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $input: /MVW_v10/Build/SDK/ADN/Src/ADN_Sensors_Data.cpp $
// $Author: Nld $
// $Modtime: 3/05/05 18:28 $
// $Revision: 15 $
// $Workfile: ADN_Sensors_Data.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

// =============================================================================
//
// =============================================================================


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::LimitedToSensorsInfos
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
ADN_Sensors_Data::LimitedToSensorsInfos::LimitedToSensorsInfos()
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::~LimitedToSensorsInfos
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
ADN_Sensors_Data::LimitedToSensorsInfos::~LimitedToSensorsInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::GetItemName
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::LimitedToSensorsInfos::GetItemName()
{
    return strName_.GetData();
}
// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::CreateCopy
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
ADN_Sensors_Data::LimitedToSensorsInfos* ADN_Sensors_Data::LimitedToSensorsInfos::CreateCopy()
{
    LimitedToSensorsInfos* newInfos = new LimitedToSensorsInfos();
    newInfos->strName_ = strName_.GetData();
    return newInfos;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::GetNodeName
// Created: JSR 2010-03-18
// LTO
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::LimitedToSensorsInfos::GetNodeName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::ReadArchive
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::LimitedToSensorsInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ );
}
// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::WriteArchive
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::LimitedToSensorsInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "sensor" )
                << xml::attribute( "name", strName_ )
           << xml::end;
}

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ModificatorSizeInfos::ModificatorSizeInfos
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ModificatorSizeInfos::ModificatorSizeInfos(ADN_Categories_Data::SizeInfos* ptr)
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, ptrSize_(ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos(),(ADN_Categories_Data::SizeInfos*)ptr)
, rCoeff_(0.0)
{
    BindExistenceTo(&ptrSize_);
    rCoeff_.SetDataName( "le modificateur de perception dû" );
    rCoeff_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ModificatorSizeInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorSizeInfos::GetNodeName()
{
    std::string strResult( "à la catégorie de volume " );
    return strResult + ptrSize_.GetData()->GetData();
}

// -----------------------------------------------------------------------------
// Name: ModificatorSizeInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorSizeInfos::GetItemName()
{
    return ptrSize_.GetData()->GetData();
}

// -----------------------------------------------------------------------------
// Name: ModificatorSizeInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorSizeInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorSizeInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorSizeInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "type", ptrSize_.GetData()->GetData() )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::ModificatorIlluminationInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ModificatorIlluminationInfos::ModificatorIlluminationInfos(const E_LightingType& e)
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, eType_(e)
, rCoeff_(0.0)
{
    rCoeff_.SetDataName( "le modificateur de perception dû" );
    rCoeff_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorIlluminationInfos::GetNodeName()
{
    std::string strResult( "à la luminosité " );
    return strResult + ENT_Tr::ConvertFromLightingType( eType_, ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorIlluminationInfos::GetItemName()
{
    return ENT_Tr::ConvertFromLightingType( eType_, ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorIlluminationInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorIlluminationInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "type", ENT_Tr::ConvertFromLightingType( eType_ ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::ModificatorMeteoInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ModificatorMeteoInfos::ModificatorMeteoInfos(const E_SensorWeatherModifiers& e)
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, eType_(e)
, rCoeff_(0.0)
{
    rCoeff_.SetDataName( "le modificateur de perception dû" );
    rCoeff_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorMeteoInfos::GetNodeName()
{
    std::string strResult( "à la condition météo " );
    return strResult + ADN_Tr::ConvertFromSensorWeatherModifiers( eType_, ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorMeteoInfos::GetItemName()
{
    return ADN_Tr::ConvertFromSensorWeatherModifiers( eType_, ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorMeteoInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorMeteoInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "type", ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( eType_ ) ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::ModificatorEnvironmentInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ModificatorEnvironmentInfos::ModificatorEnvironmentInfos(const E_VisionObject& e)
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, eType_(e)
, rCoeff_(0.0)
{
    rCoeff_.SetDataName( "le modificateur de perception dû" );
    rCoeff_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorEnvironmentInfos::GetNodeName()
{
    std::string strResult( "au type de terrain " );
    return strResult + ADN_Tr::ConvertFromVisionObject( eType_, ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorEnvironmentInfos::GetItemName()
{
    return ADN_Tr::ConvertFromVisionObject( eType_, ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorEnvironmentInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorEnvironmentInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "value", rCoeff_ )
            << xml::attribute( "type", ADN_Tr::ConvertFromVisionObject( eType_ ) )
           << xml::end;
}

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::ModificatorUrbanBlockInfos
// Created: SLG 2010-03-02
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ModificatorUrbanBlockInfos::ModificatorUrbanBlockInfos( ADN_Urban_Data::UrbanMaterialInfos* ptr )
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, ptrMaterial_(ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(),(ADN_Urban_Data::UrbanMaterialInfos*)ptr)
, rCoeff_(0.0)
{
    BindExistenceTo(&ptrMaterial_);
    rCoeff_.SetDataName( "le modificateur de perception dû" );
    rCoeff_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: ModificatorUrbanBlockInfos::GetNodeName
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorUrbanBlockInfos::GetNodeName()
{
    std::string strResult( "au type de materiaux " );
    return ( QString( strResult.c_str() ) + QString( ptrMaterial_.GetData()->strName_.GetData().c_str() ) ).ascii();
}

// -----------------------------------------------------------------------------
// Name: ModificatorUrbanBlockInfos::GetItemName
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorUrbanBlockInfos::GetItemName()
{
    return ptrMaterial_.GetData()->strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ModificatorUrbanBlockInfos::ReadArchive
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorUrbanBlockInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorUrbanBlockInfos::WriteArchive
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorUrbanBlockInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "distance-modifier" )
        << xml::attribute( "value", rCoeff_ )
        << xml::attribute( "type", ptrMaterial_.GetData()->strName_ )
        << xml::end;
}

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ModificatorPostureInfos
// Created: JDY 03-09-29
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ModificatorPostureInfos::ModificatorPostureInfos(const E_UnitPosture& e )
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, eType_(e)
, rCoeff_(1.0)
{
    rCoeff_.SetDataName( "le modificateur de perception dû à une posture " );
    rCoeff_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorPostureInfos::GetNodeName()
{
    return ENT_Tr::ConvertFromUnitPosture( eType_,  ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorPostureInfos::GetItemName()
{
    return ENT_Tr::ConvertFromUnitPosture( eType_,  ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorPostureInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorPostureInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "type", ADN_Tools::ComputePostureScriptName( eType_ ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: TargetInfos::TargetInfos
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
ADN_Sensors_Data::TargetInfos::TargetInfos()
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, ptrObject_          ( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(),0)
, rDistanceDetection_ ( 0 )
, populationInfos_    ()
{
    // Reference connection
    BindExistenceTo(&ptrObject_);

    rDistanceDetection_.SetDataName( "la distance de détection" );
    rDistanceDetection_.SetParentNode( *this );

    // Initialize the posture modificator infos
    for( int i=0 ; i < eNbrUnitPosture ; ++i)
        vModifStance_.AddItem( new ModificatorPostureInfos( (E_UnitPosture)i ) );
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::~TargetInfos
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Sensors_Data::TargetInfos::~TargetInfos()
{
    vModifStance_.Reset();
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::TargetInfos::GetNodeName()
{
    std::string strResult( "de l'objet " );
    return strResult + ptrObject_.GetData()->strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::TargetInfos::GetItemName()
{
    return ptrObject_.GetData()->strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::CreateCopy
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
ADN_Sensors_Data::TargetInfos* ADN_Sensors_Data::TargetInfos::CreateCopy()
{
    TargetInfos* pNew         = new TargetInfos();
    pNew->ptrObject_          = ptrObject_.GetData();
    pNew->strName_            = strName_.GetData();
    pNew->rDistanceDetection_ = rDistanceDetection_.GetData();
    pNew->populationInfos_.CopyFrom( populationInfos_ );
    for( IT_ModificatorPostureInfos_Vector it = vModifStance_.begin(); it != vModifStance_.end(); ++it )
    {
        ModificatorPostureInfos* pNewModif = new ModificatorPostureInfos( (*it)->eType_ );
        pNewModif->rCoeff_ = (*it)->rCoeff_.GetData();
        pNew->vModifStance_.AddItem( pNewModif );
    }
    return pNew;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::TargetInfos::ReadPosture
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::TargetInfos::ReadPosture( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    for( unsigned i = 0; i < eNbrUnitPosture; ++i )
        if( type == ADN_Tools::ComputePostureScriptName( E_UnitPosture( i ) ) )
        {
            vModifStance_.at( i )->ReadArchive( input );
            return;
        }
    throw ADN_DataException( tools::translate( "Sensor_Data", "Invalid data" ).ascii(), tools::translate( "Sensor_Data", "Sensors - Invalid stance '%1'" ).arg( type.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::TargetInfos::ReadArchive( xml::xistream& input )
{
    std::string strType;
    input >> xml::attribute( "type", strType );
    ADN_Objects_Data::ObjectInfos* pObject = ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( strType );
    if( !pObject )
        throw ADN_DataException( tools::translate( "Sensor_Data", "Invalid data" ).ascii(),tools::translate( "Sensor_Data", "Sensors - Invalid object '%1'" ).arg( strType.c_str() ).ascii() );
    ptrObject_ = pObject;
    strName_ = ptrObject_.GetData()->strName_.GetData();

    input >> xml::attribute( "detection-distance", rDistanceDetection_ );
    populationInfos_.ReadArchive( input );
    input >> xml::start( "source-posture-modifiers" )
            >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::TargetInfos::ReadPosture )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::TargetInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "object" )
        << xml::attribute( "type", ptrObject_.GetData()->strType_ )
        << xml::attribute( "detection-distance", rDistanceDetection_ );
    populationInfos_.WriteArchive( output );

    output << xml::start( "source-posture-modifiers" );
    for( IT_ModificatorPostureInfos_Vector it = vModifStance_.begin(); it != vModifStance_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;

    output << xml::end;
}

// =============================================================================
// ADN_Sensor_Data::PopulationInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::PopulationInfos
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_Sensors_Data::PopulationInfos::PopulationInfos()
    : ADN_Ref_ABC()
    , strName_   ()
    , rDensity_  ( 0. )
    , rModifier_ ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::~PopulationInfos
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_Sensors_Data::PopulationInfos::~PopulationInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::GetItemName
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::PopulationInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::CopyFrom
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::PopulationInfos::CopyFrom( PopulationInfos& populationInfo )
{
    rDensity_  = populationInfo.rDensity_ .GetData();
    rModifier_ = populationInfo.rModifier_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::ReadArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::PopulationInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "population-modifier" )
            >> xml::attribute( "density", rDensity_ )
            >> xml::attribute( "modifier", rModifier_ )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::WriteArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::PopulationInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "population-modifier" )
            << xml::attribute( "density", rDensity_ )
            << xml::attribute( "modifier", rModifier_ )
           << xml::end;
}

// =============================================================================
// ADN_Sensor_Data::SensorInfos
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Sensors_Data::SensorInfos::SensorInfos()
    : bCanDetectAgents_    ( false )
    , bCanDetectObjects_   ( false )
    , bCanScan_            ( false )
    , rFirerDetectionRange_( 0 )
    , rAngle_              ( 0 )
    , bLimitedToSensors_   ( false ) // LTO
    , rDistProximity_      ( 0 )
    , rDistDetection_      ( 0 )
    , rDistReco_           ( 0 )
    , rDistIdent_          ( 0 )
    , vModifSizes_         ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos() )
    , vModifIlluminations_ ( false )
    , vModifWeather_       ( false )
    , vModifEnvironments_  ( false )
    , vModifUrbanBlocks_   ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
    , vModifStance_        ( false )
    , vModifTargetStance_  ( false )
    , detectionDelay_      ( "0h" )
{
    strName_.SetDataName( "le nom" );
    strName_.SetParentNode( *this );

    rDistDetection_.SetDataName( "la portée de détection" );
    rDistDetection_.SetParentNode( *this );

    rDistReco_.SetDataName( "la portée de reconnaissance" );
    rDistReco_.SetParentNode( *this );

    rDistIdent_.SetDataName( "la portée d'identification" );
    rDistIdent_.SetParentNode( *this );

    rAngle_.SetDataName( "l'angle d'ouverture" );
    rAngle_.SetParentNode( *this );

    bCanScan_.SetDataName( "la capacité d'effectuer un balayage à 360 degrés" );
    bCanScan_.SetParentNode( *this );

    rFirerDetectionRange_.SetDataName( "la capacité de détecter le tireur" );
    rFirerDetectionRange_.SetParentNode( *this );

    bCanDetectAgents_.SetDataName( "la capacité de détecter des unités" );
    bCanDetectAgents_.SetParentNode( *this );

    bLimitedToSensors_.SetDataName( "la capacité de détecter d'autres capteurs" ); // LTO
    bLimitedToSensors_.SetParentNode( *this ); // LTO

    bCanDetectObjects_.SetDataName( "la capacité de détecter des objets" );
    bCanDetectObjects_.SetParentNode( *this );

    vTargets_.SetParentNode( *this );
    vTargets_.SetItemTypeName( "une cible objet" );

    vLimitedToSensorsInfos_.SetParentNode( *this ); // LTO
    vLimitedToSensorsInfos_.SetItemTypeName( "une cible capteur" ); // LTO

    vModifIlluminations_.SetParentNode( *this );
    vModifWeather_.SetParentNode( *this );
    vModifEnvironments_.SetParentNode( *this );
    vModifUrbanBlocks_.SetParentNode( *this );
    vModifStance_.SetParentNode( *this );
    vModifTargetStance_.SetParentNode( *this );
    vModifSizes_.SetParentNode( *this );

    // initialize illumination modificator infos
    unsigned int i = 0;
    for( i = 0; i< eNbrLightingType; ++i )
    {
        ModificatorIlluminationInfos* pInfo = new ModificatorIlluminationInfos((E_LightingType)i);
        vModifIlluminations_.AddItem( pInfo );
    }

    // initialize meteo modificator infos
    for( i = 0; i< eNbrSensorWeatherModifiers; ++i )
    {
        ModificatorMeteoInfos* pInfo = new ModificatorMeteoInfos((E_SensorWeatherModifiers)i);
        vModifWeather_.AddItem( pInfo );
    }

    // initialize environment modificator infos
    for( i = 0; i< eNbrVisionObjects; ++i )
    {
        ModificatorEnvironmentInfos* pInfo = new ModificatorEnvironmentInfos((E_VisionObject)i);
        vModifEnvironments_.AddItem( pInfo );
    }

    // initialize posture modificator infos
    for( i = 0; i< eNbrUnitPosture; ++i )
    {
        ModificatorPostureInfos* pInfo1 = new ModificatorPostureInfos((E_UnitPosture)i);
        vModifStance_.AddItem( pInfo1 );
        ModificatorPostureInfos* pInfo2 = new ModificatorPostureInfos((E_UnitPosture)i);
        vModifTargetStance_.AddItem( pInfo2 );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos
// Created: JDY 03-07-15
//-----------------------------------------------------------------------------
ADN_Sensors_Data::SensorInfos::~SensorInfos()
{
    vTargets_.Reset();
    vLimitedToSensorsInfos_.Reset(); // LTO
    vModifIlluminations_.Reset();
    vModifWeather_.Reset();
    vModifEnvironments_.Reset();
    vModifUrbanBlocks_.Reset();
    vModifStance_.Reset();
    vModifTargetStance_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::SensorInfos::GetNodeName()
{
    const std::string strResult( "du senseur " ); // $$$$ _RC_ SLI 2011-02-02: translation?
    return strResult + strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::SensorInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::CreateCopy
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Sensors_Data::SensorInfos* ADN_Sensors_Data::SensorInfos::CreateCopy()
{
    ADN_Sensors_Data::SensorInfos* pCopy = new ADN_Sensors_Data::SensorInfos();
    pCopy->rDistProximity_      = rDistProximity_.GetData();
    pCopy->rDistDetection_      = rDistDetection_.GetData();
    pCopy->rDistReco_           = rDistReco_.GetData();
    pCopy->rDistIdent_          = rDistIdent_.GetData();
    pCopy->rAngle_              = rAngle_.GetData();
    pCopy->bCanScan_            = bCanScan_.GetData();
    pCopy->rFirerDetectionRange_= rFirerDetectionRange_.GetData();
    pCopy->bCanDetectAgents_    = bCanDetectAgents_.GetData();
    pCopy->bLimitedToSensors_   = bLimitedToSensors_.GetData(); // LTO
    pCopy->bCanDetectObjects_   = bCanDetectObjects_.GetData();
    pCopy->detectionDelay_ = detectionDelay_.GetData();
    pCopy->populationInfos_.CopyFrom( populationInfos_ );

    for( T_TargetsInfos_Vector::iterator itTarget = vTargets_.begin(); itTarget != vTargets_.end(); ++itTarget )
    {
        TargetInfos* pNewInfo = (*itTarget)->CreateCopy();
        pCopy->vTargets_.AddItem( pNewInfo );
    }

    // LTO begin
    pCopy->vLimitedToSensorsInfos_.reserve( vLimitedToSensorsInfos_.size() );
    for( IT_LimitedToSensorsInfos_Vector it = vLimitedToSensorsInfos_.begin(); it != vLimitedToSensorsInfos_.end(); ++it )
    {
        LimitedToSensorsInfos* newInfos = (*it)->CreateCopy();
        pCopy->vLimitedToSensorsInfos_.AddItem( newInfos );
    }
    // LTO end

    uint i;
    for( i= 0 ; i< eNbrLightingType ; ++i)
        pCopy->vModifIlluminations_[i]->rCoeff_ = vModifIlluminations_[i]->rCoeff_.GetData();

    // initialize meteo modificator infos
    for( i= 0 ; i< eNbrSensorWeatherModifiers; ++i)
        pCopy->vModifWeather_[i]->rCoeff_ = vModifWeather_[i]->rCoeff_.GetData();

    // initialize environment modificator infos
    for( i= 0 ; i< eNbrVisionObjects ; ++i)
        pCopy->vModifEnvironments_[i]->rCoeff_ = vModifEnvironments_[i]->rCoeff_.GetData();

    // initialize posture modificator infos
    for( i=0 ; i< eNbrUnitPosture ; ++i)
    {
        pCopy->vModifStance_[i]->rCoeff_ = vModifStance_[i]->rCoeff_.GetData();
        pCopy->vModifTargetStance_[i]->rCoeff_ = vModifTargetStance_[i]->rCoeff_.GetData();
    }

    for( T_ModificatorSizeInfos_Vector::iterator itSizeModif = vModifSizes_.begin(); itSizeModif != vModifSizes_.end(); ++itSizeModif )
        pCopy->vModifSizes_[ std::distance( vModifSizes_.begin(), itSizeModif ) ]->rCoeff_ = (*itSizeModif)->rCoeff_.GetData();

    for( T_ModificatorUrbanBlockInfos_Vector::iterator itUrbanBlockModif = vModifUrbanBlocks_.begin(); itUrbanBlockModif != vModifUrbanBlocks_.end(); ++itUrbanBlockModif )
        pCopy->vModifUrbanBlocks_[ std::distance( vModifUrbanBlocks_.begin(), itUrbanBlockModif ) ]->rCoeff_ = (*itUrbanBlockModif)->rCoeff_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ReadLimitedToSensorsList
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadLimitedToSensorsList( xml::xistream& input )
{
    bLimitedToSensors_ = true;

    std::auto_ptr< LimitedToSensorsInfos > spNew( new LimitedToSensorsInfos() );
    spNew->ReadArchive( input );
    vLimitedToSensorsInfos_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadBaseDistance
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadBaseDistance( xml::xistream& input )
{
    const std::string level = input.attribute< std::string >( "level" );
    if( level == "identification" )
        input >> xml::attribute( "distance", rDistIdent_ );
    else if( level == "recognition" )
        input >> xml::attribute( "distance", rDistReco_ );
    else if( level == "detection" )
        input >> xml::attribute( "distance", rDistDetection_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadSize
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadSize( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    IT_ModificatorSizeInfos_Vector it = std::find_if( vModifSizes_.begin(), vModifSizes_.end(), ModificatorSizeInfos::Cmp( type ) );
    if( it == vModifSizes_.end() )
        throw ADN_DataException( tools::translate( "Sensor_Data", "Invalid data" ).ascii(), tools::translate( "Sensor_Data", "Sensors - Invalid unit volume '%1'" ).arg( type.c_str() ).ascii() );
    (*it)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadPrecipitation
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadPrecipitation( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    for( unsigned i = 0; i < eNbrSensorWeatherModifiers; ++i )
        if( ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( E_SensorWeatherModifiers( i ) ) ) == type )
        {
            vModifWeather_.at( i )->ReadArchive( input );
            return;
        }
    throw ADN_DataException( tools::translate( "Sensor_Data", "Invalid data" ).ascii(),tools::translate( "Sensor_Data", "Sensors - Invalid weather '%1'" ).arg( type.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadVisibility
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadVisibility( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    E_LightingType n = ENT_Tr::ConvertToLightingType( type );
    vModifIlluminations_.at( n )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadSourcePosture
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadSourcePosture( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    for( unsigned i = 0; i < eNbrUnitPosture; ++i )
        if( type == ADN_Tools::ComputePostureScriptName( E_UnitPosture( i ) ) )
        {
            vModifStance_.at( i )->ReadArchive( input );
            return;
        }
    throw ADN_DataException( tools::translate( "Sensor_Data", "Invalid data" ).ascii(), tools::translate( "Sensor_Data", "Sensors - Invalid stance '%1'" ).arg( type.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadTargetPosture
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadTargetPosture( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    for( unsigned i = 0; i < eNbrUnitPosture; ++i )
        if( type == ADN_Tools::ComputePostureScriptName( E_UnitPosture( i ) ) )
        {
            vModifTargetStance_.at( i )->ReadArchive( input );
            return;
        }
    throw ADN_DataException( tools::translate( "Sensor_Data", "Invalid data" ).ascii(), tools::translate( "Sensor_Data", "Sensors - Invalid stance '%1'" ).arg( type.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadTerrain
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadTerrain( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    E_VisionObject n = ADN_Tr::ConvertToVisionObject( type );
    vModifEnvironments_.at( n )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadTerrain
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadUrbanBlockMaterial( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    IT_ModificatorUrbanBlockInfos_Vector it = std::find_if( vModifUrbanBlocks_.begin(), vModifUrbanBlocks_.end(), ModificatorUrbanBlockInfos::Cmp( type ) );
    if( it == vModifUrbanBlocks_.end() )
        throw ADN_DataException( tools::translate( "Sensor_Data", "Invalid data" ).ascii(), tools::translate( "Sensor_Data", "Sensors - Invalid unit volume '%1'" ).arg( type.c_str() ).ascii() );
    (*it)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadUnitDetection
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadUnitDetection( xml::xistream& input )
{
    bCanDetectAgents_ = true;

    input >> xml::attribute( "scanning", bCanScan_ )
          >> xml::attribute( "firer-detection-distance", rFirerDetectionRange_ )
          >> xml::attribute( "angle", rAngle_ )
          >> xml::optional >> xml::start( "limited-to-sensors" ) // LTO
            >> xml::list( "sensor", *this, &ADN_Sensors_Data::SensorInfos::ReadLimitedToSensorsList ) // LTO
          >> xml::end // LTO
          >> xml::start( "base-distances" )
            >> xml::attribute( "close-range", rDistProximity_ )
            >> xml::list( "base-distance", *this, &ADN_Sensors_Data::SensorInfos::ReadBaseDistance )
          >> xml::end
          >> xml::start( "distance-modifiers" )
            >> xml::start( "size-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadSize )
            >> xml::end
            >> xml::start( "precipitation-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadPrecipitation )
            >> xml::end
            >> xml::start( "visibility-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadVisibility )
            >> xml::end
            >> xml::start( "source-posture-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadSourcePosture )
            >> xml::end
            >> xml::start( "target-posture-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadTargetPosture )
            >> xml::end
            >> xml::start( "terrain-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadTerrain )
            >> xml::end
            >> xml::start( "urbanBlock-material-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadUrbanBlockMaterial )
            >> xml::end;
    populationInfos_.ReadArchive( input );
    input >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadItem
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadItem( const std::string& name, xml::xistream& input )
{
    if( name == "unit-detection" )
        ReadUnitDetection( input );
    else if( name == "object-detection" )
        ReadObjectDetection( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadObject
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadObject( xml::xistream& input )
{
    std::auto_ptr<TargetInfos> spNew( new TargetInfos() );
    spNew->ReadArchive( input );
    vTargets_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadObjectDetection
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadObjectDetection( xml::xistream& input )
{
    bCanDetectObjects_ = true;
    input >> xml::list( "object", *this, &ADN_Sensors_Data::SensorInfos::ReadObject );
}

// -----------------------------------------------------------------------------
// Name: SensorInfos::ReadArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "detection-delay", detectionDelay_ )
          >> xml::list( *this, &ADN_Sensors_Data::SensorInfos::ReadItem );
}

// -----------------------------------------------------------------------------
// Name: SensorInfos::WriteArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "sensor" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "detection-delay", detectionDelay_ );

    if( bCanDetectAgents_.GetData() )
    {
        output << xml::start( "unit-detection" )
                << xml::attribute( "scanning", bCanScan_ )
                << xml::attribute( "firer-detection-distance", rFirerDetectionRange_ )
                << xml::attribute( "angle", rAngle_ );

        // LTO begin
        if( bLimitedToSensors_.GetData() )
        {
            output << xml::start( "limited-to-sensors" );
            for( unsigned int i = 0; i < vLimitedToSensorsInfos_.size(); ++i )
                vLimitedToSensorsInfos_[i]->WriteArchive( output );
            output << xml::end;
        }
        // LTO end

        output  << xml::start( "base-distances" )
                    << xml::attribute( "close-range", rDistProximity_ )
                    << xml::start( "base-distance" )
                        << xml::attribute( "level", "identification" ) << xml::attribute( "distance", rDistIdent_ )
                    << xml::end
                    << xml::start( "base-distance" )
                        << xml::attribute( "level", "recognition" ) << xml::attribute( "distance", rDistReco_ )
                    << xml::end
                    << xml::start( "base-distance" )
                        << xml::attribute( "level", "detection" ) << xml::attribute( "distance", rDistDetection_ )
                    << xml::end
                << xml::end; //base-distances

        output << xml::start( "distance-modifiers" );

        populationInfos_.WriteArchive( output );

        output << xml::start( "size-modifiers" );
        for( IT_ModificatorSizeInfos_Vector it1 = vModifSizes_.begin(); it1 != vModifSizes_.end(); ++it1 )
            (*it1)->WriteArchive( output );
        output << xml::end;

        output << xml::start( "precipitation-modifiers" );
        for( IT_ModificatorMeteoInfos_Vector it2 = vModifWeather_.begin(); it2 != vModifWeather_.end(); ++it2 )
            (*it2)->WriteArchive( output );
        output << xml::end;

        output << xml::start( "visibility-modifiers" );
        for( IT_ModificatorIlluminationInfos_Vector it3 = vModifIlluminations_.begin(); it3 != vModifIlluminations_.end(); ++it3 )
            (*it3)->WriteArchive( output );
        output << xml::end;

        output << xml::start( "source-posture-modifiers" );
        for( IT_ModificatorPostureInfos_Vector it4 = vModifStance_.begin(); it4 != vModifStance_.end(); ++it4 )
            (*it4)->WriteArchive( output );
        output << xml::end;

        output << xml::start( "target-posture-modifiers" );
        for( IT_ModificatorPostureInfos_Vector it5 = vModifTargetStance_.begin(); it5 != vModifTargetStance_.end(); ++it5 )
            (*it5)->WriteArchive( output );
        output << xml::end;

        output << xml::start( "terrain-modifiers" );
        for( IT_ModificatorEnvironmentInfos_Vector it6 = vModifEnvironments_.begin(); it6 != vModifEnvironments_.end(); ++it6 )
            (*it6)->WriteArchive( output );
        output << xml::end;

        output << xml::start( "urbanBlock-material-modifiers" );
        for( IT_ModificatorUrbanBlockInfos_Vector it7 = vModifUrbanBlocks_.begin(); it7 != vModifUrbanBlocks_.end(); ++it7 )
            (*it7)->WriteArchive( output );
        output << xml::end;

        output << xml::end; // distance-modifiers
        output << xml::end; // unit-detection
    }

    if( bCanDetectObjects_.GetData() )
    {
        output << xml::start( "object-detection" );
        for( IT_TargetsInfos_Vector it = vTargets_.begin(); it != vTargets_.end(); ++it )
            (*it)->WriteArchive( output );
        output << xml::end;
    }

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ALATInfos::ALATInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::ALATInfos::ALATInfos()
{
    for( int n = 1; n < eNbrVisionObjects; ++n )
        surveyTimes_[ n - 1 ] = "0s";
}

// -----------------------------------------------------------------------------
// Name: ALATInfos::~ALATInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::ALATInfos::~ALATInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ALATInfos::ReadTime
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ALATInfos::ReadTime( xml::xistream& input )
{
    std::string terrain, time;
    input >> xml::attribute( "terrain", terrain )
          >> xml::attribute( "time", time );
    E_VisionObject n = ADN_Tr::ConvertToVisionObject( terrain );
    surveyTimes_[ n-1 ] = time;
}

// -----------------------------------------------------------------------------
// Name: ALATInfos::ReadArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ALATInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "alat-monitoring-times" )
            >> xml::list( "alat-monitoring-time", *this, &ADN_Sensors_Data::ALATInfos::ReadTime )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ALATInfos::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ALATInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "alat-monitoring-times" );
    for( int n = 1; n < eNbrVisionObjects; ++n )
        output << xml::start( "alat-monitoring-time" )
                << xml::attribute( "terrain", ADN_Tr::ConvertFromVisionObject( E_VisionObject(n) ) )
                << xml::attribute( "time", surveyTimes_[ n-1 ] )
               << xml::end;
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: CobraInfos::CobraInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::CobraInfos::CobraInfos()
    : rRange_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CobraInfos::~CobraInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::CobraInfos::~CobraInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CobraInfos::ReadArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::CobraInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "cobra-radar" )
            >> xml::attribute( "action-range", rRange_ )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: CobraInfos::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::CobraInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "cobra-radar" )
            << xml::attribute( "action-range", rRange_ )
           << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data constructor
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ADN_Sensors_Data()
    : radarData_( *new ADN_Radars_Data() )
{
    vSensors_.SetItemTypeName( "le senseur" ); // $$$$ _RC_ SLI 2011-02-02: translation ?
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data destructor
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
ADN_Sensors_Data::~ADN_Sensors_Data()
{
    Reset();
    delete &radarData_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Sensors_Data::FilesNeeded(T_StringList& files) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSensors_.GetData() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::Reset
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
void ADN_Sensors_Data::Reset()
{
    vSensors_.Reset();
    radarData_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ReadSensor
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ReadSensor( xml::xistream& input )
{
    std::auto_ptr<SensorInfos> spNew( new SensorInfos() );
    spNew->ReadArchive( input );
    vSensors_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ReadArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "sensors" )
            >> xml::start( "sensors" )
                >> xml::list( "sensor", *this, &ADN_Sensors_Data::ReadSensor )
            >> xml::end;
    alatInfos_ .ReadArchive( input );
    cobraInfos_.ReadArchive( input );
    radarData_ .ReadArchive( input );
    input >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::WriteArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "sensors" );
    ADN_Tools::AddSchema( output, "Sensors" );
    alatInfos_ .WriteArchive( output );
    cobraInfos_.WriteArchive( output );
    radarData_ .WriteArchive( output );

    output << xml::start( "sensors" );
    for( IT_SensorsInfos_Vector it = vSensors_.begin(); it != vSensors_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;

    output << xml::end;
}
