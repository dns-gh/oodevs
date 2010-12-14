// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportFactory_h_
#define __ReportFactory_h_

#include "tools/Resolver_ABC.h"
#include "protocol/Protocol.h"

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
    class OrderReport;

// =============================================================================
/** @class  ReportFactory
    @brief  ReportFactory
*/
// Created: SBO 2008-05-23
// =============================================================================
class ReportFactory
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ReportFactory( const tools::Resolver_ABC< kernel::MissionType >& missions );
    virtual ~ReportFactory();
    //@}

    //! @name Operations
    //@{
    OrderReport* CreateOrderReport( const dispatcher::Agent_ABC& entity, const sword::UnitOrder& asn ) const;
    OrderReport* CreateOrderReport( const dispatcher::Automat_ABC& entity, const sword::AutomatOrder& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReportFactory( const ReportFactory& );            //!< Copy constructor
    ReportFactory& operator=( const ReportFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::MissionType >& missions_;
    //@}
};

}
}

#endif // __ReportFactory_h_
