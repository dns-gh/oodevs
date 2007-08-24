// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectMessageService_h_
#define __ObjectMessageService_h_

#include "DIN/MessageService/DIN_MessageService.h"
#include "MessageIdentifierFactory.h"
#include "MessageDispatcher_ABC.h"
#include "AsnMessageEncoder.h"
#include <boost/function.hpp>

namespace tools
{
// =============================================================================
/** @class  ObjectMessageService
    @brief  Object message service
*/
// Created: AGE 2007-03-07
// =============================================================================
class ObjectMessageService : public DIN::DIN_MessageService, public MessageDispatcher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectMessageService( DIN::DIN_Engine& engine, const DIN::DIN_Connector_ABC& connector );
    virtual ~ObjectMessageService();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void Send( DIN::DIN_Link& link, const T& message );
    template< typename T >
    void Send( DIN::DIN_Link& link, const T& , const DIN::DIN_BufferedMessage& message );

    using MessageDispatcher_ABC::RegisterMessage;
    using DIN_MessageService::Enable;
    using DIN_MessageService::Disable;

    void SetCallbackOnError( const boost::function2< bool, DIN::DIN_Link&, const DIN::DIN_ErrorDescription& >& callback );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback );
    virtual bool OnError( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectMessageService( const ObjectMessageService& );            //!< Copy constructor
    ObjectMessageService& operator=( const ObjectMessageService& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< ObjectMessageCallback_ABC* > T_Callbacks;
    typedef T_Callbacks::const_iterator             CIT_Callbacks;
    //@}

private:
    //! @name Member data
    //@{
    T_Callbacks callbacks_;
    boost::function2< bool, DIN::DIN_Link&, const DIN::DIN_ErrorDescription& > error_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ObjectMessageService::Send
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
template< typename T >
void ObjectMessageService::Send( DIN::DIN_Link& link, const T& message )
{
    static const unsigned long id = MessageIdentifierFactory::GetIdentifier< T >();
    AsnMessageEncoder< T > asnEncoder( *this, message );
    DIN::DIN_MessageService::Send( link, id, asnEncoder.GetDinMsg() );
}

// -----------------------------------------------------------------------------
// Name: ObjectMessageService::Send
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
template< typename T >
void ObjectMessageService::Send( DIN::DIN_Link& link, const T& , const DIN::DIN_BufferedMessage& message )
{
    static const unsigned long id = MessageIdentifierFactory::GetIdentifier< T >();
    DIN::DIN_MessageService::Send( link, id, message );
}

}

#endif // __ObjectMessageService_h_
