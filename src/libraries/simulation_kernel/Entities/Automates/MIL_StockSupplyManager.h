// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_StockSupplyManager.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:33 $
// $Revision: 12 $
// $Workfile: MIL_StockSupplyManager.h $
//
// *****************************************************************************

#ifndef __MIL_StockSupplyManager_h_
#define __MIL_StockSupplyManager_h_

#include "MIL.h"
#include "MIL_SupplyManager.h"
#include <boost/serialization/export.hpp>

class MIL_Automate;

namespace logistic
{
    class SupplyRequestBuilder_ABC;
    class SupplyRequestContainer;
    class SupplySupplier_ABC;
}

namespace sword
{
    class PullFlowParameters;
}

// =============================================================================
// @class  MIL_StockSupplyManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_StockSupplyManager : public MIL_SupplyManager
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_StockSupplyManager( MIL_Automate& automate );
             MIL_StockSupplyManager();
    virtual ~MIL_StockSupplyManager();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name Network
    //@{
    bool OnReceiveLogSupplyPullFlow( const sword::PullFlowParameters& parameters, logistic::SupplySupplier_ABC& supplier );

    void SendChangedState() const;
    void SendFullState   () const;
    //@}

    //! @name SupplyRecipient_ABC
    //@{
    virtual const MT_Vector2D&   GetPosition() const;
    virtual const MIL_AgentPion* GetPC      () const;
    virtual bool                 BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const;
    virtual void OnSupplyScheduled     ( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign );
    virtual void OnSupplyCanceled      ( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign );
    virtual void OnSupplyDone          ( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign );
    virtual void OnSupplyConvoyArriving( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign );
    virtual void OnSupplyConvoyLeaving ( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign );
    virtual void Serialize( sword::Id& msg ) const;
    //@}

    //! @name Logistic
    void NotifyStockSupplyNeeded( const PHY_DotationCategory& dotationCategory );
    void ResetConsignsForConvoyPion( const MIL_Agent_ABC& pion );
    void ResetAutoConsignForProvider( const MIL_Agent_ABC& pion );
    void ResetAllConsigns();
    //@}

private:
    //! @name Helpers
    //@{
    bool IsSupplyInProgress( const PHY_DotationCategory& dotationCategory ) const;
    bool IsSupplyManual() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< logistic::SupplyRequestContainer > > T_SupplyRequests;
    typedef std::vector< boost::shared_ptr< logistic::SupplyConsign_ABC > > T_Supplies;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Automate*         pAutomate_;
    bool                  bSupplyNeeded_;
    boost::shared_ptr< logistic::SupplyRequestBuilder_ABC > supplyRequestBuilder_;
    std::auto_ptr< logistic::SupplyRequestContainer > autoSupplyRequest_;
    T_SupplyRequests manualSupplyRequests_;
    T_Supplies scheduledSupplies_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_StockSupplyManager )

#endif // __MIL_StockSupplyManager_h_
