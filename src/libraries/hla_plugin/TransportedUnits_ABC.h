// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportedUnits_ABC_h
#define plugins_hla_TransportedUnits_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    class TransportedUnitsVisitor_ABC;

// =============================================================================
/** @class  TransportedUnits_ABC
    @brief  Transported units definition
*/
// Created: SLI 2011-10-07
// =============================================================================
class TransportedUnits_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportedUnits_ABC() {}
    virtual ~TransportedUnits_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( TransportedUnitsVisitor_ABC& visitor ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_TransportedUnits_ABC_h
