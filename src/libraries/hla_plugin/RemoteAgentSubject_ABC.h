// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAgentSubject_ABC_h
#define plugins_hla_RemoteAgentSubject_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    class RemoteAgentListener_ABC;

// =============================================================================
/** @class  RemoteAgentSubject_ABC
    @brief  Remote agent subject definition
*/
// Created: VPR 2011-09-07
// =============================================================================
class RemoteAgentSubject_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentSubject_ABC() {}
    virtual ~RemoteAgentSubject_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( RemoteAgentListener_ABC& listener ) = 0;
    virtual void Unregister( RemoteAgentListener_ABC& listener ) = 0;
    //@}
};

}
}

#endif // plugins_hla_RemoteAgentSubject_ABC_h
