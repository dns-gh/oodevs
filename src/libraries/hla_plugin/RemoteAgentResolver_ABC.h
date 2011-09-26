// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef RemoteAgentResolver_ABC_h
#define RemoteAgentResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RemoteAgentResolver_ABC
    @brief  Remote agent resolver definition
*/
// Created: SLI 2011-09-22
// =============================================================================
class RemoteAgentResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentResolver_ABC() {}
    virtual ~RemoteAgentResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string Resolve( unsigned int identifier ) const = 0;
    virtual unsigned int Resolve( const std::string& identifier ) const = 0;
    //@}
};

}
}

#endif // RemoteAgentResolver_ABC_h
