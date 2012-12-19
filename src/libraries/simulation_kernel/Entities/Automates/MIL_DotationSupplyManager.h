// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_Automate.h $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 17 $
// $Workfile: MIL_Automate.h $
//
// *****************************************************************************

#ifndef __MIL_DotationSupplyManager_h_
#define __MIL_DotationSupplyManager_h_

#include "MIL.h"
#include "MIL_SupplyManager.h"
#include <boost/noncopyable.hpp>
#include <boost/serialization/export.hpp>

class MIL_Automate;

namespace logistic 
{
    class SupplyRequestBuilder_ABC;
    class SupplyRequestContainer;
}

// =============================================================================
// @class  MIL_Automate
// Created: JVT 2004-08-03
// =============================================================================
class MIL_DotationSupplyManager : public MIL_SupplyManager
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_DotationSupplyManager( MIL_Automate& automate );
             MIL_DotationSupplyManager();
    virtual ~MIL_DotationSupplyManager();
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
    void SendChangedState() const;
    void SendFullState   () const;
    //@}

    //! @name SupplyRecipient_ABC
    //@{
    virtual const MT_Vector2D&   GetPosition() const;
    virtual const MIL_AgentPion* GetPC      () const;
    virtual bool                 BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const;
    virtual void OnSupplyScheduled     ( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign );
    virtual void OnSupplyCanceled      ( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign );
    virtual void OnSupplyDone          ( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign );
    virtual void OnSupplyConvoyArriving( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign );
    virtual void OnSupplyConvoyLeaving ( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign );
    virtual void Serialize( sword::AutomatId& msg ) const;
    //@}

    //! @name Notifications
    //@{
    void NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory );
    void RequestDotationSupply     ();
    //@}

private:
    //! @name Helpers
    //@{
    bool IsSupplyInProgress( const PHY_DotationCategory& dotationCategory ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< boost::shared_ptr< const logistic::SupplyConsign_ABC > > T_Supplies;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Automate* pAutomate_;
    bool                     bSupplyNeeded_;
    bool                     bDotationSupplyExplicitlyRequested_;
    boost::shared_ptr< logistic::SupplyRequestBuilder_ABC > supplyRequestBuilder_;
    std::auto_ptr< logistic::SupplyRequestContainer > supplyRequests_;
    T_Supplies scheduledSupplies_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_DotationSupplyManager )

#endif // MIL_DotationSupplyManager
