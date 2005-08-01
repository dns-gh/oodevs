// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.cpp $
// $Author: Nld $
// $Modtime: 22/06/05 18:30 $
// $Revision: 6 $
// $Workfile: PHY_RoleAction_Transport.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_RoleAction_Transport.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_Transport, "PHY_RoleAction_Transport" )
BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_Transport::sTransportData, "PHY_RoleAction_Transport::sTransportData" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::sTransportData
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::sTransportData::sTransportData()
    : bTransportOnlyLoadable_( false )
    , rTotalWeight_          ( 0. )
    , rRemainingWeight_      ( 0. )
    , rTransportedWeight_    ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::sTransportData
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::sTransportData::sTransportData( const sTransportData& rhs )
    : bTransportOnlyLoadable_( rhs.bTransportOnlyLoadable_ )
    , rTotalWeight_          ( rhs.rTotalWeight_           )
    , rRemainingWeight_      ( rhs.rRemainingWeight_       )
    , rTransportedWeight_    ( rhs.rTransportedWeight_     )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::sTransportData
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::sTransportData::sTransportData( MT_Float rTotalWeight, bool bTransportOnlyLoadable )
    : bTransportOnlyLoadable_( bTransportOnlyLoadable )
    , rTotalWeight_          ( rTotalWeight )
    , rRemainingWeight_      ( rTotalWeight )
    , rTransportedWeight_    ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::sTransportData::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Transport::sTransportData::serialize( Archive& file, const uint )
{
    file & bTransportOnlyLoadable_
         & rTotalWeight_
         & rRemainingWeight_
         & rTransportedWeight_;
}

// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport constructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::PHY_RoleAction_Transport( MT_RoleContainer& role, MIL_AgentPion& pion )
    : MT_Role_ABC               ( role )
    , pTransporter_             ( &pion )
    , nState_                   ( eNothing )
    , bLoadUnloadHasBeenUpdated_( false )
    , rWeightTransported_       ( 0. )
    , bHasChanged_              ( true )
    , transportedPions_         ()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::PHY_RoleAction_Transport()
    : MT_Role_ABC               ()
    , pTransporter_             ( 0 )
    , nState_                   ( eNothing )
    , bLoadUnloadHasBeenUpdated_( false )
    , rWeightTransported_       ( 0. )
    , bHasChanged_              ( true )
    , transportedPions_         ()

{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport destructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Transport::~PHY_RoleAction_Transport()
{
    Cancel();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Transport::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this )
         & const_cast< MIL_AgentPion*& >( pTransporter_ )
         & rWeightTransported_
         & transportedPions_;
}


// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::GetTransportWeightCapacity
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
struct sTransporterData
{
    sTransporterData( const MIL_AgentPion& transporter)
        : rWeightCapacity_                ( 0. )
        , rWeightLoadedPerTimeStep_       ( 0. )
        , rWeightUnloadedPerTimeStep_     ( 0. )
        , rMaxComposanteTransportedWeight_( 0. )
    {
        transporter.GetRole< PHY_RolePion_Composantes >().Apply( *this );
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if( composante.CanTransport() )
        {
            const PHY_ComposanteTypePion& compType = composante.GetType();
            rWeightCapacity_                 += compType.GetTransportWeightCapacity           ();
            rWeightLoadedPerTimeStep_        += compType.GetTransportWeightLoadedPerTimeStep  ();
            rWeightUnloadedPerTimeStep_      += compType.GetTransportWeightUnloadedPerTimeStep();
            rMaxComposanteTransportedWeight_  = std::max( rMaxComposanteTransportedWeight_, compType.GetTransportWeightCapacity() );
        }
    }

    MT_Float rWeightCapacity_;
    MT_Float rWeightLoadedPerTimeStep_;
    MT_Float rWeightUnloadedPerTimeStep_;
    MT_Float rMaxComposanteTransportedWeight_;
};

// -----------------------------------------------------------------------------
// Name: sTransportedData
// Created: JVT 2005-02-01
// -----------------------------------------------------------------------------
struct sTransportedData
{
    sTransportedData( const MIL_AgentPion& transported, const bool bTransportOnlyLoadable )
        : rTotalWeight_             ( 0. )
        , rHeaviestComposanteWeight_( 0. )
        , bTransportOnlyLoadable_   ( bTransportOnlyLoadable )
    {
        transported.GetRole< PHY_RolePion_Composantes >().Apply( *this );
    }

    void operator() ( const PHY_ComposantePion& composante )
    {
        if(  composante.CanBeTransported() && ( !bTransportOnlyLoadable_ || composante.CanBeLoaded() ) )
        {
            rTotalWeight_             += composante.GetWeight();
            rHeaviestComposanteWeight_ = std::max( rHeaviestComposanteWeight_, composante.GetWeight() );
        }
    }

          MT_Float rTotalWeight_;
          MT_Float rHeaviestComposanteWeight_;
    const bool     bTransportOnlyLoadable_;

    MT_COPYNOTALLOWED( sTransportedData );
};

// =============================================================================
// LOADING
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Load
// Created: NLD 2004-11-18
// -----------------------------------------------------------------------------
int PHY_RoleAction_Transport::Load()
{
    bLoadUnloadHasBeenUpdated_ = true;
    if( nState_ == eNothing )
        nState_ = eLoading;

    if( nState_ == eLoading )
    {
        sTransporterData func( *pTransporter_ );
        if( func.rWeightLoadedPerTimeStep_ == 0. )
            return eErrorNoCarriers;

        const MT_Float rWeightToLoad = std::min( func.rWeightLoadedPerTimeStep_, func.rWeightCapacity_ - rWeightTransported_ );
        const MT_Float rWeightLoaded = DoLoad( rWeightToLoad );
        rWeightTransported_ += rWeightLoaded;
        if( rWeightLoaded == 0 || rWeightTransported_ >= func.rWeightCapacity_ )
        {
            nState_ = eNothing;
            return eFinished;
        }
        return eRunning;
    }
    return eImpossible;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::DoLoad
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Transport::DoLoad( const MT_Float rWeightToLoad )
{
    MT_Float rWeightLoaded = 0.;

    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightLoaded < rWeightToLoad; ++it )
    {
        MIL_AgentPion&  pion          = *it->first;
        sTransportData& transportData =  it->second;

        if( it->second.rRemainingWeight_ <= 0. )
            continue;

        if( it->second.rTransportedWeight_ <= 0. )
        {
            pion.GetRole< PHY_RolePion_Transported >().LoadForTransport( *pTransporter_, transportData.bTransportOnlyLoadable_ ); // Filer position embarquement si bTransportOnlyLoadable_  + transporteur
            bHasChanged_ = true;
        }

        const MT_Float rTmpWeight = std::min( rWeightToLoad - rWeightLoaded, it->second.rRemainingWeight_ );
        it->second.rTransportedWeight_ += rTmpWeight;
        it->second.rRemainingWeight_   -= rTmpWeight;
        rWeightLoaded                  += rTmpWeight;
    }
    return rWeightLoaded;
}

// =============================================================================
// UNLOADING
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::Unload
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Transport::Unload()
{
    bLoadUnloadHasBeenUpdated_ = true;
    if( nState_ == eNothing )
        nState_ = eUnloading;

    if( nState_ == eUnloading )
    {
        sTransporterData func( *pTransporter_ );
        if( func.rWeightUnloadedPerTimeStep_ == 0. )
            return eErrorNoCarriers;

        const MT_Float rWeightUnloaded = DoUnload( func.rWeightUnloadedPerTimeStep_ );
        rWeightTransported_ -= rWeightUnloaded;
        if( rWeightUnloaded == 0. || rWeightTransported_ <= 0. )
        {
            rWeightTransported_ = 0.;
            nState_ = eNothing;
            return eFinished;
        }
        return eRunning;
    }
    return eImpossible;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::DoUnload
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Transport::DoUnload( const MT_Float rWeightToUnload )
{
    MT_Float rWeightUnloaded = 0.;
    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightUnloaded < rWeightToUnload ; )
    {
        if( it->second.rTransportedWeight_ <= 0. )
        {
            ++it;
            continue;
        }

        const MT_Float rTmpWeight = std::min( rWeightToUnload - rWeightUnloaded, it->second.rTransportedWeight_ );
        it->second.rTransportedWeight_ -= rTmpWeight;
        rWeightUnloaded                += rTmpWeight;
        if( it->second.rTransportedWeight_ <= 0. && it->second.rRemainingWeight_ <= 0. )
        {
            bHasChanged_ = true;
            MIL_AgentPion& pion = *it->first;
            pion.GetRole< PHY_RolePion_Transported >().UnloadFromTransport( it->second.bTransportOnlyLoadable_ );
            it = transportedPions_.erase( it );
        }
        else
            ++it;
    }
    return rWeightUnloaded;
}

// =============================================================================
// TRANSPORTER STATE MODIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::NotifyComposanteChanged
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::NotifyComposanteChanged( const PHY_ComposantePion& composante )
{
    if( !composante.GetState().IsDamaged() )
        return;

    MT_Float rWeightDamaged = std::min( composante.GetType().GetTransportWeightCapacity(), rWeightTransported_ );
    if( rWeightDamaged == 0 )
        return;

    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end() && rWeightDamaged > 0.; ++it )
    {
        if( it->second.rTransportedWeight_ )
        {
            const MT_Float rTmpWeight = std::min( rWeightDamaged, it->second.rTransportedWeight_ );
            rWeightDamaged -= rTmpWeight; 
            (*it->first).GetRole< PHY_RolePion_Composantes >().DamageTransported( it->second.rTransportedWeight_,composante.GetState(), it->second.bTransportOnlyLoadable_ );

            if( !composante.GetState().IsUsable() )
            {
                rWeightTransported_           -= rTmpWeight;
                it->second.rTransportedWeight_ = 0.; 
            }
        }
    }       
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::NotifyComposanteContaminated
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::NotifyComposanteContaminated( const MIL_NbcAgentType& nbcAgentType )
{
    MT_Float rWeightDamaged = rWeightTransported_;
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
// Name: PHY_RoleAction_Transport::Cancel
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::Cancel()
{
    for( IT_TransportedPionMap it = transportedPions_.begin(); it != transportedPions_.end(); ++it)
    {
        it->first->GetRole< PHY_RolePion_Transported >().CancelTransport();
        it->second.rTransportedWeight_ = 0;
    }
    transportedPions_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::CheckConsistency
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::CheckConsistency()
{
    sTransporterData func( *pTransporter_ );
    if( func.rWeightCapacity_ == 0. )
        Cancel();
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::AddPion
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::AddPion( MIL_AgentPion& transported, bool bTransportOnlyLoadable )
{
    if(   transported == *pTransporter_
        || transported.GetRole< PHY_RolePion_Transported >().IsTransported()
        || transportedPions_.find( &transported ) != transportedPions_.end() )
        return false;
   
    sTransportedData transportedData( transported, bTransportOnlyLoadable );
    if( transportedData.rTotalWeight_ <= 0. )
        return false;

    sTransporterData transporterData( *pTransporter_ );
    if( !bTransportOnlyLoadable && transportedData.rHeaviestComposanteWeight_ > transporterData.rMaxComposanteTransportedWeight_ )
        return false;

    transportedPions_[ &transported ].sTransportData::sTransportData( transportedData.rTotalWeight_, bTransportOnlyLoadable );
    return true;
}       

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::MagicLoadPion
// Created: NLD 2005-04-18
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::MagicLoadPion( MIL_AgentPion& transported, bool bTransportOnlyLoadable )
{
    if(   transported == *pTransporter_
        || transported.GetRole< PHY_RolePion_Transported >().IsTransported()
        || transportedPions_.find( &transported ) != transportedPions_.end() )
        return false;

    sTransportedData transportedData( transported, bTransportOnlyLoadable );

    sTransportData& data = transportedPions_[ &transported ].sTransportData::sTransportData( transportedData.rTotalWeight_, bTransportOnlyLoadable );
    data.rRemainingWeight_   = 0.;
    data.rTransportedWeight_ = data.rTotalWeight_;
    rWeightTransported_ += data.rTotalWeight_;

    transported.GetRole< PHY_RolePion_Transported >().LoadForTransport( *pTransporter_, bTransportOnlyLoadable ); // Filer position embarquement si bTransportOnlyLoadable_  + transporteur
    bHasChanged_ = true;

    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::MagicUnloadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::MagicUnloadPion( MIL_AgentPion& transported )
{
    IT_TransportedPionMap it = transportedPions_.find( &transported );
    if( it == transportedPions_.end() )
        return false;

    transported.GetRole< PHY_RolePion_Transported >().UnloadFromTransport( it->second.bTransportOnlyLoadable_ );

    assert( rWeightTransported_ >= it->second.rTransportedWeight_ );
    rWeightTransported_ -= it->second.rTransportedWeight_;
    transportedPions_.erase( it );
    bHasChanged_ = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::CanTransportPion
// Created: JVT 2005-02-01
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Transport::CanTransportPion( const MIL_AgentPion& transported, bool bTransportOnlyLoadable ) const
{
    assert( pTransporter_ );
    if( *pTransporter_ == transported )
        return false;

    sTransportedData transportedData( transported, bTransportOnlyLoadable );
    if( transportedData.rTotalWeight_ <= 0. )
        return false;

    sTransporterData transporterData( *pTransporter_ );
    if( !bTransportOnlyLoadable && transportedData.rHeaviestComposanteWeight_ > transporterData.rMaxComposanteTransportedWeight_ )
        return false;
    return true;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::SendFullState
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
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
        if( it->second.rTransportedWeight_ > 0. )
        {            
            pTransports[ nNbrTransported ] = (*it->first).GetID();
            ++nNbrTransported;
        }
    }
    msg.GetAsnMsg().pions_transportes.n    = nNbrTransported;
    msg.GetAsnMsg().pions_transportes.elem = pTransports;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Transport::SendChangedState
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Transport::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}