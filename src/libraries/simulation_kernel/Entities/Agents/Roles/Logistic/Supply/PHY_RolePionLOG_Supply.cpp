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

#include "simulation_kernel_pch.h"
#include "PHY_RolePionLOG_Supply.h"
#include "PHY_SupplyDotationConsign.h"
#include "PHY_SupplyDotationRequest.h"
#include "PHY_SupplyDotationRequestContainer.h"
#include "PHY_SupplyResourcesAlarms.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStockContainer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_ASN_Messages.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( PHY_RolePionLOG_Supply, "PHY_RolePionLOG_Supply" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePionLOG_Supply* role, const unsigned int /*version*/ )
{
    archive << role->pPion_;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePionLOG_Supply* role, const unsigned int /*version*/ )
{
	MIL_AgentPionLOG_ABC* pion;
    archive >> pion;
    ::new( role )PHY_RolePionLOG_Supply( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply constructor
// Created: NLD 2005-01-21
// -----------------------------------------------------------------------------
PHY_RolePionLOG_Supply::PHY_RolePionLOG_Supply( MIL_AgentPionLOG_ABC& pion )
    : pPion_             ( &pion )
    , bSystemEnabled_    ( false )
    , bHasChanged_       ( true )
    , pStocks_           ( 0 )
{
    pStocks_ = new PHY_DotationStockContainer( *this );
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
    file & boost::serialization::base_object< PHY_RoleInterface_Supply >( *this )
		 & bSystemEnabled_
         & pStocks_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::WriteODB( xml::xostream& xos ) const
{
    assert( pStocks_ );
    pStocks_->WriteODB( xos );
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::ReadOverloading
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::ReadOverloading( xml::xistream& xis )
{
    assert( pStocks_ );
    pStocks_->ReadValues( xis );
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
    assert( pPion_ );
    return pPion_->GetRole< PHY_RoleInterface_Composantes >().GetAvailableConvoyTransporter( dotationCategory );
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
// Name: PHY_RolePionLOG_Supply::GetConvoyTransportersAvailabilityRatio
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
MT_Float PHY_RolePionLOG_Supply::GetConvoyTransportersAvailabilityRatio() const
{
    assert( pPion_ );
    PHY_RoleInterface_Composantes::T_ComposanteUseMap composanteUse;
    pPion_->GetRole< PHY_RoleInterface_Composantes >().GetConvoyTransportersUse( composanteUse );

    uint nNbrTotal                  = 0;
    uint nNbrAvailableAllowedToWork = 0;
    for( PHY_RoleInterface_Composantes::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
    {
        nNbrTotal                  += it->second.nNbrTotal_;
        nNbrAvailableAllowedToWork += ( it->second.nNbrAvailable_ - it->second.nNbrUsed_ );
    }
    if( nNbrTotal == 0 )
        return 1.;
    return (MT_Float)nNbrAvailableAllowedToWork / (MT_Float)nNbrTotal;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::StartUsingForLogistic
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::StartUsingForLogistic( PHY_ComposantePion& composante )
{
    MT_Float rTransporterRatio = GetConvoyTransportersAvailabilityRatio();

    bHasChanged_ = true;
    composante.StartUsingForLogistic();

    if( PHY_SupplyResourcesAlarms::IsConvoyTransporterResourcesLevelReached( rTransporterRatio, GetConvoyTransportersAvailabilityRatio() ) )
        MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_ConvoyTransporterResourcesLevelReached );
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
        MIL_Report::PostEvent( *pPion_, MIL_Report::eReport_LogisticStockThresholdExceeded );

    pPion_->GetLogAutomate().NotifyStockSupplyNeeded( dotationCategory );
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::AddStock
// Created: NLD 2006-03-28
// -----------------------------------------------------------------------------
PHY_DotationStock* PHY_RolePionLOG_Supply::AddStock( const PHY_DotationCategory& category ) const
{
    assert( pStocks_ );
    return pStocks_->AddStock( category );
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
    assert( pStocks_ );
    pStocks_->Update(); // Stock checking
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
void SendComposanteUse( const PHY_RoleInterface_Composantes::T_ComposanteUseMap& data, ASN1T__SeqOfLogSupplyEquimentAvailability& asn )
{
    asn.n = data.size();
    if( data.empty() )
        return;

    ASN1T_LogSupplyEquimentAvailability* pData = new ASN1T_LogSupplyEquimentAvailability[ data.size() ];
    uint i = 0;
    for( PHY_RoleInterface_Composantes::CIT_ComposanteUseMap itData = data.begin(); itData != data.end(); ++itData )
    {
        ASN1T_LogSupplyEquimentAvailability& data = pData[ i++ ];
        data.type_equipement = itData->first->GetMosID();
        assert( itData->second.nNbrTotal_ );

        data.nbr_total       = itData->second.nNbrTotal_;
        data.nbr_au_travail  = itData->second.nNbrUsed_;
        data.nbr_disponibles = itData->second.nNbrAvailable_ - itData->second.nNbrUsed_; // nNbrAvailableAllowedToWork
        data.nbr_pretes      = itData->second.nNbrLent_;
    }
    asn.elem = pData;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::SendFullState() const
{
    assert( pPion_ );

    NET_ASN_MsgLogSupplyState asn;

    asn().m.chaine_activeePresent                       = 1;
    asn().m.disponibilites_transporteurs_convoisPresent = 1;

    asn().oid_pion        = pPion_->GetID();
    asn().chaine_activee  = bSystemEnabled_;
  
    PHY_RoleInterface_Composantes::T_ComposanteUseMap composanteUse;
    pPion_->GetRole< PHY_RoleInterface_Composantes >().GetConvoyTransportersUse( composanteUse );
    SendComposanteUse( composanteUse, asn().disponibilites_transporteurs_convois  );

    assert( pStocks_ );
    pStocks_->SendFullState( asn );

    asn.Send();

    if( asn().disponibilites_transporteurs_convois.n > 0 )
        delete [] asn().disponibilites_transporteurs_convois.elem;
    if( asn().stocks.n > 0 )
        delete [] asn().stocks.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::SendChangedState() const
{
    assert( pPion_ );
    assert( pStocks_ );    
    if( !( bHasChanged_ || pPion_->GetRole< PHY_RoleInterface_Composantes >().HasChanged() || pStocks_->HasChanged() ) )
        return;

    NET_ASN_MsgLogSupplyState asn;
    assert( pPion_ );
    asn().oid_pion = pPion_->GetID();

    if( bHasChanged_ || pPion_->GetRole< PHY_RoleInterface_Composantes >().HasChanged() )
    {
        asn().m.chaine_activeePresent                       = 1;
        asn().m.disponibilites_transporteurs_convoisPresent = 1;

        asn().chaine_activee  = bSystemEnabled_;
      
        PHY_RoleInterface_Composantes::T_ComposanteUseMap composanteUse;
        pPion_->GetRole< PHY_RoleInterface_Composantes >().GetConvoyTransportersUse( composanteUse );
        SendComposanteUse( composanteUse, asn().disponibilites_transporteurs_convois );
    }

    pStocks_->SendChangedState( asn );    

    asn.Send();

    if( asn().m.disponibilites_transporteurs_convoisPresent && asn().disponibilites_transporteurs_convois.n > 0 )
        delete [] asn().disponibilites_transporteurs_convois.elem;
    if( asn().m.stocksPresent && asn().stocks.n > 0 )
        delete [] asn().stocks.elem;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::EnableSystem()
{
    bSystemEnabled_ = true;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePionLOG_Supply::DisableSystem()
{
    bSystemEnabled_ = false;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Supply::GetPion
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const MIL_AgentPionLOG_ABC& PHY_RolePionLOG_Supply::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}
