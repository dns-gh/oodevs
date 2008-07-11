//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Objects_Data.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 16:06 $
// $Revision: 14 $
// $Workfile: ADN_Objects_Data.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Objects_Data.h"

#include "ADN_Tools.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_SaveFile_Exception.h"

#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::ScoreLocationInfos
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
ADN_Objects_Data::ScoreLocationInfos::ScoreLocationInfos()
: ADN_Ref_ABC           ()
, ADN_DataTreeNode_ABC  ()
, nLocation_            ( (E_Location)0 )
, nScore_               ( 0 )
{
    nScore_.SetDataName( "le poids de placement" );
    nScore_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::GetNodeName
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data::ScoreLocationInfos::GetNodeName()
{
    std::string strResult( "sur un terrain de type " );
    return strResult + ADN_Tr::ConvertFromLocation( nLocation_.GetData(), ADN_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::GetItemName
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data::ScoreLocationInfos::GetItemName()
{
    return ADN_Tr::ConvertFromLocation( nLocation_.GetData(), ADN_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::ReadArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ScoreLocationInfos::ReadArchive( xml::xistream& input )
{
    std::string strTerrain;
    input >> xml::attribute( "terrain", strTerrain )
          >> xml::attribute( "value", nScore_ );
    nLocation_ = ADN_Tr::ConvertToLocation( strTerrain );
}

// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ScoreLocationInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "sensible-position" )
             << xml::attribute( "terrain", ADN_Tr::ConvertFromLocation( nLocation_.GetData() ) )
             << xml::attribute( "value", nScore_ )
           << xml::end();
}

// =============================================================================
// PopulationAttritionInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::PopulationAttritionInfos::PopulationAttritionInfos
// Created: SBO 2006-04-25
// -----------------------------------------------------------------------------
ADN_Objects_Data::PopulationAttritionInfos::PopulationAttritionInfos()
    : rSurface_( 0. )
    , rPh_ ( 0. )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::PopulationAttritionInfos::~PopulationAttritionInfos
// Created: SBO 2006-04-25
// -----------------------------------------------------------------------------
ADN_Objects_Data::PopulationAttritionInfos::~PopulationAttritionInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::PopulationAttritionInfos::ReadArchive
// Created: SBO 2006-04-25
// -----------------------------------------------------------------------------
void ADN_Objects_Data::PopulationAttritionInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::optional() >> xml::start( "population-attrition" )
                >> xml::attribute( "surface", rSurface_ )
                >> xml::attribute( "ph", rPh_ )
            >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::PopulationAttritionInfos::WriteArchive
// Created: SBO 2006-04-25
// -----------------------------------------------------------------------------
void ADN_Objects_Data::PopulationAttritionInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "population-attrition" )
                << xml::attribute( "surface", rSurface_ )
                << xml::attribute( "ph", rPh_ )
            << xml::end();
}

// =============================================================================
// ObjectInfos
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ObjectInfos::ObjectInfos
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Objects_Data::ObjectInfos::ObjectInfos( E_ObjectType nType )
    : ADN_Ref_ABC       ()
    , nObjectType_      ( nType )
    , strName_          ( nType == (E_ObjectType)-1 ? "" : ENT_Tr::ConvertFromObjectType( nType, ENT_Tr::eToTr ) )
    , bDangerous_       ( false )
    , bCanBeValorized_  ( false )
    , bCanBeReservedObstacle_( false )
    , bCanBeBypassed_   ( false )
    , rAvoidDistance_   ( 0 )
    , rDefaultSpeed_    ( -1 )
    , rDefaultBypassSpeed_( -1 )
    , rMaxInteractionHeight_( 0 )
    , nMaxNbrUsers_     ( 0 )
    , bAttritions_      ( false )
    , attritions_       ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos())
    , bToReinforce_     ( false )
    , bToBuild_         ( false )
    , nNbrToBuild_      ( 0 )
    , nNbrToReinforce_  ( 0 )
    , rMaxAgentSpeedPercentage_( 0 )
    , rOutgoingPopulationDensity_( 0 )
    , bHasOutgoingPopulationDensity_( false )
    , populationAttrition_()
    , bPopulationAttrition_( false )
{
    rAvoidDistance_.SetDataName( "la distance d'évitement" );
    rAvoidDistance_.SetParentNode( *this );
    bDangerous_.SetDataName( "l'objet est dangereux" ); 
    bDangerous_.SetParentNode( *this );
    bCanBeValorized_.SetDataName( "la capacité d'être valorisé" );
    bCanBeValorized_.SetParentNode( *this );
    bCanBeReservedObstacle_.SetDataName( "la capacité d'être un obstacle de manoeuvre" );
    bCanBeReservedObstacle_.SetParentNode( *this );
    bCanBeBypassed_.SetDataName( "la capacité d'être contourné" );
    bCanBeBypassed_.SetParentNode( *this );
    rDefaultSpeed_.SetDataName( "la vitesse par défaut" );
    rDefaultSpeed_.SetParentNode( *this );
    rDefaultBypassSpeed_.SetDataName( "la vitesse par défaut quand contourné" );
    rDefaultBypassSpeed_.SetParentNode( *this );
    rMaxInteractionHeight_.SetDataName( "la hauteur maximale d'interaction" );
    rMaxInteractionHeight_.SetParentNode( *this );
    rOutgoingPopulationDensity_.SetDataName( "le densité des populations sortantes" );
    rOutgoingPopulationDensity_.SetParentNode( *this );

    vScoreLocation_.SetParentNode( *this );
    vScoreLocation_.SetItemTypeName( "un poids de placement sur un terrain de type" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos::~ObjectInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Objects_Data::ObjectInfos::~ObjectInfos()
{
    vScoreLocation_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ObjectInfos::GetNodeName
// Created: AGN 2004-05-18
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data::ObjectInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ObjectInfos::GetNodeName
// Created: AGN 2004-05-14
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data::ObjectInfos::GetNodeName()
{
    return std::string( "de l'objet " ) + strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos::ReadSensiblePosition
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadSensiblePosition( xml::xistream& input )
{
    std::auto_ptr<ScoreLocationInfos> spNew( new ScoreLocationInfos() );
    spNew->ReadArchive( input );
    vScoreLocation_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos::ReadDotation
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadDotation( const std::string& type, xml::xistream& input )
{
    std::string dotation, category;
    unsigned count;
    input >> xml::attribute( "dotation", dotation )
          >> xml::attribute( "category", category )
          >> xml::attribute( "count", count );
    ADN_Equipement_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( category, dotation );
    if( pCategory == 0 )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Objects - Object '%1' - Invalid resource '%2'" ).arg( strName_.GetData().c_str(), category.c_str() ).ascii() );
    if( type == "construction" )
    {
        ptrToBuild_ = pCategory;
        bToBuild_ = true;
        nNbrToBuild_ = count;
    }
    else if( type == "valorization" )
    {
        ptrToReinforce_ = pCategory;
        bToReinforce_ = true;
        nNbrToReinforce_ = count;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos::ReadUnitAttrition
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadUnitAttrition( xml::xistream& input )
{
    bAttritions_ = true;
    std::string protection;
    input >> xml::attribute( "protection", protection );
    IT_AttritionInfosVector itAttrition = std::find_if( attritions_.begin(), attritions_.end(), AttritionInfos::Cmp(protection));
    if( itAttrition != attritions_.end() )
        (*itAttrition)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ObjectInfos::ReadArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadArchive( xml::xistream& input )
{
    std::string strComsuption;
    input >> xml::attribute( "default-consumption-mode", strComsuption );
    E_ConsumptionType nConsumption = ADN_Tr::ConvertToConsumptionType( strComsuption );
    nDefaultConsumption_ = nConsumption;

    input >> xml::attribute( "dangerous", bDangerous_ )
          >> xml::attribute( "can-be-maneuver-obstacle", bCanBeReservedObstacle_ )
          >> xml::attribute( "can-be-developed", bCanBeValorized_ )
          >> xml::attribute( "can-be-bypassed", bCanBeBypassed_ )
          >> xml::optional() >> xml::attribute( "avoid-distance", rAvoidDistance_ )
          >> xml::attribute( "default-speed", rDefaultSpeed_ )
          >> xml::attribute( "default-bypassed-speed", rDefaultBypassSpeed_ )
          >> xml::attribute( "max-interaction-height", rMaxInteractionHeight_ )
          >> xml::optional() >> xml::attribute( "population-density", rOutgoingPopulationDensity_ )
          >> xml::optional() >> xml::attribute( "max-animating-units", nMaxNbrUsers_ )
          >> xml::optional() >> xml::attribute( "geometry", geometries_ );


    if( rDefaultSpeed_.GetData() < -1 )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Objects - Object '%1' - Default speed must be >= 0" ).arg( strName_.GetData().c_str() ).ascii() );
    if( rDefaultBypassSpeed_.GetData() < -1 )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Objects - Object '%1' - Default speed when bypassed >= 0" ).arg( strName_.GetData().c_str() ).ascii() );
    if( rMaxInteractionHeight_.GetData() < 0 )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Objects - Object '%1' - Max interaction height must be >= 0" ).arg( strName_.GetData().c_str() ).ascii() );
    if( rOutgoingPopulationDensity_.GetData() < 0 )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Objects - Object '%1' - Outgoing population density must be >= 0" ).arg( strName_.GetData().c_str() ).ascii() );
     if( nMaxNbrUsers_.GetData() < -1 )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Objects - Object '%1' - Number of users must be > 0" ).arg( strName_.GetData().c_str() ).ascii() );
    bHasOutgoingPopulationDensity_ = rOutgoingPopulationDensity_.GetData() != 0;

    std::string impact = xml::attribute< std::string >( input, "unit-speed-impact-mode" );
    nSpeedImpact_ = ADN_Tr::ConvertToSpeedImpact( impact );
    if( nSpeedImpact_ == eSpeedImpact_VitesseMaxAgent )
        input >> xml::attribute( "max-unit-percentage-speed", rMaxAgentSpeedPercentage_ );

    input >> xml::optional() 
            >> xml::start( "sensible-positions" )
                >> xml::list( "sensible-position", *this, &ADN_Objects_Data::ObjectInfos::ReadSensiblePosition )
            >> xml::end();

    input >> xml::optional()
          >> xml::start( "dotations" )
            >> xml::list( *this, &ADN_Objects_Data::ObjectInfos::ReadDotation )
          >> xml::end();

    input >> xml::optional()
            >> xml::start( "unit-attritions" )
                >> xml::list( "unit-attrition", *this, &ADN_Objects_Data::ObjectInfos::ReadUnitAttrition )
            >> xml::end();

    populationAttrition_.ReadArchive( input );
    bPopulationAttrition_ = populationAttrition_.rPh_ != 0 || populationAttrition_.rSurface_ != 0;
}


// -----------------------------------------------------------------------------
// Name: ObjectInfos::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::WriteArchive( xml::xostream& output )
{
    if( nObjectType_ == eObjectType_SiteDecontamination && nMaxNbrUsers_.GetData() <= 0 )
        throw ADN_DataException( "Invalid data", tr( "Objects - Invalid number of 'users'" ).ascii() );

    output << xml::start( "object" )
            << xml::attribute( "type", ENT_Tr::ConvertFromObjectType( nObjectType_.GetData() ) )
            << xml::attribute( "default-consumption-mode", ADN_Tr::ConvertFromConsumptionType( nDefaultConsumption_.GetData() ) )
            << xml::attribute( "dangerous", bDangerous_ )
            << xml::attribute( "can-be-maneuver-obstacle", bCanBeReservedObstacle_ )
            << xml::attribute( "can-be-developed", bCanBeValorized_.GetData() || bToReinforce_.GetData() )
            << xml::attribute( "can-be-bypassed", bCanBeBypassed_ );
    if( rAvoidDistance_.GetData() != 0 )
        output << xml::attribute( "avoid-distance", rAvoidDistance_ );
    output  << xml::attribute( "default-speed", rDefaultSpeed_ )
            << xml::attribute( "default-bypassed-speed", rDefaultBypassSpeed_ )
            << xml::attribute( "max-interaction-height", rMaxInteractionHeight_ );
    if( !geometries_.GetData().empty() )
        output << xml::attribute( "geometry", geometries_.GetData() );
            
    if( bHasOutgoingPopulationDensity_.GetData() )
        output << xml::attribute( "population-density", rOutgoingPopulationDensity_ );
    if( nMaxNbrUsers_.GetData() != -1 )
        output << xml::attribute( "max-animating-units", nMaxNbrUsers_ );

    output << xml::attribute( "unit-speed-impact-mode", ADN_Tr::ConvertFromSpeedImpact( nSpeedImpact_.GetData() ) );
    if( nSpeedImpact_.GetData() == eSpeedImpact_VitesseMaxAgent )
        output << xml::attribute( "max-unit-percentage-speed", rMaxAgentSpeedPercentage_ );

    if( ! vScoreLocation_.empty() )
    {
        output << xml::start( "sensible-positions" );
        for( T_ScoreLocationInfosVector::iterator itScore = vScoreLocation_.begin(); itScore != vScoreLocation_.end(); ++itScore )
            (*itScore)->WriteArchive( output );
        output << xml::end();
    }

    if( bToBuild_.GetData() || bToReinforce_.GetData() )
    {
        output << xml::start( "dotations" );
        if( bToBuild_.GetData() == true )
        {
            if( ptrToBuild_.GetData() == 0 )
                throw ADN_DataException( "Invalid data", tr( "Objects - Invalid resource category for construction" ).ascii() );
            output << xml::start( "construction" )
                    << xml::attribute( "category", ptrToBuild_.GetData()->parentDotation_.strName_ )
                    << xml::attribute( "dotation", ptrToBuild_.GetData()->strName_ )
                    << xml::attribute( "count", nNbrToBuild_ )
                   << xml::end();
        }
        if( bToReinforce_.GetData() == true )
        {
            if( ptrToReinforce_.GetData() == 0 )
                throw ADN_DataException( "Invalid data", tr( "Objects - Invalid resource category for mining" ).ascii() );
            output << xml::start( "valorization" )
                    << xml::attribute( "category", ptrToReinforce_.GetData()->parentDotation_.strName_ )
                    << xml::attribute( "dotation", ptrToReinforce_.GetData()->strName_ )
                    << xml::attribute( "count", nNbrToReinforce_ )
                   << xml::end();
        }
        output << xml::end();
    }

    if( bAttritions_.GetData() )
    {
        output << xml::start( "unit-attritions" );
        for( IT_AttritionInfosVector it = attritions_.begin(); it != attritions_.end(); ++it )
            (*it)->WriteArchive( output, "unit-attrition" );
        output << xml::end();
    }

    if( bPopulationAttrition_.GetData() )
        populationAttrition_.WriteArchive( output );

    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: AreaControlInformations::AreaControlInformations
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
ADN_Objects_Data::AreaControlInformations::AreaControlInformations()
: ADN_Ref_ABC           ()
, ADN_DataTreeNode_ABC  ()
, ptrSize_              ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos(), 0 )
, nPercentage_          ( 0 )
{
    BindExistenceTo( &ptrSize_ );
}

// -----------------------------------------------------------------------------
// Name: AreaControlInformations::~AreaControlInformations
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
ADN_Objects_Data::AreaControlInformations::~AreaControlInformations()
{
}

// -----------------------------------------------------------------------------
// Name: AreaControlInformations::GetNodeName
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data::AreaControlInformations::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: AreaControlInformations::GetItemName
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data::AreaControlInformations::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: AreaControlInformations::ReadArchive
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
void ADN_Objects_Data::AreaControlInformations::ReadArchive( xml::xistream& input )
{
    std::string strSize;
    input >> xml::attribute( "percentage", nPercentage_ )
          >> xml::attribute( "volume", strSize );
    ADN_Categories_Data::SizeInfos* pSize = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindSize( strSize );
    ptrSize_ = pSize;
}


// -----------------------------------------------------------------------------
// Name: AreaControlInformations::WriteArchive
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
void ADN_Objects_Data::AreaControlInformations::WriteArchive( xml::xostream& output )
{
    output << xml::start( "shot-percentage-per-human-per-hectare" )
            << xml::attribute( "percentage", nPercentage_ )
            << xml::attribute( "volume", ptrSize_.GetData()->GetData() )
           << xml::end();
}

//-----------------------------------------------------------------------------
// Name: ADN_Objects_Data constructor
// Created: JDY 03-06-25
//-----------------------------------------------------------------------------
ADN_Objects_Data::ADN_Objects_Data()
: ADN_Data_ABC()
{
    for( int i = 0; i < eNbrObjectType; ++i )
        vObjectInfos_.AddItem( new ObjectInfos( (E_ObjectType)i ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_Objects_Data destructor
// Created: JDY 03-06-25
//-----------------------------------------------------------------------------
ADN_Objects_Data::~ADN_Objects_Data()
{
    vObjectInfos_.Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Objects_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Objects_Data::FilesNeeded(T_StringList& files) const
{
    files.push_back(ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szObjects_.GetData());
}

//-----------------------------------------------------------------------------
// Name: ADN_Objects_Data::Reset
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
void ADN_Objects_Data::Reset()
{
    vObjectInfos_.Reset();

    for( int i = 0; i < eNbrObjectType; ++i )
        vObjectInfos_.AddItem( new ObjectInfos( (E_ObjectType)i ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadObject
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ReadObject( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "type", type );
    E_ObjectType nObjectType = ENT_Tr::ConvertToObjectType( type );
    if( nObjectType == (E_ObjectType)-1)
        throw ADN_DataException( "Invalid data", tr( "Objects - Invalid object type '%1'" ).arg( type.c_str() ).ascii() );

    ObjectInfos* pObjInfo = vObjectInfos_[ (int)nObjectType ];
    //pObjInfo->strName_ = ENT_Tr::ConvertFromObjectType( nObjectType, ENT_Tr::eToTr ); //$$$$ C LA QUE C POURRI
    pObjInfo->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadShotPercentage
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ReadShotPercentage( xml::xistream& input )
{
    std::auto_ptr<AreaControlInformations> spNew( new AreaControlInformations() );
    spNew->ReadArchive( input );
    vAreaControlInformations_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "objects" )
            >> xml::start( "real-objects" )
                >> xml::list( "object", *this, &ADN_Objects_Data::ReadObject )
            >> xml::end()
            >> xml::start( "virtual-objects" )
                >> xml::start( "object" )
                    >> xml::list( "shot-percentage-per-human-per-hectare", *this, &ADN_Objects_Data::ReadShotPercentage )
                >> xml::end()
            >> xml::end()
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "objects" )
            << xml::start( "real-objects" );
    for( IT_ObjectsInfos_Vector it = vObjectInfos_.begin(); it!= vObjectInfos_.end(); ++it)
        (*it)->WriteArchive( output );
    output  << xml::end()
            << xml::start( "virtual-objects" )
                << xml::start( "object" )
                << xml::attribute( "type", "controle de zone" );
    for( IT_AreaControlInformations_Vector it = vAreaControlInformations_.begin(); it != vAreaControlInformations_.end(); ++it )
        (*it)->WriteArchive( output );
    output      << xml::end()
            << xml::end()
           << xml::end();
}


