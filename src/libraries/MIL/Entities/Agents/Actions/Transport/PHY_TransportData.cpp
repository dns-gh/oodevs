// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Transport/PHY_TransportData.cpp $
// $Author: Jvt $
// $Modtime: 5/04/05 15:36 $
// $Revision: 9 $
// $Workfile: PHY_TransportData.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_TransportData.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::sTransportWeights::sTransportWeights
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
PHY_TransportData::sTransportWeights::sTransportWeights()
    : rRemainingWeight_  ( 0. )
    , rTotalWeight_      ( 0. )
    , rTransportedWeight_( 0. )
{
}
 
// -----------------------------------------------------------------------------
// Name: PHY_TransportData::sTransportWeights::sTransportWeights
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
PHY_TransportData::sTransportWeights::sTransportWeights( const sTransportWeights& rhs )
    : rRemainingWeight_  ( rhs.rRemainingWeight_   )
    , rTotalWeight_      ( rhs.rTotalWeight_       )
    , rTransportedWeight_( rhs.rTransportedWeight_ )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::sTransportWeights
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
PHY_TransportData::sTransportWeights::sTransportWeights( MT_Float rTotalWeight )
    : rRemainingWeight_  ( rTotalWeight )
    , rTotalWeight_      ( rTotalWeight )
    , rTransportedWeight_( 0. )
{

}


// -----------------------------------------------------------------------------
// Name: sMaxTransportedWeightForAComposante
// Created: JVT 2005-02-01
// -----------------------------------------------------------------------------
struct sMaxTransportedWeightForAComposante
{
    sMaxTransportedWeightForAComposante( const MIL_AgentPion& transporter, bool bTransportOnlyLoadable )
        : rMaxTransportedWeightForAComposante_( bTransportOnlyLoadable ? std::numeric_limits< MT_Float >::max() : 0. )
    {
        if ( !bTransportOnlyLoadable )
            transporter.GetRole< PHY_RolePion_Composantes >().Apply( *this );
    }
    
    void operator () ( const PHY_ComposantePion& composante )
    {
        if ( composante.CanTransport() )
            rMaxTransportedWeightForAComposante_ = std::max( rMaxTransportedWeightForAComposante_, composante.GetType().GetTransportWeightCapacity() );
    }
    
    MT_Float rMaxTransportedWeightForAComposante_;

    MT_COPYNOTALLOWED( sMaxTransportedWeightForAComposante );
};


// -----------------------------------------------------------------------------
// Name: sGetWeight
// Created: JVT 2005-02-01
// -----------------------------------------------------------------------------
struct sGetWeight
{
    sGetWeight( const MIL_AgentPion& pion, const bool bTransportOnlyLoadable )
        : rTotalWeight_             ( 0. )
        , rHeaviestComposanteWeight_( 0. )
        , bTransportOnlyLoadable_   ( bTransportOnlyLoadable )
    {
        pion.GetRole< PHY_RolePion_Composantes >().Apply( *this );   
    }
    
    void operator () ( const PHY_ComposantePion& composante )
    {
        if ( composante.CanBeTransported() && ( !bTransportOnlyLoadable_ || composante.CanBeLoaded() ) )
        {
            rTotalWeight_             += composante.GetWeight();
            rHeaviestComposanteWeight_ = std::max( rHeaviestComposanteWeight_, composante.GetWeight() );
        }
    }

          MT_Float rTotalWeight_;
          MT_Float rHeaviestComposanteWeight_;
    const bool     bTransportOnlyLoadable_;
    
    MT_COPYNOTALLOWED( sGetWeight );
};

// -----------------------------------------------------------------------------
// Name: PHY_TransportData constructor
// Created: NLD 2004-11-18
// Modified: JVT 2005-02-01
// -----------------------------------------------------------------------------
PHY_TransportData::PHY_TransportData( const MIL_AgentPion& transporter, DIA_Call_ABC& diaCall )
    : bTransportOnlyLoadable_( diaCall.GetParameters().GetParameters().size() > 2 ? diaCall.GetParameter( 2 ).ToBool() : false )
    , rWeightToLoad_         ( 0. )
    , diaReturnCode_         ( diaCall.GetParameter( 0 ) )
{
    assert( DEC_Tools::CheckTypeListePions( diaCall.GetParameter( 1 ) ) );

    sMaxTransportedWeightForAComposante weightPolicy( transporter, bTransportOnlyLoadable_ );

    T_ObjectVector pions = diaCall.GetParameter( 1 ).ToSelection();
    for( CIT_ObjectVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {   
        MIL_AgentPion& pion = static_cast< DEC_RolePion_Decision* >( *itPion )->GetPion();
        if( pion != transporter && !pion.GetRole< PHY_RolePion_Transported >().IsTransported() )
        {
            sGetWeight weightsToTransport( pion, bTransportOnlyLoadable_ );
        
            if ( weightsToTransport.rTotalWeight_ > 0. && weightsToTransport.rHeaviestComposanteWeight_ <= weightPolicy.rMaxTransportedWeightForAComposante_ )
                transportedPions_[ &pion ].sTransportWeights::sTransportWeights( weightsToTransport.rTotalWeight_ );
        }
    }

    if( transportedPions_.empty() )
        diaReturnCode_.SetValue( eImpossible );
    else
        diaReturnCode_.SetValue( eRunning );
}

// -----------------------------------------------------------------------------
// Name: PHY_TransportData destructor
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
PHY_TransportData::~PHY_TransportData()
{
    Cancel();
    diaReturnCode_.SetValue( eFinished );
}

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::IsPionTransportable
// Created: JVT 2005-02-01
// -----------------------------------------------------------------------------
bool PHY_TransportData::IsPionTransportable( const MIL_AgentPion& transporter, const MIL_AgentPion& toTransport, bool bTransportOnlyLoadable )
{
    if ( transporter == toTransport )
        return false;

    sMaxTransportedWeightForAComposante weightPolicy      ( transporter, bTransportOnlyLoadable );
    sGetWeight                          weightsToTransport( toTransport, bTransportOnlyLoadable );
        
    return weightsToTransport.rTotalWeight_ > 0. && weightsToTransport.rHeaviestComposanteWeight_ <= weightPolicy.rMaxTransportedWeightForAComposante_;
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::NotifyLoaded
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
MT_Float PHY_TransportData::NotifyLoaded( const MIL_Agent_ABC& transporter )
{
    const MT_Float rWeightLoaded = rWeightToLoad_;
    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightToLoad_ ; ++it )
    {
        MIL_AgentPion& pion = *it->first;
        pion.GetRole< PHY_RolePion_Transported >().LoadForTransport( transporter, bTransportOnlyLoadable_ ); // Filer position embarquement si bTransportOnlyLoadable_  + transporteur
        
        const MT_Float rTmpWeight = std::min( rWeightToLoad_, it->second.rRemainingWeight_ );
        it->second.rTransportedWeight_  = rTmpWeight;
        it->second.rRemainingWeight_   -= rTmpWeight;

        rWeightToLoad_ -= rTmpWeight;
    }
    return rWeightLoaded - rWeightToLoad_;
}

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::NotifyUnloaded
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_TransportData::NotifyUnloaded()
{
    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() ; )
    {
        if( it->second.rRemainingWeight_ == 0. )
        {
            MIL_AgentPion& pion = *it->first;
            pion.GetRole< PHY_RolePion_Transported >().UnloadFromTransport( bTransportOnlyLoadable_ ); 
            it = transportedPions_.erase( it );
        }
        else 
        {
            it->second.rTransportedWeight_ = 0.;
            ++it;
        }
    }

    if( transportedPions_.empty() )
        diaReturnCode_.SetValue( eFinished );
}

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::Damage
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
void PHY_TransportData::Damage( MT_Float rWeightDamaged, const PHY_ComposanteState& newState )
{
    for( CIT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightDamaged > 0; ++it )
    {
        if( it->second.rTransportedWeight_ )
        {
            rWeightDamaged -= it->second.rTransportedWeight_;
            (*it->first).GetRole< PHY_RolePion_Composantes >().DamageTransported( it->second.rTransportedWeight_, newState, bTransportOnlyLoadable_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::Contaminate
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_TransportData::Contaminate( MT_Float rWeightDamaged, const MIL_NbcAgentType& nbcAgentType )
{
    for( CIT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightDamaged > 0; ++it )
    {
        if( it->second.rTransportedWeight_ )
        {
            rWeightDamaged -= it->second.rTransportedWeight_;
            (*it->first).GetRole< PHY_RolePion_NBC >().Contaminate( nbcAgentType );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::Cancel
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_TransportData::Cancel()
{
    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end(); ++it)
    {
        it->first->GetRole< PHY_RolePion_Transported >().CancelTransport();
        it->second.rTransportedWeight_ = 0;
    }
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_TransportData::SendFullState
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
void PHY_TransportData::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg.GetAsnMsg().m.pions_transportesPresent = 1;

    if( transportedPions_.empty() )
    {
        msg.GetAsnMsg().pions_transportes.n = 0;
        return;
    }

    ASN1T_OID* pTransports = new ASN1T_OID[ transportedPions_.size() ];
    uint nNbrTransported = 0;
    for( CIT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end(); ++it )
    {
        if( it->second.rTransportedWeight_ )
        {            
            pTransports[ nNbrTransported ] = (*it->first).GetID();
            ++nNbrTransported;
        }
    }

    msg.GetAsnMsg().pions_transportes.n    = nNbrTransported;
    msg.GetAsnMsg().pions_transportes.elem = pTransports;
}
