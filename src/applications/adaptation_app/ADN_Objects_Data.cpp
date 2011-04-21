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
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::ScoreLocationInfos
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
ADN_Objects_Data::ScoreLocationInfos::ScoreLocationInfos()
    : ADN_Ref_ABC         ()
    , ADN_DataTreeNode_ABC()
    , nLocation_          ( static_cast< E_Location >( 0 ) )
    , nScore_             ( 0 )
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

namespace
{
    std::vector< std::string > VectorBuilder( const char* choice[], uint size )
    {
        std::vector< std::string > stack( size );
        for ( uint i = 0; i < size; ++i )
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
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );

    xis >> xml::optional
        >> xml::start( "resources" )
            >> xml::list( "resource", *this, &ADN_Objects_Data::ADN_CapacityInfos_Buildable::ReadDotation )
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
        ADN_Composantes_Data::CategoryInfos* infos = new ADN_Composantes_Data::CategoryInfos( category->parentResource_ );
        infos->ptrCategory_ = category;
        infos->rNbr_ = quantity;
        categories_.AddItem( infos );
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
        ADN_Composantes_Data::CategoryInfos* infos = reinterpret_cast< ADN_Composantes_Data::CategoryInfos* >( *it );
        xos << xml::start( "resource" )
                << xml::attribute( "name", infos->ptrCategory_.GetData()->strName_ ) << xml::attribute( "count", infos->rNbr_ )
            << xml::end;
    }
    xos << xml::end;
}

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
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );

    xis >> xml::optional
         >> xml::start( "resources" )
            >> xml::list( "resource", *this, &ADN_Objects_Data::ADN_CapacityInfos_Improvable::ReadDotation )
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
        ADN_Composantes_Data::CategoryInfos* infos = new ADN_Composantes_Data::CategoryInfos( category->parentResource_ );
        infos->ptrCategory_ = category;
        infos->rNbr_ = quantity;
        categories_.AddItem( infos );
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
        ADN_Composantes_Data::CategoryInfos* infos = reinterpret_cast< ADN_Composantes_Data::CategoryInfos* >( *it );
        xos << xml::start( "resource" )
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
    : nDefaultConsumption_( eWorking )
    , unitType_           ( VectorBuilder( choices_, 2 ) )
    , ptrBuildable_       ( new ADN_CapacityInfos_Buildable() )
    , ptrImprovable_      ( new ADN_CapacityInfos_Improvable() )
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
    std::string strConsumption;
    xis >> xml::attribute( "default-consumption-mode", strConsumption );
    nDefaultConsumption_ = ADN_Tr::ConvertToConsumptionType( strConsumption );
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
        << xml::attribute( "unit-type", unitType_.GetData() == "" ? ADN_Objects_Data::ADN_CapacityInfos_Constructor::choices_[ 0 ]:unitType_.GetData() );
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

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Avoidable
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Avoidable::ADN_CapacityInfos_Avoidable()
    : fields_   ( "Distance" )
    , rDistance_( 0. )
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

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Bypassable
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Bypassable::ADN_CapacityInfos_Bypassable()
    : fields_( "Speed" )
    , rSpeed_( 0. )
{
    rSpeed_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Bypassable::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
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

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Mobility
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Mobility::ADN_CapacityInfos_Mobility()
    : rDefaultSpeed_ ( 0. )
    , nSpeedModifier_( eSpeedImpact_AuPlusLent )
    , rMaxAgentSpeed_( 0 )
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
    : ammoCategory_( ( ADN_Equipement_Data::T_AmmoCategoryInfo_Vector& )ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition ).categories_, 0, "" )
    , useAmmo_     ( false )
{
    category_.SetParentNode( *this );
    ammoCategory_.SetParentNode( *this );
    useAmmo_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_Attrition::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    std::string dotation( xis.attribute< std::string >( "category" ) );
    if( dotation != "" )
    {
        ADN_Equipement_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( dotation );
        if( pCategory == 0 )
            throw ADN_DataException( "Donnée invalide", "Dotation invalide : " + dotation );
        category_ = pCategory;
        ADN_Equipement_Data::AmmoCategoryInfo* ptrAmmo = dynamic_cast< ADN_Equipement_Data::AmmoCategoryInfo* >( pCategory );
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
    : max_toxic_( 0 )
{
    max_toxic_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_Contamination::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::attribute( "max-toxic", max_toxic_ );
}

void ADN_Objects_Data::ADN_CapacityInfos_Contamination::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "max-toxic", max_toxic_ );
}
//@}

// -----------------------------------------------------------------------------
// Name: ADN_CapacityInfos_Flood::ADN_CapacityInfos_Flood
// Created: JSR 2011-01-10
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Flood::ADN_CapacityInfos_Flood()
    : nNbHurtHumans1_( 0 )
    , nNbHurtHumans2_( 0 )
    , nNbHurtHumans3_( 0 )
    , nNbHurtHumansE_( 0 )
    , nNbDeadHumans_ ( 0 )
{
    nNbHurtHumans1_.SetParentNode( *this );
    nNbHurtHumans2_.SetParentNode( *this );
    nNbHurtHumans3_.SetParentNode( *this );
    nNbHurtHumansE_.SetParentNode( *this );
    nNbDeadHumans_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_CapacityInfos_Flood::ReadArchive
// Created: JSR 2011-01-10
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Flood::ReadArchive( xml::xistream& xis )
{
    ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::optional >> xml::start( "injuries" )
            >> xml::list( "injury", *this, &ADN_Objects_Data::ADN_CapacityInfos_Flood::ReadInjury );
    if( nNbHurtHumans1_.GetData() + nNbHurtHumans2_.GetData() + nNbHurtHumans3_.GetData() + nNbHurtHumansE_.GetData() + nNbDeadHumans_.GetData() > 100 )
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Flood - Injuries data sum > 100" ).ascii() );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_CapacityInfos_Flood::::ReadInjury
// Created: JSR 2011-01-10
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Flood::ReadInjury( xml::xistream& xis )
{
    std::string wound = xis.attribute< std::string >( "type" );
    std::transform( wound.begin(), wound.end(), wound.begin(), std::tolower );
    ADN_Type_Int* pWound =
        wound == "u1"   ? &nNbHurtHumans1_ :
        wound == "u2"   ? &nNbHurtHumans2_ :
        wound == "u3"   ? &nNbHurtHumans3_ :
        wound == "ue"   ? &nNbHurtHumansE_ :
        wound == "dead" ? &nNbDeadHumans_ :
        0;
    if( pWound )
    {
        *pWound = static_cast< int >( xis.attribute< double >( "percentage" ) * 100. );
        if( pWound->GetData() < 0 || pWound->GetData() > 100 )
            throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Flood - Wound '%1' data < 0 or > 1" ).arg( wound.c_str() ).ascii() );
    }
    else
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Flood - Invalid wound type '%1'" ).arg( wound.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_CapacityInfos_Flood::WriteArchive
// Created: JSR 2011-01-10
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Flood::WriteArchive( xml::xostream& xos )
{
    xos << xml::start( "injuries" );
    if( nNbHurtHumans1_.GetData() + nNbHurtHumans2_.GetData() + nNbHurtHumans3_.GetData() + nNbHurtHumansE_.GetData() + nNbDeadHumans_.GetData() > 100 )
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Flood - Injuries data sum > 100" ).ascii() );
    xos  << xml::start( "injury" )
                << xml::attribute( "type", "u1" )
                << xml::attribute( "percentage", nNbHurtHumans1_.GetData() / 100. )
            << xml::end
            << xml::start( "injury" )
                << xml::attribute( "type", "u2" )
                << xml::attribute( "percentage", nNbHurtHumans2_.GetData() / 100. )
            << xml::end
            << xml::start( "injury" )
                << xml::attribute( "type", "u3" )
                << xml::attribute( "percentage", nNbHurtHumans3_.GetData() / 100. )
            << xml::end
            << xml::start( "injury" )
                << xml::attribute( "type", "ue" )
                << xml::attribute( "percentage", nNbHurtHumansE_.GetData() / 100. )
            << xml::end
            << xml::start( "injury" )
                << xml::attribute( "type", "dead" )
                << xml::attribute( "percentage", nNbDeadHumans_.GetData() / 100. )
            << xml::end
           << xml::end;
}

//! @name ADN_CapacityInfos_InteractionHeight
//@{
ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight::ADN_CapacityInfos_InteractionHeight()
    : height_( 0 )
{
    height_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
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
    : max_toxic_( 0 )
{
    max_toxic_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_Intoxication::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
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
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_Decontamination::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
}

void ADN_Objects_Data::ADN_CapacityInfos_Decontamination::WriteArchive( xml::xostream& /* xos */ )
{
    // NOTHING
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
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
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
    : model_( VectorBuilder( choices_, 3 ) )
{
    model_.SetParentNode( *this );
}

void ADN_Objects_Data::ADN_CapacityInfos_Propagation::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
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
    : max_size_     ( 1 )
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
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
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
    : worker_( 0 )
{
    worker_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Workable::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
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
    : emergencyBedsRate_   ( 0 )
    , emergencyDoctorsRate_( 0 )
    , nightDoctorsRate_    ( 0 )
{
    emergencyBedsRate_.SetParentNode( *this );
    emergencyDoctorsRate_.SetParentNode( *this );
    nightDoctorsRate_.SetParentNode( *this );
}
void ADN_Objects_Data::ADN_CapacityInfos_Medical::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );

    xis >> xml::attribute( "night-doctors-rate", nightDoctorsRate_ )
        >> xml::start( "emergency-plan" )
            >> xml::attribute( "doctors-rate", emergencyDoctorsRate_ )
            >> xml::attribute( "beds-rate", emergencyBedsRate_ )
        >> xml::end;
}

void ADN_Objects_Data::ADN_CapacityInfos_Medical::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "night-doctors-rate", nightDoctorsRate_ )
        << xml::start( "emergency-plan" )
            << xml::attribute( "doctors-rate", emergencyDoctorsRate_ )
            << xml::attribute( "beds-rate", emergencyBedsRate_ )
        << xml::end;
}
//@}

//! @name ADN_CapacityInfos_Occupable
//@{
ADN_Objects_Data::ADN_CapacityInfos_Occupable::ADN_CapacityInfos_Occupable()
{
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_Occupable::ReadArchive( xml::xistream& /*xis*/ )
{
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_Occupable::WriteArchive( xml::xostream& /*xos*/ )
{
    // NOTHING
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
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
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
            ( *itScore )->WriteArchive( xos );
}
//@}

//! @name ADN_CapacityInfos_SupplyRoute
//@{
ADN_Objects_Data::ADN_CapacityInfos_SupplyRoute::ADN_CapacityInfos_SupplyRoute()
{
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_SupplyRoute::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
}

void ADN_Objects_Data::ADN_CapacityInfos_SupplyRoute::WriteArchive( xml::xostream& /*xos*/ )
{
    // NOTHING
}
//@}

ADN_Objects_Data::ADN_CapacityInfos_Bridging::ADN_CapacityInfos_Bridging()
{
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_Bridging::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
}

void ADN_Objects_Data::ADN_CapacityInfos_Bridging::WriteArchive( xml::xostream& /*xos*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Detection
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Detection::ADN_CapacityInfos_Detection()
    : bDetectTime_( false )
    , detectTime_ ( "0s" ) // LTO
    , bIdentTime_ ( false ) // LTO
    , recoTime_   ( "0s" ) // LTO
    , bRecoTime_  ( false ) // LTO
    , identTime_  ( "0s" ) // LTO
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
    if( level == "identification" )
    {
        bIdentTime_ = true;
        identTime_ = time;
    }
    else if( level == "recognition" )
    {
        bRecoTime_ = true;
        recoTime_ = time;
    }
    else if( level == "detection" )
    {
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

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Spawn
// Created: SLG 2010-02-18
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Spawn::ADN_CapacityInfos_Spawn()
    : strObjectType_( "" )
    , rActionRange_ ( 0 )
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

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::ADN_CapacityInfos_AttitudeModifier()
    : attitude_( ePopulationAttitude_Calme )
{
    // NOTHING
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
    if( attitude_ == static_cast< E_PopulationAttitude >( -1 ) )
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Crowd types - Invalid crowd attitude '%1'" ).arg( strAttitude.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::WriteArchive
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::WriteArchive( xml::xostream& output )
{
    output << xml::attribute( "attitude", ENT_Tr::ConvertFromPopulationAttitude( attitude_.GetData() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Perception
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Perception::ADN_CapacityInfos_Perception()
    : blinded_( false )
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Scattering
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Scattering::ADN_CapacityInfos_Scattering()
    : humanByTimeStep_( 1 )
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ADN_CapacityInfos_FirePropagationModifier
// Created: BCI 2010-12-03
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ADN_CapacityInfos_FirePropagationModifier()
    : modifiers_( ADN_Workspace::GetWorkspace().GetFireClasses().GetData().GetFireClassesInfos() )
{
    modifiers_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ReadArchive
// Created: BCI 2010-12-03
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::list( "modifier", *this, &ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ReadModifier );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::WriteArchive
// Created: BCI 2010-12-03
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::WriteArchive( xml::xostream& xos )
{
    for( IT_ModifierByFireClass_Vector it = modifiers_.begin(); it != modifiers_.end(); ++it )
        ( *it )->WriteArchive( xos );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ReadModifier
// Created: BCI 2010-12-03
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ReadModifier( xml::xistream& xis )
{
    std::string fireClass = xis.attribute< std::string >( "fire-class" );
    IT_ModifierByFireClass_Vector itModifier = std::find_if( modifiers_.begin(), modifiers_.end(), ModifierByFireClass::Cmp( fireClass ) );
    if( itModifier == modifiers_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Fire propagation modifier - Invalid fire class '%1'" ).arg( fireClass.c_str() ).ascii() );
    ( *itModifier )->ReadArchive( xis );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ModifierByFireClass
// Created: BCI 2010-12-06
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ModifierByFireClass::ModifierByFireClass( ADN_FireClass_Data::FireClassInfos* p )
    : ptrFireClass_       ( ADN_Workspace::GetWorkspace().GetFireClasses().GetData().GetFireClassesInfos(), p )
    , ignitionThreshold_  ( 0 )
    , maxCombustionEnergy_( 0 )
{
    BindExistenceTo( &ptrFireClass_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: BCI 2010-12-03
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ModifierByFireClass::ReadArchive( xml::xistream& xis )
{
    xis >> xml::attribute( "ignition-threshold", ignitionThreshold_ )
        >> xml::attribute( "max-combustion-energy", maxCombustionEnergy_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ModifierByFireClass::WriteArchive
// Created: BCI 2010-12-03
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ModifierByFireClass::WriteArchive( xml::xostream& xos )
{
    if( ptrFireClass_.GetData()->isSurface_.GetData() )
        xos << xml::start( "modifier" )
            << xml::attribute( "fire-class", ptrFireClass_.GetData()->strName_ )
            << xml::attribute( "ignition-threshold", ignitionThreshold_ )
            << xml::attribute( "max-combustion-energy", maxCombustionEnergy_ )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::GetItemName
// Created: BCI 2010-12-03
// -----------------------------------------------------------------------------
std::string ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::ModifierByFireClass::GetItemName()
{
    return std::string();
}

#define INIT_DATA( CLASS, NAME, TAG_NAME ) \
    const std::string ADN_Objects_Data::##CLASS##::TAG( TAG_NAME ); \
    const std::string ADN_Objects_Data::##CLASS##::DISPLAY_NAME( NAME )

#pragma warning( push )
#pragma warning( disable : 4356 )

INIT_DATA( ADN_CapacityInfos_Activable,               "Activable",               "activable" );
INIT_DATA( ADN_CapacityInfos_Attrition,               "Attrition",               "attrition" );
INIT_DATA( ADN_CapacityInfos_Avoidable,               "Avoidable",               "avoidable" );
INIT_DATA( ADN_CapacityInfos_Bridging,                "Bridging",                "bridging" );
INIT_DATA( ADN_CapacityInfos_Buildable,               "Buildable",               "buildable" );
INIT_DATA( ADN_CapacityInfos_Bypassable,              "Bypassable",              "bypassable" );
INIT_DATA( ADN_CapacityInfos_Constructor,             "Constructor",             "constructor" );
INIT_DATA( ADN_CapacityInfos_Contamination,           "Contamination",           "contamination" );
INIT_DATA( ADN_CapacityInfos_Decontamination,         "Decontamination",         "decontamination" );
INIT_DATA( ADN_CapacityInfos_Delay,                   "Delay",                   "delay" );
INIT_DATA( ADN_CapacityInfos_Detection,               "Detection",               "detection" ); // LTO
INIT_DATA( ADN_CapacityInfos_Extinguishable,          "Extinguishable",          "extinguishable" );
INIT_DATA( ADN_CapacityInfos_Medical,                 "Medical",                 "medical" );
INIT_DATA( ADN_CapacityInfos_InteractionHeight,       "InteractionHeight",       "interaction-height" );
INIT_DATA( ADN_CapacityInfos_InteractWithEnemy,       "InteractWithEnemy",       "interact-with-enemy" );
INIT_DATA( ADN_CapacityInfos_Interference,            "Interference",            "interference" );
INIT_DATA( ADN_CapacityInfos_Improvable,              "Improvable",              "improvable" );
INIT_DATA( ADN_CapacityInfos_Intoxication,            "Intoxication",            "intoxication" );
INIT_DATA( ADN_CapacityInfos_Logistic,                "Logistic",                "logistic" );
INIT_DATA( ADN_CapacityInfos_Mobility,                "Mobility",                "mobility" );
INIT_DATA( ADN_CapacityInfos_Occupable,               "Occupable",               "occupable" );
INIT_DATA( ADN_CapacityInfos_Population,              "Population",              "population-filter" );
INIT_DATA( ADN_CapacityInfos_Propagation,             "Propagation",             "propagation" );
INIT_DATA( ADN_CapacityInfos_Protection,              "Protection",              "protection" );
INIT_DATA( ADN_CapacityInfos_Supply,                  "Supply",                  "supply" );
INIT_DATA( ADN_CapacityInfos_SupplyRoute,             "Supply-Route",            "supply-route" );
INIT_DATA( ADN_CapacityInfos_TerrainHeuristic,        "Terrain Heuristic",       "heuristic" );
INIT_DATA( ADN_CapacityInfos_TimeLimited,             "TimeLimited",             "time-limited" );
INIT_DATA( ADN_CapacityInfos_Workable,                "Workable",                "workable" );
INIT_DATA( ADN_CapacityInfos_Spawn,                   "Spawn",                   "spawn" );
INIT_DATA( ADN_CapacityInfos_Structural,              "Structural",              "structural" );
INIT_DATA( ADN_CapacityInfos_AttitudeModifier,        "AttitudeModifier",        "attitude-modifier" );
INIT_DATA( ADN_CapacityInfos_Perception,              "Perception",              "perception" );
INIT_DATA( ADN_CapacityInfos_Scattering,              "Scattering",              "scattering" );
INIT_DATA( ADN_CapacityInfos_FirePropagationModifier, "FirePropagationModifier", "fire-propagation-modifier" );
INIT_DATA( ADN_CapacityInfos_Burn,                    "Burn",                    "burn" );
INIT_DATA( ADN_CapacityInfos_Flood,                   "Flood",                   "flood" );
INIT_DATA( ADN_CapacityInfos_Universal,               "Universal",               "universal" );
INIT_DATA( ADN_CapacityInfos_Stock,                   "Stock",                   "stock" );
INIT_DATA( ADN_CapacityInfos_ResourceNetwork,         "ResourceNetwork",         "resources" );

#pragma warning( pop )

//-----------------------------------------------------------------------------
// Name: ObjectInfos::ObjectInfos
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Objects_Data::ObjectInfos::ObjectInfos( const std::string& type )
    : ADN_Ref_ABC()
    , strType_   ( type )
    , geometries_ ( "polygon" )
{
    symbol_.SetParentNode( *this );
    geometries_.SetParentNode( *this );
    InitializeCapacities();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
ADN_Objects_Data::ObjectInfos::ObjectInfos()
    : ADN_Ref_ABC ()
    , strType_    ()
    , geometries_ ( "polygon" )
{
    symbol_.SetParentNode( *this );
    geometries_.SetParentNode( *this );
    InitializeCapacities();
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ObjectInfos::~ObjectInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Objects_Data::ObjectInfos::~ObjectInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::InitializeCapacities
// Created: JCR 2008-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::InitializeCapacities()
{
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
    capacities_[ ADN_CapacityInfos_FirePropagationModifier::TAG ].reset( new ADN_CapacityInfos_FirePropagationModifier() );
    capacities_[ ADN_CapacityInfos_Burn::TAG ].reset( new ADN_CapacityInfos_Burn() );
    capacities_[ ADN_CapacityInfos_Flood::TAG ].reset( new ADN_CapacityInfos_Flood() );
    capacities_[ ADN_CapacityInfos_Universal::TAG ].reset( new ADN_CapacityInfos_Universal() );
    capacities_[ ADN_CapacityInfos_Stock::TAG ].reset( new ADN_CapacityInfos_Stock() );
    capacities_[ ADN_CapacityInfos_ResourceNetwork::TAG ].reset( new ADN_CapacityInfos_ResourceNetwork() );
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
// Name: ADN_Objects_Data::ReadCapacityArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ObjectInfos::ReadCapacityArchive( const std::string& type, xml::xistream& xis )
{
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
    std::string code = "";
    xis >> xml::attribute( "name", strName_ )
        >> xml::attribute( "type", strType_ )
        >> xml::attribute( "geometry", geometries_ )
        >> xml::optional >> xml::attribute( "symbol", code )
        >> xml::list( *this, &ADN_Objects_Data::ObjectInfos::ReadCapacityArchive );

    ADN_Symbols_Data& symbolsData = ADN_Workspace::GetWorkspace().GetSymbols().GetData();
    symbol_.SetVector( symbolsData.GetSymbols( geometries_.GetData() ) );
    symbol_.SetData( symbolsData.GetSymbol( code ), false );
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
        << xml::attribute( "symbol", symbol_.GetData()->GetCode() );
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
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadObject
// Created: AGE 2007-08-20
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ReadObject( xml::xistream& xis )
{
    std::string type = xis.attribute< std::string >( "type" );
    ObjectInfos* pObjInfo = new ObjectInfos( type );
    if( pObjInfo )
    {
        pObjInfo->ReadArchive( xis );
        vObjectInfos_.AddItem( pObjInfo );
    }
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
    for( IT_ObjectsInfos_Vector it = vObjectInfos_.begin(); it != vObjectInfos_.end(); ++it )
        ( *it )->WriteArchive( xos );
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
