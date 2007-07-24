// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConnectorFacade_h_
#define __ConnectorFacade_h_

#include "game_asn/Asn.h"

namespace kernel
{
    class AgentTypes;
    class OrderTypes;
}

namespace dispatcher
{
    class Model;
    class Config;
    class Publisher_ABC;
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
class ConnectorFacade
{

public:
    //! @name Constructors/Destructor
    //@{
             ConnectorFacade( const dispatcher::Model& model, const dispatcher::Config& config, dispatcher::Publisher_ABC& simulation );
    virtual ~ConnectorFacade();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgsSimToClient& asn );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ConnectorFacade( const ConnectorFacade& );            //!< Copy constructor
    ConnectorFacade& operator=( const ConnectorFacade& ); //!< Assignement operator
    //@}

    //! @name 
    //@{
    void ListenDatabaseEvents();
    void UpdateCurrentState();
    void UpdateOnTick( const ASN1T_MsgsSimToClient& asn );
    bool IsRelevant( const ASN1T_MsgsSimToClient& asn ) const;
    //@}

private:
    //! @name 
    //@{
    typedef boost::shared_ptr< Listener_ABC >   T_SharedListener;
    typedef std::list< T_SharedListener >       T_ListenerList;
    typedef T_ListenerList::iterator            IT_ListenerList;
    //@}

private:
    //! @name Member data
    //@{    
    std::auto_ptr< kernel::AgentTypes > types_;
    std::auto_ptr< kernel::OrderTypes > orderTypes_;
    std::auto_ptr< Connector >          connector_;
    T_ListenerList                      listeners_;
    bool                                bLoaded_;
    //@}
};

}

#endif // __ConnectorFacade_h_
