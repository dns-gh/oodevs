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
#include "protocol/protocol.h"

using namespace Common;


namespace kernel
{
    class MissionType;
}

namespace dispatcher
{
    class Agent;
    class Automat;
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
    OrderReport* CreateOrderReport( const dispatcher::Agent& entity, const MsgUnitOrder& asn ) const;
    OrderReport* CreateOrderReport( const dispatcher::Automat& entity, const MsgAutomatOrder& asn ) const;
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
