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

#include "MessageIdentifierFactory.h"
#include "MessageDispatcher_ABC.h"
#include "AsnMessageEncoder.h"
#include "MessageCallback_ABC.h"
#include <boost/function.hpp>
#include <vector>

namespace tools
{
    class Message;

// =============================================================================
/** @class  ObjectMessageService
    @brief  Object message service
*/
// Created: AGE 2007-03-07
// =============================================================================
class ObjectMessageService : public MessageDispatcher_ABC
                           , public MessageCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectMessageService();
    virtual ~ObjectMessageService();
    //@}

    //! @name Operations
    //@{
    using MessageDispatcher_ABC::RegisterMessage;

    void RegisterErrorCallback( const boost::function2< void, const std::string&, const std::string& >& error );

    virtual ObjectMessageCallback_ABC* Retrieve( unsigned long id );
    virtual void Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectMessageService( const ObjectMessageService& );            //!< Copy constructor
    ObjectMessageService& operator=( const ObjectMessageService& ); //!< Assignement operator
    //@}

    //! @name Operations
    //@{
    virtual void OnError  ( const std::string& endpoint, const std::string& error );
    virtual void OnMessage( const std::string& endpoint, Message& message );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, ObjectMessageCallback_ABC* > T_Callbacks;
    typedef T_Callbacks::const_iterator                        CIT_Callbacks;
    //@}

private:
    //! @name Member data
    //@{
    T_Callbacks callbacks_;
    boost::function2< void, const std::string&, const std::string& > error_;
    //@}
};

}

#endif // __ObjectMessageService_h_
