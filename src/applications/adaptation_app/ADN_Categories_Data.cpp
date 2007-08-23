//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Categories_Data.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 16:12 $
// $Revision: 12 $
// $Workfile: ADN_Categories_Data.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Categories_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Tools.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tr.h"

// =============================================================================
// AttritionEffectOnHuman
// =============================================================================


// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::AttritionEffectOnHuman
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
ADN_Categories_Data::AttritionEffectOnHuman::AttritionEffectOnHuman()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::GetItemName
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
std::string ADN_Categories_Data::AttritionEffectOnHuman::GetItemName()
{
    return "";
}

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::ReadArchive
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
void ADN_Categories_Data::AttritionEffectOnHuman::ReadArchive( xml::xistream& input )
{
    std::string equipment;
    input >> xml::attribute( "equipment-state", equipment )
          >> xml::attribute( "injured-percentage", nInjuredPercentage_ )
          >> xml::attribute( "dead-percentage", nDeadPercentage_ );
    nEquipmentState_ = ADN_Tr::ConvertToEquipmentState( equipment );
    if( nEquipmentState_ == E_EquipmentState( -1 ) )
        throw ADN_DataException( "AttritionEffectOnHuman", "Unknown state + '" + equipment + "'" );
}

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::WriteArchive
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
void ADN_Categories_Data::AttritionEffectOnHuman::WriteArchive( xml::xostream& output )
{
    output << xml::start( "attrition-effect" )
            << xml::attribute( "equipment-state", ADN_Tr::ConvertFromEquipmentState( nEquipmentState_.GetData() ) )
            << xml::attribute( "injured-percentage", nInjuredPercentage_ )
            << xml::attribute( "dead-percentage", nDeadPercentage_ )
           << xml::end();
}

// =============================================================================
// ArmorInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ArmorInfos::ArmorInfos
// Created: APE 2004-11-09
// -----------------------------------------------------------------------------
ADN_Categories_Data::ArmorInfos::ArmorInfos()
: strName_      ()
, neutralizationAverageTime_ ( "0s" )
, neutralizationVariance_    ( "0s" )
, rBreakdownEVA_  ( 0 )
, rBreakdownNEVA_ ( 0 )
{
    strName_.SetDataName( "le nom de la catégorie de blindage" );
    strName_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ArmorInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ArmorInfos::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "type", type );
    nType_ = ADN_Tr::ConvertToProtectionType( type );
    if( nType_ == E_ProtectionType( -1 ) )
        throw ADN_DataException( "ArmorInfos", __FUNCTION__ );

    input >> xml::start( "neutralization" )
            >> xml::attribute( "average-time", neutralizationAverageTime_ )
            >> xml::attribute( "variance", neutralizationVariance_ )
          >> xml::end();

    if( nType_ != eProtectionType_Human )
    {
        input >> xml::start( "random-breakdown-probability" )
                >> xml::attribute( "eva", rBreakdownEVA_ )
                >> xml::attribute( "neva", rBreakdownNEVA_ )
              >> xml::end()
              >> xml::start( "attrition-effects" )
                >> xml::list( "attrition-effect", *this, &ADN_Categories_Data::ArmorInfos::ReadAttrition )
              >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ArmorInfos::ReadAttrition
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ArmorInfos::ReadAttrition( xml::xistream& input )
{
    AttritionEffectOnHuman* pNewEffect = new AttritionEffectOnHuman();
    pNewEffect->ReadArchive( input );
    vAttritionEffects_.AddItem( pNewEffect );
}

// -----------------------------------------------------------------------------
// Name: ArmorInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ArmorInfos::WriteArchive( xml::xostream& output )
{
    if( strName_.GetData().empty() )
        throw ADN_DataException( "Mauvaises données dans les catégories",
        "Il existe une ou plusieurs catégories de blindage dont le nom n'est pas valide." );

    if( nType_ == eProtectionType_Human )
    {
        rBreakdownEVA_  = 0.;
        rBreakdownNEVA_ = 0.;
    }

    output << xml::start( "protection" )
            << xml::attribute( "name", trim( strName_.GetData() ) )
            << xml::attribute( "type", ADN_Tr::ConvertFromProtectionType( nType_.GetData() ) );

    output << xml::start( "neutralization" )
            << xml::attribute( "average-time", neutralizationAverageTime_ )
            << xml::attribute( "variance", neutralizationVariance_ )
          << xml::end();

    if( nType_ != eProtectionType_Human )
    {
        output << xml::start( "random-breakdown-probability" )
                << xml::attribute( "eva", rBreakdownEVA_ )
                << xml::attribute( "neva", rBreakdownNEVA_ )
               << xml::end()
               << xml::start( "attrition-effects" );
        for( IT_AttritionEffectOnHuman_Vector it = vAttritionEffects_.begin(); it != vAttritionEffects_.end(); ++it )
            (*it)->WriteArchive( output );
        output << xml::end();
    }
    output << xml::end();
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_Data::ADN_Categories_Data()
: ADN_Data_ABC()
, vArmors_()
, vSizes_()
, vDotationNatures_()
{
    vArmors_.SetNodeName( "la liste des catégories de blindage" );
    vArmors_.SetItemTypeName( "la catégorie de blindage" );
    vSizes_.SetNodeName( "la liste des catégories de volume" );
    vSizes_.SetItemTypeName( "la catégorie de volume" );
    vDotationNatures_.SetNodeName( "la liste des nature de dotations" );
    vDotationNatures_.SetItemTypeName( "la nature de dotation" );
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_Data::~ADN_Categories_Data()
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Categories_Data::FilesNeeded(T_StringList& files) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSizes_.GetData() );
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szArmors_.GetData() );
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDotationNatures_.GetData() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::Reset
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_Data::Reset()
{
    vArmors_.Reset();
    vSizes_.Reset();
    vDotationNatures_.Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::Load
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_Data::Load()
{
    std::string szArmorsFile= ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
                      + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szArmors_.GetData();
    xml::xifstream armorsInput( szArmorsFile );
    ReadArmors( armorsInput );

    std::string szSizesFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
        + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSizes_.GetData();
    xml::xifstream sizesInput( szSizesFile );
    ReadSizes( sizesInput );

    std::string szDotationNaturesFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
        + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDotationNatures_.GetData();
    xml::xifstream dotationNaturesInput( szDotationNaturesFile );
    ReadDotationNatures( dotationNaturesInput );
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::Save
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_Data::Save()
{
    {
        std::string szArmorFile= ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() 
            + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szArmors_.GetData();
        ADN_Tools::CreatePathToFile( szArmorFile );
        xml::xofstream armorOutput( szArmorFile );
        WriteArmors( armorOutput );
    }

    {
        std::string szSizesFile= ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() 
            + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSizes_.GetData();
        ADN_Tools::CreatePathToFile( szSizesFile );
        xml::xofstream sizeOutput( szSizesFile );
        WriteSizes( sizeOutput );
    }
    {
        std::string szNaturesFile= ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() 
            + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDotationNatures_.GetData();
        ADN_Tools::CreatePathToFile( szNaturesFile );
        xml::xofstream naturesOutput( szNaturesFile );
        WriteDotationNatures( naturesOutput );
    }
}


class ADN_String_Cmp : public std::unary_function< ADN_Type_String* , bool >
{
public:
    ADN_String_Cmp(const std::string& val) : val_(val) {}
    virtual ~ADN_String_Cmp() {}

    bool operator()( ADN_Type_String* tgtnfos ) const
    {   return tgtnfos->GetData()==val_;}

private:
    std::string val_;
};

namespace
{
    template< typename Vector, typename Extractor >
    bool HasDuplicates( const Vector& vect, const Extractor& extractor )
    {
        std::vector< std::string > tmp;
        tmp.reserve( vect.size() );
        for( Vector::const_iterator it = vect.begin(); it != vect.end(); ++it )
            tmp.push_back( extractor( **it ) );

        std::sort( tmp.begin(), tmp.end() );
        std::vector< std::string >::iterator itNewEnd = std::unique( tmp.begin(), tmp.end() );
        size_t total = std::distance( tmp.begin(), itNewEnd );
        return total != tmp.size();
    }

    struct StringExtractor
    {
        std::string operator()( ADN_Type_String& value ) const
        {
            return value.GetData();
        }
    };

    struct ArmorExtractor
    {
        std::string operator()( ADN_Categories_Data::ArmorInfos& armor ) const
        {
            return armor.strName_.GetData();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadVolume
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadVolume( xml::xistream& input )
{
    std::string strName = xml::attribute< std::string >( input, "name" );
    T_SizeInfos_Vector::iterator foundSize = std::find_if( vSizes_.begin(), vSizes_.end(), ADN_String_Cmp( strName ) );
    if( foundSize != vSizes_.end() )
        throw ADN_DataException( "ReadVolume", "La catégorie de taille '" + strName + "' est définie plusieurs fois dans le fichier." );

    SizeInfos* pNewArmor = new SizeInfos(strName);
    pNewArmor->SetDataName( "le nom de la catégorie de volume" );
    vSizes_.AddItem( pNewArmor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadSizes
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadSizes( xml::xistream& input )
{
    input >> xml::start( "volumes" )
            >> xml::list( "volume", *this, &ADN_Categories_Data::ReadVolume )
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadProtection
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadProtection( xml::xistream& input )
{
    std::auto_ptr<ArmorInfos> spNewArmor( new ArmorInfos() );
    spNewArmor->ReadArchive( input );
    vArmors_.AddItem( spNewArmor.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadArmors
// Created: APE 2004-11-09
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadArmors( xml::xistream& input )
{
    input >> xml::start( "protections" )
            >> xml::list( "protection", *this, &ADN_Categories_Data::ReadProtection )
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadNature
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadNature( xml::xistream& input )
{
    std::string strName;
    input >> xml::attribute( "type", strName );
    T_DotationNatureInfos_Vector::iterator found = std::find_if( vDotationNatures_.begin(), vDotationNatures_.end(), ADN_String_Cmp( strName ) );
    if( found != vDotationNatures_.end() )
        throw ADN_DataException( "ReadNature", "La nature de dotation '" + strName + "' est définie plusieurs fois dans le fichier." );

    DotationNatureInfos* pNew = new DotationNatureInfos( strName );
    pNew->SetDataName( "le nom de la nature de dotation" );
    vDotationNatures_.AddItem( pNew );
    
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadDotationNatures
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadDotationNatures( xml::xistream& input )
{
    input >> xml::start( "natures" )
            >> xml::list( "nature", *this, &ADN_Categories_Data::ReadNature )
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteSizes
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteSizes( xml::xostream& output )
{
    // Check the sizes data for duplicates.
    if( HasDuplicates( vSizes_, StringExtractor() ) )
        throw ADN_DataException( "Duplication de données dans les catégories",
        "Il existe une ou plusieurs catégories de taille avec le même nom.",
        "Assurez vous qu'il n'y a pas de doublons dans l'onglet 'Catégories'" );

    output << xml::start( "volumes" );
    for( T_SizeInfos_Vector::const_iterator itSize = vSizes_.begin(); itSize != vSizes_.end(); ++itSize )
    {
        if( (*itSize)->GetData().empty() )
            throw ADN_DataException( "Mauvaises données dans les catégories", "Il existe une ou plusieurs catégories de taille dont le nom n'est pas valide." );
        output << xml::start( "volume" )
                << xml::attribute( "name", trim( (*itSize)->GetData() ) )
               << xml::end();

    }
    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteArmors
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteArmors( xml::xostream& output )
{
    // Check the armors data for duplicates.
    if( HasDuplicates( vArmors_, ArmorExtractor() ) )
        throw ADN_DataException( "Duplication de données dans les catégories",
        "Il existe une ou plusieurs catégories de blindage avec le même nom.",
        "Assurez vous qu'il n'y a pas de doublons dans l'onglet 'Catégories'" );

    output << xml::start( "protections" );
    for( T_ArmorInfos_Vector::const_iterator itArmor = vArmors_.begin(); itArmor != vArmors_.end(); ++itArmor )
        (*itArmor)->WriteArchive( output );
    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteDotationNatures
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteDotationNatures( xml::xostream& output )
{
    // Check the dotation natures for duplicates.
    if( HasDuplicates( vDotationNatures_, StringExtractor() ) )
        throw ADN_DataException( "Duplication de données dans les catégories",
        "Il existe une ou plusieurs nature de dotation avec le même nom.",
        "Assurez vous qu'il n'y a pas de doublons dans l'onglet 'Catégories'" );

    output << xml::start( "natures" );
    for( T_DotationNatureInfos_Vector::const_iterator it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
    {
        if( (*it)->GetData().empty() )
            throw ADN_DataException( "Mauvaises données dans les catégories",
            "Il existe une ou plusieurs nature de dotation dont le nom n'est pas valide." );
        output << xml::start( "nature" )
                << xml::attribute( "type", trim( (*it)->GetData() ) )
               << xml::end();
    }
    output << xml::end();
}
