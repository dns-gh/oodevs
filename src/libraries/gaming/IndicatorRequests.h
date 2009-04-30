// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IndicatorRequests_h_
#define __IndicatorRequests_h_

#include "clients_kernel/Iterator.h"
#include "game_asn/Aar.h"

namespace kernel
{
    class Controller;
}

class IndicatorDefinition_ABC;
class IndicatorRequest;
class Publisher_ABC;
class Score;

// =============================================================================
/** @class  IndicatorRequests
    @brief  IndicatorRequests
*/
// Created: AGE 2007-09-25
// =============================================================================
class IndicatorRequests
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorRequests( kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~IndicatorRequests();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Update( const ASN1T_MsgPlotResult& asnMsg );

    IndicatorRequest& CreateRequest( const IndicatorDefinition_ABC& definition );
    kernel::Iterator< const IndicatorRequest& > CreateIterator();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorRequests( const IndicatorRequests& );            //!< Copy constructor
    IndicatorRequests& operator=( const IndicatorRequests& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< IndicatorRequest* > T_Requests;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    T_Requests requests_;
    //@}
};

#endif // __IndicatorRequests_h_
