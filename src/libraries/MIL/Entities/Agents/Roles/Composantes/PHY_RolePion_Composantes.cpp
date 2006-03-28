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

#include "MIL_pch.h"

#include "PHY_RolePion_Composantes.h"

#include "Entities/RC/MIL_RC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Roles/Network/NET_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Humans/PHY_RolePion_Humans.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_SmokeData.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Actions/PHY_FireResults_ABC.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Network/NET_ASN_Messages.h"
#include "Hla/HLA_UpdateFunctor.h"

MT_Float PHY_RolePion_Composantes::rOpStateWeightNonMajorComposante_             = 0.;
MT_Float PHY_RolePion_Composantes::rOpStateWeightMajorComposante_                = 0.;
MT_Float PHY_RolePion_Composantes::rMaxDangerosityDegradationByNeutralizedState_ = 0.;
MT_Float PHY_RolePion_Composantes::rMaxDangerosityDegradationByOpState_          = 0.;

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Composantes, "PHY_RolePion_Composantes" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::T_ComposanteTypeProperties constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::T_ComposanteTypeProperties::T_ComposanteTypeProperties()
    : nbrsPerState_( PHY_ComposanteState::GetNbrStates(), 0 )
    , bHasChanged_ ( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::T_ComposanteTypeProperties::HasUsableComposantes
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Composantes::T_ComposanteTypeProperties::HasUsableComposantes() const
{
    uint i  = 0;
    for( std::vector< uint >::const_iterator it = nbrsPerState_.begin(); it != nbrsPerState_.end(); ++it, ++i )
    {
        if( PHY_ComposanteState::FindComposanteState( i ).IsUsable() && *it > 0 )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::T_ComposanteTypeProperties::serialize
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Composantes::T_ComposanteTypeProperties::serialize( Archive& file, const uint )
{
    file & nbrsPerState_
         & bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::PHY_RolePion_Composantes( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_Composantes( role )
    , pPion_                       ( &pion )
    , composantes_                 ()
    , lentComposantes_             ()
    , nNbrComposanteChanged_       ( 0 )
    , nNbrUsableComposantes_       ( 0 )
    , rOperationalState_           ( 0. )
    , rMajorOperationalState_      ( 0. )
    , bOperationalStateChanged_    ( false )
    , pMajorComposante_            ( 0 )
    , nNeutralizationEndTimeStep_  ( 0 )
    , bLendsChanged_               ( false )
    , nTickRcMaintenanceQuerySent_ ( 0 )
{
    assert( pPion_ );
    pPion_->GetType().GetUnitType().InstanciateComposantes( *this );
    DistributeCommanders ();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::PHY_RolePion_Composantes()
    : PHY_RoleInterface_Composantes()
    , pPion_                       ()
    , composantes_                 ()
    , lentComposantes_             ()
    , bLendsChanged_               ( false )
    , composanteTypes_             ()
    , nNbrComposanteChanged_       ( 0 )
    , nNbrUsableComposantes_       ( 0 )
    , rOperationalState_           ( 0. )
    , rMajorOperationalState_      ( 0. )
    , bOperationalStateChanged_    ( false )
    , pMajorComposante_            ( 0 )
    , nNeutralizationEndTimeStep_  ( 0 )
    , maintenanceComposanteStates_ ()
    , nTickRcMaintenanceQuerySent_ ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RolePion_Composantes::~PHY_RolePion_Composantes()
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        delete *it;
    composantes_.clear();
}


// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_RolePion_Composantes::T_ComposantePionVector& vector, const uint nVersion )
        {
            split_free( file, vector, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Composantes::T_ComposantePionVector& vector, const uint )
        {
            file << vector.size();
            for(  PHY_RolePion_Composantes::CIT_ComposantePionVector it = vector.begin(); it != vector.end(); ++it )
                file << *it;
        }

        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Composantes::T_ComposantePionVector& vector, const uint )
        {
            uint nNbr;
            file >> nNbr;
            vector.reserve( nNbr );
            while ( nNbr-- )
            {
                PHY_ComposantePion* pComp;
                file >> pComp;
                vector.push_back( pComp );
            }
        }

        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_RolePion_Composantes::T_LentComposanteMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Composantes::T_LentComposanteMap& map, const uint )
        {
            file << map.size();
            for(  PHY_RolePion_Composantes::CIT_LentComposanteMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Composantes::T_LentComposanteMap& map, const uint )
        {
            uint nNbr;
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
        void serialize( Archive& file, PHY_RolePion_Composantes::T_ComposanteTypeMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Composantes::T_ComposanteTypeMap& map, const uint )
        {
            file << map.size();
            for(  PHY_RolePion_Composantes::CIT_ComposanteTypeMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first->GetMosID();
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Composantes::T_ComposanteTypeMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                ASN1T_TypeEquipement nID;

                file >> nID;
                file >> map[ PHY_ComposanteTypePion::FindComposanteType( nID ) ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::serialize
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Composantes::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Composantes >( *this )
         & pPion_
         & composantes_
         & lentComposantes_
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

// =============================================================================
// INIT
// =============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_UnitCanHaveComposante_ABC::DistributeCommanders
// Created: JVT 03-08-27
//-----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DistributeCommanders()
{
    // Répartition des officiers
    CIT_ComposantePionVector itCurrentComp = composantes_.begin();
    if( itCurrentComp == composantes_.end() )
    {
        MT_LOG_WARNING_MSG( "Agent " << pPion_->GetID() << " - Not enough composantes to distribute commanders" );
        return;
    }

    assert( pPion_ );
    const PHY_UnitType::T_CommanderRepartitionMap& commanderRepartition = pPion_->GetType().GetUnitType().GetCommanderRepartition();
    for( PHY_UnitType::CIT_CommanderRepartitionMap it = commanderRepartition.begin(); it != commanderRepartition.end(); ++it )
    {
        const PHY_HumanRank& rank = *it->first;
              uint           nNbr =  it->second;

        while( nNbr-- )
        {
            if( (*itCurrentComp)->ChangeHumanRank( PHY_HumanRank::militaireDuRang_, rank, PHY_HumanWound::notWounded_ ) )
            {
                if( ++itCurrentComp == composantes_.end() )
                    itCurrentComp = composantes_.begin();
            }
            else
                MT_LOG_WARNING_MSG( "Agent " << pPion_->GetID() << " - Not enough humans in crew to distribute commanders" );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::DistributeHumanWounds
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::DistributeHumanWounds( const PHY_HumanRank& rank, const PHY_HumanWound& newWound, uint nNbr, CIT_ComposantePionVector& itCurrentComp )
{
    assert( itCurrentComp != composantes_.end() );
    if( newWound == PHY_HumanWound::notWounded_ )
        return;

    CIT_ComposantePionVector itEndComp = itCurrentComp;
    while( nNbr )
    {
        nNbr -= (*itCurrentComp)->WoundHumans( rank, nNbr, newWound );
        if( ++itCurrentComp == composantes_.end() )
            itCurrentComp = composantes_.begin();
        if( itCurrentComp == itEndComp && nNbr > 0 )
        {
            MT_LOG_WARNING_MSG( "Agent " << pPion_->GetID() << " - Cannot apply all the human wounds overloading specified in ODB" );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadComposantesOverloading
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadComposantesOverloading( MIL_InputArchive& archive )
{
    if( !archive.BeginList( "Equipements", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        archive.Section( "Equipement" );

        std::string strType;
        archive.ReadAttribute( "type", strType );

        const PHY_ComposanteTypePion* pType = PHY_ComposanteTypePion::FindComposanteType( strType );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknwon composante type", archive.GetContext() );

        uint nNbrUndamaged;
        uint nNbrRepairable;
        uint nNbrDead;

        archive.ReadAttribute( "disponible"  , nNbrUndamaged  );
        archive.ReadAttribute( "reparable"   , nNbrRepairable );
        archive.ReadAttribute( "indisponible", nNbrDead       );

        for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
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
            MT_LOG_WARNING_MSG( "Agent " << pPion_->GetID() << " - Cannot apply all the composantes states overloading specified in ODB" );

        archive.EndSection(); // Equipement
    }
    archive.EndList(); // Equipements
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadHumansOverloading
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadHumansOverloading( MIL_InputArchive& archive )
{
    if( !archive.BeginList( "Personnels", MIL_InputArchive::eNothing ) )
        return;

    CIT_ComposantePionVector itCurrentComp = composantes_.begin();

    while( archive.NextListElement() )
    {
        archive.Section( "Personnel" );

        std::string strState;
        archive.ReadAttribute( "etat", strState );

        const PHY_HumanWound* pWound = PHY_HumanWound::Find( strState );
        if( !pWound )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown human wound", archive.GetContext() );

        const PHY_HumanRank::T_HumanRankMap& ranks = PHY_HumanRank::GetHumanRanks();
        for( PHY_HumanRank::CIT_HumanRankMap itRank = ranks.begin(); itRank != ranks.end(); ++itRank )
        {
            const PHY_HumanRank& rank = *itRank->second;
            std::stringstream strRank;
            strRank << "nb" << rank.GetName();
            uint nNbr;
            archive.ReadAttribute( strRank.str(), nNbr );
            DistributeHumanWounds( rank, *pWound, nNbr, itCurrentComp );
        }
        archive.EndSection(); // Personnel
    }
    archive.EndList(); // Personnels
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ReadOverloading
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ReadOverloading( MIL_InputArchive& archive )
{
    ReadComposantesOverloading( archive );
    ReadHumansOverloading     ( archive );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ChangeComposantesAvailability
// Created: NLD 2005-08-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ChangeComposantesAvailability( const PHY_ComposanteTypePion& composanteType, uint nNewNbrUndamagedComposantes )
{
    uint nNbrUndamagedComposantes = 0;
    uint nNbrComposantes          = 0;
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
        for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end() && nNbrUndamagedComposantes < nNewNbrUndamagedComposantes; ++it )
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
        for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end() && nNbrUndamagedComposantes > nNewNbrUndamagedComposantes; ++it )
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::WoundHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::WoundHumans( const PHY_HumanRank& rank, uint nNbr )
{
    T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );

    IT_ComposantePionVector itCurrentComp = composantes.begin();
    while( nNbr && itCurrentComp != composantes.end() )
    {
        uint nNbrChanged = (*itCurrentComp)->WoundHumans( rank, 1, PHY_HumanWound::killed_ );
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
void PHY_RolePion_Composantes::HealHumans( const PHY_HumanRank& rank, uint nNbr )
{
    T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );

    IT_ComposantePionVector itCurrentComp = composantes.begin();
    while( nNbr && itCurrentComp != composantes.end() )
    {
        uint nNbrChanged = (*itCurrentComp)->HealHumans( rank, 1 );
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
// Name: PHY_RolePion_Composantes::UpdateOperationalStates
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::UpdateOperationalStates()
{
    if( !HasChanged() && !GetRole< PHY_RolePion_Humans >().HasChanged() )
        return;

    MT_Float rMajorOpStateValue    = 0.;
    uint     nMajorOpStateNbr      = 0;
    MT_Float rNonMajorOpStateValue = 0.;
    uint     nNonMajorOpStateNbr   = 0;
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
    else if( nNonMajorOpStateNbr == 0 ) // Pas de composantes non majeures (unité homogène)
    {
        if( nMajorOpStateNbr != 0 )
            rNewOpState = rMajorOpStateValue / nMajorOpStateNbr;
    }
    else // Pion hétérogène
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
    if( !HasChanged() && !GetRole< PHY_RoleAction_Loading >().HasChanged() && !GetRole< PHY_RolePion_Transported >().HasChanged() )
        return;

    pMajorComposante_ = 0;
    uint nMajorScore  = 0;
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).Update();

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
    bLendsChanged_            = false;
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
        GetRole< PHY_RolePion_Dotations >().RegisterDotationsCapacities( composante.GetType().GetDotationCapacities() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteRemoved
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyComposanteRemoved( PHY_ComposantePion& composante )
{
    assert( composanteTypes_.find( &composante.GetType() ) != composanteTypes_.end() );
    UpdateDataWhenComposanteRemoved( composante.GetState(), composanteTypes_[ &composante.GetType() ] );

    IT_ComposantePionVector it = std::find( composantes_.begin(), composantes_.end(), &composante );
    assert( it != composantes_.end() );
    composantes_.erase( it );

    if( composante.GetState().IsUsable() )
        GetRole< PHY_RolePion_Dotations >().UnregisterDotationsCapacities( composante.GetType().GetDotationCapacities() );

    GetRole< PHY_RoleAction_Loading   >().CheckConsistency();
    GetRole< PHY_RoleAction_Transport >().CheckConsistency();
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
        GetRole< PHY_RolePion_Dotations >().UnregisterDotationsCapacities( composante.GetType().GetDotationCapacities() );
    else if( newState.IsUsable() && !oldState.IsUsable() )
        GetRole< PHY_RolePion_Dotations >().RegisterDotationsCapacities( composante.GetType().GetDotationCapacities() );

    GetRole< PHY_RoleAction_Transport >().NotifyComposanteChanged( composante );
    GetRole< PHY_RolePion_Supply      >().NotifyComposanteChanged( composante );

    GetRole< PHY_RoleAction_Loading   >().CheckConsistency();
    GetRole< PHY_RoleAction_Transport >().CheckConsistency();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteRepaired
// Created: NLD 2005-08-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyComposanteRepaired()
{
    assert( pPion_ );
    if( !IsUsable() )
    {
        pPion_->MagicMove( pPion_->GetAutomate().GetAlivePionsBarycenter() );
        MIL_RC::pRcANouveauDisponibleApresReparation_->Send( *pPion_, MIL_RC::eRcTypeOperational );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyHumanAdded
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyHumanAdded( PHY_Human& human )
{
    GetRole< PHY_RolePion_Humans >().NotifyHumanAdded( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyHumanRemoved
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyHumanRemoved( PHY_Human& human )
{
    GetRole< PHY_RolePion_Humans >().NotifyHumanRemoved( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyHumanChanged
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState )
{
    GetRole< PHY_RolePion_Humans >().NotifyHumanChanged( human, copyOfOldHumanState );
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
    MT_Float rSignificantVolumeFactor     = 0.;
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
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
    T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );
    for( CIT_ComposantePionVector it = composantes.begin(); it != composantes.end() && rWeightToDamage > 0; ++it )
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
void PHY_RolePion_Composantes::ApplyContamination( const MIL_NbcAgentType& nbcAgentType )
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.GetState().IsUsable() )
            composante.ApplyContamination( nbcAgentType );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyExplosion
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyExplosion( const MIL_RealObjectType& objectType, PHY_FireResults_ABC& fireResult )
{
    assert( pPion_ );

    T_ComposantePionVector composantes = composantes_;
    std::random_shuffle( composantes.begin(), composantes.end() );

    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( *pPion_ );

    for( CIT_ComposantePionVector it = composantes.begin(); it != composantes.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanBeFired() )
        {
            composante.ApplyExplosion( objectType, fireDamages );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyIndirectFire
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyIndirectFire( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& fireResult )
{
    assert( pPion_ );

    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( *pPion_ );
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanBeFired() )
            composante.ApplyIndirectFire( dotationCategory, fireDamages );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyDirectFire
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyDirectFire( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& fireResult )
{
    assert( pPion_ );
    assert( std::find( composantes_.begin(), composantes_.end(), &compTarget ) != composantes_.end() );

    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( *pPion_ );
    PHY_ComposantePion& compTargetPion = static_cast< PHY_ComposantePion& >( compTarget );

    if( compTargetPion.CanBeFired() )
        compTargetPion.ApplyDirectFire( dotationCategory, fireDamages );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::ApplyPopulationFire
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::ApplyPopulationFire( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& fireResult )
{
    assert( pPion_ );
    assert( std::find( composantes_.begin(), composantes_.end(), &compTarget ) != composantes_.end() );

    PHY_FireDamages_Agent& fireDamages = fireResult.GetDamages( *pPion_ );
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
    const uint nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        nNeutralizationEndTimeStep_ = std::max( nNeutralizationEndTimeStep_, nCurrentTimeStep + (**it).GetNeutralizationTime() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::WoundLoadedHumans
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::WoundLoadedHumans( const PHY_ComposantePion& composanteChanged, const PHY_ComposanteState& newState, PHY_FireDamages_Agent& fireDamages )
{
    if( !GetRole< PHY_RoleAction_Loading >().IsLoaded() )
        return;

    if( !composanteChanged.CanTransportHumans() )
        return;

    uint nNbrHumansCarrier = 0;
    uint nNbrHumansLoaded  = 0;
    CIT_ComposantePionVector itComp;
    for( itComp = composantes_.begin(); itComp != composantes_.end(); ++itComp )
    {
        const PHY_ComposantePion& composante = **itComp;
        if( composante.CanTransportHumans() )
            ++ nNbrHumansCarrier;
        if( composante.CanBeLoaded() )
            nNbrHumansLoaded += composante.GetNbrUsableHumans();
    }
    assert( nNbrHumansCarrier != 0 );

    uint nNbrHumansToWound = nNbrHumansLoaded / nNbrHumansCarrier;
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
// Name: PHY_RolePion_Composantes::FillDirectFireData
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::FillDirectFireData( PHY_DirectFireData& data ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanFire() )
            composante.FillDirectFireData( data );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::FillIndirectFireData
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::FillIndirectFireData( PHY_IndirectFireData& data ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanFire() )
            composante.FillIndirectFireData( data );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::FillIndirectFireData
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::FillIndirectFireData( PHY_SmokeData& data ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;
        if( composante.CanFire() )
        {
            composante.FillIndirectFireData( data );
            if( data.HasWeapon() )
                return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetComposantesAbleToBeFired
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetComposantesAbleToBeFired( T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes /*= false*/ ) const
{
    targets.clear();
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;

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
void PHY_RolePion_Composantes::GetComposantesAbleToBeFired( T_ComposanteVector& targets, uint nNbrFirer, bool bFireOnlyOnMajorComposantes /*= false*/ ) const
{
    targets.clear();
    T_ComposanteVector availableTargets;
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
// Name: PHY_RolePion_Composantes::SendFullLends
// Created: NLD 2005-01-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendFullLends( NET_ASN_MsgUnitDotations& asn ) const
{
    asn.GetAsnMsg().m.prets_equipementPresent = 1;

    typedef std::pair< const MIL_AgentPion*, const PHY_ComposanteTypePion* > T_Key;
    typedef std::map < T_Key, uint >                                         T_LentMap;
    typedef T_LentMap::const_iterator                                        CIT_LentMap;

    T_LentMap lentData;
    for( CIT_LentComposanteMap it = lentComposantes_.begin(); it != lentComposantes_.end(); ++it )
    {
        const MIL_AgentPion&          pion        = it->first->GetPion();
        const T_ComposantePionVector& composantes = it->second;
        for( CIT_ComposantePionVector itComp = composantes.begin(); itComp != composantes.end(); ++itComp )
            ++lentData[ T_Key( &pion, &(**itComp).GetType() ) ];
    }

    asn.GetAsnMsg().prets_equipement.n = lentData.size();
    if( lentData.empty() )
        return;

    ASN1T_PretEquipement* pLent = new ASN1T_PretEquipement[ lentData.size() ];
    uint i = 0;
    for( CIT_LentMap itData = lentData.begin(); itData != lentData.end(); ++itData, ++i )
    {
        ASN1T_PretEquipement& lent = pLent[ i ];

        lent.oid_pion_emprunteur = itData->first.first ->GetID();
        lent.type_equipement     = itData->first.second->GetMosID();
        lent.nombre              = itData->second;
    }

    asn.GetAsnMsg().prets_equipement.elem = pLent;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendChangedLends
// Created: NLD 2005-01-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendChangedLends( NET_ASN_MsgUnitDotations& asn ) const
{
    // Prets
    if( bLendsChanged_ )
        SendFullLends( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendChangedState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendChangedState( NET_ASN_MsgUnitDotations& asn ) const
{
    if( nNbrComposanteChanged_ == 0 )
        return;

    ASN1T_DotationEquipement* pEquipments = new ASN1T_DotationEquipement[ nNbrComposanteChanged_ ];
    uint i = 0;
    for( CIT_ComposanteTypeMap itComposanteType = composanteTypes_.begin(); itComposanteType != composanteTypes_.end(); ++itComposanteType )
    {
        const PHY_ComposanteTypePion&     compType   = *itComposanteType->first;
        const T_ComposanteTypeProperties& properties =  itComposanteType->second;

        if( !properties.bHasChanged_ )
            continue;

        ASN1T_DotationEquipement& value  = pEquipments[ i++ ];
        value.type_equipement            = compType.GetMosID();
        value.nb_disponibles             = properties.nbrsPerState_[ PHY_ComposanteState::undamaged_  .GetID() ];
        value.nb_indisponibles           = properties.nbrsPerState_[ PHY_ComposanteState::dead_       .GetID() ];
        value.nb_reparables              = properties.nbrsPerState_[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ] + properties.nbrsPerState_[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ];
        value.nb_dans_chaine_maintenance = properties.nbrsPerState_[ PHY_ComposanteState::maintenance_.GetID() ];
        value.nb_prisonniers             = properties.nbrsPerState_[ PHY_ComposanteState::prisoner_   .GetID() ];
    }

    asn.GetAsnMsg().dotation_eff_materiel.n        = nNbrComposanteChanged_;
    asn.GetAsnMsg().dotation_eff_materiel.elem     = pEquipments;
    asn.GetAsnMsg().m.dotation_eff_materielPresent = 1;

    SendChangedLends( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendFullState( NET_ASN_MsgUnitDotations& asn ) const
{
    asn.GetAsnMsg().dotation_eff_materiel.n        = composanteTypes_.size();
    asn.GetAsnMsg().m.dotation_eff_materielPresent = 1;

    if( !composanteTypes_.empty() )
    {
        ASN1T_DotationEquipement* pEquipments = new ASN1T_DotationEquipement[ composanteTypes_.size() ];
        uint i = 0;
        for( CIT_ComposanteTypeMap itComposanteType = composanteTypes_.begin(); itComposanteType != composanteTypes_.end(); ++itComposanteType, ++i )
        {
            const PHY_ComposanteTypePion&    compType   = *itComposanteType->first;
            const T_ComposanteTypeProperties& properties =  itComposanteType->second;

            ASN1T_DotationEquipement& value  = pEquipments[ i ];
            value.type_equipement            = compType.GetMosID();
            value.nb_disponibles             = properties.nbrsPerState_[ PHY_ComposanteState::undamaged_ .GetID() ];
            value.nb_indisponibles           = properties.nbrsPerState_[ PHY_ComposanteState::dead_      .GetID() ];
            value.nb_reparables              = properties.nbrsPerState_[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ] + properties.nbrsPerState_[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ];
            value.nb_dans_chaine_maintenance = properties.nbrsPerState_[ PHY_ComposanteState::maintenance_.GetID() ];
            value.nb_prisonniers             = properties.nbrsPerState_[ PHY_ComposanteState::prisoner_   .GetID() ];
        }
        asn.GetAsnMsg().dotation_eff_materiel.elem = pEquipments;
    }

    SendFullLends( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg.GetAsnMsg().m.etat_operationnel_brutPresent = 1;
    msg.GetAsnMsg().etat_operationnel_brut          = (uint)( rOperationalState_ * 100. );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( !HasChanged() )
        return;

    SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::Serialize
// Created: AGE 2004-11-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::Serialize( HLA_UpdateFunctor& functor ) const
{
    typedef std::pair< std::string, uint32 > T_Composante;
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToFireOn
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMaxRangeToFireOn( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const
{
    // Get back the most dangerous composante type of the target (from our point of view ...)
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();
    if( !pTargetComposante )
        return 0.;

    MT_Float rRange = 0;
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
    if( !pTargetComposante )
        return 0.;

    MT_Float rRange = 0;
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::max( rRange, (**it).GetOnlyLoadableMaxRangeToFireOn( *pTargetComposante, rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMinRangeToFireOn
// Created: JVT 2004-12-17
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMinRangeToFireOn( const DEC_Knowledge_Agent& target, MT_Float rWantedPH ) const
{
    // Get The most dangerous composante type of the target
    const DEC_Knowledge_AgentComposante* pTargetComposante = target.GetMajorComposante();
    if( !pTargetComposante )
        return std::numeric_limits< MT_Float >::max();

    MT_Float rRange = std::numeric_limits< MT_Float >::max();
    for(  CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
    if( !pTargetComposante )
        return 0.;

    MT_Float rRange = 0;
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
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
    if( !pTargetComposante )
        return std::numeric_limits< MT_Float >::max();

    MT_Float rRange = std::numeric_limits< MT_Float >::max();
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::min( rRange, (**it).GetMinRangeToFireOnWithPosture( *pTargetComposante, target.GetAgentKnown(), rWantedPH ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMaxRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMaxRangeToIndirectFire( const PHY_IndirectFireDotationClass& ammoClass, bool bCheckDotationsAvailability ) const
{
    MT_Float rRange = -1.;

    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::max( rRange, (*it)->GetMaxRangeToIndirectFire( ammoClass, bCheckDotationsAvailability ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetMinRangeToIndirectFire
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetMinRangeToIndirectFire( const PHY_IndirectFireDotationClass& ammoClass, bool bCheckDotationsAvailability ) const
{
    MT_Float rRange = std::numeric_limits< MT_Float >::max();

    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        rRange = std::min( rRange, (*it)->GetMinRangeToIndirectFire( ammoClass, bCheckDotationsAvailability ) );
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetDangerosity
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Composantes::GetDangerosity( const DEC_Knowledge_Agent& target ) const
{
    assert( pPion_ );

    if( pPion_->GetArmy().IsAFriend( target ) == eTristate_True
        ||  pPion_->GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        return 0.;

    // Target is dead
    const DEC_Knowledge_AgentComposante* pTargetMajorComposante = target.GetMajorComposante();
    if( !pTargetMajorComposante )
        return 0.;

    MT_Float rDangerosity = 0.;

    // Fight score
    const PHY_RolePion_Location& myLocation = GetRole< PHY_RolePion_Location >();
    const MT_Vector3D sourcePosition( myLocation.GetPosition().rX_, myLocation.GetPosition().rY_, myLocation.GetAltitude() );
    const MT_Vector3D targetPosition( target.GetPosition().rX_, target.GetPosition().rY_, target.GetAltitude() );
    const MT_Float    rDistBtwSourceAndTarget = sourcePosition.Distance( targetPosition );

    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
        rDangerosity = std::max( rDangerosity, (**itComposante).GetDangerosity( *pTargetMajorComposante, rDistBtwSourceAndTarget ) );

    // Etat opérationel
    rDangerosity *= ( 1 - ( (-rMaxDangerosityDegradationByOpState_ * rOperationalState_ ) + rMaxDangerosityDegradationByOpState_ ) );
    if( rOperationalState_ == 0. ) // L'unité est morte
        rDangerosity = 0;

    // Source is neutralized
    if( pPion_->IsNeutralized() )
        rDangerosity *= 1 - rMaxDangerosityDegradationByNeutralizedState_;

    return rDangerosity;
}

// =============================================================================
// LOGISTIC - MAINTENANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::PreprocessRandomBreakdowns( uint nEndDayTimeStep ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).PreprocessRandomBreakdowns( nEndDayTimeStep );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteWaitingForMaintenance
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState* PHY_RolePion_Composantes::NotifyComposanteWaitingForMaintenance( PHY_ComposantePion& composante )
{
    assert( pPion_ );

    MIL_AutomateLOG* pTC2 = pPion_->GetAutomate().GetTC2();
    if( !pTC2 )
        return 0;

    // Pas de RC si log non branchée ou si RC envoyé au tick précédent
    const uint nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nCurrentTick > ( nTickRcMaintenanceQuerySent_ + 1 ) || nTickRcMaintenanceQuerySent_ == 0 )
        MIL_RC::pRcDemandeEvacuationMateriel_->Send( *pPion_, MIL_RC::eRcTypeOperational );
    nTickRcMaintenanceQuerySent_ = nCurrentTick;

    PHY_MaintenanceComposanteState* pMaintenanceComposanteState = pTC2->MaintenanceHandleComposanteForTransport( *pPion_, composante );
    if( !pMaintenanceComposanteState )
        return 0;

    bool bOut = maintenanceComposanteStates_.insert( pMaintenanceComposanteState ).second;
    assert( bOut );
    return pMaintenanceComposanteState;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyComposanteBackFromMaintenance
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyComposanteBackFromMaintenance( PHY_MaintenanceComposanteState& composanteState )
{
    int nOut = maintenanceComposanteStates_.erase( &composanteState );
    assert( nOut == 1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasUsableHauler
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::HasUsableHauler( const PHY_ComposanteTypePion& composanteType ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).GetType().CanHaul( composanteType ) && (**it).GetState().IsUsable() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAvailableHauler
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Composantes::GetAvailableHauler( const PHY_ComposanteTypePion& composanteType ) const
{
    MT_Float            rScore          = std::numeric_limits< MT_Float >::max();
    PHY_ComposantePion* pSelectedHauler = 0;

    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& hauler = **it;
        if( !hauler.CanHaul( composanteType ) )
            continue;

        MT_Float rNewScore = hauler.GetType().GetHaulWeightCapacity() - composanteType.GetWeight();
        assert( rNewScore >= 0. );
        if( rNewScore < rScore )
        {
            rScore          = rNewScore;
            pSelectedHauler = &hauler;
        }
    }
    return pSelectedHauler;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasUsableRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::HasUsableRepairer( const PHY_Breakdown& breakdown ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).GetType().CanRepair( breakdown ) && (**it).GetState().IsUsable() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAvailableRepairer
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Composantes::GetAvailableRepairer( const PHY_Breakdown& breakdown ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).CanRepair( breakdown ) )
            return *it;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetHaulersUse
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetHaulersUse( T_ComposanteUseMap& composanteUse ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanHaul() )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanHaul() )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetRepairersUse
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetRepairersUse( T_ComposanteUseMap& composanteUse ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanRepair() )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanRepair() )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetRepairersUse
// Created: NLD 2005-11-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetRepairersUse( T_ComposanteUseMap& composanteUse, const PHY_Breakdown& breakdown ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanRepair( breakdown ) )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanRepair( breakdown ) )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// =============================================================================
// LOGISTIC - MEDICAL
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::HasWoundedHumansToEvacuate() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).HasWoundedHumansToEvacuate() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
        (**it).EvacuateWoundedHumans( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyHumanEvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_RolePion_Composantes::NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 )
{
    return GetRole< PHY_RolePion_Humans >().NotifyHumanEvacuatedByThirdParty( human, destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyHumanWaitingForMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_RolePion_Composantes::NotifyHumanWaitingForMedical( PHY_Human& human )
{
    return GetRole< PHY_RolePion_Humans >().NotifyHumanWaitingForMedical( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyHumanBackFromMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyHumanBackFromMedical( PHY_MedicalHumanState& humanState )
{
    GetRole< PHY_RolePion_Humans >().NotifyHumanBackFromMedical( humanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasUsableEvacuationAmbulance
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::HasUsableEvacuationAmbulance() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).GetType().CanEvacuateCasualties() && (**it).GetState().IsUsable() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAvailableEvacuationAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Composantes::GetAvailableEvacuationAmbulance() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).CanEvacuateCasualties() )
            return *it;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetEvacuationAmbulancesUse
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetEvacuationAmbulancesUse( T_ComposanteUseMap& composanteUse ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanEvacuateCasualties() )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanEvacuateCasualties() )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasUsableCollectionAmbulance
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::HasUsableCollectionAmbulance() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).GetType().CanCollectCasualties() && (**it).GetState().IsUsable() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAvailableCollectionAmbulance
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Composantes::GetAvailableCollectionAmbulance() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).CanCollectCasualties() )
            return *it;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetCollectionAmbulancesUse
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetCollectionAmbulancesUse( T_ComposanteUseMap& composanteUse ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanCollectCasualties() )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {   
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanCollectCasualties() )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAvailableDoctorForDiagnosing
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Composantes::GetAvailableDoctorForDiagnosing() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).CanDiagnoseHumans() )
            return *it;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetDoctorsUse
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetDoctorsUse( T_ComposanteUseMap& composanteUse ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanDiagnoseHumans() )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanDiagnoseHumans() )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasUsableDoctorForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::HasUsableDoctorForSorting() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).GetType().CanSortHumans() && (**it).GetState().IsUsable() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAvailableDoctorForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Composantes::GetAvailableDoctorForSorting() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).CanSortHumans() )
            return *it;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetDoctorsUseForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetDoctorsUseForSorting( T_ComposanteUseMap& composanteUse ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanSortHumans() )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanSortHumans() )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasUsableDoctorForHealing
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::HasUsableDoctorForHealing() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).GetType().CanHealHumans() && (**it).GetState().IsUsable() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::HasUsableDoctorForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Composantes::HasUsableDoctorForHealing( const PHY_Human& human ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).GetType().CanHealHuman( human ) && (**it).GetState().IsUsable() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAvailableDoctorForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Composantes::GetAvailableDoctorForHealing( const PHY_Human& human ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).CanHealHuman( human ) )
            return *it;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetDoctorsUseForHealing
// Created: NLD 2005-11-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetDoctorsUseForHealing( T_ComposanteUseMap& composanteUse, const PHY_Human& human ) const
{
     composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanHealHuman( human )  )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanHealHuman( human ) )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// =============================================================================
//  PRET DE COMPOSANTE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::LendComposante
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::LendComposante( PHY_RolePion_Composantes& newRole, PHY_ComposantePion& composante )
{
    assert( composante.CanBeLent() );
    assert( std::find( composantes_.begin(), composantes_.end(), &composante ) != composantes_.end() );

    lentComposantes_[ &newRole ].push_back( &composante );
    composante.TransfertComposante( newRole );
    bLendsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::UndoLendComposante
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::UndoLendComposante( PHY_RolePion_Composantes& role, PHY_ComposantePion& composante )
{
    T_ComposantePionVector& lentComps = lentComposantes_[ &role ];

    IT_ComposantePionVector itComp = std::find( lentComps.begin(), lentComps.end(), &composante );
    assert( itComp != lentComps.end() );

    composante.TransfertComposante( *this );

    lentComps.erase( itComp );
    if( lentComps.empty() )
        lentComposantes_.erase( lentComposantes_.find( &role ) );
    bLendsChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::LendCollectionComposantes
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
uint PHY_RolePion_Composantes::LendCollectionComposantes( PHY_RolePion_Composantes& newRole, uint nNbr )
{
    uint nNbrDone = 0;

    for( RIT_ComposantePionVector it = composantes_.rbegin(); it != composantes_.rend() && nNbrDone < nNbr ; )
    {
        PHY_ComposantePion& composante = **it;

        if( composante.CanBeLent() && composante.CanCollectCasualties() )
        {
            ++nNbrDone;
            LendComposante( newRole, composante );
            it = composantes_.rbegin(); // LendComposante->TransfertComposante modifie composantes_
        }
        else
            ++it;
    }
    return nNbrDone;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetLendCollectionComposantesTime
// Created: JVT 2005-05-12
// -----------------------------------------------------------------------------
uint PHY_RolePion_Composantes::GetLendCollectionComposantesTime( PHY_RolePion_Composantes& newRole, uint nNbr )
{
    uint nNbrDone = 0;
    uint nTime    = 0;

    const MT_Vector2D& srcPos  =         GetRole< PHY_RolePion_Location >().GetPosition();
    const MT_Vector2D& destPos = newRole.GetRole< PHY_RolePion_Location >().GetPosition();

    for( RIT_ComposantePionVector it = composantes_.rbegin(); it != composantes_.rend() && nNbrDone < nNbr; ++it )
    {
        assert( *it );

        PHY_ComposantePion& composante = **it;

        if( composante.CanBeLent() && composante.CanCollectCasualties() )
        {
            ++nNbrDone;
            nTime = std::max( nTime, composante.ApproximateTravelTime( srcPos, destPos ) );
        }
    }
    return nTime;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::UndoLendCollectionComposantes
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
uint PHY_RolePion_Composantes::UndoLendCollectionComposantes( PHY_RolePion_Composantes& role, uint nNbr )
{
    T_ComposantePionVector& lentComps = lentComposantes_[ &role ];

    const uint nNbrDone = std::min( nNbr, lentComps.size() );

    for( CIT_ComposantePionVector it = lentComps.end() - nNbrDone; it != lentComps.end(); ++it )
        (*it)->TransfertComposante( *this );

    lentComps.erase( lentComps.end() - nNbrDone, lentComps.end() );
    if( lentComps.empty() )
        lentComposantes_.erase( lentComposantes_.find( &role ) );

    if( nNbrDone > 0 )
        bLendsChanged_ = true;
    return nNbrDone;
}

// =============================================================================
// LOGISTIC - SUPPLY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAvailableConvoyTransporter
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Composantes::GetAvailableConvoyTransporter( const PHY_DotationCategory& dotationCategory ) const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        PHY_ComposantePion& composante = **it;

        if( composante.CanBePartOfConvoy() && composante.CanTransportStock( dotationCategory ) )
            return &composante;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetConvoyTransporters
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetConvoyTransporters( T_ComposanteUseMap& composanteUse ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanBePartOfConvoy()  )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanBePartOfConvoy() )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetAvailableConvoyCommander
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePion_Composantes::GetAvailableConvoyCommander() const
{
    for( CIT_ComposantePionVector it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        if( (**it).CanCommandConvoy() )
            return *it;
    }
    return 0;

}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::GetConvoyCommanders
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::GetConvoyCommanders( T_ComposanteUseMap& composanteUse ) const
{
    composanteUse.clear();
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        if( (**itComposante).GetType().CanCommandConvoy() )
        {
            T_ComposanteUse& data = composanteUse[ &(**itComposante).GetType() ];
            ++ data.nNbrTotal_;

            if( (**itComposante).GetState().IsUsable() )
            {
                ++ data.nNbrAvailable_;
                if( !(**itComposante).CanCommandConvoy() )
                    ++ data.nNbrUsed_;
            }
        }
    }
}

// =============================================================================
// PRISONERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Composantes::NotifyTakenPrisoner
// C: NLD 2005-03-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Composantes::NotifyTakenPrisoner()
{
    for( CIT_ComposantePionVector itComposante = composantes_.begin(); itComposante != composantes_.end(); ++itComposante )
    {
        PHY_ComposantePion& composante = **itComposante;
        if( composante.GetState().IsUsable() )
            composante.ReinitializeState( PHY_ComposanteState::prisoner_ );
    }
}
