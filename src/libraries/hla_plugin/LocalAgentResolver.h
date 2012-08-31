// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_LocalAgentResolver_h
#define plugins_hla_LocalAgentResolver_h

#include "LocalAgentResolver_ABC.h"
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  LocalAgentResolver
    @brief  Local agent resolver
*/
// Created: SLI 2011-09-26
// =============================================================================
class LocalAgentResolver : public LocalAgentResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LocalAgentResolver();
    virtual ~LocalAgentResolver();
    //@}

    //! @name Operations
    //@{
    virtual void Add( unsigned long simulationIdentifier, const std::string& objectIdentifier );
    virtual std::string Resolve( unsigned long simulationIdentifier ) const;
    virtual unsigned long Resolve( const std::string& objectIdentifier ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::bimap< unsigned long, std::string > T_Identifiers;
    //@}

private:
    //! @name Member data
    //@{
    T_Identifiers identifiers_;
    //@}
};

}
}

#endif // plugins_hla_LocalAgentResolver_h
