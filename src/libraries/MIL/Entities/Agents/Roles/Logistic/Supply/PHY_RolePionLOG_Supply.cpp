// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOG_Supply.cpp $
// $Author: Nld $
// $Modtime: 13/04/05 14:16 $
// $Revision: 9 $
// $Workfile: PHY_RolePionLOG_Supply.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RolePionLOG_Supply.h"

#include "PHY_SupplyDotationConsign.h"
#include "PHY_SupplyDotationRequest.h"
#include "PHY_SupplyDotationRequestContainer.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStockContainer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/RC/MIL_RC_SeuilLogistiqueStockDepasse.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePionLOG_Supply, "PHY_RolePionLOG_Supply" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply constructor
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Supply::PHY_RolePionLOG_Supply( MT_RoleContainer& role, MIL_AgentPionLOG_ABC& pion )
    : PHY_RolePion_Supply( role )
    , pPion_             ( &pion )
    , bSystemEnabled_    ( false )
    , bHasChanged_       ( true )
    , pStocks_           ( 0 )
{
    pStocks_ = new PHY_DotationStockContainer( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Supply::PHY_RolePionLOG_Supply()
    : PHY_RolePion_Supply()
    , pPion_             ()
    , bSystemEnabled_    ( false )
    , bHasChanged_       ( true )
    , pStocks_           ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply destructor
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Supply::~PHY_RolePionLOG_Supply()
{
    delete pStocks_;
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePionLOG_Supply::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RolePion_Supply >( *this )
         & pPion_
         & pStocks_;
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::ReadOverloading( MIL_InputArchive& archive )
{
    assert( pStocks_ );
    pStocks_->ReadValues( archive );
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetAvailableConvoyTransporter
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Supply::GetAvailableConvoyTransporter( const PHY_DotationCategory& dotationCategory ) const
{
    if( !bSystemEnabled_ )
        return 0;
    return GetRole< PHY_RolePion_Composantes >().GetAvailableConvoyTransporter( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetAvailableConvoyCommander
// Created: NLD 2005-02-07
// -----------------------------------------------------------------------------
PHY_ComposantePion* PHY_RolePionLOG_Supply::GetAvailableConvoyCommander() const
{
    if( !bSystemEnabled_ )
        return 0;
    return GetRole< PHY_RolePion_Composantes >().GetAvailableConvoyCommander();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetStockAvailablity
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MT_Float PHY_RolePionLOG_Supply::GetStockAvailablity( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue ) const
{
    assert( pPion_ );
    
    if( !bSystemEnabled_ && !pPion_->IsDead() ) // <== Stock à terre quand pion mort = libre service
        return 0.;
    return std::min( GetStockValue( dotationCategory ), rRequestedValue );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::AddStockReservation
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
MT_Float PHY_RolePionLOG_Supply::AddStockReservation( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue )
{
    assert( pStocks_ );
    return pStocks_->AddReservation( dotationCategory, rRequestedValue );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::RemoveStockReservation
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::RemoveStockReservation( const PHY_DotationCategory& dotationCategory, MT_Float rRequestedValue )
{
    assert( pStocks_ );
    return pStocks_->RemoveReservation( dotationCategory, rRequestedValue );
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::StartUsingForLogistic
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::StartUsingForLogistic( PHY_ComposantePion& composante )
{
    bHasChanged_ = true;
    composante.StartUsingForLogistic();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::StopUsingForLogistic
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::StopUsingForLogistic( PHY_ComposantePion& composante )
{
    bHasChanged_ = true;
    composante.StopUsingForLogistic();
}

// =============================================================================
// STOCK SUPPLY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::NotifySupplyNeeded
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const
{
    assert( pPion_ );   
    
    if( bNewNeed )
        MIL_RC::pRcSeuilLogistiqueStockDepasse_->Send( *pPion_, MIL_RC::eRcTypeOperational, dotationCategory );

    pPion_->GetAutomate().NotifyStockSupplyNeeded( dotationCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::FillSupplyRequest
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::FillSupplyRequest( PHY_SupplyStockRequestContainer& supplyRequest ) const
{
    assert( pStocks_ );
    pStocks_->FillSupplyRequest( supplyRequest );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::ResupplyStocks
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::ResupplyStocks()
{
    assert( pStocks_ );
    pStocks_->Resupply();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::ResupplyStocks
// Created: SBO 2005-12-12
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::ResupplyStocks( const PHY_DotationCategory& category, MT_Float rNbr )
{
    assert( pStocks_ );
    pStocks_->Resupply( category, rNbr );
}

// =============================================================================
// STOCK MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetStockValue
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
MT_Float PHY_RolePionLOG_Supply::GetStockValue( const PHY_DotationCategory& category ) const
{
    assert( pStocks_ );
    return pStocks_->GetValue( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetStock
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_RolePionLOG_Supply::GetStock( const PHY_DotationCategory& category ) const
{
    assert( pStocks_ );
    return pStocks_->GetStock( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::CanContainStock
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
bool PHY_RolePionLOG_Supply::CanContainStock( const PHY_DotationCategory& category ) const
{
    assert( pStocks_ );
    return pStocks_->GetStock( category ) != 0;
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::Update
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::UpdateLogistic( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::Clean
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::Clean()
{
    bHasChanged_ = false;
    assert( pStocks_ );
    pStocks_->Clean();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SendComposanteUse
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
static
void SendComposanteUse( const PHY_RolePion_Composantes::T_ComposanteUseMap& data, ASN1T__SeqOfRavitaillementDisponibiliteMoyens& asn )
{
    asn.n = data.size();
    if( data.empty() )
        return;

    ASN1T_RavitaillementDisponibiliteMoyens* pData = new ASN1T_RavitaillementDisponibiliteMoyens[ data.size() ];
    uint i = 0;
    for( PHY_RolePion_Composantes::CIT_ComposanteUseMap itData = data.begin(); itData != data.end(); ++itData )
    {
        ASN1T_RavitaillementDisponibiliteMoyens& data = pData[ i++ ];
        data.type_equipement           = itData->first->GetMosID();
        assert( itData->second.nNbrTotal_ );
        const uint nNbrUsable = itData->second.nNbrAvailable_ - itData->second.nNbrUsed_;
        data.pourcentage_disponibilite = (uint)( nNbrUsable * 100. / itData->second.nNbrTotal_ );
    }
    asn.elem = pData;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::SendFullState() const
{
    NET_ASN_MsgLogRavitaillementEtat asn;

    asn.GetAsnMsg().m.chaine_activeePresent                       = 1;
    asn.GetAsnMsg().m.disponibilites_chefs_convoisPresent         = 1;
    asn.GetAsnMsg().m.disponibilites_transporteurs_convoisPresent = 1;

    assert( pPion_ );
    asn.GetAsnMsg().oid_pion        = pPion_->GetID();
    asn.GetAsnMsg().chaine_activee  = bSystemEnabled_;
  
    PHY_RolePion_Composantes::T_ComposanteUseMap composanteUse;
    GetRole< PHY_RolePion_Composantes >().GetConvoyTransporters( composanteUse );
    SendComposanteUse( composanteUse, asn.GetAsnMsg().disponibilites_transporteurs_convois  );

    composanteUse.clear();
    GetRole< PHY_RolePion_Composantes >().GetConvoyCommanders( composanteUse );
    SendComposanteUse( composanteUse, asn.GetAsnMsg().disponibilites_chefs_convois );

    assert( pStocks_ );
    pStocks_->SendFullState( asn );

    asn.Send();

    if( asn.GetAsnMsg().disponibilites_chefs_convois.n > 0 )
        delete [] asn.GetAsnMsg().disponibilites_chefs_convois.elem;
    if( asn.GetAsnMsg().disponibilites_transporteurs_convois.n > 0 )
        delete [] asn.GetAsnMsg().disponibilites_transporteurs_convois.elem;
    if( asn.GetAsnMsg().stocks.n > 0 )
        delete [] asn.GetAsnMsg().stocks.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::SendChangedState() const
{
    assert( pStocks_ );    
    if( !( bHasChanged_ || GetRole< PHY_RolePion_Composantes >().HasChanged() || pStocks_->HasChanged() ) )
        return;

    NET_ASN_MsgLogRavitaillementEtat asn;
    assert( pPion_ );
    asn.GetAsnMsg().oid_pion = pPion_->GetID();

    if( bHasChanged_ || GetRole< PHY_RolePion_Composantes >().HasChanged() )
    {
        asn.GetAsnMsg().m.chaine_activeePresent                       = 1;
        asn.GetAsnMsg().m.disponibilites_chefs_convoisPresent         = 1;
        asn.GetAsnMsg().m.disponibilites_transporteurs_convoisPresent = 1;

        asn.GetAsnMsg().chaine_activee  = bSystemEnabled_;
      
        PHY_RolePion_Composantes::T_ComposanteUseMap composanteUse;
        GetRole< PHY_RolePion_Composantes >().GetConvoyTransporters( composanteUse );
        SendComposanteUse( composanteUse, asn.GetAsnMsg().disponibilites_transporteurs_convois );

        composanteUse.clear();
        GetRole< PHY_RolePion_Composantes >().GetConvoyCommanders( composanteUse );
        SendComposanteUse( composanteUse, asn.GetAsnMsg().disponibilites_chefs_convois );
    }

    pStocks_->SendChangedState( asn );    

    asn.Send();

    if( asn.GetAsnMsg().m.disponibilites_chefs_convoisPresent && asn.GetAsnMsg().disponibilites_chefs_convois.n > 0 )
        delete [] asn.GetAsnMsg().disponibilites_chefs_convois.elem;
    if( asn.GetAsnMsg().m.disponibilites_transporteurs_convoisPresent && asn.GetAsnMsg().disponibilites_transporteurs_convois.n > 0 )
        delete [] asn.GetAsnMsg().disponibilites_transporteurs_convois.elem;
    if( asn.GetAsnMsg().m.stocksPresent && asn.GetAsnMsg().stocks.n > 0 )
        delete [] asn.GetAsnMsg().stocks.elem;
}
