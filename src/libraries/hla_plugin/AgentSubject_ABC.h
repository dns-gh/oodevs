// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AgentSubject_ABC_h
#define plugins_hla_AgentSubject_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    class AgentListener_ABC;

// =============================================================================
/** @class  AgentSubject_ABC
    @brief  Agent subject definition
*/
// Created: SLI 2011-01-10
// =============================================================================
class AgentSubject_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentSubject_ABC() {}
    virtual ~AgentSubject_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( AgentListener_ABC& listener ) = 0;
    virtual void Unregister( AgentListener_ABC& listener ) = 0;
    //@}
};

}
}

#endif // plugins_hla_AgentSubject_ABC_h
