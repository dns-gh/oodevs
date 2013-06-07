// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_BrainLogistic.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:33 $
// $Revision: 12 $
// $Workfile: MIL_BrainLogistic.h $
//
// *****************************************************************************

#ifndef __MIL_LogisticVisitor_h_
#define __MIL_LogisticVisitor_h_

#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Roles/Logistic/FuneralConfig.h"
#include "Entities/Agents/Roles/Logistic/FuneralPackagingResource.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Deployment/PHY_RolePion_Deployment.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include <boost/foreach.hpp>

class MIL_AutomateLOG;

// =============================================================================
// Visitor
// =============================================================================
class MIL_LogisticEntitiesVisitor : public MIL_EntitiesVisitor_ABC
{
public:
    MIL_LogisticEntitiesVisitor( )
        : initialLogisticBase_( 0 )
    {
    }

    virtual bool Visit( const MIL_Formation& formation )
    {
       return ShouldVisit( formation );
    }

    virtual bool Visit( const MIL_Automate& automat)
    {
        return ShouldVisit( automat );
    }

    virtual void Visit( const MIL_AgentPion& )
    {
        // Nothing
    }

private:
    template< typename T >
    bool ShouldVisit( const T& entity )
    {
        const MIL_AutomateLOG* logisticBase = entity.GetBrainLogistic();
        if( initialLogisticBase_ && logisticBase )
            return false;
        if( logisticBase )
            initialLogisticBase_ = logisticBase;
        return true;
    }

private:
    const MIL_AutomateLOG* initialLogisticBase_;
};


// =============================================================================
// MAINTENANCE
// =============================================================================
class MaintenanceTransportVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        MaintenanceTransportVisitor( const PHY_ComposantePion& composante )
            : nScore_    ( std::numeric_limits< int >::min() )
            , pSelected_ ( 0 )
            , composante_( composante )
        {
        }

        virtual void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Maintenance* candidate = tmp.RetrieveRole< PHY_RoleInterface_Maintenance >();
            const int nNewScore = candidate!=0 ? candidate->GetAvailabilityScoreForTransport( composante_ ) : std::numeric_limits< int >::min();
            if( nNewScore > nScore_ )
            {
                nScore_    = nNewScore;
                pSelected_ = const_cast<PHY_RoleInterface_Maintenance*>( candidate );
            }
        }

    public:
                int                            nScore_;
                PHY_RoleInterface_Maintenance* pSelected_;
        const PHY_ComposantePion&              composante_;
};

class MaintenanceRepairVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        MaintenanceRepairVisitor( const PHY_MaintenanceComposanteState& state )
            : nScore_    (  std::numeric_limits< int >::min() )
            , pSelected_ ( 0 )
            , state_     ( state )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Maintenance* candidate = tmp.RetrieveRole< PHY_RoleInterface_Maintenance >();
            const int nNewScore = candidate!=0 ? candidate->GetAvailabilityScoreForRepair( state_ ) : std::numeric_limits< int >::min();
            if( nNewScore > nScore_ )
            {
                nScore_    = nNewScore;
                pSelected_ = const_cast<PHY_RoleInterface_Maintenance*>( candidate );
            }
        }

    public:
              int                             nScore_;
              PHY_RoleInterface_Maintenance*       pSelected_;
        const PHY_MaintenanceComposanteState& state_;
};

// =============================================================================
// MEDICAL
// =============================================================================
class MedicalThirdPartyEvacuationVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        MedicalThirdPartyEvacuationVisitor( MIL_AgentPion& pion, Human_ABC& human )
            : pion_  ( pion )
            , human_ ( human )
            , pState_( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            pState_ = (candidate!= 0 ? const_cast<PHY_RoleInterface_Medical*>(candidate)->HandleHumanEvacuatedByThirdParty( pion_, human_ ) : 0);
        }

    public:
        MIL_AgentPion&         pion_;
        Human_ABC&             human_;
        PHY_MedicalHumanState* pState_;
};

class MedicalEvacuationVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        MedicalEvacuationVisitor( const Human_ABC& human )
            : nScore_   ( std::numeric_limits< int >::min() )
            , human_    ( human )
            , pSelected_( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            const int nNewScore = (candidate!=0? candidate->GetAvailabilityScoreForEvacuation( human_ ) : std::numeric_limits< int >::min());
            if( nNewScore > nScore_ )
            {
                nScore_    = nNewScore;
                pSelected_ = const_cast<PHY_RoleInterface_Medical*>(candidate);
            }
        }

    public:
              int                   nScore_;
        const Human_ABC&            human_;
              PHY_RoleInterface_Medical* pSelected_;
};

class MedicalCollectionVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        MedicalCollectionVisitor( const PHY_MedicalHumanState& humanState )
            : nScore_    ( std::numeric_limits< int >::min() )
            , humanState_( humanState )
            , pSelected_ ( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            const int nNewScore = (candidate!=0 ? candidate->GetAvailabilityScoreForCollection( humanState_ ) : std::numeric_limits< int >::min());
            if( nNewScore > nScore_ )
            {
                nScore_    = nNewScore;
                pSelected_ = const_cast<PHY_RoleInterface_Medical*>(candidate);
            }
        }

    public:
              int                    nScore_;
        const PHY_MedicalHumanState& humanState_;
              PHY_RoleInterface_Medical*  pSelected_;
};

class MedicalSortingVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        MedicalSortingVisitor()
            : nScore_    ( std::numeric_limits< int >::min() )
            , pSelected_ ( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            const int nNewScore = (candidate!=0 ? candidate->GetAvailabilityScoreForSorting() : std::numeric_limits< int >::min());
            if( nNewScore > nScore_ )
            {
                nScore_    = nNewScore;
                pSelected_ = const_cast<PHY_RoleInterface_Medical*>(candidate);
            }
        }

    public:
              int                             nScore_;
              PHY_RoleInterface_Medical*      pSelected_;
};

class MedicalHealingVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        MedicalHealingVisitor( const PHY_MedicalHumanState& humanState )
            : nScore_    ( std::numeric_limits< int >::min() )
            , humanState_( humanState )
            , pSelected_ ( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            const int nNewScore = (candidate!=0 ? candidate->GetAvailabilityScoreForHealing( humanState_ ) : std::numeric_limits< int >::min() );
            if( nNewScore > nScore_ )
            {
                nScore_    = nNewScore;
                pSelected_ = const_cast<PHY_RoleInterface_Medical*>(candidate);
            }
        }

    public:
              int                    nScore_;
        const PHY_MedicalHumanState& humanState_;
              PHY_RoleInterface_Medical*  pSelected_;
};

class MedicalCollectionAmbulanceAuthorizedToGoVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        MedicalCollectionAmbulanceAuthorizedToGoVisitor( const PHY_MedicalCollectionAmbulance& ambulance  )
            : ambulance_ ( ambulance )
            , bAuthorized_( true )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            if( !bAuthorized_ )
                return;

            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            bAuthorized_ = (candidate!=0 ? candidate->CanCollectionAmbulanceGo( ambulance_ ) : true );
        }

    public:
        const PHY_MedicalCollectionAmbulance& ambulance_;
              bool                            bAuthorized_;
};

// =============================================================================
// SUPPLY
// =============================================================================

class SupplyStockReservationVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        SupplyStockReservationVisitor( const PHY_DotationCategory& dotationCategory, double requestedQuantity )
            : dotationCategory_ ( dotationCategory )
            , requestedQuantity_( requestedQuantity )
            , remainingQuantity_( requestedQuantity )
            , provider_         ( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            // NLD 2011-04-07 : Totally bugged ...
            // We must not use BL internal TC2 for external use
            //MIL_AutomateLOG* testBrain = tmp.GetAutomate().GetBrainLogistic();
            //if( bExternalTransfert_ && testBrain )
            //    return;

            if( remainingQuantity_ <= 0 )
                return;

            PHY_RoleInterface_Supply* candidate = const_cast< PHY_RoleInterface_Supply* >( tmp.RetrieveRole< PHY_RoleInterface_Supply >() );
            if( candidate )
            {
                double reservation = candidate->AddStockReservation( dotationCategory_, remainingQuantity_ );
                remainingQuantity_ -= reservation;
                if( !provider_ && reservation )
                    provider_ = &tmp;
            }
        }

    public:
        const PHY_DotationCategory&  dotationCategory_;
        const double requestedQuantity_;
        double remainingQuantity_;
        const MIL_AgentPion* provider_;

};

class SupplyStockReturnVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        SupplyStockReturnVisitor( const PHY_DotationCategory& dotationCategory, double quantity )
            : dotationCategory_( dotationCategory )
            , quantity_        ( quantity )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            // NLD 2011-04-07 : Totally bugged ...
            // We must not use BL internal TC2 for external use
            //MIL_AutomateLOG* testBrain = tmp.GetAutomate().GetBrainLogistic();
            //if( bExternalTransfert_ && testBrain )
            //    return;

            if( quantity_ <= 0 )
                return;

            //$$ TODO
            PHY_RoleInterface_Supply* candidate = const_cast< PHY_RoleInterface_Supply* >( tmp.RetrieveRole< PHY_RoleInterface_Supply >() );
            if( candidate )
                quantity_-= candidate->RemoveStockReservation( dotationCategory_, quantity_ );
        }

    public:
        const PHY_DotationCategory&  dotationCategory_;
        double quantity_;
};

class SupplyStockContainerVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        SupplyStockContainerVisitor( const PHY_DotationCategory& dotationCategory )
            : dotationCategory_( dotationCategory )
            , pSelected_       ( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            if( pSelected_ )
                return;

            const PHY_RoleInterface_Supply* candidate = tmp.RetrieveRole< PHY_RoleInterface_Supply >();
            if( candidate!=0 && candidate->CanReserveStock( dotationCategory_ ) )
                pSelected_ = const_cast<PHY_RoleInterface_Supply*>( candidate );
        }

    public:
        const PHY_DotationCategory&  dotationCategory_;
              PHY_RoleInterface_Supply*   pSelected_;
};

class SupplyConvoyAvailabilityVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        SupplyConvoyAvailabilityVisitor( const PHY_DotationCategory& dotationCategory )
            : dotationCategory_( dotationCategory )
            , pConvoySelected_( 0 )
            , pSelected_( 0 )
            , rTotalWeightMax_( 0. )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            // NLD 2011-04-07 : Totally bugged ...
            // We must not use BL internal TC2 for external use
            //MIL_AutomateLOG* testBrain = tmp.GetAutomate().GetBrainLogistic();
            //if( bExternalTransfert_ && testBrain )
                //return;

            const PHY_RoleInterface_Supply* candidate = tmp.RetrieveRole< PHY_RoleInterface_Supply >();
            PHY_ComposantePion* pTmpConvoySelected = candidate!=0 ? candidate->GetAvailableConvoyTransporter( dotationCategory_ ) : 0;
            if( pTmpConvoySelected )
            {
                double rTotalWeightMax = 0.;
                double rTotalVolumeMax = 0.;
                pTmpConvoySelected->GetStockTransporterCapacity( rTotalWeightMax, rTotalVolumeMax );
                if( !pSelected_ || rTotalWeightMax_ > rTotalWeightMax )
                {
                    rTotalWeightMax_ = rTotalWeightMax;
                    pConvoySelected_ = pTmpConvoySelected;
                    pSelected_       = const_cast<MIL_AgentPion*>(&tmp);
                }
            }
        }

    public:
        const PHY_DotationCategory& dotationCategory_;
              PHY_ComposantePion* pConvoySelected_;
              MIL_AgentPion* pSelected_;
              double rTotalWeightMax_;
};

class SupplyConvoyTransporterVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        SupplyConvoyTransporterVisitor( const PHY_ComposanteTypePion& type )
            : type_            ( type )
            , pConvoySelected_ ( 0 )
            , pSelected_       ( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            // NLD 2011-04-07 : Totally bugged ...
            // We must not use BL internal TC2 for external use
            //MIL_AutomateLOG* testBrain = tmp.GetAutomate().GetBrainLogistic();
            //if( bExternalTransfert_ && testBrain )
                //return;

            if( pSelected_ )
                return;

            const PHY_RoleInterface_Supply* candidate = tmp.RetrieveRole< PHY_RoleInterface_Supply >();
            PHY_ComposantePion* pTmpConvoySelected = candidate!=0 ? candidate->GetAvailableConvoyTransporter( type_ ) : 0;
            if( pTmpConvoySelected )
            {
                pConvoySelected_ = pTmpConvoySelected;
                pSelected_       = const_cast<MIL_AgentPion*>(&tmp);
            }
        }

    public:
        const PHY_ComposanteTypePion& type_;
              PHY_ComposantePion*     pConvoySelected_;
              MIL_AgentPion*          pSelected_;
};

class SupplyConvoyCapacityVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        SupplyConvoyCapacityVisitor()
            : nScore_   ( std::numeric_limits< int >::min() )
            , pSelected_( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            // NLD 2011-04-07 : Totally bugged ...
            // We must not use BL internal TC2 for external use
            //MIL_AutomateLOG* testBrain = tmp.GetAutomate().GetBrainLogistic();
            //if( bExternalTransfert_ && testBrain )
                //return;

            PHY_RoleInterface_Composantes::T_ComposanteUseMap composanteUse;
            tmp.GetRole< PHY_RoleInterface_Composantes >().GetConvoyTransportersUse( composanteUse );
            int nNewScore = 0;
            for( PHY_RolePion_Composantes::CIT_ComposanteUseMap it = composanteUse.begin(); it != composanteUse.end(); ++it )
                nNewScore += it->second.nNbrTotal_;
            if( nNewScore > nScore_ )
            {
                nScore_    = nNewScore;
                pSelected_ = const_cast<MIL_AgentPion*>(&tmp);
            }
        }

    public:
        int             nScore_;
        MIL_AgentPion*  pSelected_;
};

class SupplyDeployementVisitor : public MIL_LogisticEntitiesVisitor
{
public:
    SupplyDeployementVisitor( const PHY_DotationCategory& dotation )
        : dotation_( dotation ), pDeployedStock_( 0 ), pUndeployedStock_( 0 )
    {
    }

    void Visit( const MIL_AgentPion& agent )
    {
        const PHY_RoleInterface_Supply* pSupply = agent.RetrieveRole< PHY_RoleInterface_Supply >();
        if( !pSupply )
            return;
        const PHY_RolePion_Deployment* pDeployment = agent.RetrieveRole< PHY_RolePion_Deployment >();
        if( !pDeployment )
            return;
        MIL_AgentPion* pAgent =  const_cast< MIL_AgentPion* >( &agent );
        if( pAgent->GetType().GetUnitType().IsStockLogisticTypeDefined( dotation_.GetLogisticType() ) )
        {
            if( pDeployment->IsDeployed() && !agent.IsDead() )
                pDeployedStock_ = pAgent;
            else
                pUndeployedStock_ = pAgent;
        }
    }

public:
    const PHY_DotationCategory& dotation_;
    MIL_AgentPion* pDeployedStock_;
    MIL_AgentPion* pUndeployedStock_;
};

//??? XXX
class PCVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
{
    public:
        PCVisitor()
            : pSelected_( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            if( !pSelected_ && tmp.IsPC() )
                pSelected_ = &tmp;
        }

    public:
        const MIL_AgentPion*  pSelected_;
};

// =============================================================================
// FUNERAL
// =============================================================================

class FuneralPackagingResourceVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        FuneralPackagingResourceVisitor( const logistic::FuneralPackagingResource* currentPackagingResource )
            : pSelected_                    ( 0 )
            , packagingResourcesToSearchFor_()
            , nextPackagingResource_        ( 0 )
        {
            logistic::FuneralConfig::GetOrderedPackagingResources( packagingResourcesToSearchFor_, currentPackagingResource );
        }

        void Visit( const MIL_AgentPion& pion )
        {
            if( nextPackagingResource_ )
                return;
            BOOST_FOREACH( const logistic::FuneralPackagingResource* packagingResource, packagingResourcesToSearchFor_ )
            {
                if( pion.GetRole< dotation::PHY_RoleInterface_Dotations >().GetDotationNumber( packagingResource->GetDotationCategory() ) > 1 )
                {
                    pSelected_ = const_cast< MIL_AgentPion* >( &pion );
                    nextPackagingResource_ = packagingResource;
                    break;
                }
            }
        }

    public:
        MIL_AgentPion* pSelected_;
        const logistic::FuneralPackagingResource* nextPackagingResource_;
        std::vector< const logistic::FuneralPackagingResource* > packagingResourcesToSearchFor_;
};


#endif // __MIL_LogisticVisitor_h_