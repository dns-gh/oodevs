// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionRequests_h_
#define __AfterActionRequests_h_

#include "clients_kernel/Iterator.h"
#include "game_asn/asn.h"
namespace kernel
{
    class Controller;
}
class Publisher_ABC;
class AfterActionFunction;
class AfterActionRequest;

// =============================================================================
/** @class  AfterActionRequests
    @brief  AfterActionRequests
*/
// Created: AGE 2007-09-25
// =============================================================================
class AfterActionRequests
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionRequests( kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~AfterActionRequests();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Update( const ASN1T_MsgIndicatorResult& asnMsg );

    AfterActionRequest& CreateRequest( const AfterActionFunction& function );
    kernel::Iterator< const AfterActionRequest& > CreateIterator();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionRequests( const AfterActionRequests& );            //!< Copy constructor
    AfterActionRequests& operator=( const AfterActionRequests& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< AfterActionRequest* > T_Requests;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    T_Requests requests_;
    //@}
};

#endif // __AfterActionRequests_h_
