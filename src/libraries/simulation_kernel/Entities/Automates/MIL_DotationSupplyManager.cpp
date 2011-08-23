// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_Automate.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 17:39 $
// $Revision: 32 $
// $Workfile: MIL_Automate.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DotationSupplyManager.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/SupplyDotationRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestHierarchyDispatcher.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "MIL_AgentServer.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_DotationSupplyManager )

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_DotationSupplyManager::MIL_DotationSupplyManager( MIL_Automate& automate )
    : pAutomate_                         ( &automate )
    , bDotationSupplyNeeded_             ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , nTickRcDotationSupplyQuerySent_    ( 0 )
    , supplyRequestBuilder_              ( new logistic::SupplyDotationRequestBuilder( automate, *this ) )
    , supplyRequests_                    ( new logistic::SupplyRequestContainer( supplyRequestBuilder_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_DotationSupplyManager::MIL_DotationSupplyManager()
    : pAutomate_                         ( 0 )
    , bDotationSupplyNeeded_             ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , nTickRcDotationSupplyQuerySent_    ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_DotationSupplyManager::~MIL_DotationSupplyManager()
{
    // NOTHING
}

// =============================================================================
// CHEKPOINTS
// =============================================================================
namespace boost
{
    /*namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, MIL_DotationSupplyManager::T_SupplyDotationStateMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const MIL_DotationSupplyManager::T_SupplyDotationStateMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for ( MIL_DotationSupplyManager::CIT_SupplyDotationStateMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, MIL_DotationSupplyManager::T_SupplyDotationStateMap& map, const unsigned int )
        {
            std::size_t nNbr;

            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_AutomateLOG* pBrainLogistic;

                file >> pBrainLogistic;
                file >> map[ pBrainLogistic ];
            }
        }
    }*/
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::serialize
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> pAutomate_
         >> bDotationSupplyNeeded_
         >> bDotationSupplyExplicitlyRequested_
         >> nTickRcDotationSupplyQuerySent_;

    // $$ TMP
    assert( pAutomate_ );
    supplyRequestBuilder_.reset( new logistic::SupplyDotationRequestBuilder( *pAutomate_, *this ) );
    supplyRequests_.reset( new logistic::SupplyRequestContainer( supplyRequestBuilder_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::serialize
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pAutomate_
         << bDotationSupplyNeeded_
         << bDotationSupplyExplicitlyRequested_
         << nTickRcDotationSupplyQuerySent_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::Update
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::Update()
{
    supplyRequests_->Update();
    if( bDotationSupplyNeeded_ && pAutomate_->GetLogisticHierarchy().HasSuperior() )
    {
        logistic::SupplyRequestHierarchyDispatcher dispatcher( pAutomate_->GetLogisticHierarchy() );
        bDotationSupplyNeeded_ = !supplyRequests_->Execute( dispatcher );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::Clean
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::Clean()
{
    bDotationSupplyExplicitlyRequested_ = false;
    supplyRequests_->Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::NotifyDotationSupplyNeeded
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    if( bDotationSupplyNeeded_ || supplyRequests_->IsSupplying( dotationCategory ) )
        return;
    bDotationSupplyNeeded_ = true;

    // Pas de RC si RC envoyé au tick précédent
    const unsigned int nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( ( nCurrentTick > ( nTickRcDotationSupplyQuerySent_ + 1 ) || nTickRcDotationSupplyQuerySent_ == 0 ) )
        MIL_Report::PostEvent( *pAutomate_, MIL_Report::eReport_DotationSupplyRequest );
    nTickRcDotationSupplyQuerySent_ = nCurrentTick;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::RequestDotationSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::RequestDotationSupply()
{
    bDotationSupplyNeeded_              = true;
    bDotationSupplyExplicitlyRequested_ = true;
}

// =============================================================================
// SupplyRecipient_ABC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::GetPosition
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_DotationSupplyManager::GetPosition() const
{
    return pAutomate_->GetPionPC().GetRole< PHY_RoleInterface_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::GetPC
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
const MIL_AgentPion& MIL_DotationSupplyManager::GetPC() const
{
    return pAutomate_->GetPionPC();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyCanceled
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyCanceled()
{
    MIL_Report::PostEvent( *pAutomate_, MIL_Report::eReport_DotationSupplyCanceled );
    bDotationSupplyNeeded_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyDone
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyDone()
{
    MIL_Report::PostEvent( *pAutomate_, MIL_Report::eReport_DotationSupplyDone );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::Serialize
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::Serialize( sword::AutomatId& msg ) const
{
    msg.set_id( pAutomate_->GetID() );
}

// =============================================================================
// Network
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::SendChangedState
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::SendChangedState() const
{
    supplyRequests_->SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::SendFullState() const
{
    supplyRequests_->SendFullState();
}
