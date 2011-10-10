// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Subordinates_h
#define plugins_hla_Subordinates_h

#include "Subordinates_ABC.h"

namespace dispatcher
{
    class Automat_ABC;
}

namespace tools
{
    template< typename T, typename Identifier > class Resolver_ABC;
}

namespace plugins
{
namespace hla
{
    class CallsignResolver_ABC;

// =============================================================================
/** @class  Subordinates
    @brief  Subordinates
*/
// Created: SLI 2011-10-10
// =============================================================================
class Subordinates : public Subordinates_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Subordinates( const CallsignResolver_ABC& callsignResolver, const tools::Resolver_ABC< dispatcher::Automat_ABC, unsigned long >& automatResolver );
    virtual ~Subordinates();
    //@}

    //! @name Operations
    //@{
    virtual void Apply( unsigned long automatIdentifier, TransportedUnitsVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const CallsignResolver_ABC& callsignResolver_;
    const tools::Resolver_ABC< dispatcher::Automat_ABC, unsigned long >& automatResolver_;
    //@}
};

}
}

#endif // plugins_hla_Subordinates_h
