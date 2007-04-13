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
    class SimulationDispatcher;
    class ClientsNetworker;
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
             LoaderFacade( ClientsNetworker& clients, SimulationDispatcher& simulation, const std::string& directory );
    virtual ~LoaderFacade();
    //@}

    //! @name Operations
    //@{
    void Update();

    void OnReceive( const ASN1T_MsgsOutClient& asnInMsg );
    void Send( Publisher_ABC& publisher ) const;
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
    ClientsNetworker& clients_;

    std::auto_ptr< Loader > loader_;
    unsigned factor_;
    bool running_;
    int skipToFrame_;

    MT_TimerManager manager_;
    //@}
};

}

#endif // __LoaderFacade_h_
