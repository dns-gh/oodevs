// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Population.h"
#include "DEC_PopulationDecision.h"
#include "DEC_PopulationKnowledge.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Workspace.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "MIL_AgentServer.h"
#include "MIL_PopulationAttitude.h"
#include "MIL_PopulationConcentration.h"
#include "MIL_PopulationFlow.h"
#include "MIL_PopulationType.h"
#include "MissionController_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_AffinitiesMap.h"
#include "Tools/MIL_Color.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_MessageParameters.h"
#include "Tools/MIL_Tools.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "simulation_terrain/TER_Geometry.h"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/scoped_ptr.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Population )

namespace
{
    MIL_IDManager idManager_;
    template< typename R >
    void SaveRole( const MIL_Population& population, MIL_CheckPointOutArchive& file )
    {
        const R* const role = &population.GetRole< R >();
        file << role;
    }
}

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Population* population, const unsigned int /*version*/ )
{
    unsigned int nTypeID = population->GetType().GetID();
    const MissionController_ABC* const controller = &population->orderManager_->GetController();
    archive << nTypeID
            << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Population* population, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    MissionController_ABC* controller = 0;
    archive >> nTypeID
            >> controller;
    const MIL_PopulationType* pType = MIL_PopulationType::Find( nTypeID );
    assert( pType );
    ::new( population ) MIL_Population( *pType, *controller );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population( const MIL_PopulationType& type,
                                xml::xistream& xis,
                                MIL_Army_ABC& army,
                                MissionController_ABC& controller,
                                unsigned int gcPause,
                                unsigned int gcMult,
                                sword::DEC_Logger* logger )
    : MIL_Entity_ABC( xis, idManager_.GetId( xis.attribute< unsigned int >( "id" ), true ) )
    , pType_                      ( &type )
    , pArmy_                      ( &army )
    , pDefaultAttitude_           ( 0 )
    , rArmedIndividuals_          ( type.GetArmedIndividuals() )
    , rNewArmedIndividuals_       ( type.GetArmedIndividuals() )
    , rMale_                      ( type.GetMale() )
    , rFemale_                    ( type.GetFemale() )
    , rChildren_                  ( type.GetChildren() )
    , pKnowledge_                 ( new DEC_PopulationKnowledge( *this ) )
    , orderManager_               ( new MIL_PopulationOrderManager( controller, *this ) )
    , vBarycenter_                ( new MT_Vector2D() )
    , pColor_                     ( new MIL_Color( xis ) )
    , bPionMaxSpeedOverloaded_    ( false )
    , rOverloadedPionMaxSpeed_    ( 0. )
    , bBlinded_                   ( false )
    , bHasDoneMagicMove_          ( false )
    , criticalIntelligenceChanged_( true )
    , armedIndividualsChanged_    ( true )
    , isDamagingUrbanBlock_       ( false )
    , isDemonstrating_            ( false )
    , isInFire_                   ( false )
    , wasInFire_                  ( false )
    , pAffinities_                ( new MIL_AffinitiesMap( xis ) )
    , pExtensions_                ( new MIL_DictionaryExtensions( xis ) )
{
    std::string strAttitude;
    xis >> xml::attribute( "attitude", strAttitude );
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( strAttitude );
    if( !pDefaultAttitude_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown attitude" );
    xis >> xml::optional >> xml::start( "armed-individuals" )
            >> xml::attribute( "value", rArmedIndividuals_ )
        >> xml::end
        >> xml::optional >> xml::start( "repartition" )
            >> xml::attribute( "male", rMale_ )
            >> xml::attribute( "female", rFemale_ )
            >> xml::attribute( "children", rChildren_ )
        >> xml::end
        >> xml::optional >> xml::start( "critical-intelligence" )
            >> xml::attribute( "content", criticalIntelligence_ )
        >> xml::end;
    rNewArmedIndividuals_ = rArmedIndividuals_;
    RegisterRole( *new DEC_PopulationDecision( *this, gcPause, gcMult, logger ) );
    RegisterRole( *new DEC_Representations() );
    concentrations_.push_back( boost::make_shared< MIL_PopulationConcentration >( *this, xis ) );
    pArmy_->RegisterPopulation( *this );
    UpdateBarycenter();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population( const MIL_PopulationType& type, MissionController_ABC& controller )
    : MIL_Entity_ABC( type.GetName(), 0 )
    , pType_                      ( &type )
    , pArmy_                      ( 0 )
    , pDefaultAttitude_           ( MIL_PopulationAttitude::Find( "calme" ) )
    , rArmedIndividuals_          ( type.GetArmedIndividuals() )
    , rMale_                      ( type.GetMale() )
    , rFemale_                    ( type.GetFemale() )
    , rChildren_                  ( type.GetChildren() )
    , pKnowledge_                 ( new DEC_PopulationKnowledge( *this ) )
    , orderManager_               ( new MIL_PopulationOrderManager( controller, *this ) )
    , vBarycenter_                ( new MT_Vector2D() )
    , pColor_                     ( 0 )
    , bPionMaxSpeedOverloaded_    ( false )
    , rOverloadedPionMaxSpeed_    ( 0. )
    , bBlinded_                   ( false )
    , bHasDoneMagicMove_          ( false )
    , criticalIntelligenceChanged_( true )
    , armedIndividualsChanged_    ( true )
    , isDamagingUrbanBlock_       ( false )
    , isDemonstrating_            ( false )
    , isInFire_                   ( false )
    , wasInFire_                  ( false )
    , pAffinities_                ( 0 )
    , pExtensions_                ( 0 )
{
    UpdateBarycenter();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population( const MIL_PopulationType& type,
                                MIL_Army_ABC& army,
                                MissionController_ABC& controller,
                                const MT_Vector2D& point,
                                int number,
                                const std::string& name,
                                unsigned int gcPause,
                                unsigned int gcMult,
                                sword::DEC_Logger* logger,
                                unsigned int context )
    : MIL_Entity_ABC( name, idManager_.GetId() )
    , pType_                      ( &type )
    , pArmy_                      ( &army )
    , pDefaultAttitude_           ( 0 )
    , rArmedIndividuals_          ( type.GetArmedIndividuals() )
    , rNewArmedIndividuals_       ( rArmedIndividuals_ )
    , rMale_                      ( type.GetMale() )
    , rFemale_                    ( type.GetFemale() )
    , rChildren_                  ( type.GetChildren() )
    , pKnowledge_                 ( new DEC_PopulationKnowledge( *this ) )
    , orderManager_               ( new MIL_PopulationOrderManager( controller, *this ) )
    , vBarycenter_                ( new MT_Vector2D() )
    , bPionMaxSpeedOverloaded_    ( false )
    , rOverloadedPionMaxSpeed_    ( 0. )
    , bBlinded_                   ( false )
    , bHasDoneMagicMove_          ( false )
    , criticalIntelligenceChanged_( true )
    , armedIndividualsChanged_    ( true )
    , isDamagingUrbanBlock_       ( false )
    , isDemonstrating_            ( false )
    , isInFire_                   ( false )
    , wasInFire_                  ( false )
    , pAffinities_                ( new MIL_AffinitiesMap() )
    , pExtensions_                ( new MIL_DictionaryExtensions() )
{
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( "calme" );
    RegisterRole( *new DEC_PopulationDecision( *this, gcPause, gcMult, logger ) );
    RegisterRole( *new DEC_Representations() );
    pKnowledgeGroup_ = army.FindCrowdKnowledgeGroup();
    if( pKnowledgeGroup_ )
        pKnowledgeGroup_->RegisterPopulation( *this );
    pColor_.reset( new MIL_Color( army.GetColor() ) );
    SendCreation( context );
    concentrations_.push_back( boost::make_shared< MIL_PopulationConcentration >( *this, point, number ) );
    pArmy_->RegisterPopulation( *this );
    UpdateBarycenter();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population destructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population::~MIL_Population()
{
    if( pArmy_ )
        pArmy_->UnregisterPopulation( *this );
    if( pKnowledgeGroup_ )
        pKnowledgeGroup_->UnregisterPopulation( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDecision
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
const DEC_PopulationDecision& MIL_Population::GetDecision() const
{
    return GetRole< DEC_PopulationDecision>();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDecision
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
DEC_PopulationDecision& MIL_Population::GetDecision()
{
    return GetRole< DEC_PopulationDecision>();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::load
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_Population::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file >> pArmy_;
    idManager_.GetId( GetID(), true );
    unsigned int nAttitudeID;
    file >> nAttitudeID;
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( nAttitudeID );
    DEC_PopulationDecision* pRole;
    MIL_Color* pColor;
    assert( pDefaultAttitude_ );
    file >> rArmedIndividuals_
         >> rNewArmedIndividuals_
         >> rMale_
         >> rFemale_
         >> rChildren_
         >> criticalIntelligence_
         >> concentrations_
         >> flows_
         >> trashedConcentrations_
         >> trashedFlows_
         >> bPionMaxSpeedOverloaded_
         >> rOverloadedPionMaxSpeed_
         >> pKnowledge_
         >> pKnowledgeGroup_
         >> bHasDoneMagicMove_
         >> pAffinities_
         >> pExtensions_
         >> vBarycenter_
         >> pColor
         >> pRole;
    RegisterRole( *pRole );
    RegisterRole( *new DEC_Representations() );
    pColor_.reset( pColor );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_Population::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this );
    unsigned attitude = pDefaultAttitude_->GetID();
    const MIL_Color* const pColor = pColor_.get();
    file << pArmy_
         << attitude
         << rArmedIndividuals_
         << rNewArmedIndividuals_
         << rMale_
         << rFemale_
         << rChildren_
         << criticalIntelligence_
         << concentrations_
         << flows_
         << trashedConcentrations_
         << trashedFlows_
         << bPionMaxSpeedOverloaded_
         << rOverloadedPionMaxSpeed_
         << pKnowledge_
         << pKnowledgeGroup_
         << bHasDoneMagicMove_
         << pAffinities_
         << pExtensions_
         << vBarycenter_
         << pColor;
    SaveRole< DEC_PopulationDecision >( *this, file );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::WriteODB
// Created: NLD 2006-06-01
// -----------------------------------------------------------------------------
void MIL_Population::WriteODB( xml::xostream& xos ) const
{
    assert( pType_ );
    assert( pArmy_ );
    assert( pDefaultAttitude_ );
    assert( !concentrations_.empty() || !flows_.empty() );
    if( concentrations_.empty() && flows_.empty() )
        MT_LOG_ERROR_MSG( "Saving Population with no flow nor concentration." );
    xos << xml::start( "population" );
    MIL_Entity_ABC::WriteODB ( xos ) ;
    xos << xml::attribute( "id", GetID() )
        << xml::attribute( "type", pType_->GetName() )
        << xml::attribute( "attitude", pDefaultAttitude_->GetName() );
    pColor_->WriteODB( xos );
    if( !concentrations_.empty() )
        xos << xml::attribute( "position", MIL_Tools::ConvertCoordSimToMos( concentrations_.front()->GetPosition() ) );
    else if( !flows_.empty() )
        xos << xml::attribute( "position", MIL_Tools::ConvertCoordSimToMos( flows_.front()->GetPosition() ) );
    xos << xml::start( "composition" )
            << xml::attribute( "healthy", GetHealthyHumans() )
            << xml::attribute( "wounded", GetWoundedHumans() )
            << xml::attribute( "dead", GetDeadHumans() )
            << xml::attribute( "contaminated", GetContaminatedHumans() )
        << xml::end;
    if( rArmedIndividuals_ != pType_->GetArmedIndividuals() )
        xos << xml::start( "armed-individuals" )
                << xml::attribute( "value", rArmedIndividuals_ )
            << xml::end;
    if( rMale_ != pType_->GetMale() || rFemale_ != pType_->GetFemale() || rChildren_ != pType_->GetChildren() )
        xos << xml::start( "repartition" )
                << xml::attribute( "male", rMale_ )
                << xml::attribute( "female", rFemale_ )
                << xml::attribute( "children", rChildren_ )
            << xml::end;
    if( !criticalIntelligence_.empty() )
        xos << xml::start( "critical-intelligence" )
                << xml::attribute( "content", criticalIntelligence_ )
            << xml::end;
    pAffinities_->WriteODB( xos );
    pExtensions_->WriteODB( xos );
    xos << xml::end; // population
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::NotifyAttackedBy
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void MIL_Population::NotifyAttackedBy( const MIL_Agent_ABC& attacker )
{
    assert( pKnowledge_ );
    pKnowledge_->NotifyAttackedBy( attacker );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Secure
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void MIL_Population::Secure( const MIL_AgentPion& securer )
{
    assert( pKnowledge_ );
    pKnowledge_->NotifySecuredBy( securer );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Exterminate
// Created: SBO 2005-12-22
// -----------------------------------------------------------------------------
void MIL_Population::Exterminate( const MIL_AgentPion& exterminator, double rSurface )
{
    MIL_PopulationElement_ABC* pElement;
    while( rSurface > 0. )
    {
        pElement = GetClosestAliveElement( exterminator );
        if( !pElement )
            break;
        const double damage = pElement->Exterminate( rSurface );
        if( !damage )
            break;
        rSurface -= damage;
    }
    NotifyAttackedBy( exterminator );
    MIL_Report::PostEvent( *this, report::eRC_AttentatTerroristeDansPopulation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::NotifyChanneled
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
void MIL_Population::NotifyChanneled( const TER_Localisation& localisation )
{
    assert( pKnowledge_ );
    pKnowledge_->NotifyChanneled( localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::NotifyBurning
// Created: LDC 2014-07-10
// -----------------------------------------------------------------------------
void MIL_Population::NotifyBurning()
{
    if( !wasInFire_ )
        MIL_Report::PostEvent( *this, report::eRC_InFireObject );
    isInFire_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::NotifyUrbanDestructionStart
// Created: NPT 2013-02-13
// -----------------------------------------------------------------------------
void MIL_Population::NotifyUrbanDestructionStart()
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        (*it )->NotifyUrbanDestructionStart();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        ( *it )->NotifyUrbanDestructionStart();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateKnowledges
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void MIL_Population::UpdateKnowledges()
{
    assert( pKnowledge_ );
    pKnowledge_->Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::CleanKnowledges
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
void MIL_Population::CleanKnowledges()
{
    assert( pKnowledge_ );
    pKnowledge_->Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateDecision
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
void MIL_Population::UpdateDecision( float duration )
{
    try
    {
        orderManager_->Update();
        GetRole< DEC_Decision_ABC >().UpdateDecision( duration );
    }
    catch( const std::exception& e )
    {
        DEC_Decision_ABC* role = RetrieveRole< DEC_Decision_ABC >();
        if( role )
            role->LogError( &e );
        MIL_Report::PostEvent( *this, report::eRC_MissionImpossible );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateCrowdCollisions
// Created: JSR 2014-02-19
// -----------------------------------------------------------------------------
void MIL_Population::UpdateCrowdCollisions()
{
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        ( *itFlow )->UpdateCrowdCollisions();
}

namespace
{
    template< typename T >
    void Update( T& elements, T& trashed )
    {
        for( auto it = elements.begin(); it != elements.end(); )
        {
            auto element = *it;
            if( !element->Update() )
            {
                it = elements.erase( it );
                trashed.push_back( element );
            }
            else
                ++it;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_Population::UpdateState()
{
    try
    {
        trashedConcentrations_.clear();
        trashedFlows_.clear();
        Update( flows_, trashedFlows_ );
        Update( concentrations_, trashedConcentrations_ );
        UpdateBarycenter();
        UpdateAttackedPopulations();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating population " << GetName().c_str() << " : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_Population::Clean()
{
    DEC_PopulationDecision* roleDec = RetrieveRole< DEC_PopulationDecision >();
    if( roleDec )
        roleDec->Clean();
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        ( **itConcentration ).Clean();
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        ( **itFlow ).Clean();
    bHasDoneMagicMove_ = false;
    wasInFire_ = isInFire_;
    isInFire_ = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::ClearObjectCollisions
// Created: LDC 2014-01-16
// -----------------------------------------------------------------------------
void MIL_Population::ClearObjectCollisions()
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        (*it)->ClearObjectCollisions();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        (*it)->ClearObjectCollisions();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::IsDead
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
bool MIL_Population::IsDead() const
{
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        if( !( **itConcentration ).IsDead() )
            return false;
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        if( !( **itFlow ).IsDead() )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::IsInZone
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
bool MIL_Population::IsInZone( const TER_Localisation& loc ) const
{
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        if( ( **itConcentration ).IsInZone( loc ) )
            return true;
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        if( ( **itFlow ).IsInZone( loc ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetClosestPoint
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
MT_Vector2D MIL_Population::GetClosestPoint( const MT_Vector2D& refPos ) const
{
    MT_Vector2D closestPoint;
    double rMinDistance = std::numeric_limits< double >::max();
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        MT_Vector2D nearestPointTmp = ( **itConcentration ).GetLocation().ComputeBarycenter();
        double rDistance = refPos.Distance( nearestPointTmp );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            closestPoint = nearestPointTmp;
        }
    }
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
    {
        MT_Vector2D nearestPointTmp;
        ( **itFlow ).GetLocation().ComputeNearestPoint( refPos, nearestPointTmp );
        double rDistance = refPos.Distance( nearestPointTmp );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            closestPoint = nearestPointTmp;
        }
    }
    return closestPoint;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetClosestAliveElement
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
MIL_PopulationElement_ABC* MIL_Population::GetClosestAliveElement( const MIL_Agent_ABC& reference ) const
{
    const MT_Vector2D& position = reference.GetRole< PHY_RoleInterface_Location >().GetPosition();
    MIL_PopulationElement_ABC* pResult = 0;
    double rMinDistance = 0.0f;
    ComputeClosestAliveElement( position, pResult, rMinDistance );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetClosestConcentration
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
MIL_PopulationConcentration* MIL_Population::GetClosestConcentration( const MT_Vector2D& position, int maxDistance ) const
{
    MIL_PopulationConcentration* ret = 0;
    double minDistance = std::numeric_limits< double >::max();
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( ( *itConcentration)->IsValid() )
        {
            const double d = ( *itConcentration )->GetLocation().ComputeBarycenter().Distance( position );
            if( d <= maxDistance && d < minDistance )
            {
                ret = itConcentration->get();
                minDistance = d;
            }
        }
    }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetClosestElement
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void MIL_Population::ComputeClosestAliveElement( const MT_Vector2D& position, MIL_PopulationElement_ABC*& pClosestElement, double& rMinDistance ) const
{
    pClosestElement = 0;
    rMinDistance = std::numeric_limits< double >::max();
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( ( **itConcentration ).IsDead() )
            continue;
        MT_Vector2D nearestPoint;
        ( **itConcentration ).GetLocation().ComputeNearestPoint( position, nearestPoint );
        const double rDistance = position.Distance( nearestPoint );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            pClosestElement = itConcentration->get();
        }
}
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
    {
        if( ( **itFlow ).IsDead() )
            continue;
        MT_Vector2D nearestPoint;
        ( **itFlow ).GetLocation().ComputeNearestPoint( position, nearestPoint );
        double rDistance = position.Distance( nearestPoint );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            pClosestElement = itFlow->get();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetClosestPointAndDistance
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
void MIL_Population::GetClosestPointAndDistance( const TER_Localisation& loc, MT_Vector2D& closestPoint, double& rMinDistance ) const
{
    rMinDistance = std::numeric_limits< double >::max();
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( ( **itConcentration ).IsDead() )
            continue;
        MT_Vector2D nearestPointTmp;
        double rDistance;
        ( **itConcentration ).GetLocation().ComputeNearestPoint( loc, nearestPointTmp, rDistance );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            closestPoint = nearestPointTmp;
        }
}

    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
    {
        if( ( **itFlow ).IsDead() )
            continue;
        MT_Vector2D nearestPointTmp;
        double rDistance;
        ( **itFlow ).GetLocation().ComputeNearestPoint( loc, nearestPointTmp, rDistance );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            closestPoint = nearestPointTmp;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetClosestPoint
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
MT_Vector2D MIL_Population::GetClosestPoint( const TER_Localisation& loc ) const
{
    MT_Vector2D closestPoint;
    double rMinDistance;
    GetClosestPointAndDistance( loc, closestPoint, rMinDistance );
    return closestPoint;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDistanceTo
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
double MIL_Population::GetDistanceTo( const TER_Localisation& loc ) const
{
    MT_Vector2D closestPoint;
    double rMinDistance;
    GetClosestPointAndDistance( loc, closestPoint, rMinDistance );
    return rMinDistance;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetSecuringPoint
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_Population::GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const
{
    MIL_PopulationElement_ABC* pClosestElement = GetClosestAliveElement( securingAgent );
    if( !pClosestElement )
        return boost::shared_ptr< MT_Vector2D >();
    return pClosestElement->GetSecuringPoint( securingAgent );
}

namespace
{
    class SafetyPositionFunctor : private boost::noncopyable
    {
    public:
        virtual ~SafetyPositionFunctor() {}
        virtual boost::shared_ptr< MT_Vector2D > GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double rSeed ) = 0;
    };

    class ElementSafetyPositionFunctor : public SafetyPositionFunctor
    {
    public:
        explicit ElementSafetyPositionFunctor( const MIL_PopulationElement_ABC& closestElement ) : closestElement_( closestElement ) {}
        virtual boost::shared_ptr< MT_Vector2D > GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double rSeed )
        {
            return closestElement_.GetSafetyPosition( agent, rMinDistance, rSeed );
        }
    private:
        const MIL_PopulationElement_ABC& closestElement_;
    };

    class DeadSafetyPositionFunctor : public SafetyPositionFunctor
    {
    public:
        DeadSafetyPositionFunctor( const MT_Vector2D& agentPosition, const MT_Vector2D& closestPoint )
            : agentPosition_( agentPosition )
            , closestPoint_( closestPoint )
        {}
        virtual boost::shared_ptr< MT_Vector2D > GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance, double rSeed )
        {
            MT_Vector2D evadeDirection = ( agentPosition_ - closestPoint_ ).Normalize().Rotate( rSeed );
            if( evadeDirection.IsZero() )
                evadeDirection = -agent.GetOrderManager().GetDirDanger();
            MT_Vector2D safetyPos = closestPoint_ + evadeDirection * rMinDistance;
            TER_World::GetWorld().ClipPointInsideWorld( safetyPos );
            return boost::make_shared< MT_Vector2D >( safetyPos );
        }
    private:
        const MT_Vector2D& agentPosition_;
        const MT_Vector2D& closestPoint_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_Population::GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance ) const
{
    MIL_PopulationElement_ABC* pClosestElement = GetClosestAliveElement( agent );
    std::auto_ptr< SafetyPositionFunctor > functor;
    if( !pClosestElement )
    {
        const MT_Vector2D& agentPosition = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
        MT_Vector2D closestPoint = GetClosestPoint( agentPosition );
        functor.reset( new DeadSafetyPositionFunctor( agentPosition, closestPoint ) );
    }
    else
        functor.reset( new ElementSafetyPositionFunctor( *pClosestElement ) );
    double rSeed = 0.0f;
    int nIteration = 1;
    while( rSeed < MT_PI )
    {
        auto safetyPoint = functor->GetSafetyPosition( agent, rMinDistance, rSeed );
        // $$$$ SBO 2006-02-22: { 0; pi/4; -pi/4; pi/2; -pi/2; 3pi/4; -3pi/4; pi }
        rSeed += ( nIteration % 2 ? nIteration : -nIteration ) * MT_PI / 4;
        ++nIteration;
        if( GetClosestPoint( *safetyPoint ).Distance( *safetyPoint ) >= rMinDistance )
            return safetyPoint;
    }
    return boost::shared_ptr< MT_Vector2D >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetBarycenter
// Created: MGD 2010-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_Population::GetBarycenter() const
{
    return vBarycenter_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetConcentrationPosition
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_Population::GetConcentrationPosition( unsigned int concentrationId ) const
{
    for( auto itC = concentrations_.cbegin(); itC != concentrations_.end(); ++itC )
        if( ( *itC )->GetID() == concentrationId )
            return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( ( *itC )->GetPosition() ) );
    return boost::shared_ptr< MT_Vector2D >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HasFlow
// Created: LGY 2010-12-27
// -----------------------------------------------------------------------------
bool MIL_Population::HasFlow() const
{
    return !flows_.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetAttitude
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude& MIL_Population::GetAttitude() const
{
    const MIL_PopulationAttitude* attitude = &GetDefaultAttitude();
    for( auto itC = concentrations_.cbegin(); itC != concentrations_.end(); ++itC )
        if( (*itC)->GetAttitude() > *attitude )
            attitude = &(*itC)->GetAttitude();
    for( auto itF = flows_.cbegin(); itF != flows_.end(); ++itF )
        if( (*itF)->GetAttitude() > *attitude )
            attitude = &(*itF)->GetAttitude();
    return *attitude;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetAllHumans
// Created: JSR 2011-03-16
// -----------------------------------------------------------------------------
unsigned int MIL_Population::GetAllHumans() const
{
    unsigned int nResult = 0;
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetAllHumans();
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        nResult += ( **itFlow ).GetAllHumans();
    return nResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetHealthyHumans
// Created: SBO 2006-02-22
// -----------------------------------------------------------------------------
unsigned int MIL_Population::GetHealthyHumans() const
{
    unsigned int nResult = 0;
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetHealthyHumans();
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        nResult += ( **itFlow ).GetHealthyHumans();
    return nResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetWoundedHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_Population::GetWoundedHumans() const
{
    unsigned int nResult = 0;
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetWoundedHumans();
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        nResult += ( **itFlow ).GetWoundedHumans();
    return nResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetContaminatedHumans
// Created: JSR 2011-03-10
// -----------------------------------------------------------------------------
unsigned int MIL_Population::GetContaminatedHumans() const
{
    unsigned int nResult = 0;
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetContaminatedHumans();
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        nResult += ( **itFlow ).GetContaminatedHumans();
    return nResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDeadHumans
// Created: SBO 2006-02-22
// -----------------------------------------------------------------------------
unsigned int MIL_Population::GetDeadHumans() const
{
    unsigned int nResult = 0;
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetDeadHumans();
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        nResult += ( **itFlow ).GetDeadHumans();
    return nResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetAllHumansInConcentration
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
unsigned int MIL_Population::GetAllHumansInConcentration( unsigned int concentrationId )
{
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        if( ( *itConcentration )->GetID() == concentrationId )
            return ( *itConcentration )->GetAllHumans();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HealWounded
// Created: JSR 2012-02-15
// -----------------------------------------------------------------------------
void MIL_Population::HealWounded()
{
    ChangeComposition( GetHealthyHumans() + GetWoundedHumans(), 0, GetContaminatedHumans(), GetDeadHumans() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::AddArmedHumans
// Created: MMC 2013-06-04
// -----------------------------------------------------------------------------
void MIL_Population::AddArmedHumans( unsigned int armedHumans )
{
    double armedIndividuals = GetArmedIndividuals();
    unsigned int nCanBeArmed = 0;
    unsigned int allHumans = GetAllHumans();
    unsigned int healthyHumans = GetHealthyHumans();
    if( healthyHumans == 0 || allHumans == 0 )
        return;
    unsigned int curArmedHumans = static_cast< unsigned int >( armedIndividuals * allHumans );
    if( healthyHumans > curArmedHumans  )
        nCanBeArmed = healthyHumans - curArmedHumans;
    if( nCanBeArmed < armedHumans )
        armedHumans = nCanBeArmed;
    double newArmedRatio = static_cast< double > ( curArmedHumans + armedHumans ) / static_cast< double >( allHumans );
    SetArmedIndividuals( std::min( newArmedRatio, 1. ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetArmedIndividuals
// Created: MMC 2011-03-28
// -----------------------------------------------------------------------------
double MIL_Population::GetArmedIndividuals() const
{
    return rArmedIndividuals_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetArmedIndividuals
// Created: JSR 2011-04-04
// -----------------------------------------------------------------------------
void MIL_Population::SetArmedIndividuals( double armedIndividuals )
{
    if( rArmedIndividuals_ != armedIndividuals )
    {
        rArmedIndividuals_ = armedIndividuals;
        armedIndividualsChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetNewArmedIndividuals
// Created: LMT 2011-04-04
// -----------------------------------------------------------------------------
double MIL_Population::GetNewArmedIndividuals() const
{
    return rNewArmedIndividuals_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetNewArmedIndividuals
// Created: LMT 2011-04-04
// -----------------------------------------------------------------------------
void MIL_Population::SetNewArmedIndividuals( double newArmedIndividuals )
{
    rNewArmedIndividuals_ = newArmedIndividuals;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Move
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
void MIL_Population::Move( const MT_Vector2D& destination )
{
    if( bHasDoneMagicMove_ )
        return;
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        ( **itConcentration ).Move( destination );
    for( size_t i = 0; i < flows_.size(); ++i ) // $$$$ LDC Do NOT optimize the flow.size() away, flows_ is modified during iteration!!
    // for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow ) $$$$ LDC NO!!! flows_ can be lengthened during computation.
        flows_[i]->Move( destination );
    UpdateBarycenter();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::MoveAlong
// Created: LDC 2013-03-29
// -----------------------------------------------------------------------------
void MIL_Population::MoveAlong( const std::vector< boost::shared_ptr< MT_Vector2D > >& destination )
{
    if( bHasDoneMagicMove_ )
        return;
    if( destination.empty() )
        return;
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        ( **itConcentration ).Move( *destination.back() );
    for( size_t i = 0; i < flows_.size(); ++i ) // $$$$ LDC Do NOT optimize the flow.size() away, flows_ is modified during iteration!!
        flows_[i]->MoveAlong( destination );
    UpdateBarycenter();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::CancelMove
// Created: LDC 2013-03-29
// -----------------------------------------------------------------------------
void MIL_Population::CancelMove()
{
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        ( *it )->CancelMove();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::FireOnPions
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_Population::FireOnPions( double rIntensity, PHY_FireResults_Population& fireResult )
{
    if( !IsBlinded() )
    {
        for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
            ( **itConcentration ).FireOnPions( rIntensity, fireResult );
        for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
            ( **itFlow ).FireOnPions( rIntensity, fireResult );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::FireOnPion
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void MIL_Population::FireOnPion( double rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult )
{
    MIL_PopulationElement_ABC* pClosestElement = GetClosestAliveElement( target );
    if( pClosestElement )
        pClosestElement->FireOnPion( rIntensity, target, fireResult );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Attack
// Created: LGY 2012-02-14
// -----------------------------------------------------------------------------
void MIL_Population::Attack()
{
    for( auto itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        ( **itConcentration ).Attack();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::NotifyAttackedPopulation
// Created: JSR 2014-03-24
// -----------------------------------------------------------------------------
void MIL_Population::NotifyAttackedPopulation( const MIL_Population& target )
{
    if( attackedPopulations_.insert( &target ).second )
    {
        auto group = target.GetKnowledgeGroup();
        if( !group )
            return;
        const DEC_KnowledgeBlackBoard_KnowledgeGroup* blackboard = group->GetKnowledge();
        if( !blackboard )
            return;
        boost::shared_ptr< DEC_Knowledge_Population > pKnPopulation = blackboard->ResolveKnowledgePopulation( *this );
        if( pKnPopulation )
            MIL_Report::PostEvent( target, report::eRC_PriseAPartieParPopulation, pKnPopulation );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
double MIL_Population::GetDangerosity( const MIL_AgentPion& target ) const
{
    if( MIL_PopulationElement_ABC* pClosestElement = GetClosestAliveElement( target ) )
        return pClosestElement->GetDangerosity( target );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetAttitude
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void MIL_Population::SetAttitude( const MIL_PopulationAttitude& attitude )
{
    for( auto itConcentration = concentrations_.cbegin(); itConcentration != concentrations_.end(); ++itConcentration )
        ( **itConcentration ).SetAttitude( attitude );
    for( auto itFlow = flows_.cbegin(); itFlow != flows_.end(); ++itFlow )
        ( **itFlow ).SetAttitude( attitude );
}

// =============================================================================
// FLOWS / CONCENTRATION MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::CreateFlow
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
MIL_PopulationFlow& MIL_Population::CreateFlow( MIL_PopulationConcentration& concentration )
{
    flows_.push_back( boost::make_shared< MIL_PopulationFlow >( *this, concentration ) );
    return *flows_.back();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::CreateFlow
// Created: NLD 2007-03-01
// -----------------------------------------------------------------------------
MIL_PopulationFlow& MIL_Population::CreateFlow( const MIL_PopulationFlow& source, const MT_Vector2D& splitPoint )
{
    flows_.push_back( boost::make_shared< MIL_PopulationFlow >( *this, source, splitPoint ) );
    return *flows_.back();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetConcentration
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
MIL_PopulationConcentration& MIL_Population::GetConcentration( const MT_Vector2D& position )
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        if( ( **it ).IsNearPosition( position ) )
            return **it;
    concentrations_.push_back( boost::make_shared< MIL_PopulationConcentration >( *this, position ) );
    return *concentrations_.back();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::RetrieveConcentration
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
MIL_PopulationConcentration* MIL_Population::RetrieveConcentration( unsigned int concentrationId ) const
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        if( ( *it )->GetID() == concentrationId )
            return it->get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetMaxSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
double MIL_Population::GetMaxSpeed() const
{
    assert( pType_ );
    return pType_->GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDefaultFlowDensity
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
double MIL_Population::GetDefaultFlowDensity() const
{
    assert( pType_ );
    return pType_->GetDefaultFlowDensity();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetPionReloadingTimeFactor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
double MIL_Population::GetPionReloadingTimeFactor( double rDensity ) const
{
    assert( pType_ );
    return pType_->GetPionReloadingTimeFactor( rDensity );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetPionMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
double MIL_Population::GetPionMaxSpeed( const MIL_PopulationAttitude& attitude, double rDensity, const PHY_Volume& pionVolume ) const
{
    assert( pType_ );
    if( bPionMaxSpeedOverloaded_ )
        return rOverloadedPionMaxSpeed_;
    return pType_->GetPionMaxSpeed( attitude, rDensity, pionVolume );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
uint32_t MIL_Population::OnReceiveOrder( const sword::CrowdOrder& msg )
{
    return orderManager_->OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveFragOrder
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveFragOrder( const sword::FragOrder& msg, const std::function< void( uint32_t ) >& sendAck )
{
    orderManager_->OnReceiveFragOrder( msg, sendAck );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveCrowdMagicAction
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg )
{
    switch( msg.type() )
    {
    case sword::UnitMagicAction::crowd_total_destruction:
        OnReceiveMsgDestroyAll();
        break;
    case sword::UnitMagicAction::crowd_change_health_state:
        OnReceiveMsgChangeHealthState( msg.parameters() );
        break;
    case sword::UnitMagicAction::crowd_change_armed_individuals:
        OnReceiveMsgChangeArmedIndividuals( msg.parameters() );
        break;
    case sword::UnitMagicAction::crowd_change_attitude:
        OnReceiveMsgChangeAttitude( msg.parameters() );
        break;
    case sword::UnitMagicAction::crowd_change_affinities:
        pAffinities_->OnReceiveMsgChangeAffinities( msg );
        break;
    case sword::UnitMagicAction::change_extension:
        pExtensions_->OnReceiveMsgChangeExtensions( msg );
        break;
    case sword::UnitMagicAction::change_critical_intelligence:
        OnReceiveCriticalIntelligence( msg.parameters() );
        break;
    case sword::UnitMagicAction::reload_brain:
        OnReloadBrain( msg.parameters() );
        break;
    case sword::UnitMagicAction::change_brain_debug:
        OnChangeBrainDebug( msg.parameters() );
        break;
    default:
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode,
            sword::UnitActionAck::error_invalid_unit );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveCrowdMagicActionMoveTo
// Created: JSR 2010-04-08
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveCrowdMagicActionMoveTo( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    const auto& point = protocol::GetPoint( msg, 0 );
    MT_Vector2D position;
    MIL_Tools::ConvertCoordMosToSim( point, position );
    T_ConcentrationVector concentrations;
    concentrations.swap( concentrations_ );
    for( auto it = concentrations.begin(); it != concentrations.end(); ++it )
        (*it)->MagicMove( position );
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        (*it)->MagicMove( position );
    trashedConcentrations_.insert( trashedConcentrations_.end(), concentrations.begin(), concentrations.end() );
    trashedFlows_.insert( trashedFlows_.end(), flows_.begin(), flows_.end() );
    flows_.clear();
    bHasDoneMagicMove_ = true;
    UpdateBarycenter();
    UpdateNetwork();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgDestroyAll
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgDestroyAll()
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        ( **it ).KillAllHumans();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        ( **it ).KillAllHumans();
    DEC_PopulationDecision* roleDec = RetrieveRole< DEC_PopulationDecision >();
    if( roleDec )
        roleDec->Reset();
    orderManager_->CancelMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgChangeAttitude
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgChangeAttitude( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    const auto value = GET_ENUMERATION( sword::EnumCrowdAttitude, msg, 0 );
    const MIL_PopulationAttitude* attitude = MIL_PopulationAttitude::Find( value );
    protocol::Check( attitude, "must be a valid attitude enumeration", 0 );
    SetAttitude( *attitude );
}

namespace
{
    unsigned int Update( unsigned int& oldValue, unsigned int& number )
    {
        const unsigned int newValue = std::min( oldValue, number );
        number -= newValue;
        oldValue -= newValue;
        return newValue;
    }
    void ChangeHealthState( MIL_PopulationElement_ABC& element, unsigned int& healthy, unsigned int& contaminated, unsigned int& wounded, unsigned int& dead, float ratio )
    {
        const unsigned int elementTotal = element.GetAllHumans();
        element.PullHumans( elementTotal );
        unsigned int newNumber = static_cast< unsigned int >( ratio * elementTotal );
        const unsigned int newDead = Update( dead, newNumber );
        const unsigned int newWounded = Update( wounded, newNumber );
        const unsigned int newContaminated = Update( contaminated, newNumber );
        const unsigned int newHealthy = Update( healthy, newNumber );
        element.PushHumans( MIL_PopulationHumans( newHealthy, newContaminated, newWounded, newDead ) );
    }
    unsigned int GetParameter( const sword::MissionParameters& params, int idx )
    {
        const int quantity = protocol::GetQuantity( params, idx );
        protocol::Check( quantity >= 0, "must be positive", idx );
        return quantity;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgChangeHealthState
// Created: JSR 2011-03-16
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgChangeHealthState( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 4 );
    const unsigned int healthy = GetParameter( msg, 0 );
    const unsigned int wounded = GetParameter( msg, 1 );
    const unsigned int contaminated = GetParameter( msg, 2 );
    const unsigned int dead = GetParameter( msg, 3 );
    protocol::Check( healthy + wounded + contaminated + dead > 0, "at least one parameter must be positive" );
    ChangeComposition( healthy, wounded, contaminated, dead );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::ChangeComposition
// Created: BCI 2011-03-21
// -----------------------------------------------------------------------------
void MIL_Population::ChangeComposition( unsigned int healthy, unsigned int wounded, unsigned int contaminated, unsigned int dead )
{
    const unsigned int currentAllHumans = GetAllHumans();
    if( currentAllHumans == 0 )
    {
        concentrations_.push_back( boost::make_shared< MIL_PopulationConcentration >( *this, *vBarycenter_, MIL_PopulationHumans( healthy, contaminated, wounded, dead ) ) );
        UpdateBarycenter();
    }
    else
    {
        const float ratio = static_cast< float >( healthy + wounded + contaminated + dead ) / currentAllHumans;
        MIL_PopulationElement_ABC* last = 0;
        for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        {
            last = it->get();
            ChangeHealthState( **it, healthy, contaminated, wounded, dead, ratio );
        }
        for( auto it = flows_.begin(); it != flows_.end(); ++it )
        {
            last = it->get();
            ChangeHealthState( **it, healthy, contaminated, wounded, dead, ratio );
        }
        if( last )
            last->PushHumans( MIL_PopulationHumans( healthy, contaminated, wounded, dead ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::ComputeUrbanBlocDestruction
// Created: MMC 2011-03-31
// -----------------------------------------------------------------------------
double MIL_Population::ComputeUrbanBlocDestruction( MIL_UrbanObject_ABC* pUrbanObjet )
{
    const double densityRef = pType_->GetUrbanDestructionDensity( GetAttitude() );
    const double timeRef = pType_->GetUrbanDestructionTime( GetAttitude() ) ;
    if( densityRef <= 0.0 && timeRef <= 0.0 )
        return 0.0;

    double coveredArea = 0.0, currentArea = 0.0, areaDensitySum = 0.0;
    for( auto it = concentrations_.begin(); it != concentrations_.end();  ++it )
    {
        currentArea = TER_Geometry::IntersectionArea( pUrbanObjet->GetLocalisation(), (*it)->GetLocation() );
        coveredArea += currentArea;
        areaDensitySum += currentArea * (*it)->GetDensity();
    }
    /* FIXME : Either remove completely flows or replace IntersectionArea with proper computation
     * The geometry is a line: a polygon should be constructed out of it if we want to intersect
     * or just counting the number of people inside the block (but most often that will be 0).
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
    {
        currentArea = TER_Geometry::IntersectionArea( pUrbanObjet->GetLocalisation(), (*it)->GetLocation() );
        coveredArea += currentArea;
        areaDensitySum += currentArea * (*it)->GetDensity();
    }
    */
    if( coveredArea < 0.00001 /*epsilon*/ )
        return 0.0;

    const double urbanArea = pUrbanObjet->GetLocalisation().GetArea();
    const double areaFactor = std::min< double >( 1.0, coveredArea / urbanArea );
    if( densityRef <= 0.0 || timeRef <= 0.0 )
        return areaFactor;

    const double averageDensity = areaDensitySum / coveredArea;
    const double densityTimeFactor = std::min< double >( 1.0, averageDensity / ( densityRef * timeRef ) );
    return  areaFactor * densityTimeFactor;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgChangeArmedIndividuals
// Created: JSR 2011-03-16
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgChangeArmedIndividuals( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    const int quantity = protocol::GetQuantity( msg, 0 );
    protocol::Check( quantity >= 0 && quantity <= 100, "must be between 0 and 100", 0 );
    rArmedIndividuals_ = 0.01 * quantity;
    rNewArmedIndividuals_ = rArmedIndividuals_;
    armedIndividualsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_Population::SendCreation( unsigned int context ) const
{
    client::CrowdCreation asnMsg;
    asnMsg().mutable_crowd()->set_id( GetID() );
    asnMsg().mutable_type()->set_id( pType_->GetID() );
    asnMsg().mutable_party()->set_id( pArmy_->GetID() );
    asnMsg().set_name( GetName() );
    asnMsg().mutable_repartition()->set_male( static_cast< float >( rMale_ ) );
    asnMsg().mutable_repartition()->set_female( static_cast< float >( rFemale_ ) );
    asnMsg().mutable_repartition()->set_children( static_cast< float >( rChildren_ ) );
    pColor_->SendFullState( asnMsg );
    asnMsg.Send( NET_Publisher_ABC::Publisher(), context );
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        ( **it ).SendCreation( context );
    for( auto it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
        ( **it ).SendCreation( context );
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        ( **it ).SendCreation( context );
    for( auto it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
        ( **it ).SendCreation( context );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_Population::SendFullState() const
{
    client::CrowdUpdate asnMsg;
    asnMsg().mutable_crowd()->set_id( GetID() );
    const DEC_PopulationDecision* roleDec = RetrieveRole< DEC_PopulationDecision >();
    if( roleDec )
        roleDec->SendFullState( asnMsg );
    if( !criticalIntelligence_.empty() )
        asnMsg().set_critical_intelligence( criticalIntelligence_ );
    asnMsg().set_armed_individuals( static_cast< float >( rArmedIndividuals_ ) );
    pAffinities_->SendFullState( asnMsg );
    pExtensions_->SendFullState( asnMsg );
    asnMsg().set_healthy( GetHealthyHumans() );
    asnMsg().set_wounded( GetWoundedHumans() );
    asnMsg().set_contaminated( GetContaminatedHumans() );
    asnMsg().set_dead( GetDeadHumans() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        ( **it ).SendFullState();
    for( auto it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
        ( **it ).SendFullState();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        ( **it ).SendFullState();
    for( auto it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
        ( **it ).SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateNetwork
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_Population::UpdateNetwork()
{
    try
    {
        if( GetRole< DEC_PopulationDecision >().HasStateChanged() || criticalIntelligenceChanged_ || armedIndividualsChanged_ || pAffinities_->HasChanged() || pExtensions_->HasChanged() || HasHumansChanged() )
        {
            client::CrowdUpdate asnMsg;
            asnMsg().mutable_crowd()->set_id( GetID() );
            GetRole< DEC_PopulationDecision >().SendChangedState( asnMsg );
            if( criticalIntelligenceChanged_ && !criticalIntelligence_.empty() )
                asnMsg().set_critical_intelligence( criticalIntelligence_ );
            if( armedIndividualsChanged_ )
                asnMsg().set_armed_individuals( static_cast< float >( rArmedIndividuals_ ) );
            criticalIntelligenceChanged_ = false;
            armedIndividualsChanged_ = false;
            pAffinities_->UpdateNetwork( asnMsg );
            pExtensions_->UpdateNetwork( asnMsg );
            if( HasHumansChanged() )
            {
                asnMsg().set_healthy( GetHealthyHumans() );
                asnMsg().set_wounded( GetWoundedHumans() );
                asnMsg().set_contaminated( GetContaminatedHumans() );
                asnMsg().set_dead( GetDeadHumans() );
            }
            asnMsg.Send( NET_Publisher_ABC::Publisher() );
        }
        for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
            ( **it ).SendChangedState();
        for( auto it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
            ( **it ).SendChangedState();
        for( auto it = flows_.begin(); it != flows_.end(); ++it )
            ( **it ).SendChangedState();
        for( auto it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
            ( **it ).SendChangedState();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating network for population " << GetID() << " : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Apply
// Created: SBO 2006-02-24
// -----------------------------------------------------------------------------
void MIL_Population::Apply( MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >& visitor ) const
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        visitor.Visit( **it );
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        visitor.Visit( **it );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Apply
// Created: SBO 2006-02-24
// -----------------------------------------------------------------------------
void MIL_Population::Apply( MIL_EntitiesVisitor_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetKnowledge
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
const DEC_PopulationKnowledge& MIL_Population::GetKnowledge() const
{
    assert( pKnowledge_ );
    return *pKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetKnowledgeGroup
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
void MIL_Population::SetKnowledgeGroup( boost::shared_ptr< MIL_KnowledgeGroup > pKnowledgeGroup )
{
    pKnowledgeGroup_ = pKnowledgeGroup;
    if( pKnowledgeGroup_ )
        pKnowledgeGroup_->RegisterPopulation( *this );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MIL_KnowledgeGroup > MIL_Population::GetKnowledgeGroup
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > MIL_Population::GetKnowledgeGroup() const
{
    return pKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDefaultAttitude
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude& MIL_Population::GetDefaultAttitude() const
{
    assert( pDefaultAttitude_ );
    return *pDefaultAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetType
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
const MIL_PopulationType& MIL_Population::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetOrderManager
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
const MIL_PopulationOrderManager& MIL_Population::GetOrderManager() const
{
    return *orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetOrderManager
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
MIL_PopulationOrderManager& MIL_Population::GetOrderManager()
{
    return *orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetArmy
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_Population::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetPionMaxSpeed
// Created: NLD 2005-10-24
// -----------------------------------------------------------------------------
void MIL_Population::SetPionMaxSpeed( double rSpeed )
{
    if( rSpeed < 0. )
        throw MASA_EXCEPTION( "Setting max speed to less than 0" );
    bPionMaxSpeedOverloaded_ = true;
    rOverloadedPionMaxSpeed_ = rSpeed;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::ResetPionMaxSpeed
// Created: NLD 2005-10-24
// -----------------------------------------------------------------------------
void MIL_Population::ResetPionMaxSpeed()
{
    bPionMaxSpeedOverloaded_ = false;
    rOverloadedPionMaxSpeed_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HasDoneMagicMove
// Created: NLD 2005-12-07
// -----------------------------------------------------------------------------
bool MIL_Population::HasDoneMagicMove() const
{
    return bHasDoneMagicMove_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetBlinded
// Created: MGD 2010-03-15
// -----------------------------------------------------------------------------
void MIL_Population::SetBlinded( bool blinded )
{
    bBlinded_ = blinded;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::IsBlinded
// Created: MGD 2010-03-15
// -----------------------------------------------------------------------------
bool MIL_Population::IsBlinded() const
{
    return bBlinded_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateBarycenter
// Created: MGD 2010-09-28
// -----------------------------------------------------------------------------
void MIL_Population::UpdateBarycenter()
{
    if( concentrations_.empty() && flows_.empty() )
        return;
    MT_Vector2D barycenter;
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        barycenter += (*it)->GetPosition();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        barycenter += (*it)->GetPosition();
    barycenter = barycenter / static_cast< double >( concentrations_.size() + flows_.size() );
    vBarycenter_->rX_ = barycenter.rX_;
    vBarycenter_->rY_ = barycenter.rY_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HasHumansChanged
// Created: JSR 2011-05-16
// -----------------------------------------------------------------------------
bool MIL_Population::HasHumansChanged() const
{
    BOOST_FOREACH( const auto concentration, concentrations_ )
        if( concentration->HasHumansChanged() )
            return true;
    BOOST_FOREACH( const auto flow, flows_ )
        if( flow->HasHumansChanged() )
            return true;
    return false;
}

namespace
{
    bool IsCollidingWith( const MIL_PopulationElement_ABC& element, const MIL_Population& population )
    {
        auto flows = element.GetCollidingPopulationFlows();
        for( auto it = flows.begin(); it != flows.end(); ++it )
            if( &static_cast< const MIL_PopulationFlow* >( *it )->GetPopulation() == &population )
                return true;
        auto concentrations = element.GetCollidingPopulationConcentrations();
        for( auto it = concentrations.begin(); it != concentrations.end(); ++it )
            if( &static_cast< const MIL_PopulationConcentration* >( *it )->GetPopulation() == &population )
                return true;
        return false;
    }

    template< class T >
    bool HasElementCollidingWith( const T& elements, const MIL_Population& population )
    {
        for( auto it = elements.begin(); it != elements.end(); ++it )
            if( IsCollidingWith( **it, population ) )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateAttackedPopulations
// Created: JSR 2014-03-24
// -----------------------------------------------------------------------------
void MIL_Population::UpdateAttackedPopulations()
{
    for( auto itPopulation = attackedPopulations_.begin(); itPopulation != attackedPopulations_.end(); )
    {
        const MIL_Population& attacking = **itPopulation;
        if( HasElementCollidingWith( concentrations_, attacking) || HasElementCollidingWith( flows_, attacking ) )
            ++itPopulation;
        else
            itPopulation = attackedPopulations_.erase( itPopulation );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetUrbanDestructionState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
void MIL_Population::SetUrbanDestructionState( bool state )
{
    isDamagingUrbanBlock_ = state;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetUrbanDestructionState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool MIL_Population::GetUrbanDestructionState()
{
    return isDamagingUrbanBlock_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetDemonstrationState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
void MIL_Population::SetDemonstrationState( bool state )
{
    isDemonstrating_ = state;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDemonstrationState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool MIL_Population::GetDemonstrationState()
{
    return isDemonstrating_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetCriticalIntelligence
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
const std::string& MIL_Population::GetCriticalIntelligence() const
{
    return criticalIntelligence_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetUrbanBlockAngriness
// Created: BCI 2011-03-18
// -----------------------------------------------------------------------------
double MIL_Population::GetUrbanBlockAngriness() const
{
    return urbanBlockAngriness_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetUrbanBlockAngriness
// Created: BCI 2011-03-18
// -----------------------------------------------------------------------------
void MIL_Population::SetUrbanBlockAngriness( double angriness )
{
    urbanBlockAngriness_ = angriness;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetAffinity
// Created: ABR 2011-03-28
// -----------------------------------------------------------------------------
float MIL_Population::GetAffinity( unsigned long teamID ) const
{
    return pAffinities_->GetAffinity( teamID );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::CanEmitReports
// Created: NPT 2013-02-07
// -----------------------------------------------------------------------------
bool MIL_Population::CanEmitReports() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HasReachedDestination
// Created: NLD 2011-03-18
// -----------------------------------------------------------------------------
bool MIL_Population::HasReachedDestination( const MT_Vector2D& destination ) const
{
    const double weldValue = TER_World::GetWorld().GetWeldValue() * TER_World::GetWorld().GetWeldValue() / 10;
    BOOST_FOREACH( const auto concentration, concentrations_ )
        if( destination.SquareDistance( concentration->GetPosition() ) <= weldValue )
            return true;
    BOOST_FOREACH( const auto flow, flows_ )
        if( destination.SquareDistance( flow->GetPosition() ) <= weldValue )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HasReachedBlockBorder
// Created: DDA 2011-04-04
// -----------------------------------------------------------------------------
bool MIL_Population::HasReachedBlockBorder( const MIL_UrbanObject_ABC* pUrbanKnowledge ) const
{
    const TER_Localisation& localisation = pUrbanKnowledge->GetLocalisation();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        if( localisation.IsInside( (*it)->GetPosition() ) )
            return true;
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        if( localisation.IsInside( (*it)->GetPosition() ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HasReachedDestinationCompletely
// Created: NLD 2011-03-18
// -----------------------------------------------------------------------------
bool MIL_Population::HasReachedDestinationCompletely( const MT_Vector2D& destination ) const
{
    if( !flows_.empty() )
        return false;
    if( concentrations_.size() != 1 )
        return false;
    return destination.SquareDistance( concentrations_.front()->GetPosition() ) <= TER_World::GetWorld().GetWeldValue() * TER_World::GetWorld().GetWeldValue() / 10;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetFlowHeadPosition
// Created: EVH 2011-05-10
// -----------------------------------------------------------------------------
MT_Vector2D MIL_Population::GetFlowHeadPosition()
{
    if( !HasFlow() )
        throw MASA_EXCEPTION( "Asking head position of a crowd without flow." );
    return flows_.front()->GetPosition();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveCriticalIntelligence
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveCriticalIntelligence( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    criticalIntelligence_ = protocol::GetString( msg, 0 );
    criticalIntelligenceChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReloadBrain
// Created: AHC 2010-01-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReloadBrain( const sword::MissionParameters& msg )
{
    CancelAllActions();
    auto model = parameters::GetModel( msg, []( const std::string& model ){
        return MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPopulation( model );
    } );
    auto& role = GetRole< DEC_PopulationDecision >();
    const bool modified = model && model != &role.GetModel();
    if( modified )
        role.SetModel( *model );
    GetDecision().Reload( true, !modified );
    orderManager_->CancelMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnChangeBrainDebug
// Created: SLI 2013-06-21
// -----------------------------------------------------------------------------
void MIL_Population::OnChangeBrainDebug( const sword::MissionParameters& msg )
{
    protocol::CheckCount( msg, 1 );
    const bool activate = protocol::GetBool( msg, 0 );
    GetRole< DEC_PopulationDecision >().ActivateBrainDebug( activate );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::AddHidden
// Created: MCO 2013-05-07
// -----------------------------------------------------------------------------
void MIL_Population::AddHidden( MIL_Agent_ABC& agent )
{
    hidden_.insert( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::RemoveHidden
// Created: MCO 2013-05-07
// -----------------------------------------------------------------------------
void MIL_Population::RemoveHidden( MIL_Agent_ABC& agent )
{
    hidden_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetHidden
// Created: MCO 2013-05-07
// -----------------------------------------------------------------------------
const tools::Set< MIL_Agent_ABC* >& MIL_Population::GetHidden() const
{
    return hidden_;
}
