// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Networker_h_
#define __Networker_h_

#include "network/ClientNetworker_ABC.h"

namespace DIN
{
    class DIN_Input;
}

namespace frontend
{
    class Master;

// =============================================================================
/** @class  Networker
    @brief  Networker
*/
// Created: SBO 2007-01-25
// =============================================================================
class Networker : public network::ClientNetworker_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Networker( const std::string& host );
    virtual ~Networker();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Networker( const Networker& );            //!< Copy constructor
    Networker& operator=( const Networker& ); //!< Assignment operator
    //@}

    //! @name Connection callbacks
    //@{
    virtual void OnConnected     ( DIN::DIN_Link& link );
    virtual void OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

    //! @name Messages callbacks
    //@{
    void OnReceiveMsgOutMaster( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    void OnReceiveMsgInit     ( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Master > master_;
    //@}
};

}

#endif // __Networker_h_
