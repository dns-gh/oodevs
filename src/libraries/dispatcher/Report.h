// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
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
    enum EnumReportType;
    class MsgReport;
}

namespace dispatcher
{
    class MissionParameter_ABC;
    class Model;
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
             Report( Model&, const MsgsSimToClient::MsgReport& report );
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

    //! @name Helpers
    //@{
    typedef std::vector< MissionParameter_ABC* >  T_Parameters;
    typedef T_Parameters::const_iterator        CIT_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long                   id_;
    const unsigned long                   emitter_;
    const unsigned long                   report_;
    const MsgsSimToClient::EnumReportType type_;
    std::string                           date_;
    T_Parameters                          parameters_;
    Model&                                model_;
    //@}
};

}

#endif // __Report_h_
