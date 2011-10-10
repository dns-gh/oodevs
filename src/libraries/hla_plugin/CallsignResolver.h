// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_CallsignResolver_h
#define plugins_hla_CallsignResolver_h

#include "CallsignResolver_ABC.h"
#include <map>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  CallsignResolver
    @brief  Callsign resolver
*/
// Created: SLI 2011-10-10
// =============================================================================
class CallsignResolver : public CallsignResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CallsignResolver();
    virtual ~CallsignResolver();
    //@}

    //! @name Operations
    //@{
    virtual void Add( unsigned long simulationIdentifier, const std::string& callsign, const std::string& uniqueId );
    virtual std::string ResolveCallsign( unsigned long simulationIdentifier ) const;
    virtual std::string ResolveUniqueId( unsigned long simulationIdentifier ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, std::string > T_NetnAttributes;
    typedef std::map< unsigned long, T_NetnAttributes > T_Identifiers;
    //@}

    //! @name Member data
    //@{
    T_Identifiers identifiers_;
    //@}
};

}
}

#endif // plugins_hla_CallsignResolver_h
