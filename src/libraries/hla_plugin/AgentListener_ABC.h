// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AgentListener_ABC_h
#define plugins_hla_AgentListener_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    class Agent_ABC;

// =============================================================================
/** @class  AgentListener_ABC
    @brief  Agent listener definition
*/
// Created: SLI 2011-01-10
// =============================================================================
class AgentListener_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentListener_ABC() {}
    virtual ~AgentListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Created( Agent_ABC& agent ) = 0;
    //@}
};

}
}

#endif // plugins_hla_AgentListener_ABC_h
