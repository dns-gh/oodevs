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
#include "MIL_PopulationType.h"
#include "MIL_PopulationConcentration.h"
#include "MIL_PopulationFlow.h"
#include "MIL_PopulationAttitude.h"
#include "DEC_PopulationDecision.h"
#include "DEC_PopulationKnowledge.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Workspace.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_AffinitiesMap.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_World.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/foreach.hpp>
#include "Tools/MIL_Geometry.h"
#include "Entities/Objects/UrbanObjectWrapper.h"

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
    archive << nTypeID;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Population* population, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    archive >> nTypeID;
    const MIL_PopulationType* pType = MIL_PopulationType::Find( nTypeID );
    assert( pType );
    ::new( population )MIL_Population( *pType);
}

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population( xml::xistream& xis, const MIL_PopulationType& type, MIL_Army_ABC& army, unsigned int gcPause, unsigned int gcMult )
    : MIL_Entity_ABC( xis )
    , pType_                      ( &type )
    , nID_                        ( xis.attribute< unsigned int >( "id" ) )
    , pArmy_                      ( &army )
    , pDefaultAttitude_           ( 0 )
    , rArmedIndividuals_          ( type.GetArmedIndividuals() )
    , rNewArmedIndividuals_       ( type.GetArmedIndividuals() )
    , rMale_                      ( type.GetMale() )
    , rFemale_                    ( type.GetFemale() )
    , rChildren_                  ( type.GetChildren() )
    , pKnowledge_                 ( 0 )
    , orderManager_               ( *this )
    , bPionMaxSpeedOverloaded_    ( false )
    , rOverloadedPionMaxSpeed_    ( 0. )
    , bBlinded_                   ( false )
    , bHasDoneMagicMove_          ( false )
    , criticalIntelligenceChanged_( false )
    , armedIndividualsChanged_    ( false )
    , pAffinities_                ( new MIL_AffinitiesMap( xis ) )
    , pExtensions_                ( new MIL_DictionaryExtensions( xis ) )
{
    idManager_.Lock( nID_ );
    std::string strAttitude;
    xis >> xml::attribute( "attitude", strAttitude );
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( strAttitude );
    if( !pDefaultAttitude_ )
        xis.error( "Unknown attitude" );
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
    pKnowledge_ = new DEC_PopulationKnowledge( *this );
    RegisterRole( *new DEC_PopulationDecision( *this, gcPause, gcMult ) );
    RegisterRole( *new DEC_Representations() );
    MIL_PopulationConcentration* pConcentration = new MIL_PopulationConcentration( *this, xis );
    concentrations_.push_back( pConcentration );
    pArmy_->RegisterPopulation( *this );
    vBarycenter_.reset( new MT_Vector2D() );
    UpdateBarycenter();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population(const MIL_PopulationType& type )
    : MIL_Entity_ABC( type.GetName() )
    , pType_                      ( &type )
    , nID_                        ( 0 )
    , pArmy_                      ( 0 )
    , pDefaultAttitude_           ( MIL_PopulationAttitude::Find( "calme" ) )
    , rArmedIndividuals_          ( type.GetArmedIndividuals() )
    , rMale_                      ( type.GetMale() )
    , rFemale_                    ( type.GetFemale() )
    , rChildren_                  ( type.GetChildren() )
    , pKnowledge_                 ( 0 )
    , orderManager_               ( *this )
    , bPionMaxSpeedOverloaded_    ( false )
    , rOverloadedPionMaxSpeed_    ( 0. )
    , bBlinded_                   ( false )
    , bHasDoneMagicMove_          ( false )
    , criticalIntelligenceChanged_( false )
    , armedIndividualsChanged_    ( false )
    , pAffinities_                ( 0 )
    , pExtensions_                ( 0 )
{
    pKnowledge_ = new DEC_PopulationKnowledge( *this );
    vBarycenter_.reset( new MT_Vector2D() );
    UpdateBarycenter();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population( const MIL_PopulationType& type, MIL_Army_ABC& army, const MT_Vector2D& point, int number, const std::string& name, unsigned int gcPause, unsigned int gcMult, unsigned int context )
    : MIL_Entity_ABC( name )
    , pType_                      ( &type )
    , nID_                        ( idManager_.GetFreeId() )
    , pArmy_                      ( &army )
    , pDefaultAttitude_           ( 0 )
    , rArmedIndividuals_          ( type.GetArmedIndividuals() )
    , rMale_                      ( type.GetMale() )
    , rFemale_                    ( type.GetFemale() )
    , rChildren_                  ( type.GetChildren() )
    , pKnowledge_                 ( 0 )
    , orderManager_               ( *this )
    , bPionMaxSpeedOverloaded_    ( false )
    , rOverloadedPionMaxSpeed_    ( 0. )
    , bBlinded_                   ( false )
    , bHasDoneMagicMove_          ( false )
    , criticalIntelligenceChanged_( false )
    , armedIndividualsChanged_    ( false )
    , pAffinities_                ( new MIL_AffinitiesMap() )
    , pExtensions_                ( new MIL_DictionaryExtensions() )
{
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( "calme" );
    pKnowledge_ = new DEC_PopulationKnowledge( *this );
    RegisterRole( *new DEC_PopulationDecision( *this, gcPause, gcMult ) );
    RegisterRole( *new DEC_Representations() );
    SendCreation( context );
    MIL_PopulationConcentration* pConcentration = new MIL_PopulationConcentration( *this, point, number );
    concentrations_.push_back( pConcentration );
    pArmy_->RegisterPopulation( *this );
    vBarycenter_.reset( new MT_Vector2D() );
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
    delete pKnowledge_;
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
    file >> const_cast< unsigned int& >( nID_ )
         >> const_cast< MIL_Army_ABC*& >( pArmy_ );
    idManager_.Lock( nID_ );
    MIL_AffinitiesMap* pAffinities;
    MIL_DictionaryExtensions* pExtensions;
    unsigned int nAttitudeID;
    file >> nAttitudeID;
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( nAttitudeID );
    assert( pDefaultAttitude_ );
    file >> rArmedIndividuals_
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
         >> bHasDoneMagicMove_
         >> pAffinities
         >> pExtensions;
    pAffinities_.reset( pAffinities );
    pExtensions_.reset( pExtensions );
    MT_Vector2D tmp;
    file >> tmp;
    vBarycenter_.reset( new MT_Vector2D( tmp ) );
    {
        DEC_PopulationDecision* pRole;
        file >> pRole;
        RegisterRole( *pRole );
        RegisterRole( *new DEC_Representations() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::save
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_Population::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const MIL_AffinitiesMap* const pAffinities = pAffinities_.get();
    const MIL_DictionaryExtensions* const pExtensions = pExtensions_.get();
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this );
    unsigned attitude = pDefaultAttitude_->GetID();
    file << nID_
         << pArmy_
         << attitude
         << rArmedIndividuals_
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
         << bHasDoneMagicMove_
         << pAffinities
         << pExtensions
         << (*vBarycenter_);
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
    xos << xml::start( "population" );
    MIL_Entity_ABC::WriteODB ( xos ) ;
    xos << xml::attribute( "id", nID_ )
        << xml::attribute( "type", pType_->GetName() )
        << xml::attribute( "attitude", pDefaultAttitude_->GetName() );
    if( !concentrations_.empty() )
        xos << xml::attribute( "position", MIL_Tools::ConvertCoordSimToMos( concentrations_.front()->GetPosition() ) );
    else
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
        double damage = pElement->Exterminate( rSurface );
        if( !damage )
            break;
        rSurface -= damage;
    }
    NotifyAttackedBy( exterminator );
    MIL_Report::PostEvent( *this, MIL_Report::eReport_TerroristAttackAgainstPopulation );
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
        orderManager_.Update();
        GetRole< DEC_Decision_ABC >().UpdateDecision( duration );
    }
    catch( std::exception& e )
    {
        GetRole< DEC_Decision_ABC >().LogError( &e );
        MIL_Report::PostEvent( *this, MIL_Report::eReport_MissionImpossible_ );
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
        for( CIT_ConcentrationVector it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
            delete *it;
        trashedConcentrations_.clear();
        for( CIT_FlowVector it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
            delete *it;
        trashedFlows_.clear();
        // Flows
        for( IT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); )
        {
            MIL_PopulationFlow* pFlow = *itFlow;
            if( !pFlow->Update() )
            {
                itFlow = flows_.erase( itFlow );
                trashedFlows_.push_back( pFlow );
            }
            else
                ++itFlow;
        }
        // Concentrations
        for( IT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); )
        {
            MIL_PopulationConcentration* pConcentration = *itConcentration;
            if( !pConcentration->Update() )
            {
                itConcentration = concentrations_.erase( itConcentration );
                trashedConcentrations_.push_back( pConcentration );
            }
            else
                ++itConcentration;
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating population " << GetName().c_str() << " : " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Clean
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void MIL_Population::Clean()
{
    GetRole< DEC_PopulationDecision >().Clean();
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        ( **itConcentration ).Clean();
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        ( **itFlow ).Clean();
    bHasDoneMagicMove_ = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::IsDead
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
bool MIL_Population::IsDead() const
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        if( !( **itConcentration ).IsDead() )
            return false;
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        if( ( **itConcentration ).IsInZone( loc ) )
            return true;
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( ( **itConcentration ).IsDead() )
            continue;
        MT_Vector2D nearestPointTmp = ( **itConcentration ).GetLocation().ComputeBarycenter();
        double rDistance = refPos.Distance( nearestPointTmp );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            closestPoint = nearestPointTmp;
        }
    }
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
    {
        if( ( **itFlow ).IsDead() )
            continue;
        MT_Vector2D nearestPointTmp;
        if( !( **itFlow ).GetLocation().ComputeNearestPoint( refPos, nearestPointTmp ) )
            continue;
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( ( *itConcentration)->IsValid() )
        {
            double d = ( *itConcentration )->GetLocation().ComputeBarycenter().Distance( position );
            if( d <= maxDistance && d < minDistance )
            {
                ret = *itConcentration;
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( ( **itConcentration ).IsDead() )
            continue;
        MT_Vector2D nearestPoint;
        if( !( **itConcentration ).GetLocation().ComputeNearestPoint( position, nearestPoint ) )
            continue;
        double rDistance = position.Distance( nearestPoint );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            pClosestElement = *itConcentration;
        }
    }
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
    {
        if( ( **itFlow ).IsDead() )
            continue;
        MT_Vector2D nearestPoint;
        if( !( **itFlow ).GetLocation().ComputeNearestPoint( position, nearestPoint ) )
            continue;
        double rDistance = position.Distance( nearestPoint );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            pClosestElement = *itFlow;
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( ( **itConcentration ).IsDead() )
            continue;
        MT_Vector2D nearestPointTmp;
        double rDistance;
        if( !( **itConcentration ).GetLocation().ComputeNearestPoint( loc, nearestPointTmp, rDistance ) )
            continue;
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            closestPoint = nearestPointTmp;
        }
    }

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
    {
        if( ( **itFlow ).IsDead() )
            continue;
        MT_Vector2D nearestPointTmp;
        double rDistance;
        if( !( **itFlow ).GetLocation().ComputeNearestPoint( loc, nearestPointTmp, rDistance ) )
            continue;
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
MT_Vector2D MIL_Population::GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const
{
    MIL_PopulationElement_ABC* pClosestElement = GetClosestAliveElement( securingAgent );
    if( !pClosestElement )
        return MT_Vector2D();
    return pClosestElement->GetSecuringPoint( securingAgent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
MT_Vector2D MIL_Population::GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance ) const
{
    MIL_PopulationElement_ABC* pClosestElement = GetClosestAliveElement( agent );
    if( !pClosestElement )
        return MT_Vector2D();
    MT_Vector2D safetyPoint;
    double rSeed = 0.0f;
    int nIteration = 1;
    while( rSeed < MT_PI )
    {
        safetyPoint = pClosestElement->GetSafetyPosition( agent, rMinDistance, rSeed );
        // $$$$ SBO 2006-02-22: { 0; pi/4; -pi/4; pi/2; -pi/2; 3pi/4; -3pi/4; pi }
        rSeed += ( nIteration % 2 ? nIteration : -nIteration ) * MT_PI / 4;
        ++nIteration;
        if( GetClosestPoint( safetyPoint ).Distance( safetyPoint ) >= rMinDistance )
            return safetyPoint;
    }
    return safetyPoint;
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
    for( CIT_ConcentrationVector itC = concentrations_.begin(); itC != concentrations_.end(); ++itC )
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
    for( CIT_ConcentrationVector itC = concentrations_.begin(); itC != concentrations_.end(); ++itC )
        if( (*itC)->GetAttitude() > *attitude )
            attitude = &(*itC)->GetAttitude();
    for( CIT_FlowVector itF = flows_.begin(); itF != flows_.end(); ++itF )
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetAllHumans();
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetHealthyHumans();
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetWoundedHumans();
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetContaminatedHumans();
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        nResult += ( **itConcentration ).GetDeadHumans();
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        nResult += ( **itFlow ).GetDeadHumans();
    return nResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetAllHumansInConcentration
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
unsigned int MIL_Population::GetAllHumansInConcentration( unsigned int concentrationId )
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        if( ( *itConcentration )->GetID() == concentrationId )
            return ( *itConcentration )->GetAllHumans();
    return 0;
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
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        ( **itConcentration ).Move( destination );
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        ( **itFlow ).Move( destination );
    UpdateBarycenter();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::FireOnPions
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_Population::FireOnPions( double rIntensity, PHY_FireResults_Population& fireResult )
{
    if( !IsBlinded() )
    {
        for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
            ( **itConcentration ).FireOnPions( rIntensity, fireResult );
        for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
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
// Name: MIL_Population::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
double MIL_Population::GetDangerosity( const MIL_AgentPion& target ) const
{
    MIL_PopulationElement_ABC* pClosestElement = GetClosestAliveElement( target );
    if( pClosestElement )
        return pClosestElement->GetDangerosity( target );
    else
        return 0.;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetAttitude
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void MIL_Population::SetAttitude( const MIL_PopulationAttitude& attitude )
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        ( **itConcentration ).SetAttitude( attitude );
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
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
    MIL_PopulationFlow* pFlow = new MIL_PopulationFlow( *this, concentration );
    flows_.push_back( pFlow );
    return *pFlow;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::CreateFlow
// Created: NLD 2007-03-01
// -----------------------------------------------------------------------------
MIL_PopulationFlow& MIL_Population::CreateFlow( const MIL_PopulationFlow& source, const MT_Vector2D& splitPoint )
{
    MIL_PopulationFlow* pFlow = new MIL_PopulationFlow( *this, source, splitPoint );
    flows_.push_back( pFlow );
    return *pFlow;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetConcentration
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
MIL_PopulationConcentration& MIL_Population::GetConcentration( const MT_Vector2D& position )
{
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        if( ( **it ).IsNearPosition( position ) )
            return **it;
    MIL_PopulationConcentration* pConcentration = new MIL_PopulationConcentration( *this, position );
    concentrations_.push_back( pConcentration );
    return *pConcentration;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::RetrieveConcentration
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
MIL_PopulationConcentration* MIL_Population::RetrieveConcentration( unsigned int concentrationId ) const
{
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        if( ( *it )->GetID() == concentrationId )
            return *it;
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
void MIL_Population::OnReceiveOrder( const sword::CrowdOrder& msg )
{
    orderManager_.OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveFragOrder
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveFragOrder( const sword::FragOrder& msg )
{
    orderManager_.OnReceiveFragOrder( msg );
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
        OnReceiveMsgChangeHealthState( msg );
        break;
    case sword::UnitMagicAction::crowd_change_armed_individuals:
        OnReceiveMsgChangeArmedIndividuals( msg);
        break;
    case sword::UnitMagicAction::crowd_change_attitude:
        OnReceiveMsgChangeAttitude( msg );
        break;
    case sword::UnitMagicAction::crowd_change_affinities:
        pAffinities_->OnReceiveMsgChangeAffinities( msg );
        break;
    case sword::UnitMagicAction::change_extension:
        pExtensions_->OnReceiveMsgChangeExtensions( msg );
        break;
    case sword::UnitMagicAction::change_critical_intelligence:
        OnReceiveCriticalIntelligence( msg );
        break;
    case sword::UnitMagicAction::reload_brain:
        OnReloadBrain( msg.parameters() );
        break;
    default:
        assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveCrowdMagicActionMoveTo
// Created: JSR 2010-04-08
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveCrowdMagicActionMoveTo( const sword::UnitMagicAction& asn )
{
    if( asn.type() != sword::UnitMagicAction::move_to )
        throw NET_AsnException< sword::UnitActionAck::ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    if( !asn.has_parameters() || asn.parameters().elem_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck::ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const sword::MissionParameter& parametre = asn.parameters().elem( 0 );
    if( parametre.value_size() != 1 || !parametre.value().Get(0).has_point() )
        throw NET_AsnException< sword::UnitActionAck::ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    const sword::Point& point = parametre.value().Get(0).point();
    if( point.location().type() != sword::Location::point
        || point.location().coordinates().elem_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck::ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( point.location().coordinates().elem(0), vPosTmp );
   // merge all concentrations into new
    T_ConcentrationVector concentrations = concentrations_;
    for( IT_ConcentrationVector it = concentrations.begin(); it != concentrations.end(); ++it )
        ( **it ).MagicMove( vPosTmp );
    // merge all flows into new concentration
    for( IT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        ( **it ).MagicMove( vPosTmp );
    GetRole< DEC_PopulationDecision >().Reset();
    orderManager_.CancelMission();
    bHasDoneMagicMove_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgDestroyAll
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgDestroyAll()
{
    for( IT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        ( **it ).KillAllHumans();
    for( IT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        ( **it ).KillAllHumans();
    GetRole< DEC_PopulationDecision >().Reset();
    orderManager_.CancelMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgChangeAttitude
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgChangeAttitude( const sword::UnitMagicAction& msg )
{
    if( !msg.has_parameters() )
        throw NET_AsnException< sword::CrowdMagicActionAck::ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );
    const sword::MissionParameter& parametre = msg.parameters().elem( 0 );
    if( parametre.value_size() != 1 || !parametre.value().Get(0).has_enumeration() )
        throw NET_AsnException< sword::CrowdMagicActionAck::ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( parametre.value().Get(0).enumeration() );
    if( !pAttitude )
        throw NET_AsnException< sword::CrowdMagicActionAck::ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        ( **it ).SetAttitude( *pAttitude );
    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        ( **it ).SetAttitude( *pAttitude );
}

namespace
{
    void ChangeHealthState( MIL_PopulationElement_ABC& element, unsigned int& healthy, unsigned int& contaminated, unsigned int& wounded, unsigned int& dead, float ratio )
    {
        unsigned int elementTotal = element.GetAllHumans();
        element.PullHumans( elementTotal );
        unsigned int newNumber = static_cast< unsigned int >( ratio * elementTotal );
        unsigned int newDead = std::min( dead, newNumber );
        newNumber -= newDead;
        dead -= newDead;
        unsigned int newWounded = std::min( wounded, newNumber );
        newNumber -= newWounded;
        wounded -= newWounded;
        unsigned int newContaminated = std::min( contaminated, newNumber );
        newNumber -= newContaminated;
        contaminated -= newContaminated;
        unsigned int newHealthy = std::min( healthy, newNumber );
        newNumber -= newHealthy;
        healthy -= newHealthy;
        element.PushHumans( MIL_PopulationHumans( newHealthy, newContaminated, newWounded, newDead ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgChangeHealthState
// Created: JSR 2011-03-16
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgChangeHealthState( const sword::UnitMagicAction& msg )
{
    if( !msg.has_parameters() || msg.parameters().elem_size() != 4 )
        throw NET_AsnException< sword::CrowdMagicActionAck::ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );
    const sword::MissionParameter& healthyParam = msg.parameters().elem( 0 );
    const sword::MissionParameter& woundedParam = msg.parameters().elem( 1 );
    const sword::MissionParameter& contaminatedParam = msg.parameters().elem( 2 );
    const sword::MissionParameter& deadParam = msg.parameters().elem( 3 );
    if( healthyParam.value_size() != 1 || !healthyParam.value().Get( 0 ).has_quantity() ||
        woundedParam.value_size() != 1 || !woundedParam.value().Get( 0 ).has_quantity() ||
        contaminatedParam.value_size() != 1 || !contaminatedParam.value().Get( 0 ).has_quantity() ||
        deadParam.value_size() != 1 || !deadParam.value().Get( 0 ).has_quantity() )
        throw NET_AsnException< sword::CrowdMagicActionAck::ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );
    unsigned int healthy = healthyParam.value().Get( 0 ).quantity();
    unsigned int wounded = woundedParam.value().Get( 0 ).quantity();
    unsigned int contaminated = contaminatedParam.value().Get( 0 ).quantity();
    unsigned int dead = deadParam.value().Get( 0 ).quantity();
    ChangeComposition( healthy, wounded, contaminated, dead );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::ChangeComposition
// Created: BCI 2011-03-21
// -----------------------------------------------------------------------------
void MIL_Population::ChangeComposition( unsigned int healthy, unsigned int wounded, unsigned int contaminated, unsigned int dead )
{
    unsigned int currentAllHumans = GetAllHumans();
    if( currentAllHumans == 0 )
    {
        MIL_PopulationConcentration* pConcentration = new MIL_PopulationConcentration( *this, *vBarycenter_, MIL_PopulationHumans( healthy, contaminated, wounded, dead ) );
        concentrations_.push_back( pConcentration );
    }
    else
    {
        float ratio = static_cast< float >( healthy + wounded + contaminated + dead ) / currentAllHumans;
        MIL_PopulationElement_ABC* last = 0;
        for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        {
            last = *it;
            ChangeHealthState( **it, healthy, contaminated, wounded, dead, ratio );
        }
        for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        {
            last = *it;
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
double MIL_Population::ComputeUrbanBlocDestruction( UrbanObjectWrapper* pUrbanObjet )
{
    double densityRef = pType_->GetUrbanDestructionDensity( GetAttitude() );
    double timeRef = pType_->GetUrbanDestructionTime( GetAttitude() ) ;
    if( densityRef <= 0.0 && timeRef <= 0.0 )
        return 0.0;

    double coveredArea = 0.0, currentArea = 0.0, areaDensitySum = 0.0;
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end();  ++it )
    {
        currentArea = MIL_Geometry::IntersectionArea( pUrbanObjet->GetLocalisation(), (*it)->GetLocation() );
        coveredArea += currentArea;
        areaDensitySum += currentArea * (*it)->GetDensity();
    }
    /* FIXME : Either remove completely flows or replace IntersectionArea with proper computation
     * The geometry is a line: a polygon should be constructed out of it if we want to intersect
     * or just counting the number of people inside the block (but most often that will be 0).
    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
    {
        currentArea = MIL_Geometry::IntersectionArea( pUrbanObjet->GetLocalisation(), (*it)->GetLocation() );
        coveredArea += currentArea;
        areaDensitySum += currentArea * (*it)->GetDensity();
    }
    */
    if( coveredArea < 0.00001 /*epsilon*/ )
        return 0.0;

    double urbanArea = pUrbanObjet->GetLocalisation().GetArea();
    double areaFactor = std::min< double >( 1.0, coveredArea / urbanArea );
    if( densityRef <= 0.0 || timeRef <= 0.0 )
        return areaFactor;

    double averageDensity = areaDensitySum / coveredArea;
    double densityTimeFactor = std::min< double >( 1.0, averageDensity / ( densityRef * timeRef ) );
    return  areaFactor * densityTimeFactor;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgChangeArmedIndividuals
// Created: JSR 2011-03-16
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgChangeArmedIndividuals( const sword::UnitMagicAction& msg )
{
    if( !msg.has_parameters() )
        throw NET_AsnException< sword::CrowdMagicActionAck::ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );
    const sword::MissionParameter& parametre = msg.parameters().elem( 0 );
    if( parametre.value_size() != 1 || !parametre.value().Get( 0 ).has_quantity() )
        throw NET_AsnException< sword::CrowdMagicActionAck::ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );
    rArmedIndividuals_ = 0.01 * parametre.value().Get( 0 ).quantity();
    armedIndividualsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_Population::SendCreation( unsigned int context ) const
{
    client::CrowdCreation asnMsg;
    asnMsg().mutable_crowd()->set_id( nID_ );
    asnMsg().mutable_type()->set_id( pType_->GetID() );
    asnMsg().mutable_party()->set_id( pArmy_->GetID() );
    asnMsg().set_name( GetName() );
    asnMsg().mutable_repartition()->set_male( static_cast< float >( rMale_ ) );
    asnMsg().mutable_repartition()->set_female( static_cast< float >( rFemale_ ) );
    asnMsg().mutable_repartition()->set_children( static_cast< float >( rChildren_ ) );
    asnMsg.Send( NET_Publisher_ABC::Publisher(), context );
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        ( **it ).SendCreation( context );
    for( CIT_ConcentrationVector it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
        ( **it ).SendCreation( context );
    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        ( **it ).SendCreation( context );
    for( CIT_FlowVector it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
        ( **it ).SendCreation( context );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_Population::SendFullState() const
{
    client::CrowdUpdate asnMsg;
    asnMsg().mutable_crowd()->set_id( nID_ );
    GetRole< DEC_PopulationDecision >().SendFullState( asnMsg );
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
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        ( **it ).SendFullState();
    for( CIT_ConcentrationVector it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
        ( **it ).SendFullState();
    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        ( **it ).SendFullState();
    for( CIT_FlowVector it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
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
            asnMsg().mutable_crowd()->set_id( nID_ );
            GetRole< DEC_PopulationDecision >().SendChangedState( asnMsg );
            if( criticalIntelligenceChanged_ )
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
        for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
            ( **it ).SendChangedState();
        for( CIT_ConcentrationVector it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
            ( **it ).SendChangedState();
        for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
            ( **it ).SendChangedState();
        for( CIT_FlowVector it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
            ( **it ).SendChangedState();
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating network for population " << GetID() << " : " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::Apply
// Created: SBO 2006-02-24
// -----------------------------------------------------------------------------
void MIL_Population::Apply( MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >& visitor ) const
{
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        visitor.Visit( **it );
    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        visitor.Visit( **it );
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
// Name: MIL_Population::GetDefaultAttitude
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude& MIL_Population::GetDefaultAttitude() const
{
    assert( pDefaultAttitude_ );
    return *pDefaultAttitude_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetID
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
unsigned int MIL_Population::GetID() const
{
    return nID_;
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
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetOrderManager
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
MIL_PopulationOrderManager& MIL_Population::GetOrderManager()
{
    return orderManager_;
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
        throw std::runtime_error( "Setting max speed to less than 0" );
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
    MT_Vector2D currentBarycenter;
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        currentBarycenter += (*it)->GetPosition();
    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        currentBarycenter += (*it)->GetPosition();
    double elements = static_cast< double >( concentrations_.size() + flows_.size() );
    if( elements > 0 )
    {
        currentBarycenter = currentBarycenter / elements;
        vBarycenter_->rX_ = currentBarycenter.rX_;
        vBarycenter_->rY_ = currentBarycenter.rY_;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HasHumansChanged
// Created: JSR 2011-05-16
// -----------------------------------------------------------------------------
bool MIL_Population::HasHumansChanged() const
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        if( ( **itConcentration ).HasHumansChanged() )
            return true;
    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        if( ( **itFlow ).HasHumansChanged() )
            return true;
    return false;
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
void MIL_Population::SetUrbanBlockAngriness( double u )
{
    urbanBlockAngriness_ = u;
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
// Name: MIL_Population::HasReachedDestination
// Created: NLD 2011-03-18
// -----------------------------------------------------------------------------
bool MIL_Population::HasReachedDestination( const MT_Vector2D& destination ) const
{
    double weldValue = TER_World::GetWorld().GetWeldValue() * TER_World::GetWorld().GetWeldValue()/10;

    BOOST_FOREACH( const MIL_PopulationConcentration* concentration, concentrations_ )
        if( destination.SquareDistance( concentration->GetPosition() ) <= weldValue )
            return true;
    BOOST_FOREACH( const MIL_PopulationFlow* flow, flows_ )
        if( destination.SquareDistance( flow->GetPosition() ) <= weldValue )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::HasReachedBlockBorder
// Created: DDA 2011-04-04
// -----------------------------------------------------------------------------
bool MIL_Population::HasReachedBlockBorder( const UrbanObjectWrapper* pUrbanKnowledge ) const
{
    const TER_Localisation& localisation = pUrbanKnowledge->GetLocalisation();
    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        if( localisation.IsInside( (*it)->GetPosition() ) )
            return true;
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
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
    return destination.SquareDistance( concentrations_.front()->GetPosition() ) <= TER_World::GetWorld().GetWeldValue() * TER_World::GetWorld().GetWeldValue()/10;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetFlowHeadPosition
// Created: EVH 2011-05-10
// -----------------------------------------------------------------------------
MT_Vector2D MIL_Population::GetFlowHeadPosition()
{
    assert( HasFlow() );
    MIL_PopulationFlow* firstFlow = *flows_.begin();
    return firstFlow->GetPosition();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveCriticalIntelligence
// Created: LGY 2011-05-27
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveCriticalIntelligence( const sword::UnitMagicAction& msg )
{
    if( !msg.has_parameters() || msg.parameters().elem_size() != 1 )
        throw NET_AsnException< sword::UnitActionAck::ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
    criticalIntelligence_ = msg.parameters().elem( 0 ).value( 0 ).acharstr();
    criticalIntelligenceChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::OnReloadBrain
// Created: AHC 2010-01-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReloadBrain( const sword::MissionParameters& msg )
{
    CancelAllActions();
    if( msg.elem_size() == 1 && msg.elem( 0 ).value_size() == 1 && msg.elem( 0 ).value( 0 ).has_acharstr() )
    {
        const std::string model = msg.elem( 0 ).value( 0 ).acharstr();
        const DEC_Model_ABC* pModel = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPopulation( model );
        if( !pModel )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        GetRole< DEC_PopulationDecision >().SetModel( *pModel );
    }
    GetDecision().Reload();
    orderManager_.CancelMission();
}
