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
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
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
void ADN_Objects_Data::ScoreLocationInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Terrain" );

    std::string strTerrain;
    input.ReadAttribute( "type", strTerrain );

    nLocation_ = ADN_Tr::ConvertToLocation( strTerrain );
//    if( nTypeTerrain == (E_TypeTerrain)-1 )
//    throw ADN_DataException( "Données invalides",
//    MT_FormatString( "L'object %s définit un poids de placement sur un terrain %s non valide.", szTmp.c_str(), strTerrain.c_str() ).c_str(),
//    MT_FormatString( "Modifiez la liste PlacementScores de l'objet %s", szTmp.c_str() ).c_str() );

    int nScore;
    input.Read( nScore );
    nScore_ = nScore;

//    if( nScore_.GetData() < 0 )
//    throw ADN_DataException( "Données invalides",
//    MT_FormatString( "Le poids de placement sur un terrain de type %s de l'objet %s est négatif.", szTerrain.c_str(), szTmp.c_str() ).c_str(),
//    MT_FormatString( "Editer le fichier objetsTypes.xml pour modifier le poids de placement du terrain %s pour l'objet %s", szTerrain.c_str(), szTmp.c_str() ).c_str() );
//
    input.EndSection(); // Terrain
}


// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ScoreLocationInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Terrain" );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromLocation( nLocation_.GetData() ) );
    output << nScore_.GetData();
    output.EndSection(); // Terrain
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
void ADN_Objects_Data::PopulationAttritionInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadField( "SurfaceAttrition", rSurface_ );
    input.ReadField( "PH", rPh_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::PopulationAttritionInfos::WriteArchive
// Created: SBO 2006-04-25
// -----------------------------------------------------------------------------
void ADN_Objects_Data::PopulationAttritionInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.WriteField( "SurfaceAttrition", rSurface_.GetData() );
    output.WriteField( "PH", rPh_.GetData() );
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
, bCanBeValorized_  ( false )
, bCanBePrepared_   ( false )
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
    bCanBeValorized_.SetDataName( "la capacité d'être valorisé" );
    bCanBeValorized_.SetParentNode( *this );
    bCanBePrepared_.SetDataName( "la capacité d'être préparé" );
    bCanBePrepared_.SetParentNode( *this );
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
// Name: ObjectInfos::ReadArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    std::string strComsuption;
    input.ReadField( "ModeConsommationParDefaut", strComsuption );

    E_ConsumptionType nConsumption = ADN_Tr::ConvertToConsumptionType( strComsuption );
    assert( nConsumption != -1 );
    nDefaultConsumption_ = nConsumption;

    input.ReadField( "PeutEtrePrepare" , bCanBePrepared_ );
    input.ReadField( "PeutEtreValorise", bCanBeValorized_ );
    input.ReadField( "PeutEtreContourne", bCanBeBypassed_ );
    input.ReadField( "DistanceEvitement", rAvoidDistance_, ADN_XmlInput_Helper::eNothing );

    input.ReadField( "VitesseParDefaut", rDefaultSpeed_ );
    if( rDefaultSpeed_.GetData() < -1 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "La vitesse par défaut de l'objet %s est négative.", strName_.GetData().c_str() ).c_str(),
        "Editer le fichier pour modifier le champ Objets::Type::VitesseParDefaut de cet objet." );

    input.ReadField( "VitesseParDefautQuandContourne", rDefaultBypassSpeed_ );
    if( rDefaultBypassSpeed_.GetData() < -1 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "La vitesse par défaut de l'objet %s, lorsqu'il est contourné, est négative.", strName_.GetData().c_str() ).c_str(),
        "Editer le fichier %s pour modifier le champ Objets::Type::VitesseParDefautQuandContourne de cet objet." );

    input.ReadField( "HauteurMaxInteraction", rMaxInteractionHeight_ );
    if( rMaxInteractionHeight_.GetData() < 0 )
        throw ADN_DataException( "Donnée invalide",
        MT_FormatString( "La hauteur maximale d'interaction de l'objet %s est négative.", strName_.GetData().c_str() ).c_str(),
        "Editer le fichier %s pour modifier le champ Objets::Type::HauteurMaxInteraction de cet objet." );

    if( input.ReadField( "DensitePopulationSortante", rOutgoingPopulationDensity_, ADN_XmlInput_Helper::eNothing ) )
    {
        if( rOutgoingPopulationDensity_.GetData() < 0 )
            throw ADN_DataException( "Donnée invalide",
            MT_FormatString( "La densité des populations sortantes de l'objet %s est négative.", strName_.GetData().c_str() ).c_str(),
            "Editer le fichier %s pour modifier le champ Objets::Type::DensitePopulationSortante de cet objet." );
        bHasOutgoingPopulationDensity_ = true;
    }

    input.Section( "ModeImpactSurVitessePion" );
    input.ReadAttribute( "type", nSpeedImpact_, ADN_Tr::ConvertToSpeedImpact, ADN_XmlInput_Helper::eThrow );
    if( nSpeedImpact_.GetData() == eSpeedImpact_VitesseMaxAgent )
        input.ReadAttribute( "pourcentageVitesseMaxAgent", rMaxAgentSpeedPercentage_ );
    input.EndSection(); // ModeImpactSurVitessePion

    // Location weigths
    if( input.BeginList( "PlacementScores", ADN_XmlInput_Helper::eNothing ) )
    {
        while( input.NextListElement() )
        {
            std::auto_ptr<ScoreLocationInfos> spNew( new ScoreLocationInfos() );
            spNew->ReadArchive( input );

//            T_ScoreLocationInfosVector::iterator found = std::find_if( vScoreLocation_.begin(), vScoreLocation_.end(), ScoreLocationInfos::Cmp( spNew->nLocation_ ) );
//            if( found != vScoreLocation_.end() )
//            throw ADN_DataException( "Données dupliquées",
//            MT_FormatString( "L'objet %s définit plusieurs fois le poids de placement sur un terrain %s", strName_.GetData().c_str(), spNew->strName_.GetData().c_str() ).c_str(),
//            MT_FormatString( "Editer le fichier objetsTypes.xml pour éliminer les doublons de la liste PlacementScores de l'objet %s", strName_.GetData().c_str() ).c_str() );

            vScoreLocation_.AddItem( spNew.release() );
        }
        input.EndList(); // PlacementScores
    }

    if( input.ReadField( "NombreAnimateursMax", nMaxNbrUsers_, ADN_XmlInput_Helper::eNothing ) )
        if( nMaxNbrUsers_.GetData() < -1 )
            throw ADN_DataException( "Donnée invalide",
            MT_FormatString( "La vitesse par défaut de l'objet %s est négative.", strName_.GetData().c_str() ).c_str(),
            "Editer le fichier pour modifier le champ Objets::Type::VitesseParDefaut de cet objet." );

    if( input.Section( "Dotations", ADN_XmlInput_Helper::eNothing ) )
    {
        if( input.Section( "Construction", ADN_XmlInput_Helper::eNothing ) )
        {
            std::string strType;
            std::string strCategory;
            uint        nValue;
            input.ReadAttribute( "type", strType );
            input.ReadAttribute( "categorie", strCategory );
            input.ReadAttribute( "valeur", nValue );

            ADN_Equipement_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( strType, strCategory );
            if( pCategory == 0 )
                throw ADN_DataException( "Donnée invalide",
                MT_FormatString( "La hauteur maximale d'interaction de l'objet %s est négative.", strName_.GetData().c_str() ).c_str(),
                "Editer le fichier %s pour modifier le champ Objets::Type::HauteurMaxInteraction de cet objet." );

            ptrToBuild_ = pCategory;
            bToBuild_ = true;
            nNbrToBuild_ = nValue;

            input.EndSection(); // Construction
        }

        if( input.Section( "Valorisation", ADN_XmlInput_Helper::eNothing ) )
        {
            std::string strType;
            std::string strCategory;
            uint        nValue;
            input.ReadAttribute( "type", strType );
            input.ReadAttribute( "categorie", strCategory );
            input.ReadAttribute( "valeur", nValue );

            ADN_Equipement_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( strType, strCategory );
            if( pCategory == 0 )
                throw ADN_DataException( "Donnée invalide",
                MT_FormatString( "La hauteur maximale d'interaction de l'objet %s est négative.", strName_.GetData().c_str() ).c_str(),
                "Editer le fichier %s pour modifier le champ Objets::Type::HauteurMaxInteraction de cet objet." );

            ptrToReinforce_ = pCategory;
            bToReinforce_ = true;
            nNbrToReinforce_ = nValue;

            input.EndSection(); // Valorisation
        }

        input.EndSection(); // Dotations
    }

    if( input.Section( "Attritions", ADN_XmlInput_Helper::eNothing ) )
    {
        bAttritions_ = true;

        ADN_Categories_Data::T_ArmorInfos_Vector& armorInfos = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos();
        for( ADN_Categories_Data::T_ArmorInfos_Vector::iterator it = armorInfos.begin(); it != armorInfos.end(); ++it )
        {
            IT_AttritionInfosVector itAttrition = std::find_if( attritions_.begin(), attritions_.end(),AttritionInfos::CmpRef(*it));
            assert( itAttrition != attritions_.end() );
            (*itAttrition)->ReadArchive( input );
        }

        input.EndSection(); // Attritions
    }

    // Population Attritions
    if( input.Section( "AttritionPopulation", ADN_XmlInput_Helper::eNothing ) )
    {
        bPopulationAttrition_ = true;
        populationAttrition_.ReadArchive( input );
        input.EndSection(); // AttritionPopulation
    }
}


// -----------------------------------------------------------------------------
// Name: ObjectInfos::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Objet" );

    output.WriteAttribute( "type", ENT_Tr::ConvertFromObjectType( nObjectType_.GetData() ) );

    output.WriteField( "ModeConsommationParDefaut", ADN_Tr::ConvertFromConsumptionType( nDefaultConsumption_.GetData() ) );
    output.WriteField( "PeutEtrePrepare" , bCanBePrepared_.GetData() );
    output.WriteField( "PeutEtreValorise", bCanBeValorized_.GetData() || bToReinforce_.GetData() );
    output.WriteField( "PeutEtreContourne", bCanBeBypassed_.GetData() );

    if( rAvoidDistance_.GetData() != 0 )
        output.WriteField( "DistanceEvitement", rAvoidDistance_.GetData() );

    output.WriteField( "VitesseParDefaut", rDefaultSpeed_.GetData() );
    output.WriteField( "VitesseParDefautQuandContourne", rDefaultBypassSpeed_.GetData() );
    output.WriteField( "HauteurMaxInteraction", rMaxInteractionHeight_.GetData() );

    if( bHasOutgoingPopulationDensity_ == true )
        output.WriteField( "DensitePopulationSortante", rOutgoingPopulationDensity_.GetData() );

    if( nObjectType_ == eObjectType_SiteDecontamination && nMaxNbrUsers_.GetData() <= 0 )
        throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Nbr max users for decontamination site <= 0 when it should be >= 1." ).ascii() );

    if( nMaxNbrUsers_.GetData() != -1 )
        output.WriteField( "NombreAnimateursMax", nMaxNbrUsers_.GetData() );

    output.Section( "ModeImpactSurVitessePion" );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromSpeedImpact( nSpeedImpact_.GetData() ) );
    if( nSpeedImpact_.GetData() == eSpeedImpact_VitesseMaxAgent )
        output.WriteAttribute( "pourcentageVitesseMaxAgent", rMaxAgentSpeedPercentage_.GetData() );
    output.EndSection(); // ModeImpactSurVitessePion

    if( ! vScoreLocation_.empty() )
    {
        output.Section( "PlacementScores" );
        for( T_ScoreLocationInfosVector::iterator itScore = vScoreLocation_.begin(); itScore != vScoreLocation_.end(); ++itScore )
        {
            (*itScore)->WriteArchive( output );
        }
        output.EndSection(); // PlacementScores
    }

    if( bToBuild_.GetData() || bToReinforce_.GetData() )
    {
        output.Section( "Dotations" );

        if( bToBuild_.GetData() == true )
        {
            if( ptrToBuild_.GetData() == 0 )
                throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Undefined category for construction in object %1." ).arg( ENT_Tr::ConvertFromObjectType( nObjectType_.GetData() ).c_str() ).ascii() );

            output.Section( "Construction" );
            output.WriteAttribute( "type", ptrToBuild_.GetData()->parentDotation_.strName_.GetData() );
            output.WriteAttribute( "categorie", ptrToBuild_.GetData()->strName_.GetData() );
            output.WriteAttribute( "valeur", nNbrToBuild_.GetData() );
            output.EndSection(); // Construction
        }

        if( bToReinforce_.GetData() == true )
        {
            if( ptrToReinforce_.GetData() == 0 )
                throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Undefined category for valorization in object %1." ).arg( ENT_Tr::ConvertFromObjectType( nObjectType_.GetData() ).c_str() ).ascii() );

            output.Section( "Valorisation" );
            output.WriteAttribute( "type", ptrToReinforce_.GetData()->parentDotation_.strName_.GetData() );
            output.WriteAttribute( "categorie", ptrToReinforce_.GetData()->strName_.GetData() );
            output.WriteAttribute( "valeur", nNbrToReinforce_.GetData() );
            output.EndSection(); // Valorisation
        }

        output.EndSection(); // Dotations
    }

    if( bAttritions_.GetData() == true )
    {
        output.Section( "Attritions" );
        ADN_Categories_Data::T_ArmorInfos_Vector& armorInfos = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos();
        for( ADN_Categories_Data::T_ArmorInfos_Vector::iterator it = armorInfos.begin(); it != armorInfos.end(); ++it )
        {
            IT_AttritionInfosVector itAttrition = std::find_if( attritions_.begin(), attritions_.end(),AttritionInfos::CmpRef(*it));
            assert( itAttrition != attritions_.end() );
            (*itAttrition)->WriteArchive( output );
        }

        output.EndSection(); // Attritions
    }

    if( bPopulationAttrition_.GetData() == true )
    {
        output.Section( "AttritionPopulation" );
        populationAttrition_.WriteArchive( output );
        output.EndSection(); // AttritionPopulation
    }

    output.EndSection(); // Objet
}


// -----------------------------------------------------------------------------
// Name: AreaControlInfos::AreaControlInfos
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
ADN_Objects_Data::AreaControlInfos::AreaControlInfos()
: ADN_Ref_ABC           ()
, ADN_DataTreeNode_ABC  ()
, ptrSize_              ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos(), 0 )
, nPercentage_          ( 0 )
{
    this->BindExistenceTo( &ptrSize_ );
}


// -----------------------------------------------------------------------------
// Name: AreaControlInfos::~AreaControlInfos
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
ADN_Objects_Data::AreaControlInfos::~AreaControlInfos()
{
}


// -----------------------------------------------------------------------------
// Name: AreaControlInfos::GetNodeName
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data::AreaControlInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: AreaControlInfos::GetItemName
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data::AreaControlInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: AreaControlInfos::ReadArchive
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
void ADN_Objects_Data::AreaControlInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Pourcentage" );
    std::string strSize;
    input.ReadAttribute( "volume", strSize );
    ADN_Categories_Data::SizeInfos* pSize = ADN_Workspace::GetWorkspace().GetCategories().GetData().FindSize( strSize );
    assert( pSize != 0 );
    ptrSize_ = pSize;

    int n;
    input.Read( n );
    nPercentage_ = n;
    input.EndSection(); // Pourcentage
}


// -----------------------------------------------------------------------------
// Name: AreaControlInfos::WriteArchive
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
void ADN_Objects_Data::AreaControlInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Pourcentage" );
    output.WriteAttribute( "volume", ptrSize_.GetData()->GetData() );
    output << nPercentage_.GetData();
    output.EndSection(); // Pourcentage
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
// Name: ADN_Objects_Data::ReadArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Objets" );
    input.BeginList( "ObjetsReels" );
    while( input.NextListElement() )
    {
        input.Section( "Objet" );

        std::string strTmp;
        input.ReadAttribute( "type", strTmp );

        E_ObjectType nObjectType = ENT_Tr::ConvertToObjectType( strTmp );
        if( nObjectType == (E_ObjectType)-1)
            input.ThrowError( MT_FormatString( "Le type d'objet %s n'est pas connu", strTmp.c_str() ).c_str() );

        ObjectInfos* pObjInfo = vObjectInfos_[ (int)nObjectType ];
        pObjInfo->strName_ = strTmp;
        pObjInfo->ReadArchive( input );

        input.EndSection(); // Objet
    }
    input.EndList(); // ObjetsReels

    input.Section( "ObjetsVirtuels" );
    input.Section( "Objet" );
    input.BeginList( "PourcentagesTirParHumainParHectare" );
    while( input.NextListElement() )
    {
        std::auto_ptr<AreaControlInfos> spNew( new AreaControlInfos() );
        spNew->ReadArchive( input );
        vAreaControlInfos_.AddItem( spNew.release() );
    }
    input.EndList(); // PourcentagesTirParHumainParHectare
    input.EndSection(); // Objet
    input.EndSection(); // ObjetsVirtuels

    input.EndSection(); // Objets
}


// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Objets" );

    output.Section( "ObjetsReels" );
    for( IT_ObjectsInfos_Vector it = vObjectInfos_.begin(); it!= vObjectInfos_.end(); ++it)
    {
        (*it)->WriteArchive( output );
    }
    output.EndSection(); // ObjetsReels

    output.Section( "ObjetsVirtuels" );
    output.Section( "Objet" );
    output.WriteAttribute( "type", "controle de zone" );
    output.BeginList( "PourcentagesTirParHumainParHectare", vAreaControlInfos_.size() );
    for( IT_AreaControlInfos_Vector it = vAreaControlInfos_.begin(); it != vAreaControlInfos_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }
    output.EndList(); // PourcentagesTirParHumainParHectare
    output.EndSection(); // Objet
    output.EndSection(); // ObjetsVirtuels

    output.EndSection(); // Objets
}


