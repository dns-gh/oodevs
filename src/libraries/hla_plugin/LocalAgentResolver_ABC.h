// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_LocalAgentResolver_ABC_h
#define plugins_hla_LocalAgentResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  LocalAgentResolver_ABC
    @brief  Local agent resolver definition
*/
// Created: SLI 2011-09-26
// =============================================================================
class LocalAgentResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LocalAgentResolver_ABC() {}
    virtual ~LocalAgentResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Add( unsigned int simulationIdentifier, const std::string& objectIdentifier ) = 0;
    virtual std::string Resolve( unsigned int simulationIdentifier ) const = 0;
    virtual unsigned int Resolve( const std::string& objectIdentifier ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_LocalAgentResolver_ABC_h
