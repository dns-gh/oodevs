// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_CallsignResolver_ABC_h
#define plugins_hla_CallsignResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  CallsignResolver_ABC
    @brief  Callsign resolver definition
*/
// Created: SLI 2011-10-10
// =============================================================================
class CallsignResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CallsignResolver_ABC() {}
    virtual ~CallsignResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Add( unsigned long simulationIdentifier, const std::string& callsign, const std::string& uniqueId ) = 0;
    virtual std::string ResolveCallsign( unsigned long simulationIdentifier ) const = 0;
    virtual std::string ResolveUniqueId( unsigned long simulationIdentifier ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_CallsignResolver_ABC_h
