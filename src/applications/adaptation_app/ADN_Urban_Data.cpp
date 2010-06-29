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
#include "ADN_Urban_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Tools.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tr.h"


//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data constructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_Data::ADN_Urban_Data()
: ADN_Data_ABC()
, vMaterials_()
, vFacades_()
, vRoofShapes_()
{
    vMaterials_.SetNodeName( "la liste des types de matériau" );
    vMaterials_.SetItemTypeName( "le type matériau" );
    vFacades_.SetNodeName( "la liste des types de facade" );
    vFacades_.SetItemTypeName( "le type facade" );
    vRoofShapes_.SetNodeName( "la liste des types de toit" );
    vRoofShapes_.SetItemTypeName( "le type toir" );
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data destructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_Data::~ADN_Urban_Data()
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FilesNeeded
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
void ADN_Urban_Data::FilesNeeded(T_StringList& files) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrban_.GetData() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::Reset
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
void ADN_Urban_Data::Reset()
{
    vMaterials_.Reset();
    vFacades_.Reset();
    vRoofShapes_.Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::Load
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
void ADN_Urban_Data::Load()
{
    std::string szMaterialsFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData()
        + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrban_.GetData();
    xml::xifstream sizesInput( szMaterialsFile );
    ReadUrban( sizesInput );
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::Save
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
void ADN_Urban_Data::Save()
{
    std::string szSizesFile= ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory()
        + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrban_.GetData();
    ADN_Tools::CreatePathToFile( szSizesFile );
    xml::xofstream sizeOutput( szSizesFile );
    WriteUrban( sizeOutput );
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
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadUrban
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadUrban( xml::xistream& input )
{
    input >> xml::start( "urban" )
          >> xml::start( "urban-block-types" );
    ReadMaterials( input );
    ReadFacades( input );
    ReadRoofShapes( input );
    input >> xml::end()
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteUrban
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteUrban( xml::xostream& output )
{
    output  << xml::start( "urban" );
    ADN_Tools::AddSchema( output, "Urban" );
    output  << xml::start( "urban-block-types" );
    WriteMaterials( output );
    WriteFacades( output );
    WriteRoofShapes( output );
    output  << xml::end()
            << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadMaterials
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadMaterials( xml::xistream& input )
{
    input >> xml::start( "material-composition-types" )
          >> xml::list( "material-composition-type", *this, &ADN_Urban_Data::ReadMaterial )
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadMaterial
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadMaterial( xml::xistream& input )
{
    std::string strName = xml::attribute< std::string >( input, "name" );
    T_UrbanInfos_Vector::iterator foundMaterial = std::find_if( vMaterials_.begin(), vMaterials_.end(), ADN_String_Cmp( strName ) );
    if( foundMaterial != vMaterials_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Material - Duplicated material type name '%1'" ).arg( strName.c_str() ).ascii() );

    UrbanInfos* pNewMaterial = new UrbanInfos(strName);
    pNewMaterial->SetDataName( "le nom du type de materiau" );
    vMaterials_.AddItem( pNewMaterial );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteMaterials
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteMaterials( xml::xostream& output )
{
    // Check the sizes data for duplicates.
    if( HasDuplicates( vMaterials_, StringExtractor() ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Material - Duplicated volume type names" ).ascii() );

    output << xml::start( "material-composition-types" );
    for( T_UrbanInfos_Vector::const_iterator itMaterial = vMaterials_.begin(); itMaterial != vMaterials_.end(); ++itMaterial )
    {
        if( (*itMaterial)->GetData().empty() )
            throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Material - Invalid volume type name" ).ascii() );
        output << xml::start( "material-composition-type" )
            << xml::attribute( "name", trim( (*itMaterial)->GetData() ) )
            << xml::end();

    }
    output << xml::end();
}


// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadFacades
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadFacades( xml::xistream& input )
{
    input >> xml::start( "facade-types" )
        >> xml::list( "facade-type", *this, &ADN_Urban_Data::ReadFacade )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadFacade
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadFacade( xml::xistream& input )
{
    std::string strName = xml::attribute< std::string >( input, "name" );
    T_UrbanInfos_Vector::iterator foundFacade = std::find_if( vFacades_.begin(), vFacades_.end(), ADN_String_Cmp( strName ) );
    if( foundFacade != vFacades_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Facade - Duplicated material type name '%1'" ).arg( strName.c_str() ).ascii() );

    UrbanInfos* pNewFacade = new UrbanInfos(strName);
    pNewFacade->SetDataName( "le nom du type de facade" );
    vFacades_.AddItem( pNewFacade );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteFacades
// Created: SLG 2010-03-10
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteFacades( xml::xostream& output )
{
    // Check the sizes data for duplicates.
    if( HasDuplicates( vFacades_, StringExtractor() ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Material - Duplicated volume type names" ).ascii() );

    output << xml::start( "facade-types" );
    for( T_UrbanInfos_Vector::const_iterator itFacade = vFacades_.begin(); itFacade != vFacades_.end(); ++itFacade )
    {
        if( (*itFacade)->GetData().empty() )
            throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Facade - Invalid volume type name" ).ascii() );
        output << xml::start( "facade-type" )
            << xml::attribute( "name", trim( (*itFacade)->GetData() ) )
            << xml::end();

    }
    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadRoofShapes
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadRoofShapes( xml::xistream& input )
{
    input >> xml::start( "roof-shape-types" )
        >> xml::list( "roof-shape-type", *this, &ADN_Urban_Data::ReadRoofShape )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadRoofShape
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadRoofShape( xml::xistream& input )
{
    std::string strName = xml::attribute< std::string >( input, "name" );
    T_UrbanInfos_Vector::iterator foundRoofShape = std::find_if( vRoofShapes_.begin(), vRoofShapes_.end(), ADN_String_Cmp( strName ) );
    if( foundRoofShape != vRoofShapes_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Facade - Duplicated material type name '%1'" ).arg( strName.c_str() ).ascii() );

    UrbanInfos* pNewRoofShape = new UrbanInfos(strName);
    pNewRoofShape->SetDataName( "le nom du type de facade" );
    vRoofShapes_.AddItem( pNewRoofShape );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteRoofShapes
// Created: SLG 2010-03-10
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteRoofShapes( xml::xostream& output )
{
    // Check the sizes data for duplicates.
    if( HasDuplicates( vRoofShapes_, StringExtractor() ) )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Material - Duplicated volume type names" ).ascii() );

    output << xml::start( "roof-shape-types" );
    for( T_UrbanInfos_Vector::const_iterator itRoofShape = vRoofShapes_.begin(); itRoofShape != vRoofShapes_.end(); ++itRoofShape )
    {
        if( (*itRoofShape)->GetData().empty() )
            throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "RoofShape - Invalid volume type name" ).ascii() );
        output << xml::start( "roof-shape-type" )
            << xml::attribute( "name", trim( (*itRoofShape)->GetData() ) )
            << xml::end();

    }
    output << xml::end();
}