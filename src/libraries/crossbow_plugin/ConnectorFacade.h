// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ConnectorFacade_h_
#define __ConnectorFacade_h_

#include "game_asn/Asn.h"
#include "dispatcher/MessageHandler_ABC.h"
#include <list>

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
class ConnectorFacade : public dispatcher::MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ConnectorFacade( const dispatcher::Model& model, const dispatcher::Config& config, dispatcher::Publisher_ABC& client );
    virtual ~ConnectorFacade();
    //@}

    //! @name Operations
    //@{ 
    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
            void Send( dispatcher::Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ConnectorFacade( const ConnectorFacade& );            //!< Copy constructor
    ConnectorFacade& operator=( const ConnectorFacade& ); //!< Assignement operator
    //@}

    //! @name 
    //@{    
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
    typedef T_ListenerList::const_iterator      CIT_ListenerList;
    //@}

private:
    //! @name Member data
    //@{    
    std::auto_ptr< kernel::AgentTypes > types_;
    std::auto_ptr< kernel::OrderTypes > orderTypes_;
    std::auto_ptr< Connector >          connector_;
    T_ListenerList                      listeners_;
    bool                                bLoaded_;
    dispatcher::Publisher_ABC&          simulation_;
    //@}
};

}

#endif // __ConnectorFacade_h_
