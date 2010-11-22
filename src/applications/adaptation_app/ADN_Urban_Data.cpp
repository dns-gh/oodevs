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

using namespace helpers;

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data constructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_Data::ADN_Urban_Data()
    : ADN_Data_ABC()
    , vMaterials_ ()
    , vFacades_   ()
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
    {   return tgtnfos->GetData() == val_; }

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
    input   >> xml::end()
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteUrban
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteUrban( xml::xostream& output ) const
{
    output  << xml::start( "urban" );
    ADN_Tools::AddSchema( output, "UrbanTypes" );
    output  << xml::start( "urban-block-types" );
    WriteMaterials( output );
    WriteFacades( output );
    WriteRoofShapes( output );
    output  << xml::end
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadMaterials
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadMaterials( xml::xistream& input )
{
    input >> xml::start( "material-composition-types" )
            >> xml::list( "material-composition-type", *this, &ADN_Urban_Data::ReadMaterial )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadMaterial
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadMaterial( xml::xistream& input  )
{
    vMaterials_.AddItem( new UrbanMaterialInfos( input ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteMaterials
// Created: SLG 2010-06-30
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteMaterials( xml::xostream& output ) const
{
    output << xml::start( "material-composition-types" );
    for( CIT_UrbanMaterialInfos_Vector it = vMaterials_.begin(); it != vMaterials_.end(); ++it )
        ( *it )->WriteMaterial( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteMaterial
// Created: SLG 2010-07-05
// -----------------------------------------------------------------------------
void ADN_Urban_Data::UrbanMaterialInfos::WriteMaterial( xml::xostream& output )
{
    std::string strData = strName_.GetData();
    output << xml::start( "material-composition-type" )
        << xml::attribute( "name",  trim( strData ) )
        << xml::start( "attritions" );
    for( IT_AttritionInfos_Vector it = vAttritionInfos_.begin(); it != vAttritionInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end()
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos constructor
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanMaterialInfos::UrbanMaterialInfos( xml::xistream& input )
    : strName_ ( "" )
    , vAttritionInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos() )
{
    input >> xml::attribute( "name", strName_ )
        >> xml::start( "attritions" )
        >> xml::list( "attrition", *this, &ADN_Urban_Data::UrbanMaterialInfos::ReadAttrition )
        >> xml::end();
}


// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos
// Created: HBD 2010-11-17
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanMaterialInfos::UrbanMaterialInfos()
    : strName_ ( "" )
    , vAttritionInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos() )
{
    strName_.SetDataName( "le nom du material" );
    strName_.SetParentNode( *this );
    //vAttritionInfos_ = CreateDefaultAttritionInfos();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos destructor
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanMaterialInfos::~UrbanMaterialInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos::ReadAttrition
// Created: SLG 2010-06-30
// -----------------------------------------------------------------------------
void ADN_Urban_Data::UrbanMaterialInfos::ReadAttrition( xml::xistream& input )
{
    std::string protection = input.attribute< std::string >( "protection" );
    helpers::IT_AttritionInfos_Vector itAttrition = std::find_if( vAttritionInfos_.begin(), vAttritionInfos_.end(), helpers::AttritionInfos::Cmp( protection ));
    if( itAttrition == vAttritionInfos_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid armor type '%1'" ).arg( protection.c_str() ).ascii() );
    (*itAttrition)->ReadArchive( input );
}


// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos::GetNodeName
// Created: SLG 2010-07-02
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::UrbanMaterialInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos::GetItemName
// Created: SLG 2010-07-02
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::UrbanMaterialInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadFacades
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadFacades( xml::xistream& input )
{
    input >> xml::start( "facade-types" )
        >> xml::list( "facade-type", *this, &ADN_Urban_Data::ReadFacade )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadFacade
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadFacade( xml::xistream& input )
{
    const std::string strName = xml::attribute< std::string >( input, "name" );
    T_UrbanInfos_Vector::const_iterator it = std::find_if( vFacades_.begin(), vFacades_.end(), ADN_String_Cmp( strName ) );
    if( it != vFacades_.end() )
        throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).ascii(), tools::translate( "Urban_Data", "Facade - Duplicated material type name '%1'" ).arg( strName.c_str() ).ascii() );

    UrbanInfos* pNewFacade = new UrbanInfos( strName );
    pNewFacade->SetDataName( "le nom du type de facade" );
    vFacades_.AddItem( pNewFacade );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteFacades
// Created: SLG 2010-03-10
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteFacades( xml::xostream& output ) const
{
    // Check the sizes data for duplicates.
    if( HasDuplicates( vFacades_, StringExtractor() ) )
        throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).ascii(), tools::translate( "Urban_Data", "Material - Duplicated volume type names" ).ascii() );

    output << xml::start( "facade-types" );
    for( T_UrbanInfos_Vector::const_iterator itFacade = vFacades_.begin(); itFacade != vFacades_.end(); ++itFacade )
    {
        if( ( *itFacade )->GetData().empty() )
            throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).ascii(), tools::translate( "Urban_Data", "Facade - Invalid volume type name" ).ascii() );

        std::string strData = ( *itFacade )->GetData();
        output << xml::start( "facade-type" )
                    << xml::attribute( "name", trim( strData ) )
               << xml::end();
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadRoofShapes
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadRoofShapes( xml::xistream& input )
{
    input >> xml::start( "roof-shape-types" )
        >> xml::list( "roof-shape-type", *this, &ADN_Urban_Data::ReadRoofShape )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadRoofShape
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadRoofShape( xml::xistream& input )
{
    std::string strName = input.attribute< std::string >( "name" );
    T_UrbanInfos_Vector::iterator foundRoofShape = std::find_if( vRoofShapes_.begin(), vRoofShapes_.end(), ADN_String_Cmp( strName ) );
    if( foundRoofShape != vRoofShapes_.end() )
        throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).ascii(), tools::translate( "Urban_Data", "Facade - Duplicated material type name '%1'" ).arg( strName.c_str() ).ascii() );

    UrbanInfos* pNewRoofShape = new UrbanInfos(strName);
    pNewRoofShape->SetDataName( "le nom du type de facade" );
    vRoofShapes_.AddItem( pNewRoofShape );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteRoofShapes
// Created: SLG 2010-03-10
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteRoofShapes( xml::xostream& output ) const
{
    // Check the sizes data for duplicates.
    if( HasDuplicates( vRoofShapes_, StringExtractor() ) )
        throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).ascii(), tools::translate( "Urban_Data", "Material - Duplicated volume type names" ).ascii() );

    output << xml::start( "roof-shape-types" );
    for( T_UrbanInfos_Vector::const_iterator itRoofShape = vRoofShapes_.begin(); itRoofShape != vRoofShapes_.end(); ++itRoofShape )
    {
        if( (*itRoofShape)->GetData().empty() )
            throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).ascii(), tools::translate( "Urban_Data", "RoofShape - Invalid volume type name" ).ascii() );
        std::string strData = ( *itRoofShape )->GetData();
        output << xml::start( "roof-shape-type" )
            << xml::attribute( "name", trim( strData ) )
            << xml::end;

    }
    output << xml::end;
}