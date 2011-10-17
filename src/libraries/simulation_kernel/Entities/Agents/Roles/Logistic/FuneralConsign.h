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

#include "FuneralConsign_ABC.h"
#include "MT_Tools/MT_Vector2DTypes.h"
#include "Tools/MIL_IDManager.h"

namespace logistic {
    class FuneralRequest_ABC;
    class FuneralHandler_ABC;
    class FuneralPackagingResource;
    class SupplyConvoy_ABC;

//$$$$$ DEGUEU TEST
class LogisticVirtualAction
{
public:
    LogisticVirtualAction();
    ~LogisticVirtualAction();
    unsigned GetTimeRemaining( unsigned actionId, unsigned duration );

private:
    unsigned currentActionId_;
    unsigned timeRemainingForCurrentAction_;
};

// =============================================================================
/** @class  FuneralConsign_ABC
    @brief  FuneralConsign_ABC
*/
// Created: NLD 2011-08-24
// =============================================================================
class FuneralConsign : public FuneralConsign_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FuneralConsign( boost::shared_ptr< FuneralRequest_ABC > request );
    virtual ~FuneralConsign();
    //@}

    //! @name Operations
    //@{
    virtual bool Update();
    virtual void Cancel();
    //@}

    //! @name Events
    //@{
    virtual void OnSupplyConvoyArriving( boost::shared_ptr< const SupplyConsign_ABC > supplyConsign );
    virtual void OnSupplyConvoyLeaving ( boost::shared_ptr< const SupplyConsign_ABC > supplyConsign );
    //@}

    //! @name Network - A refactorer
    //@{
    virtual void SendChangedState() const;
    virtual void SendFullState   ( unsigned int context ) const;
    virtual void Clean           ();
    //@}

private:
    //! @name Types
    //@{
    enum E_State
    {
        eWaitingForHandling,
        eTransportingUnpackaged,
        eWaitingForPackaging,
        ePackaging,
        eWaitingForTransporter,
        eTransportingPackaged,
        eFinished
    };
    //@}

private:
    //! @name Network
    //@{
    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

    //! @name Operations
    //@{
    void UpdateTimer ( unsigned timeRemaining );
    void SetState    ( E_State newState );
    bool IsActionDone( unsigned timeRemaining );

    void DoWaitForHandling    ();
    void DoTransportUnpackaged();
    void DoWaitForPackaging   ();
    void DoPackage            ();
    void DoTransitionAfterPackaging();
    
    unsigned MoveTo( const MT_Vector2D& position );
    //@}

private:
    unsigned long id_;
    unsigned long creationTick_;
    boost::shared_ptr< FuneralRequest_ABC > request_;
    FuneralHandler_ABC* handler_; // pkoi pas shared_ptr ??
    boost::shared_ptr< SupplyConvoy_ABC > convoy_;
    const FuneralPackagingResource* packaging_;

    E_State state_;
    unsigned currentStateEndTimeStep_;
    LogisticVirtualAction currentAction_;
    MT_Vector2D position_; //$$$ TMP - wrapper dans un 'convoi virtuel'

    // Network
    bool needNetworkUpdate_;

private:
    static MIL_IDManager idManager_; //$$ Still relevant ?
};

} // end namespace logistic

#endif // __FuneralConsign_h_
