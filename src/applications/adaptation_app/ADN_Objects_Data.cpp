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
#include "ADN_DataException.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"
#include "ADN_ConsistencyChecker.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::ScoreLocationInfos
// Created: AGN 2004-05-24
// -----------------------------------------------------------------------------
ADN_Objects_Data::ScoreLocationInfos::ScoreLocationInfos()
    : nLocation_          ( static_cast< E_Location >( 0 ) )
    , nScore_             ( 0 )
{
    // NOTHING
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
    nLocation_ = ENT_Tr::ConvertToLocation( strTerrain );
}

// -----------------------------------------------------------------------------
// Name: ScoreLocationInfos::WriteArchive
// Created: APE 2004-11-18
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ScoreLocationInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "terrain" )
             << xml::attribute( "type", nLocation_.Convert() )
             << xml::attribute( "value", nScore_ )
           << xml::end;
}

namespace
{
    std::vector< std::string > VectorBuilder( const char* choice[], std::size_t size )
    {
        std::vector< std::string > stack( size );
        for( std::size_t i = 0; i < size; ++i )
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
    // NOTHING
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
    // NOTHING
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
        ADN_Composantes_Data::CategoryInfos* infos = *it;
        xos << xml::start( "resource" )
                << xml::attribute( "name", infos->ptrCategory_.GetData()->strName_ ) << xml::attribute( "count", infos->rNbr_ )
            << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Constructor
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Constructor::ADN_CapacityInfos_Constructor()
    : nDefaultConsumption_( eWorking )
    , nUnitType_          ( eRaw )
    , ptrBuildable_       ( new ADN_CapacityInfos_Buildable() )
    , ptrImprovable_      ( new ADN_CapacityInfos_Improvable() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Constructor::ReadArchive( xml::xistream& xis )
{
    ADN_TypeCapacity_Infos::ReadArchive( xis );
    nDefaultConsumption_ = ADN_Tr::ConvertToConsumptionType( xis.attribute< std::string >( "default-consumption-mode" ) );
    nUnitType_ = ADN_Tr::ConvertToConstructorType( xis.attribute< std::string >( "unit-type" ) );
    xis >> xml::optional >> xml::list( ADN_CapacityInfos_Buildable::TAG, *ptrBuildable_, &ADN_CapacityInfos_Buildable::ReadArchive );
    xis >> xml::optional >> xml::list( ADN_CapacityInfos_Improvable::TAG, *ptrImprovable_, &ADN_CapacityInfos_Improvable::ReadArchive );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Constructor::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "default-consumption-mode", nDefaultConsumption_.Convert() )
        << xml::attribute( "unit-type", nUnitType_.Convert() );
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
    : rDistance_( 0. )
{
    // NOTHING
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
    : rSpeed_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Bypassable::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    rSpeed_ = xis.attribute< double >( "bypass-speed" );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Mobility::ReadArchive( xml::xistream& xis )
{
    ADN_TypeCapacity_Infos::ReadArchive( xis );
    rDefaultSpeed_ = xis.attribute< double >( "default-speed" );
    nSpeedModifier_ = ADN_Tr::ConvertToSpeedImpact( xis.attribute< std::string >( "unit-speed-impact-mode" ) );
    if( nSpeedModifier_ == eSpeedImpact_VitesseMaxAgent )
        rMaxAgentSpeed_ = xis.attribute< double >( "max-unit-percentage-speed" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: JCR 2008-07-15
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Mobility::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "default-speed", rDefaultSpeed_ )
        << xml::attribute( "unit-speed-impact-mode", nSpeedModifier_.Convert() )
        << xml::attribute( "max-unit-percentage-speed", rMaxAgentSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Trafficability
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Trafficability::ADN_CapacityInfos_Trafficability()
    : limited_  ( false )
    , maxWeight_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Trafficability::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    limited_ = xis.attribute< bool >( "default" );
    if( limited_.GetData() )
        maxWeight_ = xis.attribute< double >( "max-weight" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: LGY 2011-08-23
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Trafficability::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "default", limited_.GetData() );
    if( limited_.GetData() )
        xos << xml::attribute( "max-weight", maxWeight_.GetData() );
}

//! @name ADN_CapacityInfos_Disaster
//@{
// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Disaster
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Disaster::ADN_CapacityInfos_Disaster()
    : disaster_( ADN_Workspace::GetWorkspace().GetDisasters().GetData().GetDisastersInfos(), 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::~ADN_CapacityInfos_Disaster
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Disaster::~ADN_CapacityInfos_Disaster()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Disaster::ReadArchive( xml::xistream& xis )
{
    std::string model;
    xis >> xml::attribute( "model", model );
    disaster_ = ADN_Workspace::GetWorkspace().GetDisasters().GetData().FindDisaster( model );
    if( disaster_.GetData() )
        bPresent_ = true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Disaster::WriteArchive( xml::xostream& xos )
{
    std::string model = disaster_.GetData() ? disaster_.GetData()->strName_.GetData() : "";
    xos << xml::attribute( "model", model );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::CheckDatabaseValidity
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Disaster::CheckDatabaseValidity( ADN_ConsistencyChecker& checker,
                                                                          const ADN_Type_String& objectName ) const
{
    if( !disaster_.GetData() && bPresent_.GetData() )
        checker.AddError( eMissingDisaster, objectName.GetData(), eObjects );
}
//@}

//! @name ADN_CapacityInfos_Attrition
//@{

ADN_Objects_Data::ADN_CapacityInfos_Attrition::ADN_CapacityInfos_Attrition()
    : ammoCategory_     ( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition ).categories_, 0 )
    , mineCategory_     ( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Mine ).categories_, 0 )
    , explosiveCategory_( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Explosif ).categories_, 0 )
    , useAmmo_          ( false )
    , useMine_          ( false )
    , useExplo_         ( false )
    , attritionSurface_ ( 0 )
    , ph_               ( 0 )
{
    // NOTHING
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
        if( pCategory->category_.GetData() == ENT_Tr::ConvertFromDotationFamily( eDotationFamily_Munition, ENT_Tr_ABC::eToSim ) )
        {
            ammoCategory_ = pCategory;
            useAmmo_ = true;
        }
        if( pCategory->category_.GetData() == ENT_Tr::ConvertFromDotationFamily( eDotationFamily_Mine, ENT_Tr_ABC::eToSim ) )
        {
            mineCategory_ = pCategory;
            useMine_ = true;
        }
        if( pCategory->category_.GetData() == ENT_Tr::ConvertFromDotationFamily( eDotationFamily_Explosif, ENT_Tr_ABC::eToSim ) )
        {
            explosiveCategory_ = pCategory;
            useExplo_ = true;
        }
        if( !useAmmo_.GetData() && !useMine_.GetData() && !useExplo_.GetData() )
            throw ADN_DataException( "Donnée invalide", "Capacité attrition invalide" );
    }
    attritionSurface_ = xis.attribute< double >( "attrition-surface", 0 );
    ph_ = xis.attribute< double >( "ph", 0 );
}

void ADN_Objects_Data::ADN_CapacityInfos_Attrition::WriteArchive( xml::xostream& xos )
{
    if( useAmmo_.GetData() )
        xos << xml::attribute( "category", ammoCategory_.GetData()->strName_ );
    else if( useMine_.GetData() )
        xos << xml::attribute( "category", mineCategory_.GetData()->strName_ );
    else if( useExplo_.GetData() )
        xos << xml::attribute( "category", explosiveCategory_.GetData()->strName_ );
    else
        xos << xml::attribute( "category", "" );

    xos << xml::attribute( "attrition-surface", attritionSurface_.GetData() )
        << xml::attribute( "ph", ph_ );
}
//@}

//! @name ADN_CapacityInfos_UrbanDestruction
//@{

ADN_Objects_Data::ADN_CapacityInfos_UrbanDestruction::ADN_CapacityInfos_UrbanDestruction()
    : modifUrbanBlocks_     ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
{
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_UrbanDestruction::ReadArchive( xml::xistream& xis )
{
    struct UrbanDestructionScore
    {
        static void Read( xml::xistream& xis, helpers::T_UrbanAttritionInfos_Vector& urbanData )
        {
            std::string material( xis.attribute< std::string >( "material-type" ) );
            helpers::IT_UrbanAttritionInfos_Vector it = std::find_if( urbanData.begin(), urbanData.end(), helpers::ADN_UrbanAttritionInfos::Cmp( material ) );
            if( it == urbanData.end() )
                throw ADN_DataException( tr( "Invalid data" ).toStdString(), tr( "Object - Invalid Urban Material type '%1'" ).arg( material.c_str() ).toStdString() );
            (*it)->ReadArchive( xis );
        }
    };

    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    xis >> xml::list( "urban-modifier", boost::bind( &UrbanDestructionScore::Read, _1, boost::ref( modifUrbanBlocks_ ) ) );
}

void ADN_Objects_Data::ADN_CapacityInfos_UrbanDestruction::WriteArchive( xml::xostream& xos )
{
    for( helpers::CIT_UrbanAttritionInfos_Vector itUrbanAttrition = modifUrbanBlocks_.begin(); itUrbanAttrition != modifUrbanBlocks_.end(); ++itUrbanAttrition )
        (*itUrbanAttrition)->WriteArchive( xos );
}

//! @name ADN_CapacityInfos_Contamination
//@{
ADN_Objects_Data::ADN_CapacityInfos_Contamination::ADN_CapacityInfos_Contamination()
    : max_toxic_( 0 )
{
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_Contamination::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    max_toxic_ = xis.attribute< int >( "max-toxic" );
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
    // NOTHING
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
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Flood - Injuries data sum > 100" ).toStdString() );
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
            throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Flood - Wound '%1' data < 0 or > 1" ).arg( wound.c_str() ).toStdString() );
    }
    else
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Flood - Invalid wound type '%1'" ).arg( wound.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_CapacityInfos_Flood::WriteArchive
// Created: JSR 2011-01-10
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Flood::WriteArchive( xml::xostream& xos )
{
    xos << xml::start( "injuries" );
    if( nNbHurtHumans1_.GetData() + nNbHurtHumans2_.GetData() + nNbHurtHumans3_.GetData() + nNbHurtHumansE_.GetData() + nNbDeadHumans_.GetData() > 100 )
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Flood - Injuries data sum > 100" ).toStdString() );
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
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_InteractionHeight::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    height_ = xis.attribute< double >( "height" );
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
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_Intoxication::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    max_toxic_ = xis.attribute< int >( "max-toxic" );
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
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_Population::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    density_ = xis.attribute< double >( "density" );
}

void ADN_Objects_Data::ADN_CapacityInfos_Population::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "density", density_ );
}
//@}

//! @name ADN_CapacityInfos_Propagation
//@{
ADN_Objects_Data::ADN_CapacityInfos_Propagation::ADN_CapacityInfos_Propagation()
    : nModel_( eFire )
{
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_Propagation::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    nModel_ = ADN_Tr::ConvertToPropagationModel( xis.attribute< std::string >( "model" ) );
}

void ADN_Objects_Data::ADN_CapacityInfos_Propagation::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "model", nModel_.Convert() );
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
    // NOTHING
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
    // NOTHING
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
    // NOTHING
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
    // NOTHING
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

//! @name ADN_CapacityInfos_Bridging
//@{
ADN_Objects_Data::ADN_CapacityInfos_Bridging::ADN_CapacityInfos_Bridging()
    : type_( eBridge )
{
    // NOTHING
}

void ADN_Objects_Data::ADN_CapacityInfos_Bridging::ReadArchive( xml::xistream& xis )
{
    helpers::ADN_TypeCapacity_Infos::ReadArchive( xis );
    type_ = ENT_Tr::ConvertToCrossingType( xis.attribute< std::string >( "type" ) );
}

void ADN_Objects_Data::ADN_CapacityInfos_Bridging::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( "type", type_.Convert() );
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
    // NOTHING
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
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Objects - Invalid level '%1'" ).arg( level.c_str() ).toStdString() );
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
    : object_      ( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(), 0 )
    , rActionRange_( 0 )
    , objectName_  ( "" )
    , nbc_         ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Spawn::ReadArchive( xml::xistream& input )
{
    bPresent_ = true;
    input >> xml::attribute( "object", objectName_ )
          >> xml::attribute( "action-range", rActionRange_ )
          >> xml::attribute( "nbc", nbc_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::Load
// Created: LGY 2011-05-24
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Spawn::Load( const std::string& parentName )
{
    if( !bPresent_.GetData() )
        return;

    if( objectName_.empty() )
    {
        QMessageBox::warning( 0, qApp->translate( "ADN_Objects_Data", "Reference error" ),
                              qApp->translate( "ADN_Objects_Data", "Empty object referenced by object '%1'.\nThe spawn capacity will be disabled." ).arg( parentName.c_str() ).arg( objectName_.c_str() ),
                              QMessageBox::Ok | QMessageBox::Default );
        bPresent_ = false;
        return;
    }

    object_ = ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( objectName_ );
    if( object_.GetData() == 0 )
    {
        QMessageBox::warning( 0, qApp->translate( "ADN_Objects_Data", "Reference error" ),
                              qApp->translate( "ADN_Objects_Data", "Unknown object '%2' referenced by object '%1'.\nThe spawn capacity will be disabled." ).arg( parentName.c_str() ).arg( objectName_.c_str() ),
                              QMessageBox::Ok | QMessageBox::Default );
        bPresent_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Spawn::WriteArchive( xml::xostream& output )
{
    std::string type = ( object_.GetData() ) ? object_.GetData()->strType_.GetData() : objectName_;
    output << xml::attribute( "object", type )
           << xml::attribute( "action-range", rActionRange_ )
           << xml::attribute( "nbc", nbc_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_Structural
// Created: SLG 2010-06-25
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_Structural::ADN_CapacityInfos_Structural()
    : rStructuralState_( 100 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ReadArchive
// Created: SLG 2010-06-25
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_Structural::ReadArchive( xml::xistream& input )
{
    bPresent_ = true;
    rStructuralState_ = input.attribute< int >( "value" );
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
    std::string strAttitude = input.attribute< std::string >( "attitude" );
    attitude_ = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
    if( attitude_ == static_cast< E_PopulationAttitude >( -1 ) )
        throw ADN_DataException( "Invalid data", tools::translate( "Object_Data", "Crowd types - Invalid crowd attitude '%1'" ).arg( strAttitude.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::WriteArchive
// Created: MGD 2010-03-19
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_AttitudeModifier::WriteArchive( xml::xostream& output )
{
    output << xml::attribute( "attitude", attitude_.Convert() );
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
    blinded_ = input.attribute< bool >( "blinded" );
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
    humanByTimeStep_ = input.attribute< int >( "human-by-time-step" );
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
    // NOTHING
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
        throw ADN_DataException( tr( "Invalid data" ).toStdString(), tr( "Fire propagation modifier - Invalid fire class '%1'" ).arg( fireClass.c_str() ).toStdString() );
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
// Name: ADN_Objects_Data::ADN_CapacityInfos_InteractWithSide
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
ADN_Objects_Data::ADN_CapacityInfos_InteractWithSide::ADN_CapacityInfos_InteractWithSide()
    : bFriendSide_ ( false )
    , bEnemySide_  ( false )
    , bNeutralSide_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_InteractWithSide::ReadArchive
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_InteractWithSide::ReadArchive( xml::xistream& input )
{
    bPresent_ = true;
    std::string friends, enemy, neutral;
    input >> xml::optional
          >> xml::attribute( "friend", friends )
          >> xml::optional
          >> xml::attribute( "enemy", enemy )
          >> xml::optional
          >> xml::attribute( "neutral", neutral );

    bFriendSide_  = ( friends.compare( "true" ) == 0 );
    bEnemySide_   = ( enemy.compare( "true" ) == 0 );
    bNeutralSide_ = ( neutral.compare( "true" ) == 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::ADN_CapacityInfos_InteractWithSide::WriteArchive
// Created: CMA 2012-05-10
// -----------------------------------------------------------------------------
void ADN_Objects_Data::ADN_CapacityInfos_InteractWithSide::WriteArchive( xml::xostream& output )
{
    if( bFriendSide_.GetData() )
        output << xml::attribute( "friend", "true" );
    if( bEnemySide_.GetData() )
        output << xml::attribute( "enemy", "true" );
    if( bNeutralSide_.GetData() )
        output << xml::attribute( "neutral", "true" );
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
INIT_DATA( ADN_CapacityInfos_InteractWithSide,        "InteractWithSide",        "interact-with-enemy" );
INIT_DATA( ADN_CapacityInfos_Interference,            "Interference",            "interference" );
INIT_DATA( ADN_CapacityInfos_Improvable,              "Improvable",              "improvable" );
INIT_DATA( ADN_CapacityInfos_Intoxication,            "Intoxication",            "intoxication" );
INIT_DATA( ADN_CapacityInfos_Logistic,                "Logistic",                "logistic" );
INIT_DATA( ADN_CapacityInfos_Mobility,                "Mobility",                "mobility" );
INIT_DATA( ADN_CapacityInfos_Trafficability,          "Trafficability",          "trafficability" );
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
INIT_DATA( ADN_CapacityInfos_Lodging,                 "Lodging",                 "lodging" );
INIT_DATA( ADN_CapacityInfos_AltitudeModifier,        "AltitudeModifier",        "altitude-modifier" );
INIT_DATA( ADN_CapacityInfos_UndergroundNetwork,      "UndergroundNetwork",      "underground-network" );
INIT_DATA( ADN_CapacityInfos_UrbanDestruction,        "UrbanDestruction",        "urban-destruction" );
INIT_DATA( ADN_CapacityInfos_FireForbidden,           "FireForbidden",           "fire-forbidden" );
INIT_DATA( ADN_CapacityInfos_Border,                  "Border",                  "border" );
INIT_DATA( ADN_CapacityInfos_Disaster,                "Disaster",                "disaster" );

#pragma warning( pop )

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

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::CheckDatabaseValidity
// Created: JSR 2012-02-16
// -----------------------------------------------------------------------------
void ADN_Objects_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    for( CIT_ObjectsInfos_Vector it = vObjectInfos_.begin(); it != vObjectInfos_.end(); ++it )
        ( *it )->CheckDatabaseValidity( checker );
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
    ADN_Objects_Data_ObjectInfos* pObjInfo = new ADN_Objects_Data_ObjectInfos( xis.attribute< std::string >( "type" ) );
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

    for( IT_ObjectsInfos_Vector it = vObjectInfos_.begin(); it != vObjectInfos_.end(); ++it )
    {
        ADN_Objects_Data::ADN_CapacityInfos_Spawn* spawn = static_cast< ADN_Objects_Data::ADN_CapacityInfos_Spawn* >( ( *it )->capacities_[ ADN_Objects_Data::ADN_CapacityInfos_Spawn::TAG ].get() );
        spawn->Load( ( *it )->strName_.GetData() );
    }
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
// Name: ADN_Objects_Data::GetObjectsThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Objects_Data::GetObjectsThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    for( IT_ObjectsInfos_Vector it = vObjectInfos_.begin(); it != vObjectInfos_.end(); ++it )
    {
        ADN_CapacityInfos_Spawn* spawn = static_cast< ADN_CapacityInfos_Spawn* >( ( *it )->capacities_[ ADN_CapacityInfos_Spawn::TAG ].get() );
         if( spawn->bPresent_.GetData() && spawn->object_.GetData()->strName_.GetData() == object.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::GetObjectsThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Objects_Data::GetObjectsThatUse( ADN_Equipement_Data::CategoryInfo& object )
{
    QStringList result;
    for( IT_ObjectsInfos_Vector itObject = vObjectInfos_.begin(); itObject != vObjectInfos_.end(); ++itObject )
    {
        bool added = false;
        ADN_CapacityInfos_Constructor* constructor = static_cast< ADN_CapacityInfos_Constructor* >( ( *itObject )->capacities_[ ADN_CapacityInfos_Constructor::TAG ].get() );
        if( constructor && constructor->bPresent_.GetData() )
        {
            // Buildable
            if( constructor->ptrBuildable_.get() && constructor->ptrBuildable_->bPresent_.GetData() )
                for( ADN_CapacityInfos_Buildable::CIT_Categories it = constructor->ptrBuildable_->categories_.begin(); !added && it != constructor->ptrBuildable_->categories_.end(); ++it )
                    if( ( *it )->ptrCategory_.GetData()->strName_.GetData() == object.strName_.GetData() )
                    {
                        added = true;
                        result << ( *itObject )->strName_.GetData().c_str();
                    }
            // Improvable
            if( !added && constructor->ptrImprovable_.get() && constructor->ptrImprovable_->bPresent_.GetData() )
                for( ADN_CapacityInfos_Buildable::CIT_Categories it = constructor->ptrImprovable_->categories_.begin(); !added && it != constructor->ptrImprovable_->categories_.end(); ++it )
                    if( ( *it )->ptrCategory_.GetData()->strName_.GetData() == object.strName_.GetData() )
                    {
                        added = true;
                        result << ( *itObject )->strName_.GetData().c_str();
                    }
        }

        // Attrition
        ADN_CapacityInfos_Attrition* attrition = static_cast< ADN_CapacityInfos_Attrition* >( ( *itObject )->capacities_[ ADN_CapacityInfos_Attrition::TAG ].get() );
        if( !added && attrition && attrition->bPresent_.GetData() &&
            ( ( attrition->useAmmo_.GetData() && attrition->ammoCategory_.GetData()->strName_ == object.strName_.GetData() ) ||
              ( attrition->useMine_.GetData() && attrition->mineCategory_.GetData()->strName_ == object.strName_.GetData() ) ||
              ( !attrition->useExplo_.GetData() && attrition->explosiveCategory_.GetData()->strName_ == object.strName_.GetData() ) ) )
            {
                added = true;
                result << ( *itObject )->strName_.GetData().c_str();
            }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::GetObjectsWithCapacity
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Objects_Data::GetObjectsWithCapacity( const std::string& tag )
{
    QStringList result;
    for( IT_ObjectsInfos_Vector it = vObjectInfos_.begin(); it != vObjectInfos_.end(); ++it )
    {
        helpers::ADN_TypeCapacity_Infos* capacity = ( *it )->capacities_[ tag ].get();
        if( capacity->bPresent_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::GetObjectsThatUse
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
QStringList ADN_Objects_Data::GetObjectsThatUse( ADN_Disasters_Data::DisasterInfos& disaster )
{
    QStringList result;
    for( IT_ObjectsInfos_Vector it = vObjectInfos_.begin(); it != vObjectInfos_.end(); ++it )
    {
        ADN_CapacityInfos_Disaster* disasterCapacity = static_cast< ADN_CapacityInfos_Disaster* >( ( *it )->capacities_[ ADN_CapacityInfos_Disaster::TAG ].get() );
        if( disasterCapacity && disasterCapacity->bPresent_.GetData() )
        {
            ADN_Disasters_Data::DisasterInfos* disasterInfos = disasterCapacity->disaster_.GetData();
            if( disasterInfos && disasterInfos->strName_.GetData() == disaster.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
        }
    }
    return result;
}
