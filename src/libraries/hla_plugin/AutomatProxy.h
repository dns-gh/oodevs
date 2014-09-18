// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AutomatProxy_h
#define plugins_hla_AutomatProxy_h

#include "Agent_ABC.h"
#include "dispatcher/Observer.h"
#include "EventListenerComposite.h"
#include "protocol/Protocol.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace dispatcher
{
    class Automat_ABC;
}

namespace plugins
{
namespace hla
{
    class LocalAgentResolver_ABC;
    struct ChildListener;

// =============================================================================
/** @class  AutomatProxy
    @brief  Automayt proxy
*/
// Created: AHC 2012-10-02
// =============================================================================
class AutomatProxy : public Agent_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatProxy( dispatcher::Automat_ABC& agent, const LocalAgentResolver_ABC& localAgentResolver );
    virtual ~AutomatProxy();
    //@}

    //! @name Registration
    //@{
    virtual void Register( EventListener_ABC& listener );
    virtual void Unregister( EventListener_ABC& listener );
    virtual bool HasSubordinate( unsigned int id ) const;
    virtual void AddSubordinate( unsigned int id, Agent_ABC& agent );
    virtual void RemoveSubordinate( unsigned int id );
    //@}

private:
    //! @name Observer
    //@{
    //@}
        
    //! @name Helpers
    //@{
    void NotifyChildren();
    void UpdateLocationCallback( const ChildListener& );
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
    const dispatcher::Automat_ABC& agent_;
    const LocalAgentResolver_ABC& localAgentResolver_;
    EventListenerComposite listeners_;
    T_Subordinates subordinates_;
    T_ChildrenListeners childrenListeners_;
    //@}
};

}
}

#endif // plugins_hla_AutomatProxy_h
