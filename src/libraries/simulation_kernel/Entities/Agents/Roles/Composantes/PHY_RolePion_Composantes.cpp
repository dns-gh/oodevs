// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.cpp $
// $Author: Nld $
// $Modtime: 9/06/05 18:44 $
// $Revision: 39 $
// $Workfile: PHY_RolePion_Composantes.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Composantes.h"
#include "MIL_Time_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Actions/PHY_FireResults_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Logistic/PHY_BreakdownType.h"
#include "Entities/Agents/Roles/Humans/PHY_RolePion_Humans.h"
#include "Entities/Agents/Roles/Logistic/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Agents/Units/Sensors/PHY_Sensor.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeObject.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include "Network/NET_AsnException.h"
#include "TransportCapacityComputer_ABC.h"
#include "TransportWeightComputer_ABC.h"
#include "HumanLoadingTimeComputer_ABC.h"
#include "LoadedStateConsistencyComputer_ABC.h"
#include "SpeedComputer_ABC.h"
#include "OnComponentComputer_ABC.h"
#include "OnComponentLendedFunctorComputer_ABC.h"
#include "ComponentsChangedNotificationHandler_ABC.h"
#include "NetworkNotificationHandler_ABC.h"
#include "VisionConeNotificationHandler_ABC.h"
#include "DotationsActionsNotificationHandler_ABC.h"
#include "protocol/ClientSenders.h"
#include "MT_Tools/MT_Logger.h"
#include <tools/Map.h>
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/range/algorithm.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Composantes )

using namespace human;

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::T_ComposanteTypeProperties constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::T_ComposanteTypeProperties::T_ComposanteTypeProperties()
    : nbrsPerState_( PHY_ComposanteState::GetNbrStates(), 0 )
    , bHasChanged_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::T_ComposanteTypeProperties::HasUsableComposantes
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::T_ComposanteTypeProperties::HasUsableComposantes() const
{
    for( unsigned int i = 0; i < nbrsPerState_.size(); ++i )
        if( nbrsPerState_[ i ] > 0 && PHY_ComposanteState::Find( i ).IsUsable() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::T_ComposanteTypeProperties::serialize
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Composantes::T_ComposanteTypeProperties::serialize( Archive& file, const unsigned int )
{
    file & nbrsPerState_
         & bHasChanged_;
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::PHY_RolePion_Composantes()
    : owner_                      ( 0 )
    , nNbrComposanteChanged_      ( 0 )
    , nNbrUsableComposantes_      ( 0 )
    , rOperationalState_          ( 0. )
    , rMajorOperationalState_     ( 0. )
    , bOperationalStateChanged_   ( false )
    , pMajorComposante_           ( 0 )
    , nNeutralizationEndTimeStep_ ( 0 )
    , bLoansChanged_              ( false )
    , bExternalMustChange_        ( false )
    , bIsLoaded_                  ( false )
    , bUnderIndirectFire_         ( false )
    , bNeutralized_               ( false )
    , nTickRcMaintenanceQuerySent_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::PHY_RolePion_Composantes( MIL_Agent_ABC& pion, bool initialize )
    : owner_                      ( &pion )
    , nNbrComposanteChanged_      ( 0 )
    , nNbrUsableComposantes_      ( 0 )
    , rOperationalState_          ( 0. )
    , rMajorOperationalState_     ( 0. )
    , bOperationalStateChanged_   ( false )
    , pMajorComposante_           ( 0 )
    , nNeutralizationEndTimeStep_ ( 0 )
    , bLoansChanged_              ( false )
    , bExternalMustChange_        ( false )
    , bIsLoaded_                  ( false )
    , bUnderIndirectFire_         ( false )
    , bNeutralized_               ( false )
    , nTickRcMaintenanceQuerySent_( 0 )
{
    if( initialize ) // $$$$ LDC: Set to false for debug in order to avoid useless creation of crap.
    {
        owner_->GetType().GetUnitType().InstanciateComposantes( *this );
        DistributeCommanders();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::~PHY_RolePion_Composantes()
{
    while( ! composantes_.empty() )
        delete composantes_.front(); // removed from composantes_ by NotifyComposanteRemoved
}

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_RolePion_Composantes::T_ComposanteTypeMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Composantes::T_ComposanteTypeMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for(  auto it = map.begin(); it != map.end(); ++it )
            {
                sword::EquipmentType id = it->first->GetMosID();
                int equipment = id.id();
                file << equipment;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Composantes::T_ComposanteTypeMap& map, const unsigned int )
        {
            std::size_t n;
            file >> n;
            while( n-- )
            {
                sword::EquipmentType nID;
                int equipment;
                file >> equipment;
                nID.set_id( equipment );
                file >> map[ PHY_ComposanteTypePion::Find( nID ) ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::serialize
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Composantes::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Composantes >( *this )
         & owner_
         & composantes_
         & lentComposantes_
         & borrowedComposantes_
         & composanteTypes_
         & nNbrComposanteChanged_
         & nNbrUsableComposantes_
         & rOperationalState_
         & rMajorOperationalState_
         & pMajorComposante_
         & nNeutralizationEndTimeStep_
         & maintenanceComposanteStates_
         & nTickRcMaintenanceQuerySent_
         & bIsLoaded_
         & bUnderIndirectFire_
         & bNeutralized_;
}

//-----------------------------------------------------------------------------
// Name: PHY_UnitCanHaveComposante_ABC::DistributeCommanders
// Created: JVT 03-08-27
//-----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DistributeCommanders()
{
    // R�partition des officiers
    PHY_ComposantePion::T_ComposantePionVector composantes = composantes_;
    MIL_Random::random_shuffle( composantes );
    const PHY_UnitType::T_CommanderRepartitionMap& repartition = owner_->GetType().GetUnitType().GetCommanderRepartition();
    for( auto it = repartition.begin(); it != repartition.end(); ++it )
    {
        const PHY_HumanRank& rank = *it->first;
        unsigned int n = it->second;
        bool stop = false;
        while( n && !stop )
        {
            stop = true;
            for( auto it = composantes.begin(); it != composantes.end() && n; ++it )
                if( (*it)->ChangeHumanRank( PHY_HumanRank::militaireDuRang_, rank, PHY_HumanWound::notWounded_ ) )
                {
                    stop = false;
                    --n;
                }
        }
        if( n )
            MT_LOG_WARNING_MSG( "Agent " << owner_->GetID() << " - Not enough humans in crew to distribute commanders : " << n << " " << rank.GetName() << " remaining" );
    }
}

namespace
{
    void WriteEquipment( xml::xosubstream xos, const PHY_ComposantePion& composante, bool first, unsigned int id = 0 )
    {
        if( first )
            xos << xml::start( "equipements" );
        xos << xml::start( "equipment" )
                << xml::attribute( "state", composante.GetState().GetName() )
                << xml::attribute( "type", composante.GetType().GetName() );
        if( composante.GetState() == PHY_ComposanteState::repairableWithEvacuation_ )
        {
            const PHY_Breakdown* breakdown = composante.GetBreakdown();
            assert( breakdown );
            xos.attribute( "breakdown", breakdown->GetType().GetName() );
        }
        if( id )
            xos.attribute( "borrower", id );
    }
    bool IsLoanedEquipment( const PHY_ComposantePion& composante, const PHY_RolePion_Composantes::T_LoanMap& loanMap )
    {
        for( auto it = loanMap.begin(); it != loanMap.end(); ++it )
            if( boost::range::find( it->second, &composante ) != it->second.end() )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::WriteODB( xml::xostream& xos ) const
{
    bool first = true;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& composante = **it;
        if( IsLoanedEquipment( composante, borrowedComposantes_) )
            continue;
        if( &composante.GetState() != &PHY_ComposanteState::undamaged_ )
        {
            WriteEquipment( xos, composante, first );
            first = false;
        }
    }
    for( auto it = lentComposantes_.begin(); it != lentComposantes_.end(); ++it )
    {
        unsigned int id = it->first->GetID();
        for( auto itComposante = it->second.begin(); itComposante != it->second.end(); ++itComposante )
        {
            WriteEquipment( xos, **itComposante, first, id );
            first = false;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadOverloading
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadOverloading( xml::xistream& xis )
{
    ReadComposantesOverloading( xis );
    ReadHumansOverloading( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Finalize
// Created: JSR 2013-01-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Finalize()
{
    for( auto it = borrowedComposantesFromXML_.begin(); it != borrowedComposantesFromXML_.end(); ++it )
        if( MIL_Agent_ABC* borrower = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( it->first ) )
            for( auto itComposante = it->second.begin(); itComposante != it->second.end(); ++ itComposante )
                LendComposante( *borrower, **itComposante );
    borrowedComposantesFromXML_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadComposantesOverloading
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadComposantesOverloading( xml::xistream& xis )
{
    xis >> xml::optional
            >> xml::start( "equipments" )
                >> xml::list( "equipment", *this, &PHY_RolePion_Composantes::ReadEquipment )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadEquipment
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadEquipment( xml::xistream& xis )
{
    int borrowerId = xis.attribute< unsigned int >( "borrower", 0 );

    const PHY_ComposanteState* pState = PHY_ComposanteState::Find( xis.attribute< std::string >( "state" ) );
    if( !pState )
        xis.error( "Unknown composante state" );
    if( *pState == PHY_ComposanteState::undamaged_ && borrowerId == 0 )
        return;

    const PHY_ComposanteTypePion* pType = PHY_ComposanteTypePion::Find( xis.attribute< std::string >( "type" ) );
    if( !pType )
        xis.error( "Unknown composante type" );

    const PHY_BreakdownType* pBreakdownType = PHY_BreakdownType::Find( xis.attribute( "breakdown", "" ) );
    if( *pState == PHY_ComposanteState::repairableWithEvacuation_ && !pBreakdownType )
        xis.error( "Unknown breakdown type" );

    for( auto itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        PHY_ComposantePion& composante = **itComposante;
        if( composante.GetType() == *pType && composante.GetState() == PHY_ComposanteState::undamaged_ )
        {
            auto it = borrowedComposantesFromXML_.find( borrowerId );
            if( it == borrowedComposantesFromXML_.end() || std::find( it->second.begin(), it->second.end(), &composante ) == it->second.end() )
            {
                if( *pState != PHY_ComposanteState::undamaged_ )
                    composante.ReinitializeState( *pState, pBreakdownType );
                if( borrowerId > 0 )
                    borrowedComposantesFromXML_[ borrowerId ].push_back( &composante );
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadHumansOverloading
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadHumansOverloading( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "humans")
            >> xml::list( "human", *this, &PHY_RolePion_Composantes::ReadHuman )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadHuman
// Created: ABR 2011-03-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadHuman( xml::xistream& xis )
{
    const PHY_HumanRank* pRank = PHY_HumanRank::Find( xis.attribute< std::string >( "rank" ) );
    if( !pRank )
        xis.error( "Unknown human rank" );
    const PHY_HumanWound* pWound = PHY_HumanWound::Find( xis.attribute< std::string >( "state" ) );
    if( !pWound )
        xis.error( "Unknown human wound" );
    bool contaminated = xis.attribute< bool >( "contaminated", false );
    bool psyop = xis.attribute< bool >( "psyop", false );
    unsigned int number = xis.attribute< unsigned int >( "number" );
    if( !contaminated && !psyop && pWound == &PHY_HumanWound::notWounded_ )
    {
        for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        {
            unsigned int healthy = ( *it )->GetNbrHealthyHumans( *pRank );
            if( number > healthy )
                number -= healthy;
            else if( healthy )
            {
                ( *it )->RemoveHealthyHumans( *pRank, healthy - number );
                number = 0;
            }
        }
    }
    else
    {
        auto it = composantes_.begin();
        while( number )
        {
            number -= ( *it )->OverloadHumans( *pRank, number, *pWound, psyop, contaminated );
            if( ++it == composantes_.end() && number > 0 )
            {
                MT_LOG_WARNING_MSG( "Agent " << owner_->GetID() << " - Cannot apply all the human wounds overloading specified in ODB : " << number << " " << pRank->GetName() << " " << pWound->GetName() << " remaining" );
                return;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ChangeComposantesAvailability
// Created: NLD 2005-08-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, unsigned int nNewNbrUndamagedComposantes )
{
    unsigned int nNbrUndamagedComposantes = 0;
    unsigned int nNbrComposantes = 0;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& composante = **it;
        if( composante.GetType() != composanteType )
            continue;
        ++nNbrComposantes;
        if( composante.GetState() == PHY_ComposanteState::undamaged_ )
            ++nNbrUndamagedComposantes;
    }
    nNewNbrUndamagedComposantes = std::min( nNewNbrUndamagedComposantes, nNbrComposantes );
    if( nNewNbrUndamagedComposantes > nNbrUndamagedComposantes )
    {
        for( auto it = composantes_.begin(); it != composantes_.end() && nNbrUndamagedComposantes < nNewNbrUndamagedComposantes; ++it )
        {
            PHY_ComposantePion& composante = **it;
            if( composante.GetType() == composanteType && composante.GetState() != PHY_ComposanteState::undamaged_ )
            {
                composante.Repair( true );
                ++nNbrUndamagedComposantes;
            }
        }
    }
    else if( nNewNbrUndamagedComposantes < nNbrUndamagedComposantes )
    {
        for( auto it = composantes_.begin(); it != composantes_.end() && nNbrUndamagedComposantes > nNewNbrUndamagedComposantes; ++it )
        {
            PHY_ComposantePion& composante = **it;
            if( composante.GetType() == composanteType && composante.GetState() == PHY_ComposanteState::undamaged_ )
            {
                composante.ReinitializeState( PHY_ComposanteState::dead_ );
                --nNbrUndamagedComposantes;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::UpdateOperationalStates
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::UpdateOperationalStates()
{
    if( !HasChanged() && !bExternalMustChange_ )
        return;
    double rMajorOpStateValue = 0.;
    unsigned int nMajorOpStateNbr = 0;
    double rNonMajorOpStateValue = 0.;
    unsigned int nNonMajorOpStateNbr = 0;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& composante = **it;
        const double rCompOpState = composante.GetOperationalState();
        if( composante.IsMajor() )
        {
            ++nMajorOpStateNbr;
            rMajorOpStateValue += rCompOpState;
        }
        else
        {
            ++nNonMajorOpStateNbr;
            rNonMajorOpStateValue += rCompOpState;
        }
    }
    double rNewOpState = 0.;
    // Etat ops
    if( nMajorOpStateNbr == 0 ) // Pas de composantes majeures explicites
    {
        if( nNonMajorOpStateNbr != 0 )
            rNewOpState = rNonMajorOpStateValue / nNonMajorOpStateNbr;
    }
    else if( nNonMajorOpStateNbr == 0 ) // Pas de composantes non majeures (unit� homog�ne)
    {
        rNewOpState = rMajorOpStateValue / nMajorOpStateNbr;
    }
    else // Pion h�t�rog�ne
    {
        rNewOpState = ( rMajorOpStateValue    / nMajorOpStateNbr    ) * rOpStateWeightMajorComposante_
                    + ( rNonMajorOpStateValue / nNonMajorOpStateNbr ) * rOpStateWeightNonMajorComposante_;
    }
    PHY_RolePion_Humans* humans = owner_->RetrieveRole< PHY_RolePion_Humans >();
    if( humans && humans->HasNoMoreOperationalHumans() )
        rNewOpState = 0;
    if( rNewOpState != rOperationalState_ )
    {
        assert( rNewOpState >= 0. && rNewOpState <= 1. );
        bOperationalStateChanged_ = true;
        rOperationalState_ = rNewOpState;
    }

    // Etat ops majeur
    if( nMajorOpStateNbr > 0 )
        rMajorOperationalState_ = rMajorOpStateValue / nMajorOpStateNbr;
    else if( nNonMajorOpStateNbr > 0 )
        rMajorOperationalState_ = rNonMajorOpStateValue / nNonMajorOpStateNbr;
    else
        rMajorOperationalState_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::UpdateMajorComposante
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::UpdateMajorComposante()
{
    if( !HasChanged() )
        return;
    pMajorComposante_ = 0;
    unsigned int nMajorScore  = 0;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetMajorScore() >= nMajorScore )
        {
            pMajorComposante_ = &composante;
            nMajorScore = composante.GetMajorScore();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Update( bool /*bIsDead*/ )
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        (*it)->Update();
    if( HasChanged() )
    {
        owner_->Apply( &component::ComponentsChangedNotificationHandler_ABC::NotifyComponentHasChanged );
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        owner_->Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
    }
    if( bNeutralized_ && !IsNeutralized() )
    {
        bNeutralized_ = false;
        MIL_Report::PostEvent( *owner_, report::eRC_FinNeutralisation );
    }
    UpdateOperationalStates();
    UpdateMajorComposante();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Clean()
{
    if( nNbrComposanteChanged_ > 0 )
    {
        for( auto it = composanteTypes_.begin(); it != composanteTypes_.end(); ++it )
            it->second.bHasChanged_ = false;
        nNbrComposanteChanged_ = 0;
    }
    bLoansChanged_            = false;
    bExternalMustChange_      = false;
    bOperationalStateChanged_ = false;
    boost::for_each( maintenanceComposanteStates_, boost::mem_fn( &PHY_MaintenanceComposanteState::Clean ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::UpdateDataWhenComposanteAdded
// Created: NLD 2005-06-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::UpdateDataWhenComposanteAdded( const PHY_ComposanteState& compState, T_ComposanteTypeProperties& properties )
{
    if( compState.IsUsable() )
        ++nNbrUsableComposantes_;
    ++properties.nbrsPerState_[ compState.GetID() ];
    if( !properties.bHasChanged_ )
    {
        ++nNbrComposanteChanged_;
        properties.bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::UpdateDataWhenComposanteRemoved
// Created: NLD 2005-06-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::UpdateDataWhenComposanteRemoved( const PHY_ComposanteState& compState, T_ComposanteTypeProperties& properties )
{
    if( compState.IsUsable() )
    {
        assert( nNbrUsableComposantes_ > 0 );
        --nNbrUsableComposantes_;
    }

    assert( properties.nbrsPerState_[ compState.GetID() ] > 0 );
    --properties.nbrsPerState_[ compState.GetID() ];
    if( !properties.bHasChanged_ )
    {
        ++nNbrComposanteChanged_;
        properties.bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteAdded
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyComposanteAdded( PHY_ComposantePion& composante, T_Dotations* dotations )
{
    assert( std::find( composantes_.begin(), composantes_.end(), &composante ) == composantes_.end() );
    composantes_.push_back( &composante );
    UpdateDataWhenComposanteAdded( composante.GetState(), composanteTypes_[ &composante.GetType() ] );
    if( composante.GetState().IsUsable() )
        owner_->Apply( &dotation::DotationsActionsNotificationHandler_ABC::RegisterDotationsCapacities, composante.GetType().GetDotationCapacities(), dotations );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteRemoved
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::T_Dotations PHY_RolePion_Composantes::NotifyComposanteRemoved( PHY_ComposantePion& composante )
{
    assert( composanteTypes_.find( &composante.GetType() ) != composanteTypes_.end() );
    UpdateDataWhenComposanteRemoved( composante.GetState(), composanteTypes_[ &composante.GetType() ] );
    auto it = std::find( composantes_.begin(), composantes_.end(), &composante );
    assert( it != composantes_.end() );
    composantes_.erase( it );
    T_Dotations removedDotations;
    T_Dotations* pRemovedDotations = &removedDotations;
    if( composante.GetState().IsUsable() )
        owner_->Apply( &dotation::DotationsActionsNotificationHandler_ABC::UnregisterDotationsCapacities, composante.GetType().GetDotationCapacities(), pRemovedDotations );
    owner_->Apply( &transport::TransportNotificationHandler_ABC::CheckConsistency );
    return removedDotations;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyComposanteChanged( PHY_ComposantePion& composante, const PHY_ComposanteState& oldState )
{
    const PHY_ComposanteState& newState = composante.GetState();
    assert( oldState != newState );
    assert( composanteTypes_.find( &composante.GetType() ) != composanteTypes_.end() );
    T_ComposanteTypeProperties& properties = composanteTypes_[ &composante.GetType() ];
    UpdateDataWhenComposanteRemoved( oldState, properties );
    UpdateDataWhenComposanteAdded( newState, properties );
    T_Dotations* removedDotations = 0;
    if( !newState.IsUsable() && oldState.IsUsable() )
        owner_->Apply( &dotation::DotationsActionsNotificationHandler_ABC::UnregisterDotationsCapacities, composante.GetType().GetDotationCapacities(), removedDotations );
    else if( newState.IsUsable() && !oldState.IsUsable() )
        owner_->Apply( &dotation::DotationsActionsNotificationHandler_ABC::RegisterDotationsCapacities, composante.GetType().GetDotationCapacities(), removedDotations );
    owner_->Apply( &transport::TransportNotificationHandler_ABC::NotifyComposanteChanged, composante );
    PHY_RoleInterface_Supply* role = owner_->Retrieve< PHY_RoleInterface_Supply >();
    if( role )
        role->NotifyComposanteChanged( composante );
    owner_->Apply( &transport::TransportNotificationHandler_ABC::CheckConsistency );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteRepaired
// Created: NLD 2005-08-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyComposanteRepaired()
{
    if( !IsUsable() )
    {
        MT_Vector2D newPosition;
        if( owner_->GetAutomate().GetAlivePionsBarycenter( newPosition ) )
            owner_->MagicMove( newPosition );
        MIL_Report::PostEvent( *owner_, report::eRC_ANouveauDisponibleApresReparation );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetSignificantVolume
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PHY_Volume* PHY_RolePion_Composantes::GetSignificantVolume( const PHY_SensorTypeAgent_ABC& sensorType ) const
{
    const PHY_Volume* pSignificantVolume = 0;
    double rSignificantVolumeFactor = 0.;
    for( auto it = composanteTypes_.begin(); it != composanteTypes_.end(); ++it )
    {
        const PHY_Volume& compTypeVolume = it->first->GetVolume();
        const T_ComposanteTypeProperties& compProp = it->second;
        if( !compProp.HasUsableComposantes() )
            continue;
        double rVolumeFactor = sensorType.GetFactor( compTypeVolume );
        if( rVolumeFactor > rSignificantVolumeFactor )
        {
            pSignificantVolume = &compTypeVolume;
            rSignificantVolumeFactor = rVolumeFactor;
        }
    }
    return pSignificantVolume;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetVisibleVolumes
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::T_ComposanteVolumes PHY_RolePion_Composantes::GetVisibleVolumes() const
{
    tools::Set< const PHY_Volume* > volumes;
    for( auto it = composanteTypes_.begin(); it != composanteTypes_.end(); ++it )
        if( it->second.HasUsableComposantes() )
            volumes.insert( &it->first->GetVolume() );
    return T_ComposanteVolumes( volumes.begin(), volumes.end() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::BuildKnowledgeComposantes
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::BuildKnowledgeComposantes( T_KnowledgeComposanteVector& knowledge ) const
{
    knowledge.reserve( composantes_.size() );
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        knowledge.push_back( DEC_Knowledge_AgentComposante( **it ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DamageTransported
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DamageTransported( double rWeightToDamage, const PHY_ComposanteState& newState, bool bTransportOnlyLoadable ) const
{
    assert( newState.IsDamaged() );
    PHY_ComposantePion::T_ComposantePionVector composantes = composantes_;
    MIL_Random::random_shuffle( composantes, MIL_Random::eBreakdowns );
    for( auto it = composantes.begin(); it != composantes.end() && rWeightToDamage > 0; ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( ( !bTransportOnlyLoadable || composante.CanBeLoaded() )
            && newState < composante.GetState() )
        {
            composante.ReinitializeState( newState );
            rWeightToDamage -= composante.GetWeight();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CanTransportCrowd
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::CanTransportCrowd() const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        if( ( *it )->CanTransportCrowd() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetCrowdTransporterCapacity
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Composantes::GetCrowdTransporterCapacity() const
{
    unsigned int ret = 0;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        if( ( *it )->CanTransportCrowd() )
            ret += ( *it )->GetCrowdTransporterCapacity();
    return ret;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetCrowdTransporterLoadedPerTimeStep
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetCrowdTransporterLoadedPerTimeStep() const
{
    double ret = 0;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        if( ( *it )->CanTransportCrowd() )
            ret += ( *it )->GetCrowdTransporterLoadedPerTimeStep();
    return ret;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetCrowdTransporterUnloadedPerTimeStep
// Created: JSR 2011-08-10
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetCrowdTransporterUnloadedPerTimeStep() const
{
    double ret = 0;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        if( ( *it )->CanTransportCrowd() )
            ret += ( *it )->GetCrowdTransporterUnloadedPerTimeStep();
    return ret;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyEffect
// Created: LGY 2012-12-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyEffect( const WoundEffects_ABC& effect )
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ApplyEffect( effect );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyContamination
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyContamination()
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ApplyContamination();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyInjury
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyInjury( MIL_Injury_ABC& injury )
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ApplyInjury( injury );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyExplosion
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyExplosion( const AttritionCapacity& capacity, PHY_FireResults_ABC& fireResult )
{
    PHY_ComposantePion::T_ComposantePionVector composantes = composantes_;
    MIL_Random::random_shuffle( composantes, MIL_Random::eWounds );
    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( *owner_ );
    for( auto it = composantes.begin(); it != composantes.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanBeFired() )
        {
            fireResult.Hit( 1 );
            composante.ApplyExplosion( capacity, fireDamages );            
            MIL_Report::PostEvent( *owner_, report::eRC_ExplosionSurBouchonMine, fireResult.GetSourceName() );       
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyUrbanObjectCrumbling
// Created: SLG 2010-06-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyUrbanObjectCrumbling( const MIL_Object_ABC& object )
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanBeFired() )
            composante.ApplyUrbanObjectCrumbling( object );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyIndirectFire
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyIndirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& fireResult, double ratio )
{
    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( *owner_ );
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanBeFired() )
            composante.ApplyIndirectFire( dotationCategory, fireDamages, ratio );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyDirectFire
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyDirectFire( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& fireResult )
{
    assert( std::find( composantes_.begin(), composantes_.end(), &compTarget ) != composantes_.end() );
    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( *owner_ );
    PHY_ComposantePion& compTargetPion = static_cast< PHY_ComposantePion& >( compTarget );
    if( compTargetPion.CanBeFired() )
        compTargetPion.ApplyDirectFire( dotationCategory, fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyDirectFireOnMajorComposantes
// Created: MGD 2010-02-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyDirectFireOnMajorComposantes( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& fireResult )
{
    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( *owner_ );
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.IsMajor() && composante.CanBeFired() )
            composante.ApplyDirectFire( dotationCategory, fireDamages );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyPopulationFire
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyPopulationFire( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& fireResult, double armedIndividuals )
{
    assert( std::find( composantes_.begin(), composantes_.end(), &compTarget ) != composantes_.end() );
    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( *owner_ );
    PHY_ComposantePion& compTargetPion = static_cast< PHY_ComposantePion& >( compTarget );
    if( compTargetPion.CanBeFired() )
        compTargetPion.ApplyPopulationFire( populationType, populationAttitude, fireDamages, armedIndividuals );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Neutralize
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Neutralize()
{
    const unsigned int nCurrentTimeStep = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( !IsNeutralized() )
        MIL_Report::PostEvent( *owner_, report::eRC_DebutNeutralisation );
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        nNeutralizationEndTimeStep_ = std::max( nNeutralizationEndTimeStep_, nCurrentTimeStep + (*it)->GetNeutralizationTime() );
    bUnderIndirectFire_ = true;
    bNeutralized_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::WoundLoadedHumans
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::WoundLoadedHumans( const PHY_ComposantePion& composanteChanged, const PHY_ComposanteState& newState, PHY_FireDamages_Agent& fireDamages )
{
    if( !bIsLoaded_ )
        return;
    if( !composanteChanged.CanTransportHumans() )
        return;
    unsigned int nNbrHumansCarrier = 0;
    unsigned int nNbrHumansLoaded = 0;
    PHY_ComposantePion::T_ComposantePionVector::const_iterator itComp;
    for( itComp = composantes_.begin(); itComp != composantes_.end(); ++itComp )
    {
        const PHY_ComposantePion& composante = **itComp;
        if( composante.CanTransportHumans() )
            ++ nNbrHumansCarrier;
        if( composante.CanBeLoaded() )
            nNbrHumansLoaded += composante.GetNbrUsableHumans();
    }
    assert( nNbrHumansCarrier != 0 );
    unsigned int nNbrHumansToWound = nNbrHumansLoaded / nNbrHumansCarrier;
    for( itComp = composantes_.begin(); nNbrHumansToWound && itComp != composantes_.end(); ++itComp )
    {
        PHY_ComposantePion& composante = **itComp;
        if( composante.CanBeLoaded() )
        {
            nNbrHumansToWound -= std::min( nNbrHumansToWound, composante.GetNbrUsableHumans() );
            composante.ApplyHumansWounds( newState, fireDamages );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetComposantesAbleToBeFired
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetComposantesAbleToBeFired( PHY_ComposantePion::T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes /*= false*/ ) const
{
    targets.clear();
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
//TODO MGD TO REMOVE
        if( composante.CanBeFired() )
        {
            if( !bFireOnlyOnMajorComposantes || composante.IsMajor() )
                targets.push_back( &composante );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetComposantesAbleToBeFired
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetComposantesAbleToBeFired( PHY_ComposantePion::T_ComposanteVector& targets, unsigned int nNbrFirer, bool bFireOnlyOnMajorComposantes /*= false*/ ) const
{
    targets.clear();
    PHY_ComposantePion::T_ComposanteVector availableTargets;
    PHY_RolePion_Composantes::GetComposantesAbleToBeFired( availableTargets, bFireOnlyOnMajorComposantes );
    if( availableTargets.empty() )
        return;
    while( targets.size() < nNbrFirer )
    {
        MIL_Random::random_shuffle( availableTargets, MIL_Random::eFire );
        std::copy( availableTargets.begin(), availableTargets.end(), std::back_inserter( targets ) );
    }
    targets.resize( nNbrFirer );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendChangedState() const
{
    boost::for_each( maintenanceComposanteStates_, boost::mem_fn( &PHY_MaintenanceComposanteState::SendChangedState ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendFullState( unsigned int context ) const
{
    for( auto it = maintenanceComposanteStates_.begin(); it != maintenanceComposanteStates_.end(); ++it )
        (*it)->SendFullState( context );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendLoans
// Created: NLD 2005-01-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendLoans( client::UnitAttributes& message ) const
{
    typedef tools::Map< std::pair< const MIL_Agent_ABC*, const PHY_ComposanteTypePion* >, unsigned int > T_LoanCountMap;

    // Lent composantes
    {
        T_LoanCountMap loanData;
        for( auto it = lentComposantes_.begin(); it != lentComposantes_.end(); ++it )
        {
            const MIL_Agent_ABC& pion = *it->first;
            for( auto itComp = it->second.begin(); itComp != it->second.end(); ++itComp )
                ++loanData[ std::make_pair( &pion, &(*itComp)->GetType() ) ];
        }
        sword::LentEquipments& lentEquipements = *message().mutable_lent_equipments();
        for( auto it = loanData.begin(); it != loanData.end(); ++it )
        {
            sword::LentEquipments_LentEquipment& loan = *lentEquipements.add_elem();
            loan.mutable_borrower()->set_id( it->first.first ->GetID() );
            loan.mutable_type()->set_id( it->first.second->GetMosID().id() );
            loan.set_quantity( it->second );
        }
    }

    // Borrowed composantes
    {
        T_LoanCountMap loanData;
        for( auto it = borrowedComposantes_.begin(); it != borrowedComposantes_.end(); ++it )
        {
            const MIL_Agent_ABC& pion = *it->first;
            const PHY_ComposantePion::T_ComposantePionVector& composantes = it->second;
            for( auto itComp = composantes.begin(); itComp != composantes.end(); ++itComp )
                ++loanData[ std::make_pair( &pion, &(*itComp)->GetType() ) ];
        }
        sword::BorrowedEquipments& borrowedEquipements = *message().mutable_borrowed_equipments();
        for( auto it = loanData.begin(); it != loanData.end(); ++it )
        {
            sword::BorrowedEquipments_BorrowedEquipment& loan = *borrowedEquipements.add_elem();
            loan.mutable_owner()->set_id( it->first.first ->GetID() );
            loan.mutable_type()->set_id( it->first.second->GetMosID().id() );
            loan.set_quantity( it->second );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::FillEquipmentDotationMessage
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::AddEquipmentDotation( client::UnitAttributes& msg, const PHY_ComposanteTypePion& compType, const T_ComposanteTypeProperties& properties ) const
{
    sword::EquipmentDotations_EquipmentDotation& value  = *msg().mutable_equipment_dotations()->add_elem();
    value.mutable_type()->set_id( compType.GetMosID().id() );
    value.set_available( properties.nbrsPerState_[ PHY_ComposanteState::undamaged_.GetID() ] );
    value.set_unavailable( properties.nbrsPerState_[ PHY_ComposanteState::dead_.GetID() ] );
    value.set_repairable( properties.nbrsPerState_[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ] );
    value.set_on_site_fixable( properties.nbrsPerState_[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ] );
    value.set_repairing( properties.nbrsPerState_[ PHY_ComposanteState::maintenance_.GetID() ] );
    value.set_captured( properties.nbrsPerState_[ PHY_ComposanteState::prisoner_.GetID() ] );

    for( auto itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        PHY_ComposantePion& composante = **itComposante;
        if( composante.GetType() != compType || composante.GetState().GetID() != PHY_ComposanteState::repairableWithEvacuation_.GetID() )
            continue;
        const PHY_Breakdown*  breakdown = composante.GetBreakdown();
        assert( breakdown );
        value.add_breakdowns( breakdown->GetID() );
    }
    assert( value.repairable() == value.breakdowns_size() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendFullState( client::UnitAttributes& msg ) const
{
    for( auto it = composanteTypes_.begin(); it != composanteTypes_.end(); ++it )
        AddEquipmentDotation( msg, *it->first, it->second );
    msg().set_raw_operational_state( static_cast< unsigned int >( rOperationalState_ * 100. ) );
    SendLoans( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendChangedState( client::UnitAttributes& msg ) const
{
    if( nNbrComposanteChanged_ > 0 )
        for( auto it = composanteTypes_.begin(); it != composanteTypes_.end(); ++it )
            if( it->second.bHasChanged_ )
                AddEquipmentDotation( msg, *it->first, it->second );
    if( bOperationalStateChanged_ )
        msg().set_raw_operational_state( static_cast< unsigned int >( rOperationalState_ * 100. ) );
    if( bLoansChanged_ )
        SendLoans( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetDangerosity
// Created: MGD 2010-02-04
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetDangerosity( const DEC_Knowledge_AgentComposante& compTarget, float rDistBtwSourceAndTarget, bool bUseAmmo ) const
{
    double rDangerosity = 0.f;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        if( ( *it )->IsMajor() )
        rDangerosity = std::max( rDangerosity, ( *it )->GetDangerosity( compTarget, rDistBtwSourceAndTarget, bUseAmmo ) );
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMaxRangeToFireOn( const DEC_Knowledge_Agent& target, double rWantedPH, const PHY_DotationCategory* dotation ) const
{
    // Get back the most dangerous composante type of the target (from our point of view ...)
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();
    double rRange = 0;
    if( pTargetComposante )
        for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::max( rRange, (*it)->GetMaxRangeToFireOn( *pTargetComposante, rWantedPH, dotation ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetOnlyLoadableMaxRangeToFireOn
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetOnlyLoadableMaxRangeToFireOn( const DEC_Knowledge_Agent& target, double rWantedPH ) const
{
    // Get the most dangerous composante type of the target (from our point of view ...)
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();
    double rRange = 0;
    if( pTargetComposante )
        for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::max( rRange, (*it)->GetOnlyLoadableMaxRangeToFireOn( *pTargetComposante, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMinRangeToFireOn( const DEC_Knowledge_Agent& target, double rWantedPH ) const
{
    // Get The most dangerous composante type of the target
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();
    double rRange = std::numeric_limits< double >::max();
    if( pTargetComposante )
        for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::min( rRange, (*it)->GetMinRangeToFireOn( *pTargetComposante, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToFireOnActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMaxRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, double rWantedPH ) const
{
    // Get back the most dangerous composante type of the target (from our point of view ...)
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();
    double rRange = 0;
    if( pTargetComposante )
        for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::max( rRange, (*it)->GetMaxRangeToFireOnWithPosture( *pTargetComposante, target.GetAgentKnown(), rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMinRangeToFireOnActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMinRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, double rWantedPH ) const
{
    // Get The most dangerous composante type of the target
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();
    double rRange = std::numeric_limits< double >::max();
    if( pTargetComposante )
        for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::min( rRange, (*it)->GetMinRangeToFireOnWithPosture( *pTargetComposante, target.GetAgentKnown(), rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMaxRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability, bool theoric ) const
{
    double rRange = -1.;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::max( rRange, ( *it )->GetMaxRangeToIndirectFire( dotationCategory, bCheckDotationsAvailability, theoric ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToIndirectFire
// Created: GGE 2012-07-25
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMaxRangeToIndirectFire( ) const
{
    double rRange = -1.;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::max( rRange, ( *it )->GetMaxRangeToIndirectFire() );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMinRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability, bool theoric ) const
{
    double rRange = std::numeric_limits< double >::max();
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::min( rRange, ( *it )->GetMinRangeToIndirectFire( dotationCategory, bCheckDotationsAvailability, theoric ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToFire
// Created: DDA 2010-05-03
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMaxRangeToFire( const MIL_Agent_ABC&  pion, double rWantedPH ) const
{
    double rRange = 0.;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::max( rRange, (*it)->GetMaxRangeToFire( pion, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::PreprocessRandomBreakdowns( unsigned int nEndDayTimeStep ) const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        (*it)->PreprocessRandomBreakdowns( nEndDayTimeStep );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteWaitingForMaintenance
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState* PHY_RolePion_Composantes::NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante )
{
    MIL_AutomateLOG* pTC2 = owner_->GetLogisticHierarchy().GetPrimarySuperior();
    if( !pTC2 )
        return 0;
    // Pas de RC si log non branch�e ou si RC envoy� au tick pr�c�dent
    const unsigned int nCurrentTick = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( nCurrentTick > ( nTickRcMaintenanceQuerySent_ + 1 ) || nTickRcMaintenanceQuerySent_ == 0 )
        MIL_Report::PostEvent( *owner_, report::eRC_DemandeEvacuationMateriel );
    nTickRcMaintenanceQuerySent_ = nCurrentTick;
    PHY_MaintenanceComposanteState* pMaintenanceComposanteState = pTC2->MaintenanceHandleComposanteForTransport( *owner_, composante );
    if( !pMaintenanceComposanteState )
        return 0;
    maintenanceComposanteStates_.push_back( pMaintenanceComposanteState );
    return pMaintenanceComposanteState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteBackFromMaintenance
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyComposanteBackFromMaintenance( PHY_MaintenanceComposanteState& composanteState )
{
    boost::remove_erase( maintenanceComposanteStates_, &composanteState );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyLentComposanteReceived
// Created: NLD 2006-07-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyLentComposanteReceived( MIL_Agent_ABC& lender, PHY_ComposantePion& composante )
{
    borrowedComposantes_[ &lender ].push_back( &composante );
    bLoansChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyLentComposanteReturned
// Created: NLD 2006-07-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyLentComposanteReturned( MIL_Agent_ABC& lender, PHY_ComposantePion& composante )
{
    PHY_ComposantePion::T_ComposantePionVector& lentComps = borrowedComposantes_[ &lender ];
    auto itComp = std::find( lentComps.begin(), lentComps.end(), &composante );
    assert( itComp != lentComps.end() );
    lentComps.erase( itComp );
    if( lentComps.empty() )
        borrowedComposantes_.erase( borrowedComposantes_.find( &lender ) );
    bLoansChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::LendComposante
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::LendComposante( MIL_Agent_ABC& borrower, PHY_ComposantePion& composante )
{
    assert( std::find( composantes_.begin(), composantes_.end(), &composante ) != composantes_.end() );
    lentComposantes_[ &borrower ].push_back( &composante );
    composante.TransferComposante( borrower.GetRole< PHY_RoleInterface_Composantes >() );
    bLoansChanged_ = true;
    borrower.GetRole< PHY_RoleInterface_Composantes >().NotifyLentComposanteReceived( *owner_, composante );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::RetrieveLentComposante
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::RetrieveLentComposante( MIL_Agent_ABC& borrower, PHY_ComposantePion& composante )
{
    auto& lentComps = lentComposantes_[ &borrower ];
    auto it = std::find( lentComps.begin(), lentComps.end(), &composante );
    if( it == lentComps.end() )
        return;
    composante.TransferComposante( *this );
    lentComps.erase( it );
    if( lentComps.empty() )
        lentComposantes_.erase( &borrower );
    bLoansChanged_ = true;
    borrower.GetRole< PHY_RoleInterface_Composantes >().NotifyLentComposanteReturned( *owner_, composante );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::RetrieveAllLentComposantes
// Created: JSR 2013-01-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::RetrieveAllLentComposantes()
{
    while( lentComposantes_.size() > 0 )
    {
        const MIL_Agent_ABC* borrower = lentComposantes_.begin()->first;
        PHY_ComposantePion::T_ComposantePionVector vector = lentComposantes_.begin()->second;
        for( auto it = vector.begin(); it != vector.end(); ++it )
            RetrieveLentComposante( *const_cast< MIL_Agent_ABC* >( borrower ), **it );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReturnAllBorrowedComposantes
// Created: JSR 2013-01-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReturnAllBorrowedComposantes()
{
    while( borrowedComposantes_.size() > 0 )
    {
        MIL_Agent_ABC* lender = const_cast< MIL_Agent_ABC* >( borrowedComposantes_.begin()->first );
        PHY_RolePion_Composantes& lenderRole = lender->GetRole< PHY_RolePion_Composantes >();
        PHY_ComposantePion::T_ComposantePionVector vector = borrowedComposantes_.begin()->second;
        for( auto it = vector.begin(); it != vector.end(); ++it )
            lenderRole.RetrieveLentComposante( *owner_, **it );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyCaptured
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyCaptured()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyReleased
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyReleased()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifySurrendered
// Created: JSR 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifySurrendered()
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ReinitializeState( PHY_ComposanteState::prisoner_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifySurrenderCanceled
// Created: JSR 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifySurrenderCanceled()
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ReinitializeState( PHY_ComposanteState::undamaged_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::IsNeutralized
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::IsNeutralized() const
{
    return nNeutralizationEndTimeStep_ && nNeutralizationEndTimeStep_ >= MIL_Time_ABC::GetTime().GetCurrentTimeStep();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::HasChanged() const
{
    return nNbrComposanteChanged_ != 0 || bLoansChanged_ || bOperationalStateChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::IsUsable
// Created: NLD 2005-08-09
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::IsUsable() const
{
    return nNbrUsableComposantes_ > 0 || !lentComposantes_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMajorComposante
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_ComposantePion* PHY_RolePion_Composantes::GetMajorComposante() const
{
    return pMajorComposante_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxWeight
// @param loaded is true if we want to now the weight loaded
// Created: RPD 2009-11-23
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMaxWeight( bool loadedWeight ) const
{
    double weight = 0;
    if( loadedWeight || bIsLoaded_ ) // LMT : when a unit is unloaded, it can go in every urban block
        for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
            weight = std::max( weight, (*it)->GetWeight() );
    return weight;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetOperationalState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetOperationalState() const
{
    return rOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMajorOperationalState
// Created: NLD 2005-11-25
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMajorOperationalState() const
{
    return rMajorOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetPion
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& PHY_RolePion_Composantes::GetPion() const
{
    assert( owner_ );
    return *owner_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::IsImmobilized
// Created: LDC 2011-08-04
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::IsImmobilized() const
{
    return rOperationalState_ < rOpStateDecisionalThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::RepairAllComposantes
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::RepairAllComposantes( bool withLog )
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        ( **it ).Repair( withLog );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DestroyRandomComposante
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DestroyRandomComposante()
{
    PHY_ComposantePion::T_ComposantePionVector composantes = composantes_;
    MIL_Random::random_shuffle( composantes, MIL_Random::eWounds );
    for( auto it = composantes.begin(); it != composantes.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
        {
            composante.ReinitializeState( PHY_ComposanteState::dead_ );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DestroyAllComposantes
// Created: NLD 2004-12-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DestroyAllComposantes()
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        (*it)->ReinitializeState( PHY_ComposanteState::dead_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::KillAllHumans
// Created: ABR 2011-12-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::KillAllHumans()
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        (*it)->KillAllHumans();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetNbrUsableHumans
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Composantes::GetNbrUsableHumans() const
{
    unsigned int count = 0;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
       count += (*it)->GetNbrUsableHumans();
    return count;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute //@TODO MGD use OnComponentComputer hierarchie
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( firing::WeaponAvailabilityComputer_ABC& algorithm ) const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        (*it)->Execute( algorithm );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute //@TODO MGD use OnComponentComputer hierarchie
// Created: AHC 2009-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( transport::TransportCapacityComputer_ABC& algorithm ) const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        algorithm.ApplyOnComposante( **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute //@TODO MGD use OnComponentComputer hierarchie
// Created: AHC 2009-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( transport::TransportWeightComputer_ABC& algorithm ) const
{
    bool loadedTransporters = owner_->GetRole< transport::PHY_RoleInterface_Transported >().HasHumanTransportersToRecover();
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        bool canBeLoaded = ( *it )->CanBeLoaded();
        if( ( *it )->CanBeTransported() && ( !loadedTransporters || canBeLoaded ) )
            algorithm.AddTransportedWeight( ( *it )->GetWeight(), canBeLoaded );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: MGD 2009-09-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( OnComponentComputer_ABC& algorithm ) const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        algorithm.ApplyOnComponent( **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: MGD 2009-09-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( OnComponentLendedFunctorComputer_ABC& algorithm ) const
{
    for( auto itLoan = lentComposantes_.begin(); itLoan != lentComposantes_.end(); ++itLoan )
    {
        const PHY_ComposantePion::T_ComposantePionVector& composantes = itLoan->second;
        for( auto it = composantes.begin(); it != composantes.end(); ++it )
            algorithm.ApplyOnLendedComponent( **it );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: AHC 2009-09-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( transport::HumanLoadingTimeComputer_ABC& algorithm ) const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& composante= **it;
        if( composante.CanBeLoaded() )
            algorithm.AddHumans(composante.GetNbrUsableHumans());
        if( composante.CanTransportHumans() )
        {
            const PHY_ComposanteTypePion& compType = composante.GetType();
            algorithm.AddTime(compType.GetNbrHumansLoadedPerTimeStep  (), compType.GetNbrHumansUnloadedPerTimeStep());
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: AHC 2009-09-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( transport::LoadedStateConsistencyComputer_ABC& algorithm ) const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& composante= **it;
        algorithm.EnableCarrier(composante.CanTransportHumans());
        algorithm.EnableLoadable(composante.CanBeLoaded());
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: ???
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        algorithm.ApplyOnComponent( **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyHumanHasChanged
// Created: MGD 2009-09-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyHumanHasChanged()
{
    bExternalMustChange_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyIsLoaded
// Created: MGD 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyIsLoadedInVab()
{
    bIsLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyIsUnLoaded
// Created: MGD 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyIsUnLoadedInVab()
{
    bIsLoaded_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::UseAmmunition
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::UseAmmunition( const PHY_DotationCategory& category )
{
    for( auto itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        ( *itComposante )->UseAmmunition( category, *owner_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetPHModifier
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetPHModifier( const PHY_DotationCategory& category ) const
{
    double result = 1.;
    for( auto itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        result = std::min( result, ( *itComposante )->GetPHModifier( category, *owner_ ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CounterIndirectFire
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::CounterIndirectFire( const PHY_DotationCategory& category ) const
{
    for( auto itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        if( ( *itComposante )->CounterIndirectFire( category, *owner_ ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DestroyIndirectFire
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::DestroyIndirectFire( const PHY_DotationCategory& category ) const
{
    for( auto itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        if( ( *itComposante )->DestroyIndirectFire( category, *owner_ ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAttritionIndexComposante
// Created: DDA 2010-04-22
// -----------------------------------------------------------------------------
double  PHY_RolePion_Composantes::GetAttritionIndexComposante ( const PHY_MaterialCompositionType& material ) const
{
    double result = 0.;
    for( auto itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        result = std::max( result, ( *itComposante )->GetType().GetAttritionIndexComposante( material ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetConvoyTransportersUse
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
std::size_t PHY_RolePion_Composantes::GetConvoyTransportersTotal() const
{
    auto convoyable = boost::mem_fn( &PHY_ComposantePion::CouldBePartOfConvoy );
    std::size_t total = boost::count_if( composantes_, convoyable );
    for( auto it = lentComposantes_.begin(); it != lentComposantes_.end(); ++it )
        total += boost::count_if( it->second, convoyable );
    return total;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetStockTransporterCapacity
// Created: BCI 2011-02-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetStockTransporterCapacity( const PHY_DotationNature& nature, double& rTotalWeightMax, double& rTotalVolumeMax ) const
{
    rTotalWeightMax = 0;
    rTotalVolumeMax = 0;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( &nature == (*it)->GetType().GetStockTransporterNature() )
        {
            double rWeightMax = 0;
            double rVolumeMax = 0;
            (*it)->GetStockTransporterCapacity( rWeightMax, rVolumeMax );
            rTotalWeightMax += rWeightMax;
            rTotalVolumeMax += rVolumeMax;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetOrAddStock
// Created: BCI 2011-02-11
// -----------------------------------------------------------------------------
PHY_DotationStock& PHY_RolePion_Composantes::GetOrAddStock( PHY_RoleInterface_Supply& supplyRole, const PHY_DotationCategory& dotation )
{
    PHY_DotationStock* pStock = supplyRole.GetStock( dotation );
    if( !pStock )
    {
        double rWeightCapacity, rVolumeCapacity;
        GetStockTransporterCapacity( dotation.GetNature(), rWeightCapacity, rVolumeCapacity );
        double capacity = std::min( rWeightCapacity / dotation.GetWeight(), rVolumeCapacity / dotation.GetVolume() );
        pStock = supplyRole.AddEmptyStock( dotation, capacity );
    }
    return *pStock;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CanStockMoreOf
// Created: BCI 2011-02-14
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::CanStockMoreOf( PHY_RoleInterface_Supply& supplyRole, const PHY_DotationCategory& dotation ) const
{
    double rWeight, rVolume;
    supplyRole.ComputeStockWeightAndVolume( dotation.GetNature(), rWeight, rVolume );

    double rWeightCapacity, rVolumeCapacity;
    GetStockTransporterCapacity( dotation.GetNature(), rWeightCapacity, rVolumeCapacity );

    rWeightCapacity = std::floor( rWeightCapacity );
    rVolumeCapacity = std::floor( rVolumeCapacity );
    rWeight = std::ceil( rWeight );
    rVolume = std::ceil( rVolume );

    return rWeight < rWeightCapacity && rVolume < rVolumeCapacity;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GiveComposante
// Created: ABR 2011-06-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GiveComposante( unsigned int id, int quantity, PHY_RolePion_Composantes& borrower )
{
    for( auto it = composantes_.rbegin(); it != composantes_.rend() && quantity; )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanBeLent() && composante.GetType().GetMosID().id() == id )
        {
            --quantity;
            LendComposante( const_cast< MIL_Agent_ABC& >( borrower.GetPion() ), composante );
            it = composantes_.rbegin(); // TransfertComposante modifie composantes_
        }
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CreateBreakdowns
// Created: ABR 2011-08-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::CreateBreakdowns( const PHY_ComposanteTypePion& composanteType, unsigned int quantity, unsigned int breakdownId )
{
    const PHY_BreakdownType* breakdownType = ( breakdownId == 0 ) ? composanteType.GetRandomBreakdownType() : PHY_BreakdownType::Find( breakdownId );
    assert( breakdownType != 0 );
    for( auto it = composantes_.begin(); it != composantes_.end() && quantity > 0; ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( &composante.GetType() == &composanteType && composante.GetState().IsUsable() && composante.GetState() != PHY_ComposanteState::repairableWithEvacuation_ )
        {
            composante.ReinitializeState( PHY_ComposanteState::repairableWithEvacuation_, breakdownType );
            --quantity;
        }
    }
    if( quantity )
        MT_LOG_WARNING_MSG( "Agent " << owner_->GetID() << " - Cannot create all the breakdowns in the magic action, " << quantity << " breakdowns for composante " << composanteType.GetName() << " remaining." );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CreateWounds
// Created: ABR 2011-08-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::CreateWounds( unsigned int quantity, bool randomWound, sword::EnumHumanWound wound )
{
    const PHY_HumanWound* pHumanWound = 0;
    if( randomWound )
        pHumanWound = PHY_HumanWound::GetRandomWoundSeriousness();
    else
        pHumanWound = PHY_HumanWound::Find( wound );
    if( !pHumanWound )
        throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
    if( pHumanWound == &PHY_HumanWound::notWounded_ )
    {
        for( auto it = composantes_.begin(); it != composantes_.end() && quantity > 0; ++it )
            quantity -= ( *it )->HealHumans( PHY_HumanRank::militaireDuRang_, quantity );
        for( auto it = composantes_.begin(); it != composantes_.end() && quantity > 0; ++it )
            quantity -= ( *it )->HealHumans( PHY_HumanRank::sousOfficier_, quantity );
        for( auto it = composantes_.begin(); it != composantes_.end() && quantity > 0; ++it )
            quantity -= ( *it )->HealHumans( PHY_HumanRank::officier_, quantity );
    }
    else
    {
        for( auto it = composantes_.begin(); it != composantes_.end() && quantity > 0; ++it )
            quantity -= ( *it )->WoundHumans( PHY_HumanRank::militaireDuRang_, quantity, *pHumanWound );
        for( auto it = composantes_.begin(); it != composantes_.end() && quantity > 0; ++it )
            quantity -= ( *it )->WoundHumans( PHY_HumanRank::sousOfficier_, quantity, *pHumanWound );
        for( auto it = composantes_.begin(); it != composantes_.end() && quantity > 0; ++it )
            quantity -= ( *it )->WoundHumans( PHY_HumanRank::officier_, quantity, *pHumanWound );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ChangeEquipmentState
// Created: ABR 2011-08-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ChangeEquipmentState( const PHY_ComposanteTypePion& composanteType, const sword::MissionParameter_Value& message )
{
    typedef std::pair< const PHY_ComposanteState*, unsigned int > T_Repartition;
    std::vector< T_Repartition > repartition;

    repartition.push_back( T_Repartition( &PHY_ComposanteState::undamaged_,                   message.list( 1 ).quantity() ) );
    repartition.push_back( T_Repartition( &PHY_ComposanteState::dead_,                        message.list( 2 ).quantity() ) );
    repartition.push_back( T_Repartition( &PHY_ComposanteState::repairableWithEvacuation_,    message.list( 3 ).quantity() ) );
    repartition.push_back( T_Repartition( &PHY_ComposanteState::repairableWithoutEvacuation_, message.list( 4 ).quantity() ) );
    repartition.push_back( T_Repartition( &PHY_ComposanteState::maintenance_,                 message.list( 5 ).quantity() ) );
    repartition.push_back( T_Repartition( &PHY_ComposanteState::prisoner_,                    message.list( 6 ).quantity() ) );

    std::vector< T_Repartition >::iterator stateIt = repartition.begin();
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( &composante.GetType() == &composanteType )
        {
            while( stateIt != repartition.end() && stateIt->second == 0 )
                ++stateIt;
            if( stateIt == repartition.end() )
                return;
            if( stateIt->first == &PHY_ComposanteState::repairableWithEvacuation_ )
            {
                if( !message.list( 7 ).list( stateIt->second - 1 ).has_identifier() )
                    throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
                unsigned int breakdownId = message.list( 7 ).list( stateIt->second - 1 ).identifier();
                const PHY_BreakdownType* breakdownType = ( breakdownId == 0 ) ? composanteType.GetRandomBreakdownType() : PHY_BreakdownType::Find( static_cast< unsigned int >( breakdownId ) );
                if( !breakdownType )
                    throw MASA_EXCEPTION_ASN( sword::UnitActionAck_ErrorCode, sword::UnitActionAck::error_invalid_parameter );
                composante.ReinitializeState( *stateIt->first, breakdownType );
            }
            else
                composante.ReinitializeState( *stateIt->first );
            stateIt->second -= 1;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ChangeHumanState
// Created: ABR 2011-08-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ChangeHumanState( const sword::MissionParameters& msg )
{
    sword::MissionParameters copy = msg;
    for( auto itCurrentComp = composantes_.begin(); itCurrentComp != composantes_.end(); ++itCurrentComp )
        ( *itCurrentComp )->ChangeHumanState( copy );
    int remaining = 0;
    for( int i = 0 ; i < copy.elem( 0 ).value_size(); ++i )
    {
        const sword::MissionParameter_Value& elem = copy.elem( 0 ).value().Get( i );
        remaining += static_cast< unsigned int >( elem.list( 0 ).quantity() );
    }
    if( remaining )
        MT_LOG_WARNING_MSG( "Agent " << owner_->GetID() << " - Cannot apply all the human states in the magic action, " << remaining << " states remaining." );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CanEvacuateCasualties
// Created: MMC 2012-06-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::CanEvacuateCasualties() const
{
    for( auto itCurrentComp = composantes_.begin(); itCurrentComp != composantes_.end(); ++itCurrentComp )
    {
        const PHY_ComposantePion& composante = **itCurrentComp;
        const PHY_ComposanteTypePion& compType = composante.GetType();
        if( compType.CanEvacuateCasualties() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CancelLogisticRequests
// Created: JSR 2013-02-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::CancelLogisticRequests()
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        ( *it )->CancelLogisticRequests();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::IsUnderIndirectFire
// Created: LMT 2012-06-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::IsUnderIndirectFire()
{
    return bUnderIndirectFire_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ResetUnderIndirectFire
// Created: LMT 2012-06-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ResetUnderIndirectFire()
{
    bUnderIndirectFire_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CanPerceive
// Created: MMC 2013-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::CanPerceive( const MIL_ObjectType_ABC& objectType ) const
{
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion* pComposante = *it;
        if( pComposante && pComposante->CanPerceive( objectType ) )
            return true;
    }
    return false;
}
