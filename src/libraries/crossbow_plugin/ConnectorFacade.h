// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_ConnectorFacade_h_
#define __crossbow_ConnectorFacade_h_

#include "game_asn/Asn.h"
#include "dispatcher/MessageHandler_ABC.h"
#include <list>

namespace kernel
{
    class OrderTypes;
}

namespace dispatcher
{
    class Model;
    class Config;
    class SimulationPublisher_ABC;
}

namespace crossbow
{
    class Connector;
    class Listener_ABC;

// =============================================================================
/** @class  ConnectorFacade
    @brief  ConnectorFacade
*/
// Created: JCR 2007-04-30
// =============================================================================
class ConnectorFacade : public dispatcher::MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ConnectorFacade( const dispatcher::Model& model, const dispatcher::Config& config, dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~ConnectorFacade();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ConnectorFacade( const ConnectorFacade& );            //!< Copy constructor
    ConnectorFacade& operator=( const ConnectorFacade& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool IsRelevant( const ASN1T_MsgsSimToClient& asn ) const;
    void UpdateListeners() const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< Listener_ABC >   T_SharedListener;
    typedef std::list< T_SharedListener >       T_ListenerList;
    typedef T_ListenerList::iterator            IT_ListenerList;
    typedef T_ListenerList::const_iterator      CIT_ListenerList;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::OrderTypes > orderTypes_;
    std::auto_ptr< Connector >          connector_;
    T_ListenerList                      listeners_;
    bool                                loading_;
    //@}
};

}

#endif // __crossbow_ConnectorFacade_h_
