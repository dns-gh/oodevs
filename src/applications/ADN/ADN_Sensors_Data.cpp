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

#include "ADN_pch.h"
#include "ADN_Sensors_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Tools.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"


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
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ModificatorSizeInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorSizeInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadField( ptrSize_.GetData()->GetData(), rCoeff_ );
}


// -----------------------------------------------------------------------------
// Name: ModificatorSizeInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorSizeInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.WriteField( ptrSize_.GetData()->GetData(), rCoeff_.GetData() );
}


// =============================================================================
// 
// =============================================================================


//-----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::ModificatorIlluminationInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ModificatorIlluminationInfos::ModificatorIlluminationInfos(const E_TimeCategory& e)
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
    return strResult + ADN_Tr::ConvertFromTimeCategory( eType_, ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::GetItemName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::ModificatorIlluminationInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorIlluminationInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadField( ADN_Tr::ConvertFromTimeCategory( eType_ ), rCoeff_ );
}


// -----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorIlluminationInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.WriteField( ADN_Tr::ConvertFromTimeCategory( eType_ ), rCoeff_.GetData() );
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
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorMeteoInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadField( ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( eType_ ) ), rCoeff_ );
}


// -----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorMeteoInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.WriteField( ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( eType_ ) ), rCoeff_.GetData() );
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
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorEnvironmentInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadField( ADN_Tr::ConvertFromVisionObject( eType_ ), rCoeff_ );
}


// -----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorEnvironmentInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.WriteField( ADN_Tr::ConvertFromVisionObject( eType_ ), rCoeff_.GetData() );
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
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorPostureInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadField( ADN_Tools::ComputePostureScriptName( eType_ ), rCoeff_ );
}


// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorPostureInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.WriteField( ADN_Tools::ComputePostureScriptName( eType_ ), rCoeff_.GetData() );
}


// =============================================================================
// 
// =============================================================================


//-----------------------------------------------------------------------------
// Name: TargetInfos::TargetInfos
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
ADN_Sensors_Data::TargetInfos::TargetInfos()
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, ptrObject_(ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(),0)
, rDistanceDetection_( 0 )
{
    // Reference connection
    BindExistenceTo(&ptrObject_);

    rDistanceDetection_.SetDataName( "la distance de détection" );
    rDistanceDetection_.SetParentNode( *this );

    // Initialize the posture modificator infos
    for( int i=0 ; i < eNbrUnitPosture ; ++i)
    {
        vModifStance_.AddItem( new ModificatorPostureInfos( (E_UnitPosture)i ) );
    }
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
    TargetInfos* pNew = new TargetInfos();
    pNew->ptrObject_ = ptrObject_.GetData();
    pNew->strName_ = strName_.GetData();
    pNew->rDistanceDetection_ = rDistanceDetection_.GetData();
    for( IT_ModificatorPostureInfos_Vector it = vModifStance_.begin(); it != vModifStance_.end(); ++it )
    {
        ModificatorPostureInfos* pNewModif = new ModificatorPostureInfos( (*it)->eType_ );
        pNewModif->rCoeff_ = (*it)->rCoeff_.GetData();
        pNew->vModifStance_.AddItem( pNewModif );
    }
    return pNew;
}


// -----------------------------------------------------------------------------
// Name: TargetInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::TargetInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Objet" );
    
    std::string strType;
    input.ReadAttribute( "type", strType );
    E_ObjectType nType = ENT_Tr::ConvertToObjectType( strType );

    ADN_Objects_Data::T_ObjectsInfos_Vector& objects = ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos();
    ADN_Objects_Data::IT_ObjectsInfos_Vector itObject = std::find_if( objects.begin(), objects.end(), ADN_Objects_Data::ObjectInfos::Cmp( nType ) );

    if( itObject == objects.end() )
        throw ADN_Xml_Exception( input.GetContext(), "Type d'objet invalide" );
    ptrObject_ = *itObject;
    strName_ = ptrObject_.GetData()->strName_.GetData();

    input.ReadField( "DD", rDistanceDetection_ );

    input.Section( "ModificateursDeDistance" );
    input.Section( "PosturesSource" );
    for( IT_ModificatorPostureInfos_Vector it = vModifStance_.begin(); it != vModifStance_.end(); ++it )
        (*it)->ReadArchive( input );
    input.EndSection(); // PosturesSource
    input.EndSection(); // ModificateursDeDistance

    input.EndSection(); // Objet
}


// -----------------------------------------------------------------------------
// Name: TargetInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::TargetInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Objet" );

    output.WriteAttribute( "type", ptrObject_.GetData()->strName_.GetData() );

    output.WriteField( "DD", rDistanceDetection_.GetData() );

    output.Section( "ModificateursDeDistance" );
    output.Section( "PosturesSource" );
    for( IT_ModificatorPostureInfos_Vector it = vModifStance_.begin(); it != vModifStance_.end(); ++it )
        (*it)->WriteArchive( output );
    output.EndSection(); // PosturesSource
    output.EndSection(); // ModificateursDeDistance

    output.EndSection(); // Objet
}


//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Sensors_Data::SensorInfos::SensorInfos()
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, strName_()
, bCanDetectAgents_(false)
, bCanDetectObjects_(false)
, bCanScan_( false )
, rAngle_(0)
, rDistProximity_(0)
, rDistDetection_(0)
, rDistReco_(0)
, rDistIdent_(0)
, vModifSizes_(ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos())
, vModifIlluminations_( false )
, vModifWeather_( false )
, vModifEnvironments_( false )
, vModifStance_( false )
, vModifTargetStance_( false )
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

    bCanDetectAgents_.SetDataName( "la capacité de détecter des unités" );
    bCanDetectAgents_.SetParentNode( *this );

    bCanDetectObjects_.SetDataName( "la capacité de détecter des objets" );
    bCanDetectObjects_.SetParentNode( *this );

    vTargets_.SetParentNode( *this );
    vTargets_.SetItemTypeName( "une cible objet" );

    vModifIlluminations_.SetParentNode( *this );
    vModifWeather_.SetParentNode( *this );
    vModifEnvironments_.SetParentNode( *this );
    vModifStance_.SetParentNode( *this );
    vModifTargetStance_.SetParentNode( *this );
    vModifSizes_.SetParentNode( *this );

    // initialize illumination modificator infos
    uint i=0;
    for( i= 0 ; i< eNbrTimeCategory ; ++i)
    {
        ModificatorIlluminationInfos* pInfo = new ModificatorIlluminationInfos((E_TimeCategory)i);
        vModifIlluminations_.AddItem( pInfo );
    }

    // initialize meteo modificator infos
    for( i= 0 ; i< eNbrSensorWeatherModifiers ; ++i)
    {
        ModificatorMeteoInfos* pInfo = new ModificatorMeteoInfos((E_SensorWeatherModifiers)i);
        vModifWeather_.AddItem( pInfo );
    }

    // initialize environment modificator infos
    for( i= 0 ; i< eNbrVisionObjects ; ++i)
    {
        ModificatorEnvironmentInfos* pInfo = new ModificatorEnvironmentInfos((E_VisionObject)i);
        vModifEnvironments_.AddItem( pInfo );
    }

    // initialize posture modificator infos
    for( i=0 ; i< eNbrUnitPosture ; ++i)
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
    vModifIlluminations_.Reset();
    vModifWeather_.Reset();
    vModifEnvironments_.Reset();
    vModifStance_.Reset();
    vModifTargetStance_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::SensorInfos::GetNodeName()
{
    std::string strResult( "du senseur " );
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
// $$$$$$$$$
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Sensors_Data::SensorInfos* ADN_Sensors_Data::SensorInfos::CreateCopy()
{
    ADN_Sensors_Data::SensorInfos* pCopy = new ADN_Sensors_Data::SensorInfos();
    pCopy->rDistProximity_ = rDistProximity_.GetData();
    pCopy->rDistDetection_ = rDistDetection_.GetData();
    pCopy->rDistReco_ = rDistReco_.GetData();
    pCopy->rDistIdent_ = rDistIdent_.GetData();
    pCopy->rAngle_ = rAngle_.GetData();
    pCopy->bCanScan_ = bCanScan_.GetData();
    pCopy->bCanDetectAgents_ = bCanDetectAgents_.GetData();
    pCopy->bCanDetectObjects_ = bCanDetectObjects_.GetData();


    for( T_TargetsInfos_Vector::iterator itTarget = vTargets_.begin(); itTarget != vTargets_.end(); ++itTarget )
    {
        TargetInfos* pNewInfo = (*itTarget)->CreateCopy();
        pCopy->vTargets_.AddItem( pNewInfo );
    }

    uint i;
    for( i= 0 ; i< eNbrTimeCategory ; ++i)
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

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: SensorInfos::ReadArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Senseur" );
    input.ReadAttribute( "nom", strName_ );

    if( input.Section( "DetectionAgents", ADN_XmlInput_Helper::eNothing ) )
    {
        bCanDetectAgents_ = true;

        input.Section( "Angle" );

        input.ReadAttribute( "balayage", bCanScan_ );

        std::string strUnit( "degre" );
        input.ReadAttribute( "unite", strUnit, ADN_XmlInput_Helper::eNothing );

        int nAngle = 0;
        input.Read( nAngle );

        if( strUnit == "degre" )
        {
            if( nAngle < 0 || nAngle > 360 )
                throw ADN_DataException( "Mauvaises données lors du chargement d'un senseur",
                MT_FormatString( "Le senseur %s a un angle d'ouverture hors de la plage de valeur autorisée.", strName_.GetData().c_str() ).c_str(),
                "Assurez vous que l'angle du senseur soit entre 0 et 360." );
            rAngle_ = nAngle;
        }
        else if( strUnit == "radian" )
        {
            if( nAngle < 0 || nAngle > 2 * MT_PI )
                throw ADN_DataException( "Mauvaises données lors du chargement d'un senseur",
                MT_FormatString( "Le senseur %s a un angle d'ouverture hors de la plage de valeur autorisée.", strName_.GetData().c_str() ).c_str(),
                "Assurez vous que l'angle du senseur soit entre 0 et 2 PI." );
            rAngle_ = (int)(((double)nAngle)*180/MT_PI);
        }
        else
            throw ADN_DataException( "Mauvaises données lors du chargement d'un senseur",
            MT_FormatString( "Unitée inconnue pour l'angle du senseur %s.", strName_.GetData().c_str() ).c_str(),
            "Utilisez soit \"degre\" soit \"radian\"." );
        input.EndSection(); // Angle

        input.ReadField( "DistanceDeProximite", rDistProximity_ );

        input.Section( "DistancesDeBase" );
        input.ReadField( "DD", rDistDetection_ );
        input.ReadField( "DR", rDistReco_ );
        input.ReadField( "DI", rDistIdent_ );
        input.EndSection(); // DistancesDeBase

        input.Section( "ModificateursDeDistance" );

        input.Section( "VolumesCible" );
        for( IT_ModificatorSizeInfos_Vector it1 = vModifSizes_.begin(); it1 != vModifSizes_.end(); ++it1 )
            (*it1)->ReadArchive( input );
        input.EndSection(); // VolumesCible

        input.Section( "Precipitations" );
        for( IT_ModificatorMeteoInfos_Vector it2 = vModifWeather_.begin(); it2 != vModifWeather_.end(); ++it2 )
            (*it2)->ReadArchive( input );
        input.EndSection(); // Precipitations

        input.Section( "Eclairements" );
        for( IT_ModificatorIlluminationInfos_Vector it3 = vModifIlluminations_.begin(); it3 != vModifIlluminations_.end(); ++it3 )
            (*it3)->ReadArchive( input );
        input.EndSection(); // Eclairements

        input.Section( "PosturesSource" );
        for( IT_ModificatorPostureInfos_Vector it4 = vModifStance_.begin(); it4 != vModifStance_.end(); ++it4 )
            (*it4)->ReadArchive( input );
        input.EndSection(); // PosturesSource

        input.Section( "PosturesCible" );
        for( IT_ModificatorPostureInfos_Vector it5 = vModifTargetStance_.begin(); it5 != vModifTargetStance_.end(); ++it5 )
            (*it5)->ReadArchive( input );
        input.EndSection(); // PosturesCible

        input.Section( "Environnements" );
        for( IT_ModificatorEnvironmentInfos_Vector it6 = vModifEnvironments_.begin(); it6 != vModifEnvironments_.end(); ++it6 )
            (*it6)->ReadArchive( input );
        input.EndSection(); // Environnements

        input.EndSection(); // ModificateursDeDistance
        input.EndSection(); // DetectionAgents
    }

    if( input.Section( "DetectionObjets", ADN_XmlInput_Helper::eNothing ) )
    {
        bCanDetectObjects_ = true;
        input.BeginList( "Objets" );
        while( input.NextListElement() )
        {
            std::auto_ptr<TargetInfos> spNew( new TargetInfos() );
            spNew->ReadArchive( input );
            vTargets_.AddItem( spNew.release() );
        }
        input.EndList(); // Objets
        input.EndSection(); // DetectionObjets
    }

    input.EndSection(); // Senseur
}


// -----------------------------------------------------------------------------
// Name: SensorInfos::WriteArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Senseur" );
    output.WriteAttribute( "nom", strName_.GetData() );

    if( bCanDetectAgents_.GetData() )
    {
        output.Section( "DetectionAgents" );

        output.Section( "Angle" );
        output.WriteAttribute( "balayage", bCanScan_.GetData() );
        output.WriteAttribute( "unite", "degre" );
        output << rAngle_.GetData();
        output.EndSection(); // Angle

        output.WriteField( "DistanceDeProximite", rDistProximity_.GetData() );

        output.Section( "DistancesDeBase" );
        output.WriteField( "DD", rDistDetection_.GetData() );
        output.WriteField( "DR", rDistReco_.GetData() );
        output.WriteField( "DI", rDistIdent_.GetData() );
        output.EndSection(); // DistancesDeBase

        output.Section( "ModificateursDeDistance" );

        output.Section( "VolumesCible" );
        for( IT_ModificatorSizeInfos_Vector it1 = vModifSizes_.begin(); it1 != vModifSizes_.end(); ++it1 )
            (*it1)->WriteArchive( output );
        output.EndSection(); // VolumesCible

        output.Section( "Precipitations" );
        for( IT_ModificatorMeteoInfos_Vector it2 = vModifWeather_.begin(); it2 != vModifWeather_.end(); ++it2 )
            (*it2)->WriteArchive( output );
        output.EndSection(); // Precipitations

        output.Section( "Eclairements" );
        for( IT_ModificatorIlluminationInfos_Vector it3 = vModifIlluminations_.begin(); it3 != vModifIlluminations_.end(); ++it3 )
            (*it3)->WriteArchive( output );
        output.EndSection(); // Eclairements

        output.Section( "PosturesSource" );
        for( IT_ModificatorPostureInfos_Vector it4 = vModifStance_.begin(); it4 != vModifStance_.end(); ++it4 )
            (*it4)->WriteArchive( output );
        output.EndSection(); // PosturesSource

        output.Section( "PosturesCible" );
        for( IT_ModificatorPostureInfos_Vector it5 = vModifTargetStance_.begin(); it5 != vModifTargetStance_.end(); ++it5 )
            (*it5)->WriteArchive( output );
        output.EndSection(); // PosturesCible

        output.Section( "Environnements" );
        for( IT_ModificatorEnvironmentInfos_Vector it6 = vModifEnvironments_.begin(); it6 != vModifEnvironments_.end(); ++it6 )
            (*it6)->WriteArchive( output );
        output.EndSection(); // Environnements

        output.EndSection(); // ModificateursDeDistance
        output.EndSection(); // DetectionAgents
    }

    if( bCanDetectObjects_.GetData() )
    {
        output.Section( "DetectionObjets" );

        output.BeginList( "Objets", vTargets_.size() );
        for( IT_TargetsInfos_Vector it = vTargets_.begin(); it != vTargets_.end(); ++it )
        {
            (*it)->WriteArchive( output );
        }
        output.EndList(); // Objets
        output.EndSection(); // DetectionObjets
    }

    output.EndSection(); // Senseur

}


// -----------------------------------------------------------------------------
// Name: ALATInfos::ALATInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::ALATInfos::ALATInfos()
{
    for( int n = 1; n < eNbrVisionObjects; ++n )
        surveyTimes_[n-1] = 0.0;
}


// -----------------------------------------------------------------------------
// Name: ALATInfos::~ALATInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::ALATInfos::~ALATInfos()
{
}


// -----------------------------------------------------------------------------
// Name: ALATInfos::ReadArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ALATInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "SurveillanceALAT" );
    input.Section( "TempsSurveillance" );

    for( int n = 1; n < eNbrVisionObjects; ++n )
        input.ReadTimeField( ADN_Tr::ConvertFromVisionObject( E_VisionObject(n) ), surveyTimes_[n-1] );

    input.EndSection(); // TempsSurveillance
    input.EndSection(); // SurveillanceALAT
}


// -----------------------------------------------------------------------------
// Name: ALATInfos::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ALATInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "SurveillanceALAT" );
    output.Section( "TempsSurveillance" );

    for( int n = 1; n < eNbrVisionObjects; ++n )
        output.WriteField( ADN_Tr::ConvertFromVisionObject( E_VisionObject(n) ), ADN_Tools::SecondToString( (surveyTimes_[n-1]).GetData() ) );

    output.EndSection(); // TempsSurveillance
    output.EndSection(); // SurveillanceALAT
}


// =============================================================================
// 
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data constructor
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ADN_Sensors_Data()
: ADN_Data_ABC      ()
, radarData_        ( *new ADN_Radars_Data() )
{
    vSensors_.SetItemTypeName( "le senseur" );
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
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szSensors_.GetData() );
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
// Name: ADN_Sensors_Data::ReadArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Capteurs" );

    alatInfos_.ReadArchive( input );

    radarData_.ReadArchive( input );

    input.BeginList( "Senseurs" );
    while( input.NextListElement() )
    {
        std::auto_ptr<SensorInfos> spNew( new SensorInfos() );
        spNew->ReadArchive( input );
        vSensors_.AddItem( spNew.release() );
    }
    input.EndList(); // Senseurs
    input.EndSection(); // Capteurs
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::WriteArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Capteurs" );

    alatInfos_.WriteArchive( output );

    output.Section( "RadarsASS" );
    output.Section( "RadarCOBRA" );
    output.EndSection(); // RadarCOBRA
    output.EndSection(); // RadarsASS

    radarData_.WriteArchive( output );

    output.BeginList( "Senseurs", vSensors_.size() );
    for( IT_SensorsInfos_Vector it = vSensors_.begin(); it != vSensors_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }
    output.EndList(); // Senseurs
    output.EndSection(); // Capteurs
}
