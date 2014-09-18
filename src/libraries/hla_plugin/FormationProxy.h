// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_FormationProxy_h
#define plugins_hla_FormationProxy_h

#include "Agent_ABC.h"
#include "dispatcher/Observer.h"
#include "EventListenerComposite.h"
#include "protocol/Protocol.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>

namespace dispatcher
{
    class Formation_ABC;
}

namespace plugins
{
namespace hla
{
    class LocalAgentResolver_ABC;
    struct ChildListener;

// =============================================================================
/** @class  FormationProxy
    @brief  Formation proxy
*/
// Created: AHC 2012-10-02
// =============================================================================
class FormationProxy : public Agent_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationProxy( dispatcher::Formation_ABC& agent, const LocalAgentResolver_ABC& localAgentResolver );
    virtual ~FormationProxy();
    //@}

    //! @name Registration
    //@{
    virtual void Register( EventListener_ABC& listener );
    virtual void Unregister( EventListener_ABC& listener );
    virtual bool HasSubordinate( unsigned int id ) const;
    virtual void AddSubordinate( unsigned int id, Agent_ABC& agent );
    virtual void RemoveSubordinate( unsigned int id );
    virtual void PublishParent();
    //@}

private:
    //! @name Observer
    //@{
    //@}
    
    //! @name Helpers
    //@{
    void NotifyChildren();
    void UpdateLocationCallback();
    //@}
private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, Agent_ABC* > T_Subordinates;
    typedef std::map< unsigned int, boost::shared_ptr< ChildListener > > T_ChildrenListeners;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Formation_ABC& agent_;
    const LocalAgentResolver_ABC& localAgentResolver_;
    EventListenerComposite listeners_;
    T_Subordinates subordinates_;
    T_ChildrenListeners childrenListeners_;
    
    //@}
};

}
}

#endif // plugins_hla_FormationProxy_h
