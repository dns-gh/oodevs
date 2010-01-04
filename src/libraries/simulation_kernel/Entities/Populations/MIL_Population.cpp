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
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_Representations.h"
#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_AsnException.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( MIL_Population, "MIL_Population" )

namespace
{
    template< typename R >
    void SaveRole( const MIL_Population& population, MIL_CheckPointOutArchive& file )
    {
        const R* const role = & population.GetRole< R >();
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
MIL_Population::MIL_Population( xml::xistream& xis, const MIL_PopulationType& type, MIL_Army& army, DEC_DataBase& database )
    : MIL_Entity_ABC          ( xis ) 
    , pType_                  ( &type )
    , nID_                    ( xml::attribute< unsigned int >( xis, "id" ) )
    , pArmy_                  ( &army )
    , pDefaultAttitude_       ( 0 )
    , rPeopleCount_           ( 0. )
    , pKnowledge_             ( 0 )
    , orderManager_           ( *this )
    , bPionMaxSpeedOverloaded_( false )
    , rOverloadedPionMaxSpeed_( 0. )
    , bHasDoneMagicMove_      ( false )
{

    std::string strAttitude;
    xis >> xml::attribute( "attitude", strAttitude );
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( strAttitude );
    if( !pDefaultAttitude_ )
        xis.error( "Unknown attitude" );

    pKnowledge_ = new DEC_PopulationKnowledge();
    RegisterRole( *new DEC_PopulationDecision( *this, database ) );
    RegisterRole( *new DEC_Representations() );

    MIL_PopulationConcentration* pConcentration = new MIL_PopulationConcentration( *this, xis );
    concentrations_.push_back( pConcentration );
    rPeopleCount_ = pConcentration->GetNbrAliveHumans();

    pArmy_->RegisterPopulation( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population constructor
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
MIL_Population::MIL_Population(const MIL_PopulationType& type )
    : MIL_Entity_ABC          ( type.GetName() )
    , pType_                  ( &type )
    , nID_                    ( 0 )
    , pArmy_                  ( 0 )
    , pDefaultAttitude_       ( 0 )
    , rPeopleCount_           ( 0. )
    , pKnowledge_             ( 0 )
    , orderManager_           ( *this )
    , bPionMaxSpeedOverloaded_( false )
    , rOverloadedPionMaxSpeed_( 0. )
    , bHasDoneMagicMove_      ( false )
{
    pKnowledge_ = new DEC_PopulationKnowledge();
}

// =============================================================================
// Accessors
// =============================================================================

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

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::load
// Created: SBO 2005-10-18
// -----------------------------------------------------------------------------
void MIL_Population::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_Entity_ABC >( *this ); 

    file >> const_cast< uint& >( nID_ )
         >> const_cast< MIL_Army*& >( pArmy_ ); 

    uint nAttitudeID;
    file >> nAttitudeID;
    pDefaultAttitude_ = MIL_PopulationAttitude::Find( nAttitudeID );
    assert( pDefaultAttitude_ );

    file >> rPeopleCount_
         >> concentrations_
         >> flows_
         >> trashedConcentrations_
         >> trashedFlows_
         >> bPionMaxSpeedOverloaded_
         >> rOverloadedPionMaxSpeed_
         >> pKnowledge_
         >> bHasDoneMagicMove_;
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
void MIL_Population::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this );
    
    unsigned attitude = pDefaultAttitude_->GetID();
    file << nID_
         << pArmy_
         << attitude
         << rPeopleCount_
         << concentrations_
         << flows_
         << trashedConcentrations_
         << trashedFlows_
         << bPionMaxSpeedOverloaded_
         << rOverloadedPionMaxSpeed_
         << pKnowledge_
         << bHasDoneMagicMove_;
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
    
    xos << xml::attribute( "id"      , nID_ )
        << xml::attribute( "type"    , pType_->GetName() )
        << xml::attribute( "humans"  , GetNbrAliveHumans() + GetNbrDeadHumans() )
        << xml::attribute( "attitude", pDefaultAttitude_->GetName() );

    if( !concentrations_.empty() )
        xos << xml::attribute( "position", MIL_Tools::ConvertCoordSimToMos( concentrations_.front()->GetPosition() ) );
    else
        xos << xml::attribute( "position", MIL_Tools::ConvertCoordSimToMos( flows_.front()->GetPosition() ) );

    xos << xml::end(); // population
}

// =============================================================================
// EFFECTS FROM PIONS
// =============================================================================

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
void MIL_Population::Exterminate( const MIL_AgentPion& exterminator, MT_Float rSurface )
{
    MIL_PopulationElement_ABC* pElement;
    while( rSurface > 0. )
    {
        pElement = GetClosestAliveElement( exterminator );
        if( !pElement )
            break;
        rSurface -= pElement->Exterminate( rSurface );
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

// =============================================================================
// OPERATIONS
// =============================================================================

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
    catch( std::runtime_error& /*e*/ )
    {        
        MIL_Report::PostEvent( *this, MIL_Report::eReport_MissionImpossible_ );
    }
    GetRole< DEC_Decision_ABC >().GarbageCollect();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateState
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_Population::UpdateState()
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
            ++ itFlow;
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
            ++ itConcentration;
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
        (**itConcentration).Clean();

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).Clean();

    bHasDoneMagicMove_ = false;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::IsDead
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
bool MIL_Population::IsDead() const
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        if( !(**itConcentration).IsDead() )
            return false;

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        if( !(**itFlow).IsDead() )
            return false;

    return true;
}

// =============================================================================
// GEOMETRY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::IsInZone
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
bool MIL_Population::IsInZone( const TER_Localisation& loc ) const
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        if( (**itConcentration).IsInZone( loc ) )
            return true;

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        if( (**itFlow).IsInZone( loc ) )
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
    MT_Float    rMinDistance = std::numeric_limits< MT_Float >::max();

    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( (**itConcentration).IsDead() )
            continue;

        MT_Vector2D nearestPointTmp;

        if( !(**itConcentration).GetLocation().ComputeNearestPoint( refPos, nearestPointTmp ) )
            continue;
        MT_Float rDistance = refPos.Distance( nearestPointTmp );
        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            closestPoint = nearestPointTmp;
        }
    }

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
    {
        if( (**itFlow).IsDead() )
            continue;

        MT_Vector2D nearestPointTmp;

        if( !(**itFlow).GetLocation().ComputeNearestPoint( refPos, nearestPointTmp ) )
            continue;
        MT_Float rDistance = refPos.Distance( nearestPointTmp );
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
    MT_Float rMinDistance = 0.0f;
    ComputeClosestAliveElement( position, pResult, rMinDistance );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetClosestElement
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void MIL_Population::ComputeClosestAliveElement( const MT_Vector2D& position, MIL_PopulationElement_ABC*& pClosestElement, MT_Float& rMinDistance ) const
{
    pClosestElement = 0;
    rMinDistance = std::numeric_limits< MT_Float >::max();

    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( (**itConcentration).IsDead() )
            continue;

        MT_Vector2D nearestPoint;

        if( !(**itConcentration).GetLocation().ComputeNearestPoint( position, nearestPoint ) )
            continue;
        MT_Float rDistance = position.Distance( nearestPoint );
        if( rDistance < rMinDistance )
        {
            rMinDistance    = rDistance;
            pClosestElement = *itConcentration;
        }
    }

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
    {
        if( (**itFlow).IsDead() )
            continue;

        MT_Vector2D nearestPoint;

        if( !(**itFlow).GetLocation().ComputeNearestPoint( position, nearestPoint ) )
            continue;
        MT_Float rDistance = position.Distance( nearestPoint );
        if( rDistance < rMinDistance )
        {
            rMinDistance    = rDistance;
            pClosestElement = *itFlow;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetClosestPointAndDistance
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
void MIL_Population::GetClosestPointAndDistance( const TER_Localisation& loc, MT_Vector2D& closestPoint, MT_Float& rMinDistance ) const
{
    rMinDistance = std::numeric_limits< MT_Float >::max();

    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
    {
        if( (**itConcentration).IsDead() )
            continue;

        MT_Vector2D nearestPointTmp;
        MT_Float    rDistance;
        if( !(**itConcentration).GetLocation().ComputeNearestPoint( loc, nearestPointTmp, rDistance ) )
            continue;

        if( rDistance < rMinDistance )
        {
            rMinDistance = rDistance;
            closestPoint = nearestPointTmp;
        }
    }

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
    {
        if( (**itFlow).IsDead() )
            continue;

        MT_Vector2D nearestPointTmp;
        MT_Float    rDistance;
        if( !(**itFlow).GetLocation().ComputeNearestPoint( loc, nearestPointTmp, rDistance ) )
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
    MT_Float    rMinDistance;
    GetClosestPointAndDistance( loc, closestPoint, rMinDistance );
    return closestPoint;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDistanceTo
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetDistanceTo( const TER_Localisation& loc ) const
{
    MT_Vector2D closestPoint;
    MT_Float    rMinDistance;
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
MT_Vector2D MIL_Population::GetSafetyPosition( const MIL_AgentPion& agent, MT_Float rMinDistance ) const
{
    MIL_PopulationElement_ABC* pClosestElement = GetClosestAliveElement( agent );
    if( !pClosestElement )
        return MT_Vector2D();
    MT_Vector2D safetyPoint;
    MT_Float rSeed = 0.0f;
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
// Name: MIL_Population::GetNbrAliveHumans
// Created: SBO 2006-02-22
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetNbrAliveHumans() const
{
    MT_Float rResult = 0.0f;
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        rResult += (**itConcentration).GetNbrAliveHumans();

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        rResult += (**itFlow).GetNbrAliveHumans();
    return rResult;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetNbrDeadHumans
// Created: SBO 2006-02-22
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetNbrDeadHumans() const
{
    MT_Float rResult = 0.0f;
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        rResult += (**itConcentration).GetNbrDeadHumans();

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        rResult += (**itFlow).GetNbrDeadHumans();
    return rResult;
}

// =============================================================================
// ACTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::Move
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
void MIL_Population::Move( const MT_Vector2D& destination )
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        (**itConcentration).Move( destination );

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).Move( destination );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::FireOnPions
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void MIL_Population::FireOnPions( MT_Float rIntensity, PHY_FireResults_Population& fireResult )
{
    for( CIT_ConcentrationVector itConcentration = concentrations_.begin(); itConcentration != concentrations_.end(); ++itConcentration )
        (**itConcentration).FireOnPions( rIntensity, fireResult );

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).FireOnPions( rIntensity, fireResult );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::FireOnPion
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void MIL_Population::FireOnPion( MT_Float rIntensity, MIL_Agent_ABC& target, PHY_FireResults_Population& fireResult )
{
    MIL_PopulationElement_ABC* pClosestElement = GetClosestAliveElement( target );

    if( pClosestElement )
        pClosestElement->FireOnPion( rIntensity, target, fireResult );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetDangerosity( const MIL_AgentPion& target ) const
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
        (**itConcentration).SetAttitude( attitude );

    for( CIT_FlowVector itFlow = flows_.begin(); itFlow != flows_.end(); ++itFlow )
        (**itFlow).SetAttitude( attitude );
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
    {
        if( (**it).IsNearPosition( position ) )
            return **it;
    }

    MIL_PopulationConcentration* pConcentration = new MIL_PopulationConcentration( *this, position );
    concentrations_.push_back( pConcentration );
    return *pConcentration;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetMaxSpeed
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetMaxSpeed() const
{
    assert( pType_ );
    return pType_->GetMaxSpeed();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetDefaultFlowDensity
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetDefaultFlowDensity() const
{
    assert( pType_ );
    return pType_->GetDefaultFlowDensity();
}
// =============================================================================
// PION EFFECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetPionReloadingTimeFactor
// Created: NLD 2005-11-02
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetPionReloadingTimeFactor( MT_Float rDensity ) const
{
    assert( pType_ );
    return pType_->GetPionReloadingTimeFactor( rDensity );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::GetPionMaxSpeed
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
MT_Float MIL_Population::GetPionMaxSpeed( const MIL_PopulationAttitude& attitude, MT_Float rDensity, const PHY_Volume& pionVolume ) const
{
    assert( pType_ );
    if( bPionMaxSpeedOverloaded_ )
        return rOverloadedPionMaxSpeed_;
    else
        return pType_->GetPionMaxSpeed( attitude, rDensity, pionVolume );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgOrder( const ASN1T_MsgPopulationOrder& msg )
{
    orderManager_.OnReceiveMission( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgFragOrder
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgFragOrder( const ASN1T_MsgFragOrder& msg )
{
    orderManager_.OnReceiveFragOrder( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgPopulationMagicAction
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgPopulationMagicAction( const ASN1T_MsgPopulationMagicAction& asnMsg )
{
    switch( asnMsg.action.t )
    {
        case T_MsgPopulationMagicAction_action_move_to           : OnReceiveMsgMagicMove     ( *asnMsg.action.u.move_to ); break;
        case T_MsgPopulationMagicAction_action_destruction_totale: OnReceiveMsgDestroyAll    (); break;
        case T_MsgPopulationMagicAction_action_change_attitude   : OnReceiveMsgChangeAttitude( *asnMsg.action.u.change_attitude ); break;
        case T_MsgPopulationMagicAction_action_tuer              : OnReceiveMsgKill          ( asnMsg.action.u.tuer ); break;
        case T_MsgPopulationMagicAction_action_ressusciter       : OnReceiveMsgResurrect     ( asnMsg.action.u.ressusciter ); break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgMagicMove
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgMagicMove( const ASN1T_MagicActionPopulationMoveTo& asn )
{
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( asn, vPosTmp );

   // merge all concentrations into new
    T_ConcentrationVector concentrations = concentrations_;
    for( IT_ConcentrationVector it = concentrations.begin(); it != concentrations.end(); ++it )
        (**it).MagicMove( vPosTmp );

    // merge all flows into new concentration
    for( IT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        (**it).MagicMove( vPosTmp );

    GetRole< DEC_PopulationDecision >().Reset();
    orderManager_.ReplaceMission();
    bHasDoneMagicMove_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgDestroyAll
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgDestroyAll()
{
    for( IT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        (**it).KillAllHumans();

    for( IT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        (**it).KillAllHumans();

    GetRole< DEC_PopulationDecision >().Reset();
    orderManager_.ReplaceMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgChangeAttitude
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgChangeAttitude( const ASN1T_MagicActionPopulationChangeAttitude& asn )
{
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( asn.attitude );
    if( !pAttitude )
        throw NET_AsnException< ASN1T_EnumPopulationErrorCode >( EnumPopulationErrorCode::error_invalid_attribute );

    // concentration
    if( asn.beneficiaire.t == T_MagicActionPopulationChangeAttitude_beneficiaire_concentration )
    {
        for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
            if( static_cast<int>( ( **it ).GetID() ) == asn.beneficiaire.u.concentration )
            {
                ( **it ).SetAttitude( *pAttitude );
                return;
            }
        throw NET_AsnException< ASN1T_EnumPopulationErrorCode >( EnumPopulationErrorCode::error_invalid_attribute );
    }
    // flow
    else if( asn.beneficiaire.t == T_MagicActionPopulationChangeAttitude_beneficiaire_flux )
    {
        for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
            if( static_cast<int>( ( **it ).GetID() ) == asn.beneficiaire.u.flux )
            {
                ( **it ).SetAttitude( *pAttitude );
                return;
            }
        throw NET_AsnException< ASN1T_EnumPopulationErrorCode >( EnumPopulationErrorCode::error_invalid_attribute );
    }
    // global
    else if( asn.beneficiaire.t == T_MagicActionPopulationChangeAttitude_beneficiaire_global )
    {
        for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
            ( **it ).SetAttitude( *pAttitude );
        for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
            ( **it ).SetAttitude( *pAttitude );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgKill
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgKill( const ASN1T_MagicActionPopulationKill& asn )
{
    uint remainingKills = asn;
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
    {
        if( remainingKills == 0 )
            return;
        remainingKills -= (**it).Kill( remainingKills );
    }
    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
    {
        if( remainingKills == 0 )
            return;
        remainingKills -= (**it).Kill( remainingKills );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::OnReceiveMsgResurrect
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
void MIL_Population::OnReceiveMsgResurrect( const ASN1T_MagicActionPopulationResurrect& asn )
{
    uint remainingResurrections = asn;
    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
    {
        if( remainingResurrections == 0 )
            return;
        remainingResurrections -= (**it).Resurrect( remainingResurrections );
    }
    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
    {
        if( remainingResurrections == 0 )
            return;
        remainingResurrections -= (**it).Resurrect( remainingResurrections );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SendCreation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_Population::SendCreation() const
{
    NET_ASN_MsgPopulationCreation asnMsg;
    asnMsg().oid             = nID_;
    asnMsg().type_population = pType_->GetID();
    asnMsg().oid_camp        = pArmy_->GetID();
    asnMsg().nom             = GetName().c_str(); // !! pointeur sur const char*
    asnMsg.Send();

    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        (**it).SendCreation();
    for( CIT_ConcentrationVector it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
        (**it).SendCreation();

    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        (**it).SendCreation();
    for( CIT_FlowVector it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
        (**it).SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SendFullState
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
void MIL_Population::SendFullState() const
{
    NET_ASN_MsgPopulationUpdate asnMsg;
    asnMsg().oid = nID_;
    GetRole< DEC_PopulationDecision >().SendFullState( asnMsg );
    asnMsg.Send();

    sPeopleCounter counter( rPeopleCount_ );
    sPeopleCounter trashCounter( rPeopleCount_ );

    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        (**it).SendFullState( counter );

    for( CIT_ConcentrationVector it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
        (**it).SendFullState( trashCounter );

    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        (**it).SendFullState( counter );

    for( CIT_FlowVector it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
        (**it).SendFullState( trashCounter );
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::UpdateNetwork
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void MIL_Population::UpdateNetwork()
{
    if( GetRole< DEC_PopulationDecision >().HasStateChanged() )
    {
        NET_ASN_MsgPopulationUpdate asnMsg;
        asnMsg().oid = nID_;
        GetRole< DEC_PopulationDecision >().SendChangedState( asnMsg );
        asnMsg.Send();
    }

    sPeopleCounter counter( rPeopleCount_ );
    sPeopleCounter trashCounter( rPeopleCount_ );

    for( CIT_ConcentrationVector it = concentrations_.begin(); it != concentrations_.end(); ++it )
        (**it).SendChangedState( counter );
    for( CIT_ConcentrationVector it = trashedConcentrations_.begin(); it != trashedConcentrations_.end(); ++it )
        (**it).SendChangedState( trashCounter);

    for( CIT_FlowVector it = flows_.begin(); it != flows_.end(); ++it )
        (**it).SendChangedState( counter);
    for( CIT_FlowVector it = trashedFlows_.begin(); it != trashedFlows_.end(); ++it )
        (**it).SendChangedState( trashCounter );
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
// Name: MIL_Population::sPeopleCounter::sPeopleCounter
// Created: MGD 2009-10-21
// -----------------------------------------------------------------------------
MIL_Population::sPeopleCounter::sPeopleCounter( MT_Float rInit ) 
    : nPeople_ ( uint( rInit ) )
{

}    

// -----------------------------------------------------------------------------
// Name: MIL_Population::sPeopleCounter::GetBoundedPeople
// Created: SBO 2006-04-03
// -----------------------------------------------------------------------------
uint MIL_Population::sPeopleCounter::GetBoundedPeople( MT_Float rPeople )
{
    uint nResult = std::min( nPeople_, int( ceil( rPeople ) ) );
    nPeople_ -= nResult;
    if( nPeople_ < 0. )
        nPeople_ = 0;
    return nResult;
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
uint MIL_Population::GetID() const
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
const MIL_Army& MIL_Population::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Population::SetPionMaxSpeed
// Created: NLD 2005-10-24
// -----------------------------------------------------------------------------
void MIL_Population::SetPionMaxSpeed( MT_Float rSpeed )
{
    assert( rSpeed >= 0. );
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
