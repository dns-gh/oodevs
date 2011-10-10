// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Subordinates_ABC_h
#define plugins_hla_Subordinates_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    class TransportedUnitsVisitor_ABC;

// =============================================================================
/** @class  Subordinates_ABC
    @brief  Subordinates definition
*/
// Created: SLI 2011-10-10
// =============================================================================
class Subordinates_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Subordinates_ABC() {}
    virtual ~Subordinates_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( unsigned long automatIdentifier, TransportedUnitsVisitor_ABC& visitor ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_Subordinates_ABC_h
