// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FuneralConsign_h_
#define __FuneralConsign_h_

#include "SupplyConvoysObserver_ABC.h"
#include "MT_Tools/MT_Vector2DTypes.h"

class Human_ABC;

namespace sword
{
    enum sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus;
}

namespace logistic
{
    class FuneralHandler_ABC;
    class FuneralPackagingResource;
    class SupplyConvoy_ABC;

class LogisticVirtualAction
{
public:
    LogisticVirtualAction();
    unsigned GetTimeRemaining( unsigned actionId, unsigned duration );

private:
    unsigned currentActionId_;
    unsigned timeRemainingForCurrentAction_;
};

// =============================================================================
/** @class  FuneralConsign
    @brief  FuneralConsign
*/
// Created: NLD 2011-08-24
// =============================================================================
class FuneralConsign : public SupplyConvoysObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FuneralConsign( Human_ABC& human );
    virtual ~FuneralConsign();
    //@}

    //! @name Operations
    //@{
    bool Update();
    bool IsFinished() const;

    virtual void FinishSuccessfullyWithoutDelay();
    //@}

    //! @name Events
    //@{
    virtual void OnSupplyConvoyArriving( const boost::shared_ptr< const SupplyConsign_ABC >& supplyConsign );
    virtual void OnSupplyConvoyLeaving ( const boost::shared_ptr< const SupplyConsign_ABC >& supplyConsign );
    //@}

    //! @name Network
    //@{
    void SendChangedState() const;
    void SendFullState( unsigned int context ) const;
    void Clean();
    //@}

private:
    //! @name Network
    //@{
    void SendMsgCreation() const;
    void SendMsgDestruction() const;
    //@}

    //! @name Operations
    //@{
    void UpdateTimer( unsigned timeRemaining );
    void SetState( sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus newState );
    bool IsActionDone( unsigned timeRemaining );

    void DoWaitForHandling();
    void DoTransportUnpackaged();
    void DoWaitForPackaging();
    void DoPackage();
    void DoTransitionAfterPackaging();
    
    unsigned MoveTo( const MT_Vector2D& position );
    //@}

private:
    unsigned long id_;
    unsigned long creationTick_;
    Human_ABC& human_;
    FuneralHandler_ABC* handler_;
    boost::shared_ptr< SupplyConvoy_ABC > convoy_;
    const FuneralPackagingResource* packaging_;

    sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus state_;
    unsigned currentStateEndTimeStep_;
    LogisticVirtualAction currentAction_;
    MT_Vector2D position_; //$$$ TMP - wrapper dans un 'convoi virtuel'

    bool needNetworkUpdate_;
};

}

#endif // __FuneralConsign_h_
