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
        std::string operator()( helpers::ArmorInfos& armor ) const
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
    std::string strName = input.attribute< std::string >( "name" );
    T_SizeInfos_Vector::iterator foundSize = std::find_if( vSizes_.begin(), vSizes_.end(), ADN_String_Cmp( strName ) );
    if( foundSize != vSizes_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Duplicated volume type name '%1'" ).arg( strName.c_str() ).ascii() );

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
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadProtection
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadProtection( xml::xistream& input )
{
    std::auto_ptr< helpers::ArmorInfos > spNewArmor( new helpers::ArmorInfos() );
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
          >> xml::end;
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
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Duplicated resource nature type name '%1'" ).arg( strName.c_str() ).ascii() );

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
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteSizes
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteSizes( xml::xostream& output )
{
    // Check the sizes data for duplicates.
    if( HasDuplicates( vSizes_, StringExtractor() ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Duplicated volume type names" ).ascii() );

    output << xml::start( "volumes" );
    ADN_Tools::AddSchema( output, "Volumes" );
    for( T_SizeInfos_Vector::const_iterator itSize = vSizes_.begin(); itSize != vSizes_.end(); ++itSize )
    {
        if( (*itSize)->GetData().empty() )
            throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Invalid volume type name" ).ascii() );
        output << xml::start( "volume" )
                << xml::attribute( "name", trim( (*itSize)->GetData() ) )
               << xml::end;

    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteArmors
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteArmors( xml::xostream& output )
{
    // Check the armors data for duplicates.
    if( HasDuplicates( vArmors_, ArmorExtractor() ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Duplicated armor type names" ).ascii() );

    output << xml::start( "protections" );
    ADN_Tools::AddSchema( output, "Armors" );
    for( helpers::T_ArmorInfos_Vector::const_iterator itArmor = vArmors_.begin(); itArmor != vArmors_.end(); ++itArmor )
        ( *itArmor )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteDotationNatures
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteDotationNatures( xml::xostream& output )
{
    // Check the dotation natures for duplicates.
    if( HasDuplicates( vDotationNatures_, StringExtractor() ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Duplicated resource nature names" ).ascii() );

    output << xml::start( "natures" );
    ADN_Tools::AddSchema( output, "ResourceNatures" );
    for( T_DotationNatureInfos_Vector::const_iterator it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
    {
        if( (*it)->GetData().empty() )
            throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Categories - Invalid resource nature" ).ascii() );
        output << xml::start( "nature" )
                << xml::attribute( "type", trim( (*it)->GetData() ) )
               << xml::end;
    }
    output << xml::end;
}
