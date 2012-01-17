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
#include "ADN_DataException.h"
#include "ADN_Objects_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_Project_Data.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_Workspace.h"
#include "tools/Loader_ABC.h"
#include <tools/XmlCrc32Signature.h>
#include <boost/bind.hpp>

using namespace helpers;

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data constructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_Data::ADN_Urban_Data()
    : ADN_Data_ABC           ()
    , vMaterials_            ()
    , vFacades_              ()
    , vRoofShapes_           ()
    , vAccommodations_       ()
    , vInfrastructures_      ()
    , vTemplates_            ()
    , defaultNominalCapacity_( 0.1 )
    , defaultMaxCapacity_    ( 1 )
{
    vMaterials_.SetNodeName( "la liste des types de matériau" );
    vMaterials_.SetItemTypeName( "le type matériau" );
    vFacades_.SetNodeName( "la liste des types de facade" );
    vFacades_.SetItemTypeName( "le type facade" );
    vRoofShapes_.SetNodeName( "la liste des types de toit" );
    vRoofShapes_.SetItemTypeName( "le type toit" );
    vAccommodations_.SetNodeName( "la liste des types de motivations" );
    vAccommodations_.SetItemTypeName( "le type motivation" );
    vInfrastructures_.SetNodeName( "la liste des types d'infrastructure" );
    vInfrastructures_.SetItemTypeName( "le type infrastructure" );
    vTemplates_.SetNodeName( "la liste des patrons des blocs urbains" );
    vTemplates_.SetNodeName( "les caractéristiques du patron" );
    defaultNominalCapacity_.SetDataName( "la capacité nominale par défaut" );
    defaultMaxCapacity_.SetDataName( "la capacité max par défaut" );
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
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrbanTemplates_.GetData() );
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
    vAccommodations_.Reset();
    vInfrastructures_.Reset();
    vTemplates_.Reset();
    defaultNominalCapacity_ = 0.1;
    defaultMaxCapacity_ = 1;
}

class ADN_String_Cmp : public std::unary_function< ADN_Type_String* , bool >
{
public:
    ADN_String_Cmp(const std::string& val) : val_( val ) {}
    virtual ~ADN_String_Cmp() {}

    bool operator()( ADN_Type_String* tgtnfos ) const
    {
        return tgtnfos->GetData() == val_;
    }

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
// Name: ADN_Urban_Data::Save
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::Save()
{
    T_StringList fileList;
    FilesNeeded( fileList );
    for ( CIT_StringList it = fileList.begin(); it != fileList.end(); ++it )
    {
        std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() + *it;
        ADN_Tools::CreatePathToFile( strFile );
        {
            xml::xofstream output( strFile );
            if( *it == "Urban.xml" )
                WriteArchive( output );
            if( *it == "UrbanTemplates.xml" )
                WriteTemplates( output );
        }
        tools::WriteXmlCrc32Signature( strFile );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::Load
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::Load( const tools::Loader_ABC& fileLoader )
{
    T_StringList fileList;
    FilesNeeded( fileList );
    for ( CIT_StringList it = fileList.begin(); it != fileList.end(); ++it )
    {
        const std::string strFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + (*it);
        if( *it == "Urban.xml" )
            fileLoader.LoadFile( strFile, boost::bind( &ADN_Urban_Data::ReadArchive, this, _1 ) );
        if( *it == "UrbanTemplates.xml" )
            fileLoader.LoadFile( strFile, boost::bind( &ADN_Urban_Data::ReadTemplates, this, _1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadArchive
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "urban" )
            >> xml::start( "urban-block-types" );
    ReadMaterials( input );
    ReadFacades( input );
    ReadRoofShapes( input );
    input >> xml::end;
    ReadAccommodations( input );
    ReadInfrastructures( input );
    input >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteArchive
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "urban" );
    ADN_Tools::AddSchema( output, "UrbanTypes" );
    output  << xml::start( "urban-block-types" );
    WriteMaterials( output );
    WriteFacades( output );
    WriteRoofShapes( output );
    output  << xml::end;
    WriteAccommodations( output );
    WriteInfrastructures( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadTemplates
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadTemplates( xml::xistream& input )
{
    input >> xml::start( "templates" )
            >> xml::list( "template", *this, &ADN_Urban_Data::ReadTemplate )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteTemplates
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteTemplates( xml::xostream& output )
{
    output << xml::start( "templates" );
    ADN_Tools::AddSchema( output, "UrbanTemplates" );
        for( CIT_UrbanTemplateInfos_Vector it = vTemplates_.begin(); it != vTemplates_.end(); ++it )
            ( *it )->Write( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadTemplate
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadTemplate( xml::xistream& input )
{
    vTemplates_.AddItem( new UrbanTemplateInfos( input ) );
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
    output   << xml::end()
           << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos constructor
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanMaterialInfos::UrbanMaterialInfos( xml::xistream& input )
    : strName_        ( "" )
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
    : strName_        ( "" )
    , vAttritionInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos() )
{
    strName_.SetDataName( "le nom du material" );
    strName_.SetParentNode( *this );
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
    ( *itAttrition )->ReadArchive( input );
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
    vRoofShapes_.AddItem( new RoofShapeInfos( input ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteRoofShapes
// Created: SLG 2010-03-10
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteRoofShapes( xml::xostream& output ) const
{
    if( !vRoofShapes_.empty() )
    {
        output << xml::start( "roof-shape-types" );
        for( CIT_RoofShapeInfos_Vector it = vRoofShapes_.begin(); it != vRoofShapes_.end(); ++it )
            ( *it )->WriteRoofShape( output );
        output << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadAccommodations
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadAccommodations( xml::xistream& input )
{
    input >> xml::optional
          >> xml::start( "accommodations" )
            >> xml::list( "accommodation", *this, &ADN_Urban_Data::ReadAccommodation )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadAccommodation
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadAccommodation( xml::xistream& input  )
{
    std::string role = input.attribute< std::string >( "role" );
    if( role == "default" )
        input >> xml::attribute( "nominal-capacity", defaultNominalCapacity_ )
              >> xml::attribute( "max-capacity", defaultMaxCapacity_ );
    else
        vAccommodations_.AddItem( new AccommodationInfos( input, role ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteAccommodations
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteAccommodations( xml::xostream& output ) const
{
    output << xml::start( "accommodations" )
            << xml::start( "accommodation" )
                << xml::attribute( "role", "default" )
                << xml::attribute( "nominal-capacity", defaultNominalCapacity_.GetData() )
                << xml::attribute( "max-capacity", defaultMaxCapacity_.GetData() )
            << xml::end();
    for( CIT_AccommodationInfos_Vector it = vAccommodations_.begin(); it != vAccommodations_.end(); ++it )
        ( *it )->WriteAccommodation( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteAccommodation
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
void ADN_Urban_Data::AccommodationInfos::WriteAccommodation( xml::xostream& output )
{
    std::string strData = strName_.GetData();
    output << xml::start( "accommodation" )
           << xml::attribute( "role", trim( strData ) )
           << xml::attribute( "nominal-capacity", nominalCapacity_.GetData() )
           << xml::attribute( "max-capacity", maxCapacity_.GetData() )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::AccommodationInfos constructor
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::AccommodationInfos::AccommodationInfos( xml::xistream& input, const std::string& role )
    : strName_        ( role )
    , nominalCapacity_( 0 )
    , maxCapacity_    ( 0 )
{
    input >> xml::attribute( "nominal-capacity", nominalCapacity_ )
          >> xml::attribute( "max-capacity", maxCapacity_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::AccommodationInfos
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::AccommodationInfos::AccommodationInfos()
    : strName_        ( "" )
    , nominalCapacity_( 0 )
    , maxCapacity_    ( 0 )
{
    strName_.SetDataName( "le nom de la motivation" );
    strName_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::AccommodationInfos destructor
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::AccommodationInfos::~AccommodationInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::AccommodationInfos::GetNodeName
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::AccommodationInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::AccommodationInfos::GetItemName
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::AccommodationInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadInfrastructures
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadInfrastructures( xml::xistream& input )
{
    input >> xml::optional
          >> xml::start( "infrastructures" )
            >> xml::list( "infrastructure", *this, &ADN_Urban_Data::ReadInfrastructure )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadInfrastructure
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
void ADN_Urban_Data::ReadInfrastructure( xml::xistream& input  )
{
    vInfrastructures_.AddItem( new InfrastructureInfos( input ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteInfrastrucutures
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteInfrastructures( xml::xostream& output ) const
{
    if( !vInfrastructures_.empty() )
    {
        output << xml::start( "infrastructures" );
        for( CIT_InfrastructureInfos_Vector it = vInfrastructures_.begin(); it != vInfrastructures_.end(); ++it )
            ( *it )->WriteInfrastructure( output );
        output << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::AccommodationInfos::WriteInfrastructure
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
void ADN_Urban_Data::InfrastructureInfos::WriteInfrastructure( xml::xostream& output )
{
    std::string strData = strName_.GetData();
    output << xml::start( "infrastructure" )
        << xml::attribute( "name",  trim( strData ) );
    if( pSymbol_.GetData() )
        output << xml::attribute( "symbol", pSymbol_.GetData()->strName_ );
    else
        output << xml::attribute( "symbol", "" );
    if( !capacities_.empty() )
    {
        output << xml::start( "capacities" );
        for( CIT_CapacityMap it = capacities_.begin(); capacities_.end() != it; ++it )
        {
            if( it->second->bPresent_.GetData() )
            {
                output << xml::start( it->first );
                it->second->WriteArchive( output );
                output << xml::end;
            }
        }
        output << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadCapacityArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Urban_Data::InfrastructureInfos::ReadCapacityArchive( const std::string& type, xml::xistream& xis )
{
    IT_CapacityMap it = capacities_.find( type );
    if( it != capacities_.end() )
        it->second->ReadArchive( xis );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::InfrastructureInfos constructor
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::InfrastructureInfos::InfrastructureInfos( xml::xistream& input )
    : strName_( "" )
    , pSymbol_( ADN_Workspace::GetWorkspace().GetSymbols().GetData().GetSymbolsInfras(), 0, 0 )
{
    std::string symbol;
    capacities_[ "medical" ].reset( new ADN_Objects_Data::ADN_CapacityInfos_Medical() );
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "symbol", symbol )
          >> xml::start( "capacities" )
             >> xml::list( *this, &ADN_Urban_Data::InfrastructureInfos::ReadCapacityArchive )
          >> xml::end;
    ADN_Symbols_Data::SymbolsInfra* pSymbol = ADN_Workspace::GetWorkspace().GetSymbols().GetData().FindSymbolInfra( symbol );
    if( !pSymbol )
        return;
    pSymbol_ = pSymbol;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::InfrastructureInfos
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::InfrastructureInfos::InfrastructureInfos()
    : strName_( "" )
    , pSymbol_( ADN_Workspace::GetWorkspace().GetSymbols().GetData().GetSymbolsInfras(), 0, 0 )
{
    BindExistenceTo( &pSymbol_ );
    strName_.SetDataName( "le nom de l'infrastructure" );
    strName_.SetParentNode( *this );
    capacities_[ "medical" ].reset( new ADN_Objects_Data::ADN_CapacityInfos_Medical() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::InfrastructureInfos destructor
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::InfrastructureInfos::~InfrastructureInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::InfrastructureInfos::GetNodeName
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::InfrastructureInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::InfrastructureInfos::GetItemName
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::InfrastructureInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanTemplateInfos
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanTemplateInfos::UrbanTemplateInfos()
    : strName_       ( "" )
    , height_        ( 0 )
    , alpha_         ( 255 )
    , floor_         ( 0 )
    , parking_       ( 0 )
    , occupation_    ( 0. )
    , trafficability_( 0 )
    , ptrMaterial_   ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), 0 )
    , ptrRoofShape_  ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetRoofShapesInfos(), 0 )
{
    BindExistenceTo( &ptrMaterial_ );
    BindExistenceTo( &ptrRoofShape_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanTemplateInfos
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanTemplateInfos::UrbanTemplateInfos( xml::xistream& input )
    : strName_     ( input.attribute< std::string >( "name" ) )
    , ptrMaterial_ ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), 0 )
    , ptrRoofShape_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetRoofShapesInfos(), 0 )
{
    BindExistenceTo( &ptrMaterial_ );
    BindExistenceTo( &ptrRoofShape_ );
    std::string material, roofShape;
    unsigned int red, green, blue;
    double alpha;
    input >> xml::start( "architecture" )
              >> xml::attribute( "floor-number", floor_ )
              >> xml::attribute( "height", height_ )
              >> xml::attribute( "material", material )
              >> xml::attribute( "occupation", occupation_ )
              >> xml::attribute( "roof-shape", roofShape )
              >> xml::attribute( "trafficability", trafficability_ )
              >> xml::attribute( "parking-floors", parking_ )
          >> xml::end
          >> xml::start( "color" )
              >> xml::attribute( "alpha", alpha )
              >> xml::attribute( "blue", blue )
              >> xml::attribute( "green", green )
              >> xml::attribute( "red", red )
          >> xml::end
          >> xml::start( "usages" )
            >> xml::list( "usage", *this, &ADN_Urban_Data::UrbanTemplateInfos::ReadUsage )
          >> xml::end;
    QColor color( red, green, blue );
    color.setAlphaF( alpha );
    color_ = color.name().ascii();
    alpha_ = color.alpha();
    ADN_Urban_Data::UrbanMaterialInfos* pMaterial = ADN_Workspace::GetWorkspace().GetUrban().GetData().FindMaterial( material );
    if( !pMaterial )
        throw ADN_DataException( "Invalid data", tools::translate( "ADN_Urban_Data", "Urban data - Invalid material type '%1'" ).arg( material.c_str() ).ascii() );
    ptrMaterial_ = pMaterial;
    ADN_Urban_Data::RoofShapeInfos* pRoofShape = ADN_Workspace::GetWorkspace().GetUrban().GetData().FindRoofShape( roofShape );
    if( !pRoofShape )
        throw ADN_DataException( "Invalid data", tools::translate( "ADN_Urban_Data", "Urban data - Invalid roof-shape type '%1'" ).arg( roofShape.c_str() ).ascii() );
    ptrRoofShape_ = pRoofShape;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::~UrbanTemplateInfos
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanTemplateInfos::~UrbanTemplateInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::ReadUsage
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::UrbanTemplateInfos::ReadUsage( xml::xistream& xis )
{
    usages_.AddItem( new UsageTemplateInfos( xis ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetNodeName
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::UrbanTemplateInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetItemName
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::UrbanTemplateInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::Write
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::UrbanTemplateInfos::Write( xml::xostream& output )
{
    output << xml::start( "template" )
               << xml::attribute( "name", strName_ )
               << xml::start( "architecture" )
                   << xml::attribute( "floor-number", floor_ )
                   << xml::attribute( "height", height_ )
                   << xml::attribute( "material", ptrMaterial_.GetData()->strName_ )
                   << xml::attribute( "occupation", occupation_ )
                   << xml::attribute( "roof-shape", ptrRoofShape_.GetData()->strName_ )
                   << xml::attribute( "trafficability", trafficability_ )
                   << xml::attribute( "parking-floors", parking_ )
               << xml::end;
    QColor color( color_.GetData().c_str() );
    color.setAlpha( alpha_.GetData() );
    output     << xml::start( "color" )
                   << xml::attribute( "alpha", color.alphaF() )
                   << xml::attribute( "blue", color.blue() )
                   << xml::attribute( "green", color.green() )
                   << xml::attribute( "red", color.red() )
               << xml::end
               << xml::start( "usages" );
    for( CIT_UsageTemplateInfosVector it = usages_.begin(); usages_.end() != it; ++it )
            (*it)->Write( output );
    output     << xml::end
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::RoofShapeInfos::RoofShapeInfos()
    : strName_( "" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::RoofShapeInfos::RoofShapeInfos( xml::xistream& input )
    : strName_( input.attribute< std::string >( "name" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::~UrbanInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::RoofShapeInfos::~RoofShapeInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetNodeName
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::RoofShapeInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetItemName
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::RoofShapeInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteRoofShape
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::RoofShapeInfos::WriteRoofShape( xml::xostream& output )
{
    if( strName_.GetData().empty() )
        throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).ascii(), tools::translate( "Urban_Data", "RoofShape - Invalid roofShape type name" ).ascii() );
    std::string strData = strName_.GetData();
    output << xml::start( "roof-shape-type" )
               << xml::attribute( "name", trim( strData ) )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UsageTemplateInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::UsageTemplateInfos::UsageTemplateInfos()
    : strName_   ( "" )
    , proportion_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UsageTemplateInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::UsageTemplateInfos::UsageTemplateInfos( xml::xistream& input )
    : strName_   ( input.attribute< std::string >( "type" ) )
    , proportion_( static_cast< unsigned int >( input.attribute< double >( "proportion" ) * 100 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::~UsageTemplateInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::UsageTemplateInfos::~UsageTemplateInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::Write
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::UsageTemplateInfos::Write( xml::xostream& output )
{
    double proportion = static_cast< double >( proportion_.GetData() ) / 100.f;
    output << xml::start( "usage" )
           << xml::attribute( "type", strName_ )
           << xml::attribute( "proportion", proportion )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetNodeName
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::UsageTemplateInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetItemName
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
std::string ADN_Urban_Data::UsageTemplateInfos::GetItemName()
{
    return strName_.GetData();
}
