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
#include "ADN_Composantes_Data.h"

#include "ADN_Tools.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Composantes_Data.h"

#include <xeumeuleu/xml.hpp>

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
    input >> xml::attribute( "type", strTerrain )
          >> xml::attribute( "value", nScore_ );
    nLocation_ = ADN_Tr::ConvertToLocation( strTerrain );
}

// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ScoreLocationInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "terrain" )
             << xml::attribute( "type", ADN_Tr::ConvertFromLocation( nLocation_.GetData() ) )
             << xml::attribute( "value", nScore_ )
           << xml::end;
}

// =============================================================================
// PopulationAttritionInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::PopulationAttritionInfos::PopulationAttritionInfos
// Created: SBO 2006-04-25
// -----------------------------------------------------------------------------
/*ADN_Objects_Data::PopulationAttritionInfos::PopulationAttritionInfos()
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
    input >> xml::optional >> xml::start( "population-attrition" )
                >> xml::attribute( "surface", rSurface_ )
                >> xml::attribute( "ph", rPh_ )
            >> xml::end;
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
            << xml::end;
}
*/

namespace
{
    std::vector< std::string > VectorBuilder( const char* choice[], uint size )
    {
        std::vector< std::string > stack( size );
        for ( int i = 0; i < int( size ); ++i )
            stack[ i ] = std::string( choice[ i ] );
        return stack;
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Buildable
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Buildable::ADN_CapacityInfos_Buildable()
{
    categories_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Buildable::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );

    xis >> xml::optional
        >> xml::start( "resources" )
            >> xml::list( "dotation", *this, &ADN_Objects_Data::ADN_CapacityInfos_Buildable::ReadDotation )
        >> xml::end;
}

void ADN_Objects_Data::ADN_CapacityInfos_Buildable::ReadDotation( xml::xistream& xis )
{
    std::string dotation;
    int quantity;
    xis >> xml::attribute( "name", dotation )
        >> xml::attribute( "count", quantity );
    if( dotation != "" )
    {
        ADN_Equipement_Data::CategoryInfo* category = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( dotation );
        if( category == 0 )
            throw ADN_DataException( "Donnée invalide", "Dotation invalide : " + dotation );
        ADN_Composantes_Data::CategoryInfos* infos = new ADN_Composantes_Data::CategoryInfos( category->parentDotation_ );
        infos->ptrCategory_ = category;
        infos->rNbr_ = quantity;
        categories_.AddItem( infos );

//        ADN_Composantes_Data::CategoryInfos* infos = reinterpret_cast< ADN_Composantes_Data::CategoryInfos* >( categories_.back() );

    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Buildable::WriteArchive( xml::xostream& xos )
{
    if( categories_.size() == 0 )
        return;
    xos << xml::start( "resources" );
    for( CIT_Categories it = categories_.begin(); it != categories_.end(); ++it )
    {
        ADN_Composantes_Data::CategoryInfos* infos = reinterpret_cast< ADN_Composantes_Data::CategoryInfos* >(*it);
        xos << xml::start( "dotation" )
                << xml::attribute( "name", infos->ptrCategory_.GetData()->strName_ ) << xml::attribute( "count", infos->rNbr_ )
            << xml::end;
    }
    xos << xml::end;
}

// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Improvable
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Improvable::ADN_CapacityInfos_Improvable()
{
    categories_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Improvable::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );

    xis >> xml::optional
         >> xml::start( "resources" )
            >> xml::list( "dotation", *this, &ADN_Objects_Data::ADN_CapacityInfos_Improvable::ReadDotation )
        >> xml::end;
}

void ADN_Objects_Data::ADN_CapacityInfos_Improvable::ReadDotation( xml::xistream& xis )
{
    std::string dotation;
    int quantity;
    xis >> xml::attribute( "name", dotation )
        >> xml::attribute( "count", quantity );
    if( dotation != "" )
    {
        ADN_Equipement_Data::CategoryInfo* category = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( dotation );
        if( category == 0 )
            throw ADN_DataException( "Donnée invalide", "Dotation invalide : " + dotation );
        ADN_Composantes_Data::CategoryInfos* infos = new ADN_Composantes_Data::CategoryInfos( category->parentDotation_ );
        infos->ptrCategory_ = category;
        infos->rNbr_ = quantity;
        categories_.AddItem( infos );
//        ADN_Composantes_Data::CategoryInfos* infos = reinterpret_cast< ADN_Composantes_Data::CategoryInfos* >( categories_.back() );
//        infos->rNbr_ = quantity;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Improvable::WriteArchive( xml::xostream& xos )
{
    if( categories_.size() == 0 )
        return;
    xos << xml::start( "resources" );
    for( CIT_Categories it = categories_.begin(); it != categories_.end(); ++it )
    {
        ADN_Composantes_Data::CategoryInfos* infos = reinterpret_cast< ADN_Composantes_Data::CategoryInfos* >(*it);
        xos << xml::start( "dotation" )
                << xml::attribute( "name", infos->ptrCategory_.GetData()->strName_ ) << xml::attribute( "count", infos->rNbr_ )
            << xml::end;
    }
    xos << xml::end;
}


const char* ADN_Objects_Data::ADN_CapacityInfos_Constructor::choices_[] = { "raw", "density" };

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Constructor
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Constructor::ADN_CapacityInfos_Constructor()
    : nDefaultConsumption_ ( eWorking )
    , unitType_ ( VectorBuilder( choices_, 2 ) )
    , ptrBuildable_ ( new ADN_CapacityInfos_Buildable() )
    , ptrImprovable_ ( new ADN_CapacityInfos_Improvable() )
{
    nDefaultConsumption_.SetParentNode( *this );
    unitType_.SetParentNode( *this );
    ptrBuildable_->SetParentNode( *this );
    ptrImprovable_->SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Constructor::ReadArchive( xml::xistream& xis )
{
    ADN_TypeCapacity_Infos::ReadArchive( xis );

    std::string strComsuption;
    xis >> xml::attribute( "default-consumption-mode", strComsuption );
    nDefaultConsumption_ = ADN_Tr::ConvertToConsumptionType( strComsuption );
    unitType_ = xis.attribute< std::string >( "unit-type" );
    xis >> xml::optional >> xml::list( ADN_CapacityInfos_Buildable::TAG, *ptrBuildable_, &ADN_CapacityInfos_Buildable::ReadArchive );
    xis >> xml::optional >> xml::list( ADN_CapacityInfos_Improvable::TAG, *ptrImprovable_, &ADN_CapacityInfos_Improvable::ReadArchive );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Constructor::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "default-consumption-mode", ADN_Tr::ConvertFromConsumptionType( nDefaultConsumption_.GetData() ) )
        << xml::attribute( "unit-type", unitType_.GetData()==""?ADN_Objects_Data::ADN_CapacityInfos_Constructor::choices_[0]:unitType_.GetData() );
    if( ptrBuildable_->bPresent_.GetData() )
    {
        xos << xml::start( ADN_CapacityInfos_Buildable::TAG );
            ptrBuildable_->WriteArchive( xos );
        xos << xml::end;
    }
    if( ptrImprovable_->bPresent_.GetData() )
    {
        xos << xml::start( ADN_CapacityInfos_Improvable::TAG );
        ptrImprovable_->WriteArchive( xos );
        xos << xml::end;
    }
}


// =============================================================================
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Avoidable
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Avoidable::ADN_CapacityInfos_Avoidable()
    : fields_("Distance"), rDistance_ ( 0. )
{
    rDistance_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Avoidable::ReadArchive( xml::xistream& xis )
{
    ADN_TypeCapacity_Infos::ReadArchive( xis );
    rDistance_ = xis.attribute< double >( "distance" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Avoidable::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "distance", rDistance_.GetData() );
}

// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Bypassable
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Bypassable::ADN_CapacityInfos_Bypassable()
    : fields_( "Speed" ), rSpeed_ ( 0. )
{
    rSpeed_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Bypassable::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::attribute( "bypass-speed", rSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Bypassable::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "bypass-speed", rSpeed_ );
}

//
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Mobility
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Mobility::ADN_CapacityInfos_Mobility()
    : rDefaultSpeed_ ( 0. )
    , nSpeedModifier_ ( eSpeedImpact_AuPlusLent )
    , rMaxAgentSpeed_ ( 0 )
{
    rDefaultSpeed_.SetParentNode( *this );
    nSpeedModifier_.SetParentNode( *this );
    rMaxAgentSpeed_.SetParentNode( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Mobility::ReadArchive( xml::xistream& xis )
{
    ADN_TypeCapacity_Infos::ReadArchive( xis );
    std::string impact;
    xis >> xml::attribute( "default-speed", rDefaultSpeed_ )
        >> xml::attribute( "unit-speed-impact-mode", impact );
    nSpeedModifier_ = ADN_Tr::ConvertToSpeedImpact( impact );
    if( nSpeedModifier_ == eSpeedImpact_VitesseMaxAgent )
        xis >> xml::attribute( "max-unit-percentage-speed", rMaxAgentSpeed_ );

}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Mobility::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "default-speed", rDefaultSpeed_ )
        << xml::attribute( "unit-speed-impact-mode", ADN_Tr::ConvertFromSpeedImpact( nSpeedModifier_.GetData() ) )
        << xml::attribute( "max-unit-percentage-speed", rMaxAgentSpeed_ );
}



//! @name ADN_CapacityInfos_Attrition
//@{

ADN_Objects_Data::ADN_CapacityInfos_Attrition::ADN_CapacityInfos_Attrition()
: ammoCategory_( (ADN_Equipement_Data::T_AmmoCategoryInfo_Vector&)ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition ).categories_, 0, "" )
, useAmmo_( false )
{
    category_.SetParentNode( *this );
    ammoCategory_.SetParentNode( *this );
    useAmmo_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_Attrition::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    std::string dotation( xis.attribute< std::string >( "category" ) );
    if( dotation != "" )
    {
        ADN_Equipement_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( dotation );
        if( pCategory == 0 )
            throw ADN_DataException( "Donnée invalide", "Dotation invalide : " + dotation );
        category_ = pCategory;
        ADN_Equipement_Data::AmmoCategoryInfo* ptrAmmo = dynamic_cast<ADN_Equipement_Data::AmmoCategoryInfo*>( pCategory );
        ammoCategory_ = ptrAmmo;
        useAmmo_ = ( pCategory->strName_.GetData() == "munition" );
    }
}

void ADN_Objects_Data::ADN_CapacityInfos_Attrition::WriteArchive( xml::xostream& xos )
{
    if( useAmmo_.GetData() == true )
        xos << xml::attribute( "category", ammoCategory_.GetData()->strName_ );
    else
        xos << xml::attribute( "category", category_.GetData()->strName_ );
}
//@}

//! @name ADN_CapacityInfos_Contamination
//@{
ADN_Objects_Data::ADN_CapacityInfos_Contamination::ADN_CapacityInfos_Contamination()
    : max_toxic_ ( 0 )
{
    max_toxic_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_Contamination::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::attribute( "max-toxic", max_toxic_ );
}

void ADN_Objects_Data::ADN_CapacityInfos_Contamination::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "max-toxic", max_toxic_ );
}
//@}


//! @name ADN_CapacityInfos_InteractionHeight
//@{
ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight::ADN_CapacityInfos_InteractionHeight()
    : height_( 0 )
{
    height_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::attribute( "height", height_ );
}

void ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "height", height_ );
}
//@}


//! @name ADN_CapacityInfos_Intoxication
//@{
ADN_Objects_Data::ADN_CapacityInfos_Intoxication::ADN_CapacityInfos_Intoxication()
    : max_toxic_ ( 0 )
{
    max_toxic_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_Intoxication::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::attribute( "max-toxic", max_toxic_ );
}

void ADN_Objects_Data::ADN_CapacityInfos_Intoxication::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "max-toxic", max_toxic_ );
}
//@}

//! @name ADN_CapacityInfos_Decontamination
//@{
ADN_Objects_Data::ADN_CapacityInfos_Decontamination::ADN_CapacityInfos_Decontamination()
{

}
void ADN_Objects_Data::ADN_CapacityInfos_Decontamination::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
}
void ADN_Objects_Data::ADN_CapacityInfos_Decontamination::WriteArchive( xml::xostream& /* xos */ )
{
}
//@}

//! @name ADN_CapacityInfos_Population
//@{
ADN_Objects_Data::ADN_CapacityInfos_Population::ADN_CapacityInfos_Population()
: density_( 0 )
{
    density_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_Population::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::attribute( "density", density_ );
}

void ADN_Objects_Data::ADN_CapacityInfos_Population::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "density", density_ );
}

//@}

//! @name ADN_CapacityInfos_Propagation
//@{

const char* ADN_Objects_Data::ADN_CapacityInfos_Propagation::choices_[] = { "input", "fire", "cloud" };

ADN_Objects_Data::ADN_CapacityInfos_Propagation::ADN_CapacityInfos_Propagation()
    : model_ ( VectorBuilder( choices_, 3 ) )
{
    model_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_Propagation::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    model_ = xis.attribute< std::string >( "model" );
}
void ADN_Objects_Data::ADN_CapacityInfos_Propagation::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "model", model_.GetData() );
}
//@}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Protection
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Protection::ADN_CapacityInfos_Protection()
    : max_size_( 1 )
    , geniePrepared_( false )
{
    max_size_.SetParentNode( *this );
    geniePrepared_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Protection::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    max_size_ = xis.attribute< int >( "max-size" );
    geniePrepared_ = xis.attribute< bool >( "geniePrepared" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Protection::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "max-size", max_size_.GetData() )
        << xml::attribute( "geniePrepared", geniePrepared_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Workable
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Workable::ADN_CapacityInfos_Workable()
    : worker_ ( 0 )
{
    worker_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Workable::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    worker_ = xis.attribute< int >( "max-animator" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Workable::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "max-animator", worker_.GetData() );
}


//! @name ADN_CapacityInfos_Medical
//@{
ADN_Objects_Data::ADN_CapacityInfos_Medical::ADN_CapacityInfos_Medical()
    : emergencyBedsRate_ ( 0 )
    , emergencyDoctorsRate_ ( 0 )
    , nightDoctorsRate_ ( 0 )
{
    emergencyBedsRate_.SetParentNode( *this );
    emergencyDoctorsRate_.SetParentNode( *this );
    nightDoctorsRate_.SetParentNode( *this );
}
void ADN_Objects_Data::ADN_CapacityInfos_Medical::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::attribute( "emergency-plan-beds", emergencyBedsRate_ );
    xis >> xml::attribute( "emergency-plan-doctors", emergencyDoctorsRate_ );
    xis >> xml::attribute( "night-doctors", nightDoctorsRate_ );
}
void ADN_Objects_Data::ADN_CapacityInfos_Medical::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "emergency-plan-beds", emergencyBedsRate_ );
    xos << xml::attribute( "emergency-plan-doctors", emergencyDoctorsRate_ );
    xos << xml::attribute( "night-doctors", nightDoctorsRate_ );
}
//@}


//! @name ADN_CapacityInfos_Occupable
//@{
ADN_Objects_Data::ADN_CapacityInfos_Occupable::ADN_CapacityInfos_Occupable()
{
}
void ADN_Objects_Data::ADN_CapacityInfos_Occupable::ReadArchive( xml::xistream& /*xis*/ )
{
//    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
}
void ADN_Objects_Data::ADN_CapacityInfos_Occupable::WriteArchive( xml::xostream& /*xos*/ )
{
}
//@}

//! @name ADN_CapacityInfos_TerrainHeuristic
//@{
ADN_Objects_Data::ADN_CapacityInfos_TerrainHeuristic::ADN_CapacityInfos_TerrainHeuristic()
{
    scores_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_TerrainHeuristic::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );

    xis >> xml::list( "terrain", *this, &ADN_Objects_Data::ADN_CapacityInfos_TerrainHeuristic::ReadTerrain );
}

void ADN_Objects_Data::ADN_CapacityInfos_TerrainHeuristic::ReadTerrain( xml::xistream& xis )
{
    std::auto_ptr< ScoreLocationInfos > score( new ScoreLocationInfos() );
    score->ReadArchive( xis );
    scores_.AddItem( score.release() );
}

void ADN_Objects_Data::ADN_CapacityInfos_TerrainHeuristic::WriteArchive( xml::xostream& xos )
{
    for( T_ScoreLocationInfosVector::iterator itScore = scores_.begin(); itScore != scores_.end(); ++itScore )
            (*itScore)->WriteArchive( xos );
}
//@}

//! @name ADN_CapacityInfos_SupplyRoute
//@{
ADN_Objects_Data::ADN_CapacityInfos_SupplyRoute::ADN_CapacityInfos_SupplyRoute()
{
}
void ADN_Objects_Data::ADN_CapacityInfos_SupplyRoute::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
}
void ADN_Objects_Data::ADN_CapacityInfos_SupplyRoute::WriteArchive( xml::xostream& /*xos*/ )
{
    // NOTHING
}
//@}

//! @name ADN_CapacityInfos_SupplyRoute
//@{
//ADN_Objects_Data::ADN_CapacityInfos_TimeLimited::ADN_CapacityInfos_TimeLimited()
//{
//}
//void ADN_Objects_Data::ADN_CapacityInfos_TimeLimited::ReadArchive( xml::xistream& xis )
//{
//    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
//}
//void ADN_Objects_Data::ADN_CapacityInfos_TimeLimited::WriteArchive( xml::xostream& xos )
//{
//}

//@}

ADN_Objects_Data::ADN_CapacityInfos_Bridging::ADN_CapacityInfos_Bridging()
{
}
void ADN_Objects_Data::ADN_CapacityInfos_Bridging::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
}
void ADN_Objects_Data::ADN_CapacityInfos_Bridging::WriteArchive( xml::xostream& /*xos*/ )
{
    // NOTHING
}

/*
//! @name ADN_CapacityInfos_Toxic
//@{
ADN_Objects_Data::ADN_CapacityInfos_Toxic::ADN_CapacityInfos_Toxic()
{
}
void ADN_Objects_Data::ADN_CapacityInfos_Toxic::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
}
void ADN_Objects_Data::ADN_CapacityInfos_Toxic::WriteArchive( xml::xostream& xos )
{
}
//@}
*/


//! @name ADN_CapacityInfos_Detection
//@{
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::DetectTimes
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Detection::ADN_CapacityInfos_Detection()
    : bDetectTime_  ( false )
    , detectTime_   ( "0s" ) // LTO
    , bIdentTime_   ( false ) // LTO
    , recoTime_     ( "0s" ) // LTO
    , bRecoTime_    ( false ) // LTO
    , identTime_    ( "0s" ) // LTO
{
    bDetectTime_.SetParentNode( *this );
    detectTime_.SetParentNode( *this ); // LTO
    bIdentTime_.SetParentNode( *this ); // LTO
    recoTime_.SetParentNode( *this ); // LTO
    identTime_.SetParentNode( *this ); // LTO
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Detection::ReadArchive
// Created: SLG 2010-02-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Detection::ReadArchive( xml::xistream& input )
{
    bPresent_ = true;
    // LTO begin
    input >> xml::optional
        >> xml::start( "acquisition-times" )
        >> xml::list( "acquisition-time", *this, &ADN_Objects_Data::ADN_CapacityInfos_Detection::ReadAcquisitionTime )
        >> xml::end;
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Detection::ReadAcquisitionTime
// Created: SLG 2010-02-18
// LTO
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Detection::ReadAcquisitionTime( xml::xistream& input )
{
    std::string time, level;
    input >> xml::optional
          >> xml::attribute( "time", time )
          >> xml::attribute( "level", level );
    if( time.empty() )
        return;

    if( level == "identification" ) {
        bIdentTime_ = true;
        identTime_ = time;
    }
    else if( level == "recognition" ) {
        bRecoTime_ = true;
        recoTime_ = time;
    }
    else if( level == "detection" ) {
        bDetectTime_ = true;
        detectTime_ = time;
    }
    else
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Objects - Invalid level '%1'" ).arg( level.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Detection::WriteArchive
// Created: SLG 2010-02-18
// LTO
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Detection::WriteArchive( xml::xostream& output )
{
    output << xml::start( "acquisition-times" );
    if( bDetectTime_.GetData() )
        output << xml::start( "acquisition-time" )
        << xml::attribute( "level", "detection" )
        << xml::attribute( "time", detectTime_ )
        << xml::end;
    if( bRecoTime_.GetData() )
        output << xml::start( "acquisition-time" )
        << xml::attribute( "level", "recognition" )
        << xml::attribute( "time", recoTime_ )
        << xml::end;
    if( bIdentTime_.GetData() )
        output << xml::start( "acquisition-time" )
        << xml::attribute( "level", "identification" )
        << xml::attribute( "time", identTime_ )
        << xml::end;
    output << xml::end;
}
//@}

//! @name ADN_CapacityInfos_Spawn
//@{
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Spawn
// Created: SLG 2010-02-18
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Spawn::ADN_CapacityInfos_Spawn()
    : strObjectType_( "" )
    , rActionRange_( 0 )
{
    rActionRange_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Spawn::ReadArchive( xml::xistream& input )
{
    bPresent_ = true;
    input >> xml::attribute( "object", strObjectType_ )
          >> xml::attribute( "action-range", rActionRange_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Spawn::WriteArchive( xml::xostream& output )
{
    output << xml::attribute( "object", strObjectType_ )
           << xml::attribute( "action-range", rActionRange_ );
}
//@}

//! @name ADN_CapacityInfos_Structural
//@{
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Structural
// Created: SLG 2010-06-25
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Structural::ADN_CapacityInfos_Structural()
: rStructuralState_( 100 )
{
    rStructuralState_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: SLG 2010-06-25
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Structural::ReadArchive( xml::xistream& input )
{
    bPresent_ = true;
    input >> xml::attribute( "value", rStructuralState_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Structural::WriteArchive( xml::xostream& output )
{
    output << xml::attribute( "value", rStructuralState_ );
}
//@}

//! @name ADN_CapacityInfos_AttitudeModifier
//@{
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::ADN_CapacityInfos_AttitudeModifier()
    : attitude_( ePopulationAttitude_Calme )
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::ReadArchive
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::ReadArchive( xml::xistream& input )
{
    bPresent_ = true;
    std::string strAttitude;
    input >> xml::attribute( "attitude", strAttitude );
    attitude_ = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
    if( attitude_ == (E_PopulationAttitude)-1 )
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Population types - Invalid population attitude '%1'" ).arg( strAttitude.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::WriteArchive
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::WriteArchive( xml::xostream& output )
{
    output << xml::attribute( "attitude", ENT_Tr::ConvertFromPopulationAttitude( attitude_.GetData() ) );
}
//@}

//! @name ADN_CapacityInfos_Perception
//@{
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Perception
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Perception::ADN_CapacityInfos_Perception()
: blinded_( false )
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Perception::ReadArchive
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Perception::ReadArchive( xml::xistream& input )
{
    bPresent_ = true;
    input >> xml::attribute( "blinded", blinded_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Perception::WriteArchive
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Perception::WriteArchive( xml::xostream& output )
{
    bPresent_ = true;
    output << xml::attribute( "blinded", blinded_ );
}
//@}

//! @name ADN_CapacityInfos_Scattering
//@{
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Scattering
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Scattering::ADN_CapacityInfos_Scattering()
: humanByTimeStep_( 1 )
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Scattering::ReadArchive
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Scattering::ReadArchive( xml::xistream& input )
{
    bPresent_ = true;
    input >> xml::attribute( "human-by-time-step", humanByTimeStep_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Scattering::WriteArchive
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Scattering::WriteArchive( xml::xostream& output )
{
    output << xml::attribute( "human-by-time-step", humanByTimeStep_ );
}
//@}

//! @name ADN_CapacityInfos_SealOff
//@{
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_SealOff
// Created: MGD 2010-08-27
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_SealOff::ADN_CapacityInfos_SealOff()
{
}
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_SealOff
// Created: MGD 2010-08-27
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_SealOff::ReadArchive( xml::xistream& xis )
{
    ADN_Objects_Data::ADN_TypeCapacity_Infos::ReadArchive( xis );
}
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_SealOff
// Created: MGD 2010-08-27
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_SealOff::WriteArchive( xml::xostream& /*xos*/ )
{
    // NOTHING
}


// =============================================================================
// ObjectInfos
// =============================================================================

#define INIT_DATA( CLASS, NAME, TAG_NAME ) \
    const std::string ADN_Objects_Data::##CLASS##::TAG( TAG_NAME ); \
    const std::string ADN_Objects_Data::##CLASS##::DISPLAY_NAME( NAME )


#pragma warning( push )
#pragma warning( disable : 4356 )

INIT_DATA( ADN_CapacityInfos_Activable,        "Activable",         "activable" );
INIT_DATA( ADN_CapacityInfos_Attrition,        "Attrition",         "attrition" );
INIT_DATA( ADN_CapacityInfos_Avoidable,        "Avoidable",         "avoidable" );
INIT_DATA( ADN_CapacityInfos_Bridging,         "Bridging",          "bridging" );
INIT_DATA( ADN_CapacityInfos_Buildable,        "Buildable",         "buildable" );
INIT_DATA( ADN_CapacityInfos_Bypassable,       "Bypassable",        "bypassable" );
INIT_DATA( ADN_CapacityInfos_Constructor,      "Constructor",       "constructor" );
INIT_DATA( ADN_CapacityInfos_Contamination,    "Contamination",     "contamination" );
INIT_DATA( ADN_CapacityInfos_Decontamination,  "Decontamination",   "decontamination" );
INIT_DATA( ADN_CapacityInfos_Delay,            "Delay",             "delay" );
INIT_DATA( ADN_CapacityInfos_Detection,        "Detection",         "detection" ); // LTO
INIT_DATA( ADN_CapacityInfos_Extinguishable,   "Extinguishable",    "extinguishable" );
INIT_DATA( ADN_CapacityInfos_Medical,          "Medical",           "medical" );
INIT_DATA( ADN_CapacityInfos_InteractionHeight,"InteractionHeight", "interaction-height" );
INIT_DATA( ADN_CapacityInfos_InteractWithEnemy,"InteractWithEnemy", "interact-with-enemy" );
INIT_DATA( ADN_CapacityInfos_Interference,     "Interference",      "interference" );
INIT_DATA( ADN_CapacityInfos_Improvable,       "Improvable",        "improvable" );
INIT_DATA( ADN_CapacityInfos_Intoxication,     "Intoxication",      "intoxication" );
INIT_DATA( ADN_CapacityInfos_Logistic,         "Logistic",          "logistic" );
INIT_DATA( ADN_CapacityInfos_Mobility,         "Mobility",          "mobility" );
INIT_DATA( ADN_CapacityInfos_Occupable,        "Occupable",         "occupable" );
INIT_DATA( ADN_CapacityInfos_Population,       "Population",        "population-filter" );
INIT_DATA( ADN_CapacityInfos_Propagation,      "Propagation",       "propagation" );
INIT_DATA( ADN_CapacityInfos_Protection,       "Protection",        "protection" );
INIT_DATA( ADN_CapacityInfos_Supply,           "Supply",            "supply" );
INIT_DATA( ADN_CapacityInfos_SupplyRoute,      "Supply-Route",      "supply-route" );
INIT_DATA( ADN_CapacityInfos_TerrainHeuristic, "Terrain Heuristic", "heuristic" );
INIT_DATA( ADN_CapacityInfos_TimeLimited,      "TimeLimited",       "time-limited" );
INIT_DATA( ADN_CapacityInfos_Workable,         "Workable",          "workable" );
INIT_DATA( ADN_CapacityInfos_Spawn,            "Spawn",             "spawn" );
INIT_DATA( ADN_CapacityInfos_Structural      , "Structural",        "structural" );
INIT_DATA( ADN_CapacityInfos_AttitudeModifier, "AttitudeModifier",  "attitude-modifier" );
INIT_DATA( ADN_CapacityInfos_Perception      , "Perception"      ,  "perception" );
INIT_DATA( ADN_CapacityInfos_Scattering      , "Scattering"      ,  "scattering" );
INIT_DATA( ADN_CapacityInfos_SealOff         , "SealOff"         ,  "sealoff" );

#pragma warning( pop )

//-----------------------------------------------------------------------------
// Name: ObjectInfos::ObjectInfos
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Objects_Data::ObjectInfos::ObjectInfos( const std::string& type )
    : ADN_Ref_ABC()
    , strType_( type )
{
    InitializeCapacities();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ObjectInfos::ObjectInfos()
    : ADN_Ref_ABC()
    , strType_()
{
    InitializeCapacities();
    InitializeDefaultParameters();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos::~ObjectInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Objects_Data::ObjectInfos::~ObjectInfos()
{
//    vScoreLocation_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::InitializeCapacities
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::InitializeCapacities()
{
//    capacities_[ ADN_CapacityInfos_Buildable::TAG ].reset( new ADN_CapacityInfos_Buildable() );
//    capacities_[ ADN_CapacityInfos_Improvable::TAG ].reset( new ADN_CapacityInfos_Improvable() );
//    capacities_[ "propagation" ].reset( new ADN_CapacityInfos_FirePropagation() );
    capacities_[ ADN_CapacityInfos_Activable::TAG ].reset( new ADN_CapacityInfos_Activable() );
    capacities_[ ADN_CapacityInfos_Attrition::TAG ].reset( new ADN_CapacityInfos_Attrition() );
    capacities_[ ADN_CapacityInfos_Avoidable::TAG ].reset( new ADN_CapacityInfos_Avoidable() );
    capacities_[ ADN_CapacityInfos_Bridging::TAG ].reset( new ADN_CapacityInfos_Bridging() );
    capacities_[ ADN_CapacityInfos_Bypassable::TAG ].reset( new ADN_CapacityInfos_Bypassable() );
    capacities_[ ADN_CapacityInfos_Constructor::TAG ].reset( new ADN_CapacityInfos_Constructor() );
    capacities_[ ADN_CapacityInfos_Contamination::TAG ].reset( new ADN_CapacityInfos_Contamination() );
    capacities_[ ADN_CapacityInfos_Decontamination::TAG ].reset( new ADN_CapacityInfos_Decontamination() );
    capacities_[ ADN_CapacityInfos_Delay::TAG ].reset( new ADN_CapacityInfos_Delay() );
    capacities_[ ADN_CapacityInfos_Detection::TAG ].reset( new ADN_CapacityInfos_Detection() ); // LTO
    capacities_[ ADN_CapacityInfos_Extinguishable::TAG ].reset( new ADN_CapacityInfos_Extinguishable() );
    capacities_[ ADN_CapacityInfos_Medical::TAG ].reset( new ADN_CapacityInfos_Medical() );
    capacities_[ ADN_CapacityInfos_InteractionHeight::TAG ].reset( new ADN_CapacityInfos_InteractionHeight() );
    capacities_[ ADN_CapacityInfos_InteractWithEnemy::TAG ].reset( new ADN_CapacityInfos_InteractWithEnemy() );
    capacities_[ ADN_CapacityInfos_Interference::TAG ].reset( new ADN_CapacityInfos_Interference() );
    capacities_[ ADN_CapacityInfos_Intoxication::TAG ].reset( new ADN_CapacityInfos_Intoxication() );
    capacities_[ ADN_CapacityInfos_Logistic::TAG ].reset( new ADN_CapacityInfos_Logistic() );
    capacities_[ ADN_CapacityInfos_Mobility::TAG ].reset( new ADN_CapacityInfos_Mobility() );
    capacities_[ ADN_CapacityInfos_Occupable::TAG ].reset( new ADN_CapacityInfos_Occupable() );
    capacities_[ ADN_CapacityInfos_Population::TAG ].reset( new ADN_CapacityInfos_Population() );
    capacities_[ ADN_CapacityInfos_Propagation::TAG ].reset( new ADN_CapacityInfos_Propagation() );
    capacities_[ ADN_CapacityInfos_Protection::TAG ].reset( new ADN_CapacityInfos_Protection() );
    capacities_[ ADN_CapacityInfos_Supply::TAG ].reset( new ADN_CapacityInfos_Supply() );
    capacities_[ ADN_CapacityInfos_SupplyRoute::TAG ].reset( new ADN_CapacityInfos_SupplyRoute() );
    capacities_[ ADN_CapacityInfos_TerrainHeuristic::TAG ].reset( new ADN_CapacityInfos_TerrainHeuristic() );
    capacities_[ ADN_CapacityInfos_TimeLimited::TAG ].reset( new ADN_CapacityInfos_TimeLimited() );
    capacities_[ ADN_CapacityInfos_Workable::TAG ].reset( new ADN_CapacityInfos_Workable() );
    capacities_[ ADN_CapacityInfos_Spawn::TAG ].reset( new ADN_CapacityInfos_Spawn() );
    capacities_[ ADN_CapacityInfos_Structural::TAG ].reset( new ADN_CapacityInfos_Structural() );
    capacities_[ ADN_CapacityInfos_AttitudeModifier::TAG ].reset( new ADN_CapacityInfos_AttitudeModifier() );
    capacities_[ ADN_CapacityInfos_Perception::TAG ].reset( new ADN_CapacityInfos_Perception() );
    capacities_[ ADN_CapacityInfos_Scattering::TAG ].reset( new ADN_CapacityInfos_Scattering() );
    capacities_[ ADN_CapacityInfos_SealOff::TAG ].reset( new ADN_CapacityInfos_SealOff() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::InitializeDefaultParameters
// Created: SLG 2010-06-09
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::InitializeDefaultParameters()
{
    geometries_ = "polygon";
    symbol_ = "G*GPGAL---****X";
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
void ADN_Objects_Data::ObjectInfos::ReadSensiblePosition( xml::xistream& /*input*/ )
{
//    std::auto_ptr<ScoreLocationInfos> spNew( new ScoreLocationInfos() );
//    spNew->ReadArchive( input );
//    vScoreLocation_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos::ReadDotation
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadDotation( const std::string& /*type*/, xml::xistream& /*input*/ )
{
//    std::string dotation, category;
//    unsigned count;
//    input >> xml::attribute( "dotation", dotation )
//          >> xml::attribute( "category", category )
//          >> xml::attribute( "count", count );
//    ADN_Equipement_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( category, dotation );
//    if( pCategory == 0 )
//        throw ADN_DataException( "Donnée invalide", "Dotation invalide dans l'objet " + strName_.GetData() );
//    if( type == "construction" )
//    {
//        ptrToBuild_ = pCategory;
//        bToBuild_ = true;
//        nNbrToBuild_ = count;
//    }
//    else if( type == "valorization" )
//    {
//        ptrToReinforce_ = pCategory;
//        bToReinforce_ = true;
//        nNbrToReinforce_ = count;
//    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos::ReadUnitAttrition
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadUnitAttrition( xml::xistream& /*input*/ )
{
//    bAttritions_ = true;
//    std::string protection;
//    input >> xml::attribute( "protection", protection );
//    IT_AttritionInfosVector itAttrition = std::find_if( attritions_.begin(), attritions_.end(), AttritionInfos::Cmp(protection));
//    if( itAttrition != attritions_.end() )
//        (*itAttrition)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadCapacityArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadCapacityArchive( const std::string& type, xml::xistream& xis )
{
//    typedef boost::mpl::fold<ADN_CapacityTypes, NullType, Reader<boost::mpl::_1,boost::mpl::_2> >::type ReaderType;
//    ReaderType::Read( type, xis, capacities_ );

    IT_CapacityMap it = capacities_.find( type );
    if( it != capacities_.end() )
        it->second->ReadArchive( xis );
}

// -----------------------------------------------------------------------------
// Name: ObjectInfos::ReadArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadArchive( xml::xistream& xis )
{
    xis >> xml::attribute( "name", strName_ )
        >> xml::attribute( "type", strType_ )
        >> xml::attribute( "geometry", geometries_ )
        >> xml::optional >> xml::attribute( "symbol", symbol_ )
        >> xml::list( *this, &ADN_Objects_Data::ObjectInfos::ReadCapacityArchive );
}

// -----------------------------------------------------------------------------
// Name: ObjectInfos::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::WriteArchive( xml::xostream& xos )
{
    xos << xml::start( "object" )
        << xml::attribute( "name", strName_ )
        << xml::attribute( "geometry", geometries_.GetData() )
        << xml::attribute( "symbol", symbol_ );
    if( strType_ == "" )
        xos << xml::attribute( "type", strName_ );
    else
        xos << xml::attribute( "type", strType_ );

    for( CIT_CapacityMap it = capacities_.begin(); capacities_.end() != it; ++it )
        if( it->second->bPresent_.GetData() )
        {
            xos << xml::start( it->first );
            it->second->WriteArchive( xos );
            xos << xml::end;
        }
    xos << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ADN_Objects_Data constructor
// Created: JDY 03-06-25
//-----------------------------------------------------------------------------
ADN_Objects_Data::ADN_Objects_Data()
    : ADN_Data_ABC()
{
    // NOTHING
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
void ADN_Objects_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szObjects_.GetData() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Objects_Data::Reset
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
void ADN_Objects_Data::Reset()
{
    vObjectInfos_.Reset();

//    for( int i = 0; i < eNbrObjectType; ++i )
//        vObjectInfos_.AddItem( new ObjectInfos( (E_ObjectType)i ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadObject
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ReadObject( xml::xistream& xis )
{
//    E_ObjectType nObjectType = ENT_Tr::ConvertToObjectType( type );
//    if( nObjectType == (E_ObjectType)-1)
//        throw ADN_DataException( "Object", "Le type d'objet " + type + " n'est pas connu" );
//
    std::string type = xis.attribute< std::string >( "type" );
    ObjectInfos* pObjInfo = new ObjectInfos( type );
    if( pObjInfo )
    {
        pObjInfo->ReadArchive( xis );
        vObjectInfos_.AddItem( pObjInfo );
        // static_cast< std::vector< ObjectInfos* >* >( &vObjectInfos_ )->push_back( pObjInfo );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadShotPercentage
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ReadShotPercentage( xml::xistream& /*input*/ )
{
//    std::auto_ptr<AreaControlInformations> spNew( new AreaControlInformations() );
//    spNew->ReadArchive( input );
//    vAreaControlInformations_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ReadArchive( xml::xistream& xis )
{
    xis >> xml::start( "objects" )
            >> xml::list( "object", *this, &ADN_Objects_Data::ReadObject )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::WriteArchive( xml::xostream& xos )
{
    xos << xml::start( "objects" );
    ADN_Tools::AddSchema( xos, "Objects" );
    for( IT_ObjectsInfos_Vector it = vObjectInfos_.begin(); it!= vObjectInfos_.end(); ++it)
        (*it)->WriteArchive( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::CreateCopy
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
ADN_Objects_Data::ObjectInfos* ADN_Objects_Data::ObjectInfos::CreateCopy()
{
    xml::xostringstream xos;
    WriteArchive( xos );
    ADN_Objects_Data::ObjectInfos* pCopy = new ADN_Objects_Data::ObjectInfos;
    xml::xistringstream xis( xos.str() );
    xis >> xml::start( "object" );
    pCopy->ReadArchive( xis );
    return pCopy;
}
