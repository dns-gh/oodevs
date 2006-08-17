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
#include "ADN_pch.h"
#include "ADN_Categories_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Tools.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
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
void ADN_Categories_Data::AttritionEffectOnHuman::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "EffetAttritionSurHumains" );
    input.ReadAttribute( "etatEquipement", nEquipmentState_, ADN_Tr::ConvertToEquipmentState, ADN_XmlInput_Helper::eThrow );
    input.ReadAttribute( "pourcentageBlesses", nInjuredPercentage_ );
    input.ReadAttribute( "pourcentageMorts", nDeadPercentage_ );
    input.EndSection();
}

// -----------------------------------------------------------------------------
// Name: AttritionEffectOnHuman::WriteArchive
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
void ADN_Categories_Data::AttritionEffectOnHuman::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "EffetAttritionSurHumains" );
    output.WriteAttribute( "etatEquipement", ADN_Tr::ConvertFromEquipmentState( nEquipmentState_.GetData() ) );
    output.WriteAttribute( "pourcentageBlesses", nInjuredPercentage_.GetData() );
    output.WriteAttribute( "pourcentageMorts", nDeadPercentage_.GetData() );
    output.EndSection();
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
}


// -----------------------------------------------------------------------------
// Name: ArmorInfos::ReadArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ArmorInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Protection" );
    input.ReadAttribute( "nom", strName_ );
    input.ReadAttribute( "type", nType_, ADN_Tr::ConvertToProtectionType, ADN_XmlInput_Helper::eThrow );

    input.Section( "Neutralisation" );
    input.ReadAttribute( "tempsMoyen", neutralizationAverageTime_ );
    input.ReadAttribute( "variance", neutralizationVariance_ );
    input.EndSection(); // Neutralisation

    if( nType_ != eProtectionType_Human )
    {
        input.Section( "ProbabilitePanneAleatoire" );
        input.ReadAttribute( "EVA", rBreakdownEVA_ );
        input.ReadAttribute( "NEVA", rBreakdownNEVA_ );
        input.EndSection(); // ProbabilitePanneAleatoire

        input.BeginList( "EffetsAttritionSurHumains" );
        while( input.NextListElement() )
        {
            AttritionEffectOnHuman* pNewEffect = new AttritionEffectOnHuman();
            pNewEffect->ReadArchive( input );
            vAttritionEffects_.AddItem( pNewEffect );
        }
        input.EndList(); // EffetsAttritionSurHumains
    }

    input.EndSection(); // Protection
}


// -----------------------------------------------------------------------------
// Name: ArmorInfos::WriteArchive
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ArmorInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Protection" );

    if( strName_.GetData().empty() )
        throw ADN_DataException( "Mauvaises données dans les catégories",
        "Il existe une ou plusieurs catégories de blindage dont le nom n'est pas valide." );

    output.WriteAttribute( "nom", trim( strName_.GetData() ) );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromProtectionType( nType_.GetData() ) );

    // override xml archive values if Human
    if( nType_ == eProtectionType_Human )
    {
        rBreakdownEVA_  = 0.;
        rBreakdownNEVA_ = 0.;
    }

    output.Section( "Neutralisation" );
    output.WriteAttribute( "tempsMoyen", neutralizationAverageTime_.GetData() );
    output.WriteAttribute( "variance",   neutralizationVariance_.GetData() );
    output.EndSection(); // Neutralisation

    if( nType_.GetData() != eProtectionType_Human )
    {
        output.Section( "ProbabilitePanneAleatoire" );
        output.WriteAttribute( "EVA", rBreakdownEVA_.GetData() );
        output.WriteAttribute( "NEVA", rBreakdownNEVA_.GetData() );
        output.EndSection(); // ProbabilitePanneAleatoire

        output.BeginList( "EffetsAttritionSurHumains", vAttritionEffects_.size() );
        for( IT_AttritionEffectOnHuman_Vector it = vAttritionEffects_.begin(); it != vAttritionEffects_.end(); ++it )
            (*it)->WriteArchive( output );
        output.EndList(); // EffetsAttritionSurHumains
    }

    output.EndSection(); // Protection
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
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szSizes_.GetData() );
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szArmors_.GetData() );
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szDotationNatures_.GetData() );
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
    // Open and read the armors file.
    ADN_XmlInput_Helper armorsInput;

    std::string szArmorsFile= ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
                      + ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szArmors_.GetData();

    if ( !armorsInput.Open( szArmorsFile, ADN_XmlInput_Helper::eNothing ) )
        throw ADN_OpenFile_Exception( szArmorsFile );

    try
    {
        ReadArmors( armorsInput );
    }
    catch( ADN_Xml_Exception& xmlException )
    {
        throw ADN_Xml_Exception( szArmorsFile, xmlException.GetContext(), xmlException.GetErrorMessage() );
    }
    catch( MT_ArchiveLogger_Exception& xmlException )
    {
        throw ADN_DataException( "", xmlException.what() );
    }

    // Open and read the sizes file.
    ADN_XmlInput_Helper sizesInput;

    std::string szSizesFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
        + ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szSizes_.GetData();

    if ( !sizesInput.Open( szSizesFile, ADN_XmlInput_Helper::eNothing ) )
        throw ADN_OpenFile_Exception( szSizesFile );

    try
    {
        ReadSizes( sizesInput );
    }
    catch( ADN_Xml_Exception& xmlException )
    {
        throw ADN_Xml_Exception( szSizesFile, xmlException.GetContext(), xmlException.GetErrorMessage() );
    }
    catch( MT_ArchiveLogger_Exception& xmlException )
    {
        throw ADN_DataException( "", xmlException.what() );
    }

    // Open and read the dotation nature file.
    ADN_XmlInput_Helper dotationNaturesInput;

    std::string szDotationNaturesFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
        + ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szDotationNatures_.GetData();

    if ( !dotationNaturesInput.Open( szDotationNaturesFile, ADN_XmlInput_Helper::eNothing ) )
        throw ADN_OpenFile_Exception( szDotationNaturesFile );
    try
    {
        ReadDotationNatures( dotationNaturesInput );
    }
    catch( ADN_Xml_Exception& xmlException )
    {
        throw ADN_Xml_Exception( szDotationNaturesFile, xmlException.GetContext(), xmlException.GetErrorMessage() );
    }
    catch( MT_ArchiveLogger_Exception& xmlException )
    {
        throw ADN_DataException( "", xmlException.what() );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::Save
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_Data::Save()
{
    // Save the armors data.
    MT_XXmlOutputArchive armorOutput;
    WriteArmors( armorOutput );

    // Write the file.
    std::string szArmorFile= ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() 
        + ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szArmors_.GetData();

    ADN_Tools::CreatePathToFile( szArmorFile );
    if( ! armorOutput.WriteToFile( szArmorFile ) )
        throw ADN_SaveFile_Exception( szArmorFile );

    // Save the sizes data.
    MT_XXmlOutputArchive sizesOutput;
    WriteSizes( sizesOutput );

    // Write the file.
    std::string szSizesFile= ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() 
        + ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szSizes_.GetData();

    ADN_Tools::CreatePathToFile( szSizesFile );
    if( ! sizesOutput.WriteToFile( szSizesFile ) )
        throw ADN_SaveFile_Exception( szSizesFile );

    // Save the dotation nature data.
    MT_XXmlOutputArchive naturesOutput;
    WriteDotationNatures( naturesOutput );

    // Write the file.
    std::string szNaturesFile= ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() 
        + ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szDotationNatures_.GetData();

    ADN_Tools::CreatePathToFile( szNaturesFile );
    if( ! naturesOutput.WriteToFile( szNaturesFile ) )
        throw ADN_SaveFile_Exception( szNaturesFile );
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



struct StringInVectorCmp
{
    bool operator()( ADN_Type_String* pL, ADN_Type_String* pR )
    {
        return pL->GetData().compare( pR->GetData() ) < 0;
    }
};

struct StringInVectorEqual
{
    bool operator()( ADN_Type_String* pL, ADN_Type_String* pR )
    {
        return pL->GetData() == pR->GetData();
    }
};


// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadSizes
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadSizes( ADN_XmlInput_Helper& input )
{
    // Read the sizes.
    input.BeginList( "Volumes" );
    while( input.NextListElement() )
    {
        input.Section( "Volume" );

        std::string strName;
        input.Read( strName );

        T_SizeInfos_Vector::iterator foundSize = std::find_if( vSizes_.begin(), vSizes_.end(), ADN_String_Cmp( strName ) );
        if( foundSize != vSizes_.end() )
            input.ThrowError( MT_FormatString( "La catégorie de taille %s est définie plusieurs fois dans le fichier.", strName.c_str() ).c_str() );

        SizeInfos* pNewArmor = new SizeInfos(strName);
        pNewArmor->SetDataName( "le nom de la catégorie de volume" );
        vSizes_.AddItem( pNewArmor );

        input.EndSection(); // Volume
    }
    input.EndList(); // Volumes
}


// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadArmors
// Created: APE 2004-11-09
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadArmors( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Protections" );
    while( input.NextListElement() )
    {
        std::auto_ptr<ArmorInfos> spNewArmor( new ArmorInfos() );
        spNewArmor->ReadArchive( input );
        vArmors_.AddItem( spNewArmor.release() );
    }
    input.EndList();    // Protections
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadDotationNatures
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadDotationNatures( ADN_XmlInput_Helper& input )
{
    // Read the dotation natures.
    input.BeginList( "Natures" );
    while( input.NextListElement() )
    {
        input.Section( "Nature" );

        std::string strName;
        input.Read( strName );

        T_DotationNatureInfos_Vector::iterator found = std::find_if( vDotationNatures_.begin(), vDotationNatures_.end(), ADN_String_Cmp( strName ) );
        if( found != vDotationNatures_.end() )
            input.ThrowError( MT_FormatString( "La nature de dotation %s est définie plusieurs fois dans le fichier.", strName.c_str() ).c_str() );

        DotationNatureInfos* pNew = new DotationNatureInfos( strName );
        pNew->SetDataName( "le nom de la nature de dotation" );
        vDotationNatures_.AddItem( pNew );

        input.EndSection(); // Nature
    }
    input.EndList(); // Natures
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteSizes
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteSizes( MT_OutputArchive_ABC& output )
{
    // Check the sizes data for duplicates.
    T_SizeInfos_Vector::iterator itSize;

    ADN_Type_Vector_ABC< ADN_Type_String > tmpVector;
    tmpVector.reserve( vSizes_.size() );

    for( itSize = vSizes_.begin(); itSize != vSizes_.end(); ++itSize )
        tmpVector.AddItem( (*itSize) );

    std::sort( tmpVector.begin(), tmpVector.end(), StringInVectorCmp() );
    ADN_Type_Vector_ABC< ADN_Type_String >::iterator itNewEnd = std::unique( tmpVector.begin(), tmpVector.end(), StringInVectorEqual() );
    size_t nTotal = std::distance( tmpVector.begin(), itNewEnd );

    if( nTotal != tmpVector.size() )
        throw ADN_DataException( "Duplication de données dans les catégories",
        "Il existe une ou plusieurs catégories de taille avec le même nom.",
        "Assurez vous qu'il n'y a pas de doublons dans l'onglet 'Catégories'" );

    // Write the data.
    output.BeginList( "Volumes", vSizes_.size() );
    for( itSize = vSizes_.begin(); itSize != vSizes_.end(); ++itSize )
    {
        output.Section( "Volume" );

        if( (*itSize)->GetData().empty() )
            throw ADN_DataException( "Mauvaises données dans les catégories",
            "Il existe une ou plusieurs catégories de taille dont le nom n'est pas valide." );

        output << trim( (*itSize)->GetData() );

        output.EndSection(); // Volume
    }
    output.EndSection();    // Volumes
}


// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteArmors
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteArmors( MT_OutputArchive_ABC& output )
{
    // Check the armors data for duplicates.
    ADN_Type_Vector_ABC< ADN_Type_String > tmpVector;
    tmpVector.reserve( vArmors_.size() );
    T_ArmorInfos_Vector::iterator itArmor;

    for( itArmor = vArmors_.begin(); itArmor != vArmors_.end(); ++itArmor )
        tmpVector.AddItem( & (*itArmor)->strName_ );

    std::sort( tmpVector.begin(), tmpVector.end(), StringInVectorCmp() );
    ADN_Type_Vector_ABC< ADN_Type_String >::iterator itNewEnd = std::unique( tmpVector.begin(), tmpVector.end(), StringInVectorEqual() );
    size_t nTotal = std::distance( tmpVector.begin(), itNewEnd );

    if( nTotal != tmpVector.size() )
        throw ADN_DataException( "Duplication de données dans les catégories",
        "Il existe une ou plusieurs catégories de blindage avec le même nom.",
        "Assurez vous qu'il n'y a pas de doublons dans l'onglet 'Catégories'" );

    // Write the data.
    output.BeginList( "Protections", vArmors_.size() );
    for( itArmor = vArmors_.begin(); itArmor != vArmors_.end(); ++itArmor )
    {
        (*itArmor)->WriteArchive( output );
    }
    output.EndList();    // Protections

}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteDotationNatures
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteDotationNatures( MT_OutputArchive_ABC& output )
{
    // Check the dotation natures for duplicates.
    T_DotationNatureInfos_Vector::iterator it;

    ADN_Type_Vector_ABC< ADN_Type_String > tmpVector;
    tmpVector.reserve( vDotationNatures_.size() );

    for( it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
        tmpVector.AddItem( *it );

    std::sort( tmpVector.begin(), tmpVector.end(), StringInVectorCmp() );
    ADN_Type_Vector_ABC< ADN_Type_String >::iterator itNewEnd = std::unique( tmpVector.begin(), tmpVector.end(), StringInVectorEqual() );
    size_t nTotal = std::distance( tmpVector.begin(), itNewEnd );

    if( nTotal != tmpVector.size() )
        throw ADN_DataException( "Duplication de données dans les catégories",
        "Il existe une ou plusieurs nature de dotation avec le même nom.",
        "Assurez vous qu'il n'y a pas de doublons dans l'onglet 'Catégories'" );

    // Write the data.
    output.BeginList( "Natures", vDotationNatures_.size() );
    for( it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
    {
        output.Section( "Nature" );

        if( (*it)->GetData().empty() )
            throw ADN_DataException( "Mauvaises données dans les catégories",
            "Il existe une ou plusieurs nature de dotation dont le nom n'est pas valide." );

        output << trim( (*it)->GetData() );

        output.EndSection(); // Nature
    }
    output.EndSection();    // Natures
}
