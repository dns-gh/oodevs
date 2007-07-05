// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LoaderFacade_h_
#define __LoaderFacade_h_

#include "MT/MT_Time/MT_Timer_ABC.h"
#include "MT/MT_Time/MT_TimerManager.h"
#include "game_asn/Asn.h"

namespace dispatcher
{
    class Loader;
    class Publisher_ABC;

// =============================================================================
/** @class  LoaderFacade
    @brief  LoaderFacade
*/
// Created: AGE 2007-04-11
// =============================================================================
class LoaderFacade : private MT_Timer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LoaderFacade( Publisher_ABC& clients, Loader& loader );
    virtual ~LoaderFacade();
    //@}

    //! @name Operations
    //@{
    void Update();
    void SendReplayInfo( Publisher_ABC& publisher ) const;

    void OnReceive( const ASN1T_MsgsClientToSim&    asnInMsg );
    void OnReceive( const ASN1T_MsgsClientToMiddle& asnInMsg );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OnTimer();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LoaderFacade( const LoaderFacade& );            //!< Copy constructor
    LoaderFacade& operator=( const LoaderFacade& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ChangeTimeFactor( unsigned factor );
    void TogglePause( bool pause );
    void SkipToFrame( unsigned frame );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& clients_;
    Loader& loader_;

    unsigned factor_;
    bool running_;
    int skipToFrame_;

    MT_TimerManager manager_;
    //@}
};

}

#endif // __LoaderFacade_h_
