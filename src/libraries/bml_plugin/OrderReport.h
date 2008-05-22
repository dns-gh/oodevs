// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderReport_h_
#define __OrderReport_h_

#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Agent;
    class Automat;
}

namespace bml
{
    class Publisher;
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
             OrderReport( const dispatcher::Agent& agent, const ASN1T_MsgUnitOrder& asn );
             OrderReport( const dispatcher::Automat& automat, const ASN1T_MsgAutomatOrder& asn );
    virtual ~OrderReport();
    //@}

    //! @name Operations
    //@{
    void Send( Publisher& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderReport( const OrderReport& );            //!< Copy constructor
    OrderReport& operator=( const OrderReport& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
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

#endif // __OrderReport_h_
