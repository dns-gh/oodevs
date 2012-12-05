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
#include "ADN_Project_Data.h"
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
    , vRoofShapes_           ()
    , vAccommodations_       ()
    , vInfrastructures_      ()
    , vTemplates_            ()
    , defaultNominalCapacity_( 0.1 )
    , defaultMaxCapacity_    ( 1 )
{
    // NOTHING
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

    template< typename T >
    bool operator()( T* infos ) const
    {
        return infos->strName_.GetData() == val_;
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
        template< typename T >
        std::string operator()( T& infos ) const
        {
            return infos.strName_.GetData();
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
    for( auto it = fileList.begin(); it != fileList.end(); ++it )
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
    for( auto it = fileList.begin(); it != fileList.end(); ++it )
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
        for( auto it = vTemplates_.begin(); it != vTemplates_.end(); ++it )
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
    for( auto it = vMaterials_.begin(); it != vMaterials_.end(); ++it )
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
    output   << xml::end
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanMaterialInfos* ADN_Urban_Data::UrbanMaterialInfos::CreateCopy()
{
    ADN_Urban_Data::UrbanMaterialInfos* result = new ADN_Urban_Data::UrbanMaterialInfos();
    result->vAttritionInfos_.Clear();
    for( IT_AttritionInfos_Vector it = vAttritionInfos_.begin(); it != vAttritionInfos_.end(); ++it )
        result->vAttritionInfos_.AddItem( ( *it )->CreateCopy() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos constructor
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanMaterialInfos::UrbanMaterialInfos( xml::xistream& input )
    : vAttritionInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos() )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::start( "attritions" )
              >> xml::list( "attrition", *this, &ADN_Urban_Data::UrbanMaterialInfos::ReadAttrition )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos
// Created: HBD 2010-11-17
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanMaterialInfos::UrbanMaterialInfos()
    : vAttritionInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos() )
{
    // NOTHING
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
    helpers::IT_AttritionInfos_Vector itAttrition = std::find_if( vAttritionInfos_.begin(), vAttritionInfos_.end(), helpers::AttritionInfos::Cmp( protection ) );
    if( itAttrition == vAttritionInfos_.end() )
        throw ADN_DataException( tr( "Invalid data" ).toStdString(), tr( "Equipment - Invalid armor type '%1'" ).arg( protection.c_str() ).toStdString() );
    ( *itAttrition )->ReadArchive( input );
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
    output << xml::start( "roof-shape-types" );
    for( auto it = vRoofShapes_.begin(); it != vRoofShapes_.end(); ++it )
        ( *it )->WriteRoofShape( output );
    output << xml::end;
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
            << xml::end;
    for( auto it = vAccommodations_.begin(); it != vAccommodations_.end(); ++it )
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
// Name: ADN_Urban_Data::AccommodationInfos::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
ADN_Urban_Data::AccommodationInfos* ADN_Urban_Data::AccommodationInfos::CreateCopy()
{
    ADN_Urban_Data::AccommodationInfos* result = new ADN_Urban_Data::AccommodationInfos();
    result->nominalCapacity_ = nominalCapacity_.GetData();
    result->maxCapacity_ = maxCapacity_.GetData();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::AccommodationInfos constructor
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::AccommodationInfos::AccommodationInfos( xml::xistream& input, const std::string& role )
    : ADN_RefWithName( role )
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
    : nominalCapacity_( 0 )
    , maxCapacity_    ( 0 )
{
    // NOTHING
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
    output << xml::start( "infrastructures" );
    for( auto it = vInfrastructures_.begin(); it != vInfrastructures_.end(); ++it )
        ( *it )->WriteInfrastructure( output );
    output << xml::end;
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

    if( bMedical_.GetData() )
        output << xml::attribute( "medical", bMedical_.GetData() );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::InfrastructureInfos::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
ADN_Urban_Data::InfrastructureInfos* ADN_Urban_Data::InfrastructureInfos::CreateCopy()
{
    ADN_Urban_Data::InfrastructureInfos* result = new ADN_Urban_Data::InfrastructureInfos();

    result->pSymbol_ = pSymbol_.GetData();
    result->bMedical_ = bMedical_.GetData();

    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::InfrastructureInfos constructor
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::InfrastructureInfos::InfrastructureInfos( xml::xistream& input )
    : pSymbol_( ADN_Workspace::GetWorkspace().GetSymbols().GetData().GetSymbolsInfras(), 0 )
    , bMedical_( false )
{
    std::string symbol;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "symbol", symbol )
          >> xml::optional >> xml::attribute( "medical", bMedical_ );
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
    : pSymbol_( ADN_Workspace::GetWorkspace().GetSymbols().GetData().GetSymbolsInfras(), 0 )
    , bMedical_( false )
{
    // NOTHING
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
// Name: ADN_Urban_Data::T_UsageTemplateInfosVector
// Created: NPT 2012-07-24
// -----------------------------------------------------------------------------
ADN_Urban_Data::T_UsageTemplateInfosVector::T_UsageTemplateInfosVector( ADN_Ref_ABC* parent, bool bAutoRef /*= true*/ )
    : ADN_Type_Vector_ABC< UsageTemplateInfos >( bAutoRef )
    , parent_( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::~T_UsageTemplateInfosVector
// Created: NPT 2012-07-24
// -----------------------------------------------------------------------------
ADN_Urban_Data::T_UsageTemplateInfosVector::~T_UsageTemplateInfosVector()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::AddItemPrivate
// Created: NPT 2012-07-24
// -----------------------------------------------------------------------------
bool ADN_Urban_Data::T_UsageTemplateInfosVector::AddItemPrivate( void* pObj )
{
    if( parent_ && pObj )
        parent_->BindExistenceTo( &static_cast< UsageTemplateInfos* >( pObj )->accommodation_ );
    return ADN_Type_Vector_ABC< UsageTemplateInfos >::AddItemPrivate( pObj );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanTemplateInfos
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanTemplateInfos::UrbanTemplateInfos()
    : height_        ( 0 )
    , alpha_         ( 255 )
    , floor_         ( 0 )
    , parking_       ( 0 )
    , occupation_    ( 0. )
    , trafficability_( 0 )
    , ptrMaterial_   ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), 0 )
    , ptrRoofShape_  ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetRoofShapesInfos(), 0 )
    , usages_        ( this )
{
    BindExistenceTo( &ptrMaterial_ );
    BindExistenceTo( &ptrRoofShape_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanTemplateInfos
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanTemplateInfos::UrbanTemplateInfos( xml::xistream& input )
    : ADN_RefWithName( input.attribute< std::string >( "name" ) )
    , ptrMaterial_ ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), 0 )
    , ptrRoofShape_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetRoofShapesInfos(), 0 )
    , usages_      ( this )
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
    color_ = color.name().toStdString();
    alpha_ = color.alpha();
    ADN_Urban_Data::UrbanMaterialInfos* pMaterial = ADN_Workspace::GetWorkspace().GetUrban().GetData().FindMaterial( material );
    if( !pMaterial )
        throw ADN_DataException( "Invalid data", tools::translate( "ADN_Urban_Data", "Urban data - Invalid material type '%1'" ).arg( material.c_str() ).toStdString() );
    ptrMaterial_ = pMaterial;
    ADN_Urban_Data::RoofShapeInfos* pRoofShape = ADN_Workspace::GetWorkspace().GetUrban().GetData().FindRoofShape( roofShape );
    if( !pRoofShape )
        throw ADN_DataException( "Invalid data", tools::translate( "ADN_Urban_Data", "Urban data - Invalid roof-shape type '%1'" ).arg( roofShape.c_str() ).toStdString() );
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
    UsageTemplateInfos* usageTemplate = new UsageTemplateInfos( xis );
    usages_.AddItem( usageTemplate );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::Write
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::UrbanTemplateInfos::Write( xml::xostream& output )
{
    if( !ptrMaterial_.GetData() )
        throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).toStdString(), tools::translate( "Urban_Data", "Material attribute is empty for '%1' template." ).arg( strName_.GetData().c_str() ).toStdString() );
    if( !ptrRoofShape_.GetData() )
        throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).toStdString(), tools::translate( "Urban_Data", "RoofShape attribute is empty for '%1' template." ).arg( strName_.GetData().c_str() ).toStdString() );
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
    for( auto it = usages_.begin(); usages_.end() != it; ++it )
            (*it)->Write( output );
    output     << xml::end
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanTemplateInfos::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanTemplateInfos* ADN_Urban_Data::UrbanTemplateInfos::CreateCopy()
{
    ADN_Urban_Data::UrbanTemplateInfos* result = new ADN_Urban_Data::UrbanTemplateInfos();

    result->strName_= strName_.GetData();
    result->color_ = color_.GetData();
    result->alpha_ = alpha_.GetData();
    result->height_ = height_.GetData();
    result->floor_ = floor_.GetData();
    result->parking_ = parking_.GetData();
    result->occupation_ = occupation_.GetData();
    result->trafficability_ = trafficability_.GetData();
    result->ptrMaterial_ = ptrMaterial_.GetData();
    result->ptrRoofShape_ = ptrRoofShape_.GetData();
    for( auto it = usages_.begin(); it != usages_.end(); ++it )
        result->usages_.AddItem( ( *it )->CreateCopy() );

    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::RoofShapeInfos::RoofShapeInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::RoofShapeInfos::RoofShapeInfos( xml::xistream& input )
    : ADN_RefWithName( input.attribute< std::string >( "name" ) )
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
// Name: ADN_Urban_Data::WriteRoofShape
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::RoofShapeInfos::WriteRoofShape( xml::xostream& output )
{
    if( strName_.GetData().empty() )
        throw ADN_DataException( tools::translate( "Urban_Data", "Invalid data" ).toStdString(), tools::translate( "Urban_Data", "RoofShape - Invalid roofShape type name" ).toStdString() );
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
    : accommodation_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos(), 0 )
    , proportion_   ( 100 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UsageTemplateInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::UsageTemplateInfos::UsageTemplateInfos( xml::xistream& input )
    : accommodation_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos(), 0 )
    , proportion_   ( static_cast< unsigned int >( input.attribute< double >( "proportion" ) * 100 ) )
{
    std::string type = input.attribute< std::string >( "type" );
    ADN_Urban_Data::AccommodationInfos* accomodation = ADN_Workspace::GetWorkspace().GetUrban().GetData().FindAccommodation( type );
    if( !accomodation )
        throw ADN_DataException( "Invalid data", tools::translate( "ADN_Urban_Data", "Urban data - Invalid accomodation type '%1'" ).arg( type.c_str() ).toStdString() );
    accommodation_ = accomodation;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UsageTemplateInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::UsageTemplateInfos::UsageTemplateInfos( ADN_Urban_Data::AccommodationInfos& accomodation, ADN_Type_Int proportion )
    : accommodation_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos(), &accomodation )
    , proportion_   ( proportion.GetData() )
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
           << xml::attribute( "type", accommodation_.GetData()->strName_ )
           << xml::attribute( "proportion", proportion )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UsageTemplateInfos::CreateCopy
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
ADN_Urban_Data::UsageTemplateInfos* ADN_Urban_Data::UsageTemplateInfos::CreateCopy()
{
    ADN_Urban_Data::UsageTemplateInfos* result = new ADN_Urban_Data::UsageTemplateInfos();

    result->accommodation_ = accommodation_.GetData();
    result->proportion_ = proportion_.GetData();

    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetUrbanTemplateThatUse
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
QStringList ADN_Urban_Data::GetUrbanTemplateThatUse( UrbanMaterialInfos& infos )
{
    QStringList result;
    for( auto it = vTemplates_.begin(); it != vTemplates_.end(); ++it )
        if( ( *it )->ptrMaterial_.GetData()->strName_.GetData() == infos.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetUrbanTemplateThatUse
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
QStringList ADN_Urban_Data::GetUrbanTemplateThatUse( RoofShapeInfos& infos )
{
    QStringList result;
    for( auto it = vTemplates_.begin(); it != vTemplates_.end(); ++it )
        if( ( *it )->ptrRoofShape_.GetData()->strName_.GetData() == infos.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::GetUrbanTemplateThatUse
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
QStringList ADN_Urban_Data::GetUrbanTemplateThatUse( AccommodationInfos& infos )
{
    QStringList result;
    for( auto it = vTemplates_.begin(); it != vTemplates_.end(); ++it )
        for( CIT_UsageTemplateInfosVector itUsage = ( *it )->usages_.begin(); itUsage != ( *it )->usages_.end(); ++itUsage )
            if( ( *itUsage )->accommodation_.GetData()->strName_.GetData() == infos.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
    return result;
}
