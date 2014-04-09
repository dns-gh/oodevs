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
#include "Checkpoints/SerializationTools.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include <tools/Map.h>
#include <deque>

class PHY_DotationCategory;
class MIL_Agent_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace sword
{
    enum LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus;
}

namespace logistic
{
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
             SupplyConsign();
    virtual ~SupplyConsign();
    //@}

    //! @name Operations
    //@{
    virtual void AddRequest( SupplyRecipient_ABC& recipient, boost::shared_ptr< SupplyRequest_ABC > request );
    virtual void Activate  ();
    virtual bool Update    ();
    virtual bool IsSupplying( const PHY_DotationCategory& dotationCategory, const SupplyRecipient_ABC& recipient );
    virtual bool IsFinished() const;
    virtual bool ResetConsignsForConvoyPion( const MIL_Agent_ABC& pion );
    virtual bool ResetConsignsForProvider( const MIL_Agent_ABC& pion );
    virtual void ResetConsign();
    virtual bool GrantsNothing() const;
    virtual void FinishSuccessfullyWithoutDelay();
    //@}

    //! @name TEST
    //@{
    virtual bool WillGoTo( const MIL_AutomateLOG& destination ) const;
    virtual bool IsAt    ( const MIL_AutomateLOG& destination ) const;
    virtual boost::shared_ptr< SupplyConvoy_ABC > GetConvoy() const;
    //@}

    //! @name SupplyConvoyEventsObserver_ABC
    //@{
    virtual void OnResourceAssignedToConvoy( const PHY_DotationCategory& dotationCategory, double quantity );
    virtual void OnResourceRemovedFromConvoy( const PHY_DotationCategory& dotationCategory, double quantity );
    virtual void OnConvoyEndMission();
    //@}

    //! @name Network - A refactorer
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    virtual void Clean           ();

    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& archive, const unsigned int );
    void save( MIL_CheckPointOutArchive& archive, const unsigned int ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Map< const PHY_DotationCategory*, double >                                 T_Resources;
    typedef tools::Map< const PHY_DotationCategory*, boost::shared_ptr< SupplyRequest_ABC > > T_Requests;
    typedef std::deque< std::pair< SupplyRecipient_ABC*, T_Requests > >                       T_RecipientRequests;
    //@}

private:
    //! @name Operations
    //@{
    void UpdateTimer( unsigned timeRemaining );
    void SetState    ( sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus newState );
    bool IsActionDone( unsigned timeRemaining );
    SupplyRecipient_ABC* GetCurrentSupplyRecipient() const;
    void SupplyAndProceedWithNextRecipient();
    void SupplyCurrentRecipient();
    void UpdateRequestsIfUnitDestroyed();
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
    SupplySupplier_ABC* supplier_;
    const MIL_Agent_ABC* provider_;
    sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus state_;
    unsigned int currentStateEndTimeStep_;
    boost::shared_ptr< SupplyConvoy_ABC > convoy_;
    T_Resources resources_;
    T_RecipientRequests requestsQueued_;
    SupplyRecipient_ABC* currentRecipient_;
    bool needNetworkUpdate_;
    bool requestsNeedNetworkUpdate_;
    bool instant_;
};

}

BOOST_CLASS_EXPORT_KEY( logistic::SupplyConsign )

#endif // __SupplyConsign_ABC_h_
