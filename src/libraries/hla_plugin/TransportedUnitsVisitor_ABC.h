// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportedUnitsVisitor_ABC_h
#define plugins_hla_TransportedUnitsVisitor_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  TransportedUnitsVisitor_ABC
    @brief  Transported units visitor definition
*/
// Created: SLI 2011-10-07
// =============================================================================
class TransportedUnitsVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportedUnitsVisitor_ABC() {}
    virtual ~TransportedUnitsVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const std::string& callsign, const std::string& uniqueId ) = 0;
    //@}
};

}
}

#endif // plugins_hla_TransportedUnitsVisitor_ABC_h
