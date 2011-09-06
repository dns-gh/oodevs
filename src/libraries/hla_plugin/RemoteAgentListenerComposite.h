// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAgentListenerComposite_h
#define plugins_hla_RemoteAgentListenerComposite_h

#include "RemoteAgentListener_ABC.h"
#include <vector>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RemoteAgentListenerComposite
    @brief  Remote agent listener composite
*/
// Created: SLI 2011-09-02
// =============================================================================
class RemoteAgentListenerComposite : public RemoteAgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentListenerComposite();
    virtual ~RemoteAgentListenerComposite();
    //@}

    //! @name Operations
    //@{
    void Register( RemoteAgentListener_ABC& listener );
    void Unregister( RemoteAgentListener_ABC& listener );
    //@}

    //! @name Observer
    //@{
    virtual void Created( const std::string& identifier, const Aggregate_ABC& aggregate );
    virtual void Destroyed( const std::string& identifier, const Aggregate_ABC& aggregate );
    virtual void Moved( double latitude, double longitude );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< RemoteAgentListener_ABC* > T_Listeners;
    //@}

private:
    //! @name Member data
    //@{
    T_Listeners listeners_;
    //@}
};

}
}

#endif // __RemoteAgentListenerComposite_h_
