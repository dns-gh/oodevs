// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AutomatChecker_h
#define plugins_hla_AutomatChecker_h

#include "AutomatChecker_ABC.h"

namespace tools
{
    template< typename T, typename Identifier > class Resolver_ABC;
}

namespace dispatcher
{
    class Agent_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AutomatChecker
    @brief  Automat checker
*/
// Created: SLI 2011-10-12
// =============================================================================
class AutomatChecker : public AutomatChecker_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatChecker( const tools::Resolver_ABC< dispatcher::Agent_ABC, unsigned long >& resolver );
    virtual ~AutomatChecker();
    //@}

    //! @name Operations
    //@{
    virtual bool IsAutomatDisengaged( unsigned int unitIdentifier ) const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< dispatcher::Agent_ABC, unsigned long >& resolver_;
    //@}
};

}
}

#endif // plugins_hla_AutomatChecker_h
