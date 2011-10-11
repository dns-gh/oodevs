// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Transporters_ABC_h
#define plugins_hla_Transporters_ABC_h

#include <boost/noncopyable.hpp>
#include <string>
#include <geometry/Types.h>

namespace plugins
{
namespace hla
{
    class TransportedUnitsVisitor_ABC;

// =============================================================================
/** @class  Transporters_ABC
    @brief  Transporters definition
*/
// Created: SLI 2011-10-11
// =============================================================================
class Transporters_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Transporters_ABC() {}
    virtual ~Transporters_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( const std::string& transportedUnitUniqueId, const geometry::Point2d& embarkmentPoint, TransportedUnitsVisitor_ABC& visitor ) const = 0;
    //@}
};

}
}

#endif // Transporters_ABC_h
