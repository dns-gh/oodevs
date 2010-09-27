// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ClientReplayNetworker_h_
#define __ClientReplayNetworker_h_

#include "tools/ClientNetworker.h"

// =============================================================================
/** @class  ClientReplayNetworker
    @brief  ClientReplayNetworker
*/
// Created: HBD 2010-09-13
// =============================================================================
class ClientReplayNetworker : public tools::ClientNetworker
{

public:
    //! @name Constructors/Destructor
    //@{
             ClientReplayNetworker( bool& ready, const std::string& host = "", bool retry = false );
    virtual ~ClientReplayNetworker();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    //@}

protected:
    //! @name Copy/Assignment
    //@{q
    ClientReplayNetworker( const ClientReplayNetworker& );            //!< Copy constructor
    ClientReplayNetworker& operator=( const ClientReplayNetworker& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    bool& ready_;
    //@}
};

#endif // __ClientReplayNetworker_h_
