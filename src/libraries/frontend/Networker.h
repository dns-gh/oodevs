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

#include "tools/ClientNetworker.h"
#include "Messages.h"
#include "Publisher_ABC.h"

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
class Networker : public tools::ClientNetworker
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
    bool IsConnected() const;
    virtual void Send( const ASN1T_MsgsInMaster& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Networker( const Networker& );            //!< Copy constructor
    Networker& operator=( const Networker& ); //!< Assignment operator
    //@}

    //! @name Connection callbacks
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionError    ( const std::string& address, const std::string& error );
    //@}

    //! @name Messages callbacks
    //@{
    void OnReceiveMsgOutMaster( const std::string& from, const ASN1T_MsgsOutMaster& message );
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
