// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportationListener_ABC_h
#define plugins_hla_TransportationListener_ABC_h

#include <boost/noncopyable.hpp>
#include <geometry/Types.h>

namespace plugins
{
namespace hla
{
    class TransportedUnits_ABC;

// =============================================================================
/** @class  TransportationListener_ABC
    @brief  TransportationListener definition
*/
// Created: SLI 2011-10-06
// =============================================================================
class TransportationListener_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationListener_ABC() {}
    virtual ~TransportationListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ConvoyRequested( const std::string& carrier, long long embarkmentTime, const geometry::Point2d& embarkmentPoint,
                                  long long disembarkmentTime, const geometry::Point2d& disembarkmentPoint,
                                  const TransportedUnits_ABC& transportedUnits ) = 0;
    //@}
};

}
}

#endif // plugins_hla_TransportationListener_ABC_h
