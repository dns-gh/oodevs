// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __OrderReport_h_
#define __OrderReport_h_

#include "tools/Resolver_ABC.h"
#include "protocol/protocol.h"

namespace kernel
{
    class MissionType;
}

namespace dispatcher
{
    class Agent_ABC;
    class Automat_ABC;
}

namespace plugins
{
namespace bml
{
    class Publisher_ABC;
    class Who;
    class ReportingData;

// =============================================================================
/** @class  OrderReport
    @brief  OrderReport
*/
// Created: SBO 2008-05-22
// =============================================================================
class OrderReport
{
public:
    //! @name Constructors/Destructor
    //@{
             OrderReport( const dispatcher::Agent_ABC& agent, const tools::Resolver_ABC< kernel::MissionType >& missions, const Common::MsgUnitOrder& asn );
             OrderReport( const dispatcher::Automat_ABC& automat, const tools::Resolver_ABC< kernel::MissionType >& missions, const Common::MsgAutomatOrder& asn );
    virtual ~OrderReport();
    //@}

    //! @name Operations
    //@{
    void Send( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderReport( const OrderReport& );            //!< Copy constructor
    OrderReport& operator=( const OrderReport& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Who > who_;
    std::auto_ptr< ReportingData > reportingData_;
    std::string activityCode_;
    //@}
};

}
}

#endif // __OrderReport_h_
