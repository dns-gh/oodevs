// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RolePion_NBC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 18:15 $
// $Revision: 9 $
// $Workfile: PHY_RolePion_NBC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_NBC.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Objects/MIL_ToxicEffectManipulator.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_NBC, "PHY_RolePion_NBC" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_NBC* role, const unsigned int /*version*/ )
{
	archive << role->pPion_;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_NBC* role, const unsigned int /*version*/ )
{
	MIL_AgentPion* pion;
	archive >> pion;
	::new( role )PHY_RolePion_NBC( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::PHY_RolePion_NBC( MIL_AgentPion& pion )
    : pPion_                      ( &pion )
    , bNbcProtectionSuitWorn_     ( false )
    , nbcAgentTypesContaminating_ ()
    , rContaminationState_        ( 0. )
    , rContaminationQuantity_     ( 0. )
    , bHasChanged_                ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::~PHY_RolePion_NBC()
{
    // NOTHING
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
        void serialize( Archive& file, PHY_RolePion_NBC::T_NbcAgentTypeSet& set, const uint nVersion )
        {
            split_free( file, set, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_NBC::T_NbcAgentTypeSet& set, const uint )
        {
            unsigned size = set.size();
            file << size;
            for ( PHY_RolePion_NBC::CIT_NbcAgentTypeSet it = set.begin(); it != set.end(); ++it )
            {
                unsigned id = (*it)->GetID();
                file << id;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_RolePion_NBC::T_NbcAgentTypeSet& set, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                uint nID;
                file >> nID;
                set.insert( MIL_NbcAgentType::Find( nID ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_NBC::serialize( Archive& file, const uint )
{
    file & nbcAgentTypesContaminating_
         & bNbcProtectionSuitWorn_
         & rContaminationState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Poison
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Poison( const MIL_ToxicEffectManipulator& contamination )
{
    if( bNbcProtectionSuitWorn_ )
        return;

    pPion_->GetRole< PHY_RoleInterface_Composantes >().ApplyPoisonous( contamination );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Contaminate
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Contaminate( const MIL_ToxicEffectManipulator& contamination )
{
	if( contamination.GetQuantity() < 1e-15 ) // TODO
		return;
    
    pPion_->GetRole< PHY_RoleAction_Transport >().NotifyComposanteContaminated( contamination );
    if( ! bNbcProtectionSuitWorn_ )
        pPion_->GetRole< PHY_RoleInterface_Composantes >().ApplyContamination( contamination );

    nbcAgentTypesContaminating_.insert( &contamination.GetType() );
    
    rContaminationQuantity_ += contamination.GetQuantity();
    rContaminationState_ = 1.;

    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Decontaminate
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Decontaminate()
{
    if( rContaminationState_ == 0. )
    {
        assert( nbcAgentTypesContaminating_.empty() );
        return;
    }
    
    rContaminationState_ = 0.;
    rContaminationQuantity_ = 0.;
    bHasChanged_         = true;
    nbcAgentTypesContaminating_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Decontaminate
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Decontaminate( MT_Float rRatioAgentsWorking )
{
    if( rContaminationState_ == 0. )
    {
        assert( nbcAgentTypesContaminating_.empty() );
        return;
    }

    assert( pPion_ );
    MT_Float rNewContaminationState = rContaminationState_ - ( pPion_->GetType().GetUnitType().GetCoefDecontaminationPerTimeStep() * rRatioAgentsWorking );
    rNewContaminationState = std::max( rNewContaminationState, 0. );

    if( (uint)( rNewContaminationState * 100. ) != ( rContaminationState_ * 100. ) )
        bHasChanged_ = true;
    
    rContaminationState_ = rNewContaminationState;
    rContaminationQuantity_ = std::max( 0., rContaminationQuantity_ * rContaminationState_ );
    if( rContaminationState_ == 0. )
    {
        rContaminationQuantity_ = 0.;
        nbcAgentTypesContaminating_.clear();
        bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::ModifyMaxSpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_NBC::ModifyMaxSpeed( MT_Float rSpeed ) const
{
    if( bNbcProtectionSuitWorn_ )
        return rSpeed * MIL_NbcAgentType::GetCoefMaxSpeedModificator();
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::ModifyReloadingDuration
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_NBC::ModifyReloadingDuration( MT_Float rDuration ) const
{
    if( bNbcProtectionSuitWorn_ )
        return rDuration * MIL_NbcAgentType::GetCoefReloadingTimeModificator();
    return rDuration;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg().m.contamine_par_agents_nbcPresent = 1;
    msg().contamine_par_agents_nbc.n = nbcAgentTypesContaminating_.size();
    if( !nbcAgentTypesContaminating_.empty() )
    {
        ASN1T_OID* pNbcAgents = new ASN1T_OID[ nbcAgentTypesContaminating_.size() ];
        msg().contamine_par_agents_nbc.elem = pNbcAgents;

        uint i = 0;
        for( CIT_NbcAgentTypeSet itNbcAgent = nbcAgentTypesContaminating_.begin(); itNbcAgent != nbcAgentTypesContaminating_.end(); ++itNbcAgent )
            pNbcAgents[ i++ ] = (**itNbcAgent).GetID();
    }

    msg().m.en_tenue_de_protection_nbcPresent = 1;
    msg().en_tenue_de_protection_nbc          = bNbcProtectionSuitWorn_;

    msg().m.etat_contaminationPresent = 1;
    msg().etat_contamination.percentage = (uint)( rContaminationState_ * 100. );
    msg().etat_contamination.quantity = rContaminationQuantity_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitCanBeContaminated::IsContaminated
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
bool PHY_RolePion_NBC::IsContaminated() const
{
    return !nbcAgentTypesContaminating_.empty();    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::WearProtectionSuit
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::WearNbcProtectionSuit()
{
    if( bNbcProtectionSuitWorn_ )
        return;
    bNbcProtectionSuitWorn_ = true;      
    bHasChanged_            = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::RemoveProtectionSuit
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::RemoveNbcProtectionSuit()
{
    if( !bNbcProtectionSuitWorn_ )
        return;
    bNbcProtectionSuitWorn_ = false;    
    bHasChanged_            = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_NBC::HasChanged() const
{
    return bHasChanged_;
}
