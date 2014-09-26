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
#include <xeumeuleu/streams/xisubstream.hpp>
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
             CallsignResolver( xml::xisubstream xis );
    virtual ~CallsignResolver();
    //@}

    //! @name Operations
    //@{
    virtual std::string Generate( const std::string& name, unsigned long simId ) const;
    virtual void Add( unsigned long simulationIdentifier, const std::string& callsign, const std::vector< char >& uniqueId );
    virtual std::string ResolveCallsign( unsigned long simulationIdentifier ) const;
    virtual std::vector< char > ResolveUniqueId( unsigned long simulationIdentifier ) const;
    virtual unsigned long ResolveSimulationIdentifier( const std::vector< char >& uniqueId ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::string, std::vector< char > > T_NetnAttributes;
    typedef std::map< unsigned long, T_NetnAttributes > T_Identifiers;
    typedef std::map< std::vector< char >, unsigned long > T_SimulationIdentifiers;
    //@}

    //! @name Member data
    //@{
    bool addIdentifier_;
    T_Identifiers identifiers_;
    T_SimulationIdentifiers simulationIdentifiers_;
    //@}
};

}
}

#endif // plugins_hla_CallsignResolver_h
