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

#include "tools/ClientNetworker_ABC.h"
#include "Messages.h"
#include "Publisher_ABC.h"

namespace DIN
{
    class DIN_Input;
}

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class Master;
    class Model;
    class Profile;

// =============================================================================
/** @class  Networker
    @brief  Networker
*/
// Created: SBO 2007-01-25
// =============================================================================
class Networker : public tools::ClientNetworker_ABC
                , public Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Networker( const std::string& host, kernel::Controller& controller, Model& model, Profile& profile );
    virtual ~Networker();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsInMaster& message );
    virtual DIN::DIN_BufferedMessage BuildDinMsg();
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
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller&     controller_;
    std::auto_ptr< Master > master_;
    Model&                  model_;
    Profile&                profile_;
    //@}
};

}

#endif // __Networker_h_
