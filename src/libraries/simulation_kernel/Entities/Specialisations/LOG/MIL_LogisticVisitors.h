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
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Roles/Logistic/FuneralConfig.h"
#include "Entities/Agents/Roles/Logistic/FuneralPackagingResource.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include <boost/foreach.hpp>

class MIL_AutomateLOG;


// =============================================================================
// MAINTENANCE
// =============================================================================
class MaintenanceTransportVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
{
    public:
        MaintenanceTransportVisitor( const PHY_ComposantePion& composante )
            : nScore_    ( std::numeric_limits< int >::min() )
            , pSelected_ ( 0 )
            , composante_( composante )
        {
        }

        void Visit( const MIL_AgentPion& tmp )
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
                int                       nScore_;
                PHY_RoleInterface_Maintenance* pSelected_;
        const PHY_ComposantePion&       composante_;
};

class MaintenanceRepairVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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
class MedicalThirdPartyEvacuationVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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

class MedicalEvacuationVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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

class MedicalCollectionVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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

class MedicalSortingVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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

class MedicalHealingVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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

class MedicalCollectionAmbulanceAuthorizedToGoVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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

class SupplyStockReservationVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
{
    public:
        SupplyStockReservationVisitor( const PHY_DotationCategory& dotationCategory, double requestedQuantity )
            : dotationCategory_ ( dotationCategory )
            , requestedQuantity_( requestedQuantity )
            , remainingQuantity_( requestedQuantity )
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
                remainingQuantity_ -= candidate->AddStockReservation( dotationCategory_, remainingQuantity_ );
        }

    public:
        const PHY_DotationCategory&  dotationCategory_;
        const double requestedQuantity_;
        double remainingQuantity_;;

};

class SupplyStockReturnVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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

class SupplyStockContainerVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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
            if( candidate!=0 && candidate->CanContainStock( dotationCategory_ ) )
                pSelected_ = const_cast<PHY_RoleInterface_Supply*>(candidate);
        }

    public:
        const PHY_DotationCategory&  dotationCategory_;
              PHY_RoleInterface_Supply*   pSelected_;
};

class SupplyConvoyAvailabilityVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
{
    public:
        SupplyConvoyAvailabilityVisitor( const PHY_DotationCategory& dotationCategory )
            : dotationCategory_( dotationCategory )
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
            PHY_ComposantePion* pTmpConvoySelected = candidate!=0 ? candidate->GetAvailableConvoyTransporter( dotationCategory_ ) : 0;
            if( pTmpConvoySelected )
            {
                pConvoySelected_ = pTmpConvoySelected;
                pSelected_       = const_cast<MIL_AgentPion*>(&tmp);
            }
        }

    public:
        const PHY_DotationCategory&  dotationCategory_;
              PHY_ComposantePion*    pConvoySelected_;
              MIL_AgentPion*         pSelected_;
};

class SupplyConvoyTransporterVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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

class SupplyConvoyCapacityVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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

class FuneralPackagingResourceVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
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


//$$$$$ TODO pour remplacer le bExternalTransfert
/*
template< typename T >
class LogisticVisitor : public MIL_EntityVisitor_ABC< MIL_AgentPion >
{
public:
    LogisticVisitor()
        : score_    ( std::numeric_limits< int >::min() )
        , pSelected_( 0 )
    {
    }

    void Visit( const MIL_AgentPion& tmp )
    {
        //$$ TEST pas de sous hierarchie log dans log( = TC2 de la BL)

        const T* candidate = tmp.RetrieveRole< T >();
        if( !candidate )
            return;

        const int newScore = ComputeScore( *candidate );
        if( newScore > score_ )
        {
            score_ = newScore;
            pSelected_ = const_cast< T* >( candidate );
        }
    }

protected:
    virtual int ComputeScore( const T& role ) = 0;

public:
    T* pSelected_;

private:
    int score_;
};
*/

/*class TestLogisticVisitor : public LogisticVisitor< PHY_RoleInterface_Maintenance >
{
public:
    TestLogisticVisitor( const PHY_ComposantePion& composante )
        : composante_( composante )
    {
    }

private:
    virtual int ComputeScore( const PHY_RoleInterface_Maintenance& role )
    {
        return role.GetAvailabilityScoreForTransport( composante_ );
    }

private:
    const PHY_ComposantePion& composante_;
};*/


#endif // __MIL_LogisticVisitor_h_