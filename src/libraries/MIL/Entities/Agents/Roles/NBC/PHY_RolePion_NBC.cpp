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

#include "MIL_Pch.h"

#include "PHY_RolePion_NBC.h"

#include "Network/NET_ASN_Messages.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_NBC, "PHY_RolePion_NBC" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::PHY_RolePion_NBC( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_NBC  ( role )
    , pPion_                 ( &pion )
    , bNbcProtectionSuitWorn_( false )
    , rContaminationState_   ( 0. )
    , bHasChanged_           ( true )
{
}
 
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::PHY_RolePion_NBC()
    : bHasChanged_( true )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::~PHY_RolePion_NBC()
{
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
            file << set.size();
            for ( PHY_RolePion_NBC::CIT_NbcAgentTypeSet it = set.begin(); it != set.end(); ++it )
                file << (*it)->GetID();
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
                set.insert( MIL_NbcAgentType::FindNbcAgentType( nID ) );
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
    file & boost::serialization::base_object< PHY_RoleInterface_NBC >( *this )
         & pPion_
         & nbcAgentTypesContaminating_
         & bNbcProtectionSuitWorn_
         & rContaminationState_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Contaminate
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Contaminate( const MIL_NbcAgentType& nbcAgentType )
{
    GetRole< PHY_RoleAction_Transport >().NotifyComposanteContaminated( nbcAgentType );

    if( !nbcAgentTypesContaminating_.insert( &nbcAgentType ).second )
        return;

    rContaminationState_ = 1.;
    bHasChanged_         = true;
    if( bNbcProtectionSuitWorn_ )
        return;
   
    GetRole< PHY_RolePion_Composantes >().ApplyFire( nbcAgentType );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Decontaminate
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Decontaminate()
{
    if( nbcAgentTypesContaminating_.empty() )
        return;

    rContaminationState_ = 0.;
    bHasChanged_         = true;
    nbcAgentTypesContaminating_.clear();    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Decontaminate
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Decontaminate( MT_Float rRatioAgentsWorking )
{
    if( nbcAgentTypesContaminating_.empty() )
        return;

    assert( pPion_ );
    MT_Float rNewContaminationState = rContaminationState_ - ( pPion_->GetType().GetUnitType().GetCoefDecontaminationPerTimeStep() * rRatioAgentsWorking );
    rNewContaminationState = std::max( rNewContaminationState, 0. );

    if( (uint)( rNewContaminationState * 100. ) != ( rContaminationState_ * 100. ) )
        bHasChanged_ = true;

    rContaminationState_ = rNewContaminationState;
    if( rContaminationState_ == 0. )
    {
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

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg.GetAsnMsg().m.contamine_par_agents_nbcPresent = 1;
    msg.GetAsnMsg().contamine_par_agents_nbc.n = nbcAgentTypesContaminating_.size();
    if( !nbcAgentTypesContaminating_.empty() )
    {
        ASN1T_OID* pNbcAgents = new ASN1T_OID[ nbcAgentTypesContaminating_.size() ];
        msg.GetAsnMsg().contamine_par_agents_nbc.elem = pNbcAgents;

        uint i = 0;
        for( CIT_NbcAgentTypeSet itNbcAgent = nbcAgentTypesContaminating_.begin(); itNbcAgent != nbcAgentTypesContaminating_.end(); ++itNbcAgent )
            pNbcAgents[ i++ ] = (**itNbcAgent).GetID();
    }

    msg.GetAsnMsg().m.en_tenue_de_protection_nbcPresent = 1;
    msg.GetAsnMsg().en_tenue_de_protection_nbc          = bNbcProtectionSuitWorn_;

    msg.GetAsnMsg().m.etat_contaminationPresent = 1;
    msg.GetAsnMsg().etat_contamination          = (uint)( rContaminationState_ * 100. );
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
