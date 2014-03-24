// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Urban_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include "clients_kernel/XmlTranslations.h"

using namespace helpers;

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data constructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_Data::ADN_Urban_Data()
    : ADN_Data_ABC           ( eUrban )
    , vMaterials_            ()
    , vRoofShapes_           ()
    , vAccommodations_       ()
    , vInfrastructures_      ()
    , vTemplates_            ()
    , defaultNominalCapacity_( 0.1 )
    , defaultMaxCapacity_    ( 1 )
    , templateTranslations_  ( new kernel::XmlTranslations() )
{
    vMaterials_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
    vRoofShapes_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
    vAccommodations_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
    vInfrastructures_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
    vTemplates_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data destructor
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
ADN_Urban_Data::~ADN_Urban_Data()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FilesNeeded
// Created: SLG 2010-03-08
//-----------------------------------------------------------------------------
void ADN_Urban_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrban_ );
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrbanTemplates_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::Save
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::Save() const
{
    tools::Path::T_Paths fileList;
    FilesNeeded( fileList );
    for( auto it = fileList.begin(); it != fileList.end(); ++it )
    {
        const tools::Path file = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() / *it;
        if( *it == ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrban_ )
            SaveFile( file, boost::bind( &ADN_Urban_Data::WriteArchive, this, _1 ) );
        if( *it == ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrbanTemplates_ )
        {
            file.Parent().CreateDirectories();
            tools::Xofstream output( file );
            WriteTemplates( output );
            SaveTranslations( file, templateTranslations_.get() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::Load
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::Load( const tools::Loader_ABC& fileLoader )
{
    tools::Path::T_Paths fileList;
    FilesNeeded( fileList );
    for( auto it = fileList.begin(); it != fileList.end(); ++it )
    {
        const tools::Path file = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / (*it);
        if( *it == ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrban_ )
            LoadFile( fileLoader, file, boost::bind( &ADN_Urban_Data::ReadArchive, this, _1 ) );
        else if( *it == ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szUrbanTemplates_ )
        {
            LoadTranslations( file, templateTranslations_.get() );
            fileLoader.LoadFile( file, boost::bind( &ADN_Urban_Data::ReadTemplates, this, _1 ) );
        }
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

    vMaterials_.CheckValidity();
    vRoofShapes_.CheckValidity();
    vAccommodations_.CheckValidity();
    vInfrastructures_.CheckValidity();
    vTemplates_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::FixConsistency
// Created: ABR 2013-10-10
// -----------------------------------------------------------------------------
bool ADN_Urban_Data::FixConsistency()
{
    ADN_Data_ABC::FixConsistency();
    templateTranslations_->MergeDuplicateTranslations();
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::WriteArchive
// Created: SLG 2010-03-08
// -----------------------------------------------------------------------------
void ADN_Urban_Data::WriteArchive( xml::xostream& output ) const
{
    if( vMaterials_.GetErrorStatus() == eError ||
        vRoofShapes_.GetErrorStatus() == eError ||
        vAccommodations_.GetErrorStatus() == eError ||
        vInfrastructures_.GetErrorStatus() == eError ||
        vTemplates_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "urban" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "UrbanTypes" );
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
void ADN_Urban_Data::WriteTemplates( xml::xostream& output ) const
{
    output << xml::start( "templates" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "UrbanTemplates" );
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
void ADN_Urban_Data::ReadMaterial( xml::xistream& input )
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
void ADN_Urban_Data::UrbanMaterialInfos::WriteMaterial( xml::xostream& output ) const
{
    output << xml::start( "material-composition-type" )
           << xml::attribute( "name", *this )
              << xml::start( "attritions" );
    for( auto it = vAttritionInfos_.begin(); it != vAttritionInfos_.end(); ++it )
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
    for( auto it = vAttritionInfos_.begin(); it != vAttritionInfos_.end(); ++it )
        result->vAttritionInfos_.AddItem( ( *it )->CreateCopy() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos constructor
// Created: SLG 2010-07-01
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanMaterialInfos::UrbanMaterialInfos( xml::xistream& input )
    : vAttritionInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos() )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eUrban, "material-composition-types" ) );
    input >> xml::attribute( "name", *this )
          >> xml::start( "attritions" )
              >> xml::list( "attrition", *this, &ADN_Urban_Data::UrbanMaterialInfos::ReadAttrition )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanMaterialInfos
// Created: HBD 2010-11-17
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanMaterialInfos::UrbanMaterialInfos()
    : vAttritionInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos() )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eUrban, "material-composition-types" ) );
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
    auto itAttrition = std::find_if( vAttritionInfos_.begin(), vAttritionInfos_.end(), helpers::AttritionInfos::Cmp( protection ) );
    if( itAttrition == vAttritionInfos_.end() )
        throw MASA_EXCEPTION( tools::translate( "ADN_Urban_Data", "Equipment - Invalid armor type '%1'" ).arg( protection.c_str() ).toStdString() );
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
        vAccommodations_.AddItem( new AccommodationInfos( input ) );
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
void ADN_Urban_Data::AccommodationInfos::WriteAccommodation( xml::xostream& output ) const
{
    output << xml::start( "accommodation" )
           << xml::attribute( "role", *this )
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
ADN_Urban_Data::AccommodationInfos::AccommodationInfos( xml::xistream& input )
    : nominalCapacity_( 0 )
    , maxCapacity_    ( 0 )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eUrban, "accomodations" ) );
    input >> xml::attribute( "role", *this )
          >> xml::attribute( "nominal-capacity", nominalCapacity_ )
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
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eUrban, "accomodations" ) );
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
void ADN_Urban_Data::ReadInfrastructure( xml::xistream& input )
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
void ADN_Urban_Data::InfrastructureInfos::WriteInfrastructure( xml::xostream& output ) const
{
    output << xml::start( "infrastructure" )
        << xml::attribute( "name", *this );
    output << xml::attribute( "symbol", pSymbol_ );

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
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eUrban, "infrastructures" ) );
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "symbol", pSymbol_ )
          >> xml::optional >> xml::attribute( "medical", bMedical_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::InfrastructureInfos
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::InfrastructureInfos::InfrastructureInfos()
    : pSymbol_( ADN_Workspace::GetWorkspace().GetSymbols().GetData().GetSymbolsInfras(), 0 )
    , bMedical_( false )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eUrban, "infrastructures" ) );
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
        parent_->BindExistenceTo( static_cast< ADN_CrossedRef< AccommodationInfos >* >( pObj ) );
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
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetTemplateContext( "templates" ) );
    BindExistenceTo( &ptrMaterial_ );
    BindExistenceTo( &ptrRoofShape_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanTemplateInfos
// Created: LGY 2011-09-20
// -----------------------------------------------------------------------------
ADN_Urban_Data::UrbanTemplateInfos::UrbanTemplateInfos( xml::xistream& input )
    : ptrMaterial_ ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), 0 )
    , ptrRoofShape_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetRoofShapesInfos(), 0 )
    , usages_      ( this )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetTemplateContext( "templates" ) );
    BindExistenceTo( &ptrMaterial_ );
    BindExistenceTo( &ptrRoofShape_ );
    unsigned int red, green, blue;
    double alpha;
    input >> xml::attribute( "name", *this )
          >> xml::start( "architecture" )
              >> xml::attribute( "floor-number", floor_ )
              >> xml::attribute( "height", height_ )
              >> xml::attribute( "material", ptrMaterial_ )
              >> xml::attribute( "occupation", occupation_ )
              >> xml::attribute( "roof-shape", ptrRoofShape_ )
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
    occupation_ = occupation_.GetData() * 100.f;
    QColor color( red, green, blue );
    color.setAlphaF( alpha );
    color_ = color.name().toStdString();
    alpha_ = color.alpha();
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
void ADN_Urban_Data::UrbanTemplateInfos::Write( xml::xostream& output ) const
{
    output << xml::start( "template" )
               << xml::attribute( "name", *this )
               << xml::start( "architecture" )
                   << xml::attribute( "floor-number", floor_ )
                   << xml::attribute( "height", height_ )
                   << xml::attribute( "material", ptrMaterial_ )
                   << xml::attribute( "occupation", occupation_.GetData() / 100.f )
                   << xml::attribute( "roof-shape", ptrRoofShape_ )
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
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eUrban, "roof-shapes" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UrbanInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::RoofShapeInfos::RoofShapeInfos( xml::xistream& input )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eUrban, "roof-shapes" ) );
    input >> xml::attribute( "name", *this );
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
void ADN_Urban_Data::RoofShapeInfos::WriteRoofShape( xml::xostream& output ) const
{
    output << xml::start( "roof-shape-type" )
               << xml::attribute( "name", *this )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::CheckDatabaseValidity
// Created: LDC 2014-03-21
// -----------------------------------------------------------------------------
void ADN_Urban_Data::RoofShapeInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( strName_.GetKey().empty() )
        checker.AddError( eInvalidRoof, "", eUrban );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UsageTemplateInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::UsageTemplateInfos::UsageTemplateInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos(), 0, true )
    , proportion_( 100 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UsageTemplateInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::UsageTemplateInfos::UsageTemplateInfos( xml::xistream& input )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos(), 0, true )
    , proportion_   ( static_cast< unsigned int >( input.attribute< double >( "proportion" ) * 100 ) )
{
    input >> xml::attribute( "type", *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::UsageTemplateInfos
// Created: LGY 2011-09-21
// -----------------------------------------------------------------------------
ADN_Urban_Data::UsageTemplateInfos::UsageTemplateInfos( ADN_Urban_Data::AccommodationInfos& accomodation, ADN_Type_Int proportion )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos(), &accomodation )
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
void ADN_Urban_Data::UsageTemplateInfos::Write( xml::xostream& output ) const
{
    double proportion = static_cast< double >( proportion_.GetData() ) / 100.f;
    output << xml::start( "usage" )
             << xml::attribute( "type", *this )
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

    result->SetCrossedElement( GetCrossedElement() );
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
    {
        ADN_Urban_Data::UrbanMaterialInfos* infosMat = ( *it )->ptrMaterial_.GetData();
        if( infosMat && infosMat->strName_.GetData() == infos.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    }
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
    {
        ADN_Urban_Data::RoofShapeInfos* infosRoof = ( *it )->ptrRoofShape_.GetData();
        if( infosRoof && infosRoof->strName_.GetData() == infos.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    }
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
        for( auto itUsage = ( *it )->usages_.begin(); itUsage != ( *it )->usages_.end(); ++itUsage )
        {
            AccommodationInfos* infosAcc = ( *itUsage )->GetCrossedElement();
            if( infosAcc && infosAcc->strName_.GetData() == infos.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
        }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::CheckDatabaseValidity
// Created: JSR 2013-04-19
// -----------------------------------------------------------------------------
void ADN_Urban_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = vTemplates_.begin(); it != vTemplates_.end(); ++it )
    {
        ( *it )->ptrMaterial_.CheckValidity( checker, ( *it )->strName_.GetData(), eUrban, -1, tools::translate( "ADN_Urban_Data", "Material" ).toStdString() );
        ( *it )->ptrRoofShape_.CheckValidity( checker, ( *it )->strName_.GetData(), eUrban, -1, tools::translate( "ADN_Urban_Data", "Roof shape" ).toStdString() );
        for( auto itUsage = ( *it )->usages_.begin(); itUsage != ( *it )->usages_.end(); ++itUsage )
            ( *itUsage )->CheckValidity( checker, ( *it )->strName_.GetData(), eUrban, -1, tools::translate( "ADN_Urban_Data", "Accomodation" ).toStdString() );
    }
    for( auto it = vInfrastructures_.begin(); it != vInfrastructures_.end(); ++it )
        ( *it )->pSymbol_.CheckValidity( checker, ( *it )->strName_.GetData(), eUrban, -1, tools::translate( "ADN_Urban_Data", "Infrastructure" ).toStdString() );
    for( auto it = vRoofShapes_.begin(); it != vRoofShapes_.end(); ++it )
        ( *it )->CheckDatabaseValidity( checker );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< kernel::Context >& ADN_Urban_Data::GetTemplateContext
// Created: ABR 2013-10-23
// -----------------------------------------------------------------------------
const boost::shared_ptr< kernel::Context >& ADN_Urban_Data::GetTemplateContext( const std::string& context )
{
    return templateTranslations_->GetContext( context );
}

// -----------------------------------------------------------------------------
// Name: ADN_Urban_Data::function< bool
// Created: ABR 2013-10-23
// -----------------------------------------------------------------------------
bool ADN_Urban_Data::ApplyOnTranslations( const boost::function< bool( kernel::LocalizedString& ) >& functor ) const
{
    if( ADN_Data_ABC::ApplyOnTranslations( functor ) )
        return true;
    return templateTranslations_->ApplyOnTranslations( functor );
}
