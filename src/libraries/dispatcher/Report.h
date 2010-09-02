// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Report_h_
#define __Report_h_

#include "SimpleEntity.h"

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace MsgsSimToClient
{
    class MsgReport;
}

namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Report
    @brief  Report
*/
// Created: AGE 2007-10-19
// =============================================================================
class Report : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{
             Report( Model_ABC&, const MsgsSimToClient::MsgReport& report );
    virtual ~Report();
    //@}

    //! @name Operations
    //@{
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Report( const Report& );            //!< Copy constructor
    Report& operator=( const Report& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< MsgsSimToClient::MsgReport > message_;
    //@}
};

}

#endif // __Report_h_
