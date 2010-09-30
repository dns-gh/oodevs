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
#include "MIL_AgentServer.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/MIL_Injury_ABC.h"
#include "Entities/Agents/Roles/Logistic/PHY_MaintenanceComposanteState.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Actions/PHY_FireResults_ABC.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "hla/HLA_UpdateFunctor.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

#include "simulation_kernel/TransportCapacityComputer_ABC.h"
#include "simulation_kernel/TransportWeightComputer_ABC.h"
#include "simulation_kernel/HumanLoadingTimeComputer_ABC.h"
#include "simulation_kernel/LoadedStateConsistencyComputer_ABC.h"
#include "simulation_kernel/SpeedComputer_ABC.h"

#include "simulation_kernel/OnComponentComputer_ABC.h"
#include "simulation_kernel/OnComponentLendedFunctorComputer_ABC.h"
#include "simulation_kernel/ComponentsChangedNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/VisionConeNotificationHandler_ABC.h"
#include "simulation_kernel/DotationsActionsNotificationHandler_ABC.h"

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
    unsigned int i  = 0;
    for( std::vector< unsigned int >::const_iterator it = nbrsPerState_.begin(); it != nbrsPerState_.end(); ++it, ++i )
    {
        if( *it > 0 && PHY_ComposanteState::Find( i ).IsUsable() )
            return true;
    }
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
PHY_RolePion_Composantes::PHY_RolePion_Composantes( MIL_Agent_ABC& pion, bool initialise )
    : pion_                        ( pion )
    , composantes_                 ()
    , lentComposantes_             ()
    , borrowedComposantes_         ()
    , nNbrComposanteChanged_       ( 0 )
    , nNbrUsableComposantes_       ( 0 )
    , rOperationalState_           ( 0. )
    , rMajorOperationalState_      ( 0. )
    , bOperationalStateChanged_    ( false )
    , pMajorComposante_            ( 0 )
    , nNeutralizationEndTimeStep_  ( 0 )
    , bLoansChanged_               ( false )
    , bExternalMustChange_         ( false )
    , bTransportHasChanged_        ( false )
    , bIsLoaded_                   ( false )
    , bIsSurrender_                ( false )
    , nTickRcMaintenanceQuerySent_ ( 0 )
{
    if( initialise ) // $$$$ LDC: Set to false for debug in order to avoid useless creation of crap.
    {
        pion_.GetType().GetUnitType().InstanciateComposantes( *this );
        DistributeCommanders ();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::~PHY_RolePion_Composantes()
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        delete *it;
    composantes_.clear();
}

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_RolePion_Composantes::T_LoanMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Composantes::T_LoanMap& map, const unsigned int )
        {
            unsigned size = map.size();
            file << size;
            for(  PHY_RolePion_Composantes::CIT_LoanMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Composantes::T_LoanMap& map, const unsigned int )
        {
            unsigned int nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                PHY_RolePion_Composantes* pRole;
                file >> pRole;
                file >> map[ pRole ];
            }
        }

        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_RolePion_Composantes::T_ComposanteTypeMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Composantes::T_ComposanteTypeMap& map, const unsigned int )
        {
            unsigned size = map.size();
            file << size;
            for(  PHY_RolePion_Composantes::CIT_ComposanteTypeMap it = map.begin(); it != map.end(); ++it )
            {
                Common::EquipmentType id = it->first->GetMosID();
                int equipment_type = id.id();
                file << equipment_type;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Composantes::T_ComposanteTypeMap& map, const unsigned int )
        {
            unsigned int nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                Common::EquipmentType nID;
                int equipment_type;
                file >> equipment_type;
                nID.set_id( equipment_type );
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
         & nTickRcMaintenanceQuerySent_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::WriteODB( xml::xostream& /*xos*/ ) const
{
}

//-----------------------------------------------------------------------------
// Name: PHY_UnitCanHaveComposante_ABC::DistributeCommanders
// Created: JVT 03-08-27
//-----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DistributeCommanders()
{
    // R�partition des officiers

    PHY_ComposantePion::T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );


    const PHY_UnitType::T_CommanderRepartitionMap& commanderRepartition = pion_.GetType().GetUnitType().GetCommanderRepartition();
    for( PHY_UnitType::CIT_CommanderRepartitionMap it = commanderRepartition.begin(); it != commanderRepartition.end(); ++it )
    {
        const PHY_HumanRank& rank = *it->first;
              unsigned int   nNbr =  it->second;

        bool bStopRepartition = false;
        while( nNbr && !bStopRepartition )
        {
            bStopRepartition = true;
            for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes.begin(); it != composantes.end() && nNbr; ++it )
            {
                if( (**it).ChangeHumanRank( PHY_HumanRank::militaireDuRang_, rank, PHY_HumanWound::notWounded_ ) )
                {
                    bStopRepartition = false;
                    --nNbr;
                }
            }
        }
        if( nNbr )
            MT_LOG_WARNING_MSG( "Agent " << pion_.GetID() << " - Not enough humans in crew to distribute commanders : " << nNbr << " " << rank.GetName() << " remaining" );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DistributeHumanWounds
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DistributeHumanWounds( const PHY_HumanRank& rank, const PHY_HumanWound& newWound, unsigned int nNbr, PHY_ComposantePion::CIT_ComposantePionVector& itCurrentComp )
{
    assert( itCurrentComp != composantes_.end() );
    if( newWound == PHY_HumanWound::notWounded_ )
        return;

    PHY_ComposantePion::CIT_ComposantePionVector itEndComp = itCurrentComp;
    while( nNbr )
    {
        nNbr -= (*itCurrentComp)->WoundHumans( rank, nNbr, newWound );
        if( ++itCurrentComp == composantes_.end() )
            itCurrentComp = composantes_.begin();
        if( itCurrentComp == itEndComp && nNbr > 0 )
        {
            MT_LOG_WARNING_MSG( "Agent " << pion_.GetID() << " - Cannot apply all the human wounds overloading specified in ODB : " << nNbr << " " << rank.GetName() << " " << newWound.GetName() << " remaining" );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadComposantesOverloading
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadComposantesOverloading( xml::xistream& xis )
{
    xis >> xml::list( "Equipements", *this, &PHY_RolePion_Composantes::ReadEquipements );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadEquipements
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadEquipements( xml::xistream& xis )
{
    xis >> xml::list( "Equipement", *this, &PHY_RolePion_Composantes::ReadEquipement );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadEquipement
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadEquipement( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );

    const PHY_ComposanteTypePion* pType = PHY_ComposanteTypePion::Find( strType );
    if( !pType )
        xis.error( "Unknwon composante type" );

    unsigned int nNbrRepairable;
    unsigned int nNbrDead;

    xis >> xml::attribute( "reparable", nNbrRepairable )
        >> xml::attribute( "indisponible", nNbrDead );

    for( PHY_ComposantePion::CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        PHY_ComposantePion& composante = **itComposante;
        if( !( composante.GetType() == *pType && composante.GetState() == PHY_ComposanteState::undamaged_ ) )
            continue;
        if( nNbrDead )
        {
            --nNbrDead;
            composante.ReinitializeState( PHY_ComposanteState::dead_ );
        }
        else if( nNbrRepairable )
        {
            --nNbrRepairable;
            composante.ReinitializeState( PHY_ComposanteState::repairableWithEvacuation_ );
        }
        else
            break;
    }

    if( nNbrDead || nNbrRepairable )
        MT_LOG_WARNING_MSG( "Agent " << pion_.GetID() << " - Cannot apply all the composantes states overloading specified in ODB : " << nNbrDead << " deads and " << nNbrRepairable << " repairables '" << pType->GetName() << "' remaining" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadHumansOverloading
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadHumansOverloading( xml::xistream& xis )
{
    xis >> xml::list( "Personnels", *this, &PHY_RolePion_Composantes::ReadPersonnels );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadPersonnels
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadPersonnels( xml::xistream& xis )
{
    xis >> xml::list( "Personnel", *this, &PHY_RolePion_Composantes::ReadPersonnel );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadPersonnel
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadPersonnel( xml::xistream& xis )
{
    std::string strState;
    PHY_ComposantePion::CIT_ComposantePionVector itCurrentComp = composantes_.begin();

    xis >> xml::attribute( "etat", strState );

    const PHY_HumanWound* pWound = PHY_HumanWound::Find( strState );
    if( !pWound )
        xis.error( "Unknown human wound" );

    const PHY_HumanRank::T_HumanRankMap& ranks = PHY_HumanRank::GetHumanRanks();
    for( PHY_HumanRank::CIT_HumanRankMap itRank = ranks.begin(); itRank != ranks.end(); ++itRank )
    {
        const PHY_HumanRank& rank = *itRank->second;
        std::stringstream strRank;
        strRank << "nb" << rank.GetName();
        unsigned int nNbr;
        xis >> xml::attribute( strRank.str(), nNbr );
        DistributeHumanWounds( rank, *pWound, nNbr, itCurrentComp );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadOverloading
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadOverloading( xml::xistream& xis )
{
    ReadComposantesOverloading( xis );
    ReadHumansOverloading     ( xis );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ChangeComposantesAvailability
// Created: NLD 2005-08-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, unsigned int nNewNbrUndamagedComposantes )
{
    unsigned int nNbrUndamagedComposantes = 0;
    unsigned int nNbrComposantes          = 0;
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& composante = **it;
        if( composante.GetType() != composanteType )
            continue;

        ++ nNbrComposantes;
        if( composante.GetState() == PHY_ComposanteState::undamaged_ )
            ++ nNbrUndamagedComposantes;
    }

    nNewNbrUndamagedComposantes = std::min( nNewNbrUndamagedComposantes, nNbrComposantes );

    if( nNewNbrUndamagedComposantes > nNbrUndamagedComposantes )
    {
        for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end() && nNbrUndamagedComposantes < nNewNbrUndamagedComposantes; ++it )
        {
            PHY_ComposantePion& composante = **it;
            if( composante.GetType() == composanteType && composante.GetState() != PHY_ComposanteState::undamaged_ )
            {
                composante.Repair();
                ++ nNbrUndamagedComposantes;
            }
        }
    }
    else if( nNewNbrUndamagedComposantes < nNbrUndamagedComposantes )
    {
        for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end() && nNbrUndamagedComposantes > nNewNbrUndamagedComposantes; ++it )
        {
            PHY_ComposantePion& composante = **it;
            if( composante.GetType() == composanteType && composante.GetState() == PHY_ComposanteState::undamaged_ )
            {
                composante.ReinitializeState( PHY_ComposanteState::dead_ );
                -- nNbrUndamagedComposantes;
            }
        }
    }
}
/*
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::WoundHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::WoundHumans( const PHY_HumanRank& rank, unsigned int nNbr )
{
    T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );

    IT_ComposantePionVector itCurrentComp = composantes.begin();
    while( nNbr && itCurrentComp != composantes.end() )
    {
        unsigned int nNbrChanged = (*itCurrentComp)->WoundHumans( rank, 1, PHY_HumanWound::killed_ );
        if( nNbrChanged == 0 )
            itCurrentComp = composantes.erase( itCurrentComp );
        else
        {
            nNbr -= nNbrChanged;
            ++ itCurrentComp;
        }
        if( itCurrentComp == composantes.end() )
            itCurrentComp = composantes.begin();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HealHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::HealHumans( const PHY_HumanRank& rank, unsigned int nNbr )
{
    T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );

    IT_ComposantePionVector itCurrentComp = composantes.begin();
    while( nNbr && itCurrentComp != composantes.end() )
    {
        unsigned int nNbrChanged = (*itCurrentComp)->HealHumans( rank, 1 );
        if( nNbrChanged == 0 )
            itCurrentComp = composantes.erase( itCurrentComp );
        else
        {
            nNbr -= nNbrChanged;
            ++ itCurrentComp;
        }
        if( itCurrentComp == composantes.end() )
            itCurrentComp = composantes.begin();
    }
}
*/
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::UpdateOperationalStates
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::UpdateOperationalStates()
{
    if( !HasChanged() && !bExternalMustChange_ )
        return;

    MT_Float     rMajorOpStateValue    = 0.;
    unsigned int nMajorOpStateNbr      = 0;
    MT_Float     rNonMajorOpStateValue = 0.;
    unsigned int nNonMajorOpStateNbr   = 0;
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& composante   = **it;
        const MT_Float            rCompOpState = composante.GetOperationalState();

        if( composante.IsMajor() )
        {
            ++ nMajorOpStateNbr;
            rMajorOpStateValue += rCompOpState;
        }
        else
        {
            ++ nNonMajorOpStateNbr;
            rNonMajorOpStateValue += rCompOpState;
        }
    }

    MT_Float rNewOpState = 0.;

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

    if( rNewOpState != rOperationalState_ )
    {
        assert( rNewOpState >= 0. && rNewOpState <= 1. );
        bOperationalStateChanged_ = true;
        rOperationalState_        = rNewOpState;
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
    if( !HasChanged() && !bTransportHasChanged_ )
        return;

    pMajorComposante_ = 0;
    unsigned int nMajorScore  = 0;
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetMajorScore() >= nMajorScore )
        {
            pMajorComposante_ = &composante;
            nMajorScore       = composante.GetMajorScore();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Update( bool /*bIsDead*/ )
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).Update();

    if( HasChanged() )
    {
        pion_.Apply( &component::ComponentsChangedNotificationHandler_ABC::NotifyComponentHasChanged );
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        pion_.Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
    }

    UpdateOperationalStates();
    UpdateMajorComposante  ();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Clean()
{
    if( nNbrComposanteChanged_ > 0 )
    {
        for( IT_ComposanteTypeMap it = composanteTypes_.begin(); it != composanteTypes_.end(); ++it )
            it->second.bHasChanged_ = false;
        nNbrComposanteChanged_ = 0;
    }
    bLoansChanged_            = false;
    bExternalMustChange_      = false;
    bTransportHasChanged_     = false;
    bOperationalStateChanged_ = false;

    for( CIT_MaintenanceComposanteStateSet it = maintenanceComposanteStates_.begin(); it != maintenanceComposanteStates_.end(); ++it )
        (**it).Clean();
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

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
void PHY_RolePion_Composantes::NotifyComposanteAdded( PHY_ComposantePion& composante )
{
    assert( std::find( composantes_.begin(), composantes_.end(), &composante ) == composantes_.end() );
    composantes_.push_back( &composante );

    UpdateDataWhenComposanteAdded( composante.GetState(), composanteTypes_[ &composante.GetType() ] );

    if( composante.GetState().IsUsable() )
        pion_.Apply( &dotation::DotationsActionsNotificationHandler_ABC::RegisterDotationsCapacities, composante.GetType().GetDotationCapacities() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteRemoved
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyComposanteRemoved( PHY_ComposantePion& composante )
{
    assert( composanteTypes_.find( &composante.GetType() ) != composanteTypes_.end() );
    UpdateDataWhenComposanteRemoved( composante.GetState(), composanteTypes_[ &composante.GetType() ] );

    PHY_ComposantePion::IT_ComposantePionVector it = std::find( composantes_.begin(), composantes_.end(), &composante );
    assert( it != composantes_.end() );
    composantes_.erase( it );

    if( composante.GetState().IsUsable() )
        pion_.Apply( &dotation::DotationsActionsNotificationHandler_ABC::UnregisterDotationsCapacities, composante.GetType().GetDotationCapacities() );

    pion_.Apply( &transport::TransportNotificationHandler_ABC::CheckConsistency );
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
    UpdateDataWhenComposanteAdded  ( newState, properties );

    if( !newState.IsUsable() && oldState.IsUsable() )
        pion_.Apply( &dotation::DotationsActionsNotificationHandler_ABC::UnregisterDotationsCapacities, composante.GetType().GetDotationCapacities() );
    else if( newState.IsUsable() && !oldState.IsUsable() )
        pion_.Apply( &dotation::DotationsActionsNotificationHandler_ABC::RegisterDotationsCapacities, composante.GetType().GetDotationCapacities() );

    pion_.Apply( &transport::TransportNotificationHandler_ABC::NotifyComposanteChanged, composante );
    pion_.Apply( &transport::TransportNotificationHandler_ABC::CheckConsistency );
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
        if( pion_.GetAutomate().GetAlivePionsBarycenter( newPosition ) )
            pion_.MagicMove( newPosition );

        MIL_Report::PostEvent( pion_, MIL_Report::eReport_ReAvailableAfterRepairation );
    }
}

// =============================================================================
// PERCEPTION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetSignificantVolume
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PHY_Volume* PHY_RolePion_Composantes::GetSignificantVolume( const PHY_SensorTypeAgent& sensorType ) const
{
    const PHY_Volume* pSignificantVolume = 0;
    MT_Float rSignificantVolumeFactor    = 0.;
    for( CIT_ComposanteTypeMap it = composanteTypes_.begin(); it != composanteTypes_.end(); ++it )
    {
        const PHY_Volume&                  compTypeVolume = it->first->GetVolume();
        const T_ComposanteTypeProperties&  compProp       = it->second;

        if( !compProp.HasUsableComposantes() )
            continue;

        MT_Float rVolumeFactor = sensorType.GetFactor( compTypeVolume );
        if( rVolumeFactor > rSignificantVolumeFactor )
        {
            pSignificantVolume       = &compTypeVolume;
            rSignificantVolumeFactor =  rVolumeFactor;
        }
    }
    return pSignificantVolume;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetVisibleVolumes
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetVisibleVolumes( T_ComposanteVolumeSet& volumes ) const
{
    volumes.clear();
    for( CIT_ComposanteTypeMap it = composanteTypes_.begin(); it != composanteTypes_.end(); ++it )
    {
        const PHY_Volume&                 compTypeVolume = it->first->GetVolume();
        const T_ComposanteTypeProperties& compProp       = it->second;

        if( !compProp.HasUsableComposantes() )
            continue;

        volumes.insert( &compTypeVolume );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::BuildKnowledgeComposantes
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::BuildKnowledgeComposantes( T_KnowledgeComposanteVector& knowledge ) const
{
    knowledge.reserve( composantes_.size() );
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        knowledge.push_back( DEC_Knowledge_AgentComposante( **it ) );
}

// =============================================================================
// FIRE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DamageTransported
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DamageTransported( MT_Float rWeightToDamage, const PHY_ComposanteState& newState, bool bTransportOnlyLoadable ) const
{
    assert( newState.IsDamaged() );
    PHY_ComposantePion::T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes.begin(); it != composantes.end() && rWeightToDamage > 0; ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( !bTransportOnlyLoadable || composante.CanBeLoaded() )
        {
            if( newState < composante.GetState() )
            {
                composante.ReinitializeState( newState );
                rWeightToDamage -= composante.GetWeight();
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyContamination
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyContamination( const MIL_ToxicEffectManipulator& contamination )
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ApplyContamination( contamination );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyPoisonous
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyPoisonous( const MIL_ToxicEffectManipulator& contamination )
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ApplyPoisonous( contamination );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyInjury
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyInjury( MIL_Injury_ABC& injury )
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
    std::random_shuffle( composantes.begin(), composantes.end() );

    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( pion_ );
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes.begin(); it != composantes.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanBeFired() )
        {
            fireResult.Hit();
            composante.ApplyExplosion( capacity, fireDamages );
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
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
void PHY_RolePion_Composantes::ApplyIndirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& fireResult, MT_Float ratio )
{
    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( pion_ );
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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

    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( pion_ );
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
    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( pion_ );
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
void PHY_RolePion_Composantes::ApplyPopulationFire( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& fireResult )
{
    assert( std::find( composantes_.begin(), composantes_.end(), &compTarget ) != composantes_.end() );

    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( pion_ );
    PHY_ComposantePion& compTargetPion = static_cast< PHY_ComposantePion& >( compTarget );

    if( compTargetPion.CanBeFired() )
        compTargetPion.ApplyPopulationFire( populationType, populationAttitude, fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Neutralize
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Neutralize()
{
    const unsigned int nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        nNeutralizationEndTimeStep_ = std::max( nNeutralizationEndTimeStep_, nCurrentTimeStep + (**it).GetNeutralizationTime() );
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
    unsigned int nNbrHumansLoaded  = 0;
    PHY_ComposantePion::CIT_ComposantePionVector itComp;
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
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
        std::random_shuffle( availableTargets.begin(), availableTargets.end() );
        std::copy( availableTargets.begin(), availableTargets.end(), std::back_inserter( targets ) );
    }
    targets.resize( nNbrFirer );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendLogisticChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendLogisticChangedState() const
{
    for( CIT_MaintenanceComposanteStateSet it = maintenanceComposanteStates_.begin(); it != maintenanceComposanteStates_.end(); ++it )
        (**it).SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendLogisticFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendLogisticFullState() const
{
    for( CIT_MaintenanceComposanteStateSet it = maintenanceComposanteStates_.begin(); it != maintenanceComposanteStates_.end(); ++it )
        (**it).SendFullState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendLoans
// Created: NLD 2005-01-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendLoans( client::UnitAttributes& message ) const
{
    typedef std::pair< const MIL_Agent_ABC*, const PHY_ComposanteTypePion* > T_Key;
    typedef std::map < T_Key, unsigned int >                                 T_LoanCountMap;
    typedef T_LoanCountMap::const_iterator                                   CIT_LoanCountMap;

    // Lent composantes
    {
        T_LoanCountMap loanData;
        for( CIT_LoanMap it = lentComposantes_.begin(); it != lentComposantes_.end(); ++it )
        {
            const MIL_Agent_ABC&          pion        = it->first->GetPion();
            const PHY_ComposantePion::T_ComposantePionVector& composantes = it->second;
            for( PHY_ComposantePion::CIT_ComposantePionVector itComp = composantes.begin(); itComp != composantes.end(); ++itComp )
                ++loanData[ T_Key( &pion, &(**itComp).GetType() ) ];
        }
        if( !loanData.empty() )
        {
            for( CIT_LoanCountMap it = loanData.begin(); it != loanData.end(); ++it )
            {
                MsgsSimToClient::LentEquipments_LentEquipment& loan = *message().mutable_equipements_pretes()->add_elem();
                loan.mutable_borrower()->set_id( it->first.first ->GetID() );
                loan.mutable_type()->set_id( it->first.second->GetMosID().id() );
                loan.set_nombre              ( it->second );
            }
        }
    }

    // Borrowed composantes
    {
        T_LoanCountMap loanData;
        for( CIT_LoanMap it = borrowedComposantes_.begin(); it != borrowedComposantes_.end(); ++it )
        {
            const MIL_Agent_ABC&          pion        = it->first->GetPion();
            const PHY_ComposantePion::T_ComposantePionVector& composantes = it->second;
            for( PHY_ComposantePion::CIT_ComposantePionVector itComp = composantes.begin(); itComp != composantes.end(); ++itComp )
                ++loanData[ T_Key( &pion, &(**itComp).GetType() ) ];
        }
        if( !loanData.empty() )
        {
            for( CIT_LoanCountMap it = loanData.begin(); it != loanData.end(); ++it )
            {
                MsgsSimToClient::BorrowedEquipments_BorrowedEquipment& loan = *message().mutable_equipements_empruntes()->add_elem();
                loan.mutable_owner()->set_id(  it->first.first ->GetID() );
                loan.mutable_type()->set_id(  it->first.second->GetMosID().id() );
                loan.set_nombre           ( it->second );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendFullState( client::UnitAttributes& msg ) const
{
    if( !composanteTypes_.empty() )
    {
        for( CIT_ComposanteTypeMap itComposanteType = composanteTypes_.begin(); itComposanteType != composanteTypes_.end(); ++itComposanteType )
        {
            const PHY_ComposanteTypePion&    compType   = *itComposanteType->first;
            const T_ComposanteTypeProperties& properties =  itComposanteType->second;

            MsgsSimToClient::EquipmentDotations_EquipmentDotation& value  = *msg().mutable_dotation_eff_materiel()->add_elem();
            value.mutable_type()->set_id(  compType.GetMosID().id() );
            value.set_nb_disponibles             ( properties.nbrsPerState_[ PHY_ComposanteState::undamaged_ .GetID() ] );
            value.set_nb_indisponibles           ( properties.nbrsPerState_[ PHY_ComposanteState::dead_      .GetID() ] );
            value.set_nb_reparables              ( properties.nbrsPerState_[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ] + properties.nbrsPerState_[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ] );
            value.set_nb_dans_chaine_maintenance ( properties.nbrsPerState_[ PHY_ComposanteState::maintenance_.GetID() ] );
            value.set_nb_prisonniers             ( properties.nbrsPerState_[ PHY_ComposanteState::prisoner_   .GetID() ] );
        }
    }


    msg().set_etat_operationnel_brut( (unsigned int)( rOperationalState_ * 100. ) );

    SendLoans( msg );

    SendLogisticFullState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendChangedState( client::UnitAttributes& msg ) const
{
    if( nNbrComposanteChanged_ > 0 )
    {
        for( CIT_ComposanteTypeMap itComposanteType = composanteTypes_.begin(); itComposanteType != composanteTypes_.end(); ++itComposanteType )
        {
            const PHY_ComposanteTypePion&     compType   = *itComposanteType->first;
            const T_ComposanteTypeProperties& properties =  itComposanteType->second;

            if( !properties.bHasChanged_ )
                continue;

            MsgsSimToClient::EquipmentDotations_EquipmentDotation& value  = *msg().mutable_dotation_eff_materiel()->add_elem();
            value.mutable_type()->set_id( compType.GetMosID().id() );
            value.set_nb_disponibles            ( properties.nbrsPerState_[ PHY_ComposanteState::undamaged_  .GetID() ] );
            value.set_nb_indisponibles          ( properties.nbrsPerState_[ PHY_ComposanteState::dead_       .GetID() ] );
            value.set_nb_reparables             ( properties.nbrsPerState_[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ] + properties.nbrsPerState_[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ] );
            value.set_nb_dans_chaine_maintenance( properties.nbrsPerState_[ PHY_ComposanteState::maintenance_.GetID() ] );
            value.set_nb_prisonniers            ( properties.nbrsPerState_[ PHY_ComposanteState::prisoner_   .GetID() ] );
        }
    }

    if( bOperationalStateChanged_ )
    {
        msg().set_etat_operationnel_brut( (unsigned int)( rOperationalState_ * 100. ) );
    }

    if( bLoansChanged_ )
        SendLoans( msg );

    SendLogisticChangedState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Serialize
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Serialize( HLA_UpdateFunctor& functor ) const
{
    typedef std::pair< std::string, unsigned long > T_Composante;
    std::vector< T_Composante > composantes;
    for( CIT_ComposanteTypeMap it = composanteTypes_.begin(); it != composanteTypes_.end(); ++it )
    {
        assert( it->first );
        const PHY_ComposanteTypePion& composanteType = *it->first;
        const T_ComposanteTypeProperties& properties = it->second;
        const T_Composante hlaComposante = T_Composante( composanteType.GetName(), properties.nbrsPerState_[ PHY_ComposanteState::undamaged_.GetID() ] );
        if( pMajorComposante_ && pMajorComposante_->GetType() == composanteType )
            composantes.insert( composantes.begin(), hlaComposante );
        else
            composantes.push_back( hlaComposante );
    }
    functor.Serialize( "composantes", HasChanged(), composantes );
}

// =============================================================================
// FIRE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetDangerosity
// Created: MGD 2010-02-04
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetDangerosity( const DEC_Knowledge_AgentComposante& compTarget, float rDistBtwSourceAndTarget ) const
{
    double rDangerosity = 0.f;
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        rDangerosity = std::max( rDangerosity, (*it)->GetDangerosity( compTarget, rDistBtwSourceAndTarget ) );
    }
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMaxRangeToFireOn( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const
{
    // Get back the most dangerous composante type of the target (from our point of view ...)
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();

    MT_Float rRange = 0;
    if( pTargetComposante )
        for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::max( rRange, (**it).GetMaxRangeToFireOn( *pTargetComposante, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetOnlyLoadableMaxRangeToFireOn
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetOnlyLoadableMaxRangeToFireOn( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const
{
    // Get the most dangerous composante type of the target (from our point of view ...)
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();

    MT_Float rRange = 0;
    if( pTargetComposante )
        for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::max( rRange, (**it).GetOnlyLoadableMaxRangeToFireOn( *pTargetComposante, rWantedPH ) );
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
        for(  PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::min( rRange, (**it).GetMinRangeToFireOn( *pTargetComposante, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToFireOnActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMaxRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const
{
    // Get back the most dangerous composante type of the target (from our point of view ...)
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();

    MT_Float rRange = 0;
    if( pTargetComposante )
        for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::max( rRange, (**it).GetMaxRangeToFireOnWithPosture( *pTargetComposante, target.GetAgentKnown(), rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMinRangeToFireOnActualPosture
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMinRangeToFireOnActualPosture( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const
{
    // Get The most dangerous composante type of the target
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();

    MT_Float rRange = std::numeric_limits< MT_Float >::max();
    if( pTargetComposante )
        for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
            rRange = std::min( rRange, (**it).GetMinRangeToFireOnWithPosture( *pTargetComposante, target.GetAgentKnown(), rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMaxRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const
{
    MT_Float rRange = -1.;

    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::max( rRange, (*it)->GetMaxRangeToIndirectFire( dotationCategory, bCheckDotationsAvailability ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMinRangeToIndirectFire( const PHY_DotationCategory& dotationCategory, bool bCheckDotationsAvailability ) const
{
    MT_Float rRange = std::numeric_limits< MT_Float >::max();

    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::min( rRange, (*it)->GetMinRangeToIndirectFire( dotationCategory, bCheckDotationsAvailability ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToFire
// Created: DDA 2010-05-03
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetMaxRangeToFire( const MIL_Agent_ABC&  pion, double rWantedPH ) const
{
    double rRange = std::numeric_limits< double >::max();

    for(  PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        if( (*it)->IsMajor() )
            rRange = std::min( rRange, (**it).GetMaxRangeToFire( pion, rWantedPH ) );

    return rRange;
}

// =============================================================================
// LOGISTIC - MAINTENANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::PreprocessRandomBreakdowns( unsigned int nEndDayTimeStep ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).PreprocessRandomBreakdowns( nEndDayTimeStep );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteWaitingForMaintenance
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState* PHY_RolePion_Composantes::NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante )
{
    MIL_AutomateLOG* pTC2 = pion_.GetAutomate().GetTC2();
    if( !pTC2 )
        return 0;

    // Pas de RC si log non branch�e ou si RC envoy� au tick pr�c�dent
    const unsigned int nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nCurrentTick > ( nTickRcMaintenanceQuerySent_ + 1 ) || nTickRcMaintenanceQuerySent_ == 0 )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_EquipementEvacuationRequest );
    nTickRcMaintenanceQuerySent_ = nCurrentTick;

    PHY_MaintenanceComposanteState* pMaintenanceComposanteState = pTC2->MaintenanceHandleComposanteForTransport( pion_, composante );
    if( !pMaintenanceComposanteState )
        return 0;

    if( ! maintenanceComposanteStates_.insert( pMaintenanceComposanteState ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return pMaintenanceComposanteState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteBackFromMaintenance
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyComposanteBackFromMaintenance( PHY_MaintenanceComposanteState& composanteState )
{
    if( maintenanceComposanteStates_.erase( &composanteState ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
}

// =============================================================================
//  PRET DE COMPOSANTE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyLentComposanteReceived
// Created: NLD 2006-07-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyLentComposanteReceived( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante )
{
    assert( std::find ( borrowedComposantes_[ &lender ].begin(), borrowedComposantes_[ &lender ].end(), &composante ) == borrowedComposantes_[ &lender ].end() );

    borrowedComposantes_[ &lender ].push_back( &composante );
    bLoansChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyLentComposanteReturned
// Created: NLD 2006-07-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyLentComposanteReturned( PHY_RoleInterface_Composantes& lender, PHY_ComposantePion& composante )
{
    PHY_ComposantePion::T_ComposantePionVector& lentComps = borrowedComposantes_[ &lender ];

    PHY_ComposantePion::IT_ComposantePionVector itComp = std::find( lentComps.begin(), lentComps.end(), &composante );
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
void PHY_RolePion_Composantes::LendComposante( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante )
{
    assert( composante.CanBeLent() );
    assert( std::find( composantes_.begin(), composantes_.end(), &composante ) != composantes_.end() );

    lentComposantes_[ &borrower ].push_back( &composante );
    composante.TransferComposante( borrower );
    bLoansChanged_ = true;

    borrower.NotifyLentComposanteReceived( *this, composante );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::RetrieveLentComposante
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::RetrieveLentComposante( PHY_RoleInterface_Composantes& borrower, PHY_ComposantePion& composante )
{
    PHY_ComposantePion::T_ComposantePionVector& lentComps = lentComposantes_[ &borrower ];

    PHY_ComposantePion::IT_ComposantePionVector itComp = std::find( lentComps.begin(), lentComps.end(), &composante );
    assert( itComp != lentComps.end() );

    composante.TransferComposante( *this );

    lentComps.erase( itComp );
    if( lentComps.empty() )
        lentComposantes_.erase( lentComposantes_.find( &borrower ) );
    bLoansChanged_ = true;

    borrower.NotifyLentComposanteReturned( *this, composante );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyCaptured
// C: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyCaptured()
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ReinitializeState( PHY_ComposanteState::prisoner_ );
    }
    bIsSurrender_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyReleased
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyReleased()
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ReinitializeState( PHY_ComposanteState::undamaged_ );
    }
    bIsSurrender_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::IsNeutralized
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::IsNeutralized() const
{
    return nNeutralizationEndTimeStep_ && nNeutralizationEndTimeStep_ >= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
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
// Name: PHY_RolePion_Composantes::GetMajorComponentWeight
// Created: RPD 2009-11-23
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMajorComponentWeight() const
{
    MT_Float weight ( 0 );
    const PHY_ComposantePion* majorComponent = GetMajorComposante();
    if( majorComponent )
    {
        weight = ( MT_Float ) majorComponent->GetWeight();
    }
    return weight;
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetOperationalState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetOperationalState() const
{
    return rOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMajorOperationalState
// Created: NLD 2005-11-25
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMajorOperationalState() const
{
    return rMajorOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetPion
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& PHY_RolePion_Composantes::GetPion() const
{
    return pion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::RepairAllComposantes
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::RepairAllComposantes()
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).Repair();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DestroyRandomComposante
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DestroyRandomComposante()
{
    PHY_ComposantePion::T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );

    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes.begin(); it != composantes.end(); ++it )
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
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).ReinitializeState( PHY_ComposanteState::dead_ );
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute //@TODO MGD use OnComponentComputer hierarchie
// Created: MGD 2009-09-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( firing::WeaponAvailabilityComputer_ABC& algorithm ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).Execute( algorithm );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute //@TODO MGD use OnComponentComputer hierarchie
// Created: AHC 2009-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( transport::TransportCapacityComputer_ABC& algorithm ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        algorithm.ApplyOnComposante( **it );
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute //@TODO MGD use OnComponentComputer hierarchie
// Created: AHC 2009-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( transport::TransportWeightComputer_ABC& algorithm ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (*it)->CanBeTransported() )
            algorithm.AddTransportedWeight((*it)->GetWeight(), (*it)->CanBeLoaded());
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: MGD 2009-09-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( OnComponentComputer_ABC& algorithm ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        algorithm.ApplyOnComponent( **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: MGD 2009-09-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( OnComponentLendedFunctorComputer_ABC& algorithm ) const
{
    for( CIT_LoanMap itLoan = lentComposantes_.begin(); itLoan != lentComposantes_.end(); ++itLoan )
    {
        const PHY_ComposantePion::T_ComposantePionVector& composantes = itLoan->second;
        for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes.begin(); it != composantes.end(); ++it )
        {
            algorithm.ApplyOnLendedComponent( **it );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Execute
// Created: AHC 2009-09-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Execute( transport::HumanLoadingTimeComputer_ABC& algorithm ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& composante= **it;
        algorithm.EnableCarrier(composante.CanTransportHumans());
        algorithm.EnableLoadable(composante.CanBeLoaded());
    }
}

void PHY_RolePion_Composantes::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        const PHY_ComposantePion& composante= **it;
        algorithm.ApplyOnComponent(composante);
    }
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
// Name: PHY_RolePion_Composantes::NotifyTransportHasChanged
// Created: MGD 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyTransportHasChanged()
{
    bTransportHasChanged_ = true;//@TODO MGD See to remove it isn't more called
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
    for( PHY_ComposantePion::CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        (*itComposante)->UseAmmunition( category, pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetPHModifier
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
double PHY_RolePion_Composantes::GetPHModifier( const PHY_DotationCategory& category ) const
{
    double result = 1.;
    for( PHY_ComposantePion::CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        result = std::min( result, (*itComposante)->GetPHModifier( category, pion_ ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::CounterIndirectFire
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::CounterIndirectFire( const PHY_DotationCategory& category ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        if( (*itComposante)->CounterIndirectFire( category, pion_ ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DestroyIndirectFire
// Created: LDC 2010-01-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::DestroyIndirectFire( const PHY_DotationCategory& category ) const
{
    for( PHY_ComposantePion::CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        if( (*itComposante)->DestroyIndirectFire( category, pion_ ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAttritionIndexComposante
// Created: DDA 2010-04-22
// -----------------------------------------------------------------------------

double  PHY_RolePion_Composantes::GetAttritionIndexComposante ( int idMaterial ) const
{
    double result = 0.;
    for( PHY_ComposantePion::CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        const PHY_ComposanteTypePion& compType = ( *itComposante )->GetType();
        result = std::max( result, compType.GetAttritionIndexComposante( idMaterial ) );
    }
    return result;
}

