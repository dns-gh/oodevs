// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConsign_h_
#define __SupplyConsign_h_

#include "SupplyConsign_ABC.h"
#include "SupplyConvoyEventsObserver_ABC.h"
#include "Tools/MIL_IDManager.h"
#include <deque>

class PHY_DotationCategory;

namespace logistic {
    class SupplyConvoy_ABC;
    class SupplySupplier_ABC;
    class SupplyRequestParameters_ABC;

// =============================================================================
/** @class  SupplyConsign_ABC
    @brief  SupplyConsign_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConsign : public SupplyConsign_ABC
                    , public SupplyConvoyEventsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConsign( SupplySupplier_ABC& supplier, SupplyRequestParameters_ABC& parameters );
    virtual ~SupplyConsign();
    //@}

    //! @name Operations
    //@{
    virtual void AddRequest( SupplyRecipient_ABC& recipient, boost::shared_ptr< SupplyRequest_ABC > request );
    virtual void Activate  ();
    virtual bool Update    ();
    virtual bool IsSupplying( const PHY_DotationCategory& dotationCategory ) const;
    virtual bool IsFinished() const;
    //@}

    //! @name TEST
    //@{
    virtual bool WillGoTo( const MIL_AutomateLOG& destination ) const;
    virtual bool IsAt    ( const MIL_AutomateLOG& destination ) const;
    virtual boost::shared_ptr< SupplyConvoy_ABC > GetConvoy() const;
    //@}
    
    //! @name SupplyConvoyEventsObserver_ABC
    //@{
    virtual void OnAllResourcesAssignedToConvoy();
    virtual void OnResourceAssignedToConvoy    ( const PHY_DotationCategory& dotationCategory, double quantity );
    virtual void OnResourceRemovedFromConvoy   ( const PHY_DotationCategory& dotationCategory, double quantity );
    virtual void OnConvoyEndMission            ();
    //@}

    //! @name Network - A refactorer
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    virtual void Clean           ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, double >                                 T_Resources;
    typedef std::map< const PHY_DotationCategory*, boost::shared_ptr< SupplyRequest_ABC > > T_Requests;
    typedef std::deque< std::pair< SupplyRecipient_ABC*, T_Requests > >                     T_RecipientRequests;
    

    enum E_State
    {
        eConvoyWaitingForTransporters,
        eConvoySetup,
        eConvoyGoingToLoadingPoint,
        eConvoyLoading,
        eConvoyGoingToUnloadingPoint,
        eConvoyUnloading,
        eConvoyGoingBackToFormingPoint,
        eFinished
    };
    //@}

private:
    //! @name Operations
    //@{
    void UpdateTimer( unsigned timeRemaining );
    void SetState    ( E_State newState );
    bool IsActionDone( unsigned timeRemaining );
    SupplyRecipient_ABC* GetCurrentSupplyRecipient() const;
    void SupplyAndProceedWithNextRecipient();
    void SupplyCurrentRecipient();
    //@}

    //! @name FSM
    //@{
    void DoConvoyReserveTransporters();
    void DoConvoySetup();
    void DoConvoyMoveToSupplier();
    void DoConvoyLoad();
    void DoConvoyMoveToSupplyRecipient();
    void DoConvoyUnload();
    void DoConvoyMoveToTransportersProvider();
    //@}

    //! @name Network
    //@{
    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

private:
    unsigned long id_;
    unsigned long creationTick_;
    SupplySupplier_ABC& supplier_;
    E_State state_;
    unsigned currentStateEndTimeStep_;
    boost::shared_ptr< SupplyConvoy_ABC > convoy_;

    T_Resources resources_;

    // Requests
    T_RecipientRequests requestsQueued_;
    SupplyRecipient_ABC* currentRecipient_;

    // Network
    bool needNetworkUpdate_;
    bool requestsNeedNetworkUpdate_;

private:
    static MIL_IDManager idManager_; //$$ Still relevant ?
};

} // end namespace logistic

#endif // __SupplyConsign_ABC_h_
