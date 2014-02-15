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

#ifndef __MIL_LogisticVisitors_h_
#define __MIL_LogisticVisitors_h_

#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Roles/Logistic/FuneralConfig.h"
#include "Entities/Agents/Roles/Logistic/FuneralPackagingResource.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Deployment/PHY_RolePion_Deployment.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include <boost/foreach.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

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
                                  , private boost::noncopyable
{
    public:
        MaintenanceTransportVisitor( const PHY_ComposantePion& composante, const PHY_ComposanteTypePion* type = 0 )
            : score_     ( std::numeric_limits< int >::min() )
            , selected_  ( 0 )
            , composante_( composante )
            , type_      ( type )
        {}

        virtual void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Maintenance* candidate = tmp.RetrieveRole< PHY_RoleInterface_Maintenance >();
            const int score = candidate != 0 ? candidate->GetAvailabilityScoreForTransport( composante_, type_ ) : std::numeric_limits< int >::min();
            if( score > score_ )
            {
                score_    = score;
                selected_ = const_cast<PHY_RoleInterface_Maintenance*>( candidate );
            }
        }

    public:
                int                            score_;
                PHY_RoleInterface_Maintenance* selected_;
        const PHY_ComposantePion&              composante_;
        const PHY_ComposanteTypePion*          type_;
};

class MaintenanceDiagnosisVisitor : public MIL_LogisticEntitiesVisitor
                                  , private boost::noncopyable
{
    public:
        MaintenanceDiagnosisVisitor( const PHY_ComposanteTypePion* type = 0 )
            : score_   ( std::numeric_limits< int >::min() )
            , selected_( 0 )
            , type_    ( type )
        {}

        virtual void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Maintenance* candidate = tmp.RetrieveRole< PHY_RoleInterface_Maintenance >();
            const int score = candidate != 0 ? candidate->GetAvailabilityScoreForDiagnosis( type_ ) : std::numeric_limits< int >::min();
            if( score > score_ )
            {
                score_    = score;
                selected_ = const_cast<PHY_RoleInterface_Maintenance*>( candidate );
            }
        }

    public:
                int                            score_;
                PHY_RoleInterface_Maintenance* selected_;
        const PHY_ComposanteTypePion*          type_;
};

class MaintenanceRepairVisitor : public MIL_LogisticEntitiesVisitor
                               , private boost::noncopyable
{
    public:
        MaintenanceRepairVisitor( const PHY_MaintenanceComposanteState& state, const PHY_ComposanteTypePion* type = 0 )
            : score_   ( std::numeric_limits< int >::min() )
            , selected_( 0 )
            , state_   ( state )
            , type_    ( type )
        {}

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Maintenance* candidate = tmp.RetrieveRole< PHY_RoleInterface_Maintenance >();
            const int score = candidate != 0 ? candidate->GetAvailabilityScoreForRepair( state_, type_ ) : std::numeric_limits< int >::min();
            if( score > score_ )
            {
                score_    = score;
                selected_ = const_cast<PHY_RoleInterface_Maintenance*>( candidate );
            }
        }

    public:
              int                             score_;
              PHY_RoleInterface_Maintenance*  selected_;
        const PHY_MaintenanceComposanteState& state_;
        const PHY_ComposanteTypePion*         type_;
};

// =============================================================================
// MEDICAL
// =============================================================================
class MedicalThirdPartyEvacuationVisitor : public MIL_LogisticEntitiesVisitor
                                         , private boost::noncopyable
{
    public:
        MedicalThirdPartyEvacuationVisitor( MIL_AgentPion& pion, Human_ABC& human )
            : pion_ ( pion )
            , human_( human )
        {}

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            if( candidate )
                pState_ = const_cast< PHY_RoleInterface_Medical* >( candidate )->HandleHumanEvacuatedByThirdParty( pion_, human_ );
            else
                pState_.reset();
        }

    public:
        MIL_AgentPion&         pion_;
        Human_ABC&             human_;
        boost::shared_ptr< PHY_MedicalHumanState > pState_;
};

class MedicalEvacuationVisitor : public MIL_LogisticEntitiesVisitor
                               , private boost::noncopyable
{
    public:
        MedicalEvacuationVisitor( const Human_ABC& human )
            : score_   ( std::numeric_limits< int >::min() )
            , human_    ( human )
            , selected_( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            const int score = (candidate!=0? candidate->GetAvailabilityScoreForEvacuation( human_ ) : std::numeric_limits< int >::min());
            if( score > score_ )
            {
                score_    = score;
                selected_ = const_cast<PHY_RoleInterface_Medical*>(candidate);
            }
        }

    public:
              int                   score_;
        const Human_ABC&            human_;
              PHY_RoleInterface_Medical* selected_;
};

class MedicalCollectionVisitor : public MIL_LogisticEntitiesVisitor
                               , private boost::noncopyable
{
    public:
        MedicalCollectionVisitor( const PHY_MedicalHumanState& humanState )
            : score_    ( std::numeric_limits< int >::min() )
            , humanState_( humanState )
            , selected_ ( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            const int score = (candidate!=0 ? candidate->GetAvailabilityScoreForCollection( humanState_ ) : std::numeric_limits< int >::min());
            if( score > score_ )
            {
                score_    = score;
                selected_ = const_cast<PHY_RoleInterface_Medical*>(candidate);
            }
        }

    public:
              int                    score_;
        const PHY_MedicalHumanState& humanState_;
              PHY_RoleInterface_Medical*  selected_;
};

class MedicalSortingVisitor : public MIL_LogisticEntitiesVisitor
                            , private boost::noncopyable
{
    public:
        MedicalSortingVisitor()
            : score_    ( std::numeric_limits< int >::min() )
            , selected_ ( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            const int score = (candidate!=0 ? candidate->GetAvailabilityScoreForSorting() : std::numeric_limits< int >::min());
            if( score > score_ )
            {
                score_    = score;
                selected_ = const_cast<PHY_RoleInterface_Medical*>(candidate);
            }
        }

    public:
              int                             score_;
              PHY_RoleInterface_Medical*      selected_;
};

class MedicalHealingVisitor : public MIL_LogisticEntitiesVisitor
                            , private boost::noncopyable
{
    public:
        MedicalHealingVisitor( const PHY_MedicalHumanState& humanState )
            : score_    ( std::numeric_limits< int >::min() )
            , humanState_( humanState )
            , selected_ ( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            const PHY_RoleInterface_Medical* candidate = tmp.RetrieveRole< PHY_RoleInterface_Medical >();
            const int score = (candidate!=0 ? candidate->GetAvailabilityScoreForHealing( humanState_ ) : std::numeric_limits< int >::min() );
            if( score > score_ )
            {
                score_    = score;
                selected_ = const_cast<PHY_RoleInterface_Medical*>(candidate);
            }
        }

    public:
              int                    score_;
        const PHY_MedicalHumanState& humanState_;
              PHY_RoleInterface_Medical*  selected_;
};

class MedicalCollectionAmbulanceAuthorizedToGoVisitor : public MIL_LogisticEntitiesVisitor
                                                      , private boost::noncopyable
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
                                    , private boost::noncopyable
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
                               , private boost::noncopyable
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

class SupplyConvoyCapacityVisitor : public MIL_LogisticEntitiesVisitor
                                  , private boost::noncopyable
{
    public:
        SupplyConvoyCapacityVisitor()
            : score_   ( 0 )
            , selected_( 0 )
        {}

        void Visit( const MIL_AgentPion& pion )
        {
            // NLD 2011-04-07 : Totally bugged ...
            // We must not use BL internal TC2 for external use
            //MIL_AutomateLOG* testBrain = tmp.GetAutomate().GetBrainLogistic();
            //if( bExternalTransfert_ && testBrain )
                //return;
            const std::size_t score = pion.GetRole< PHY_RoleInterface_Composantes >().GetConvoyTransportersTotal();
            if( score > score_ )
            {
                score_ = score;
                selected_ = const_cast< MIL_AgentPion* >( &pion );
            }
        }

    public:
        std::size_t score_;
        MIL_AgentPion* selected_;
};

class SupplyDeployementVisitor : public MIL_LogisticEntitiesVisitor, private boost::noncopyable
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
            : selected_( 0 )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
        {
            if( !selected_ && tmp.IsPC() )
                selected_ = &tmp;
        }

    public:
        const MIL_AgentPion*  selected_;
};

// =============================================================================
// FUNERAL
// =============================================================================

class FuneralPackagingResourceVisitor : public MIL_LogisticEntitiesVisitor
{
    public:
        FuneralPackagingResourceVisitor( const logistic::FuneralPackagingResource* currentPackagingResource )
            : selected_                    ( 0 )
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
                    selected_ = const_cast< MIL_AgentPion* >( &pion );
                    nextPackagingResource_ = packagingResource;
                    break;
                }
            }
        }

    public:
        MIL_AgentPion* selected_;
        const logistic::FuneralPackagingResource* nextPackagingResource_;
        std::vector< const logistic::FuneralPackagingResource* > packagingResourcesToSearchFor_;
};

#endif // __MIL_LogisticVisitors_h_
