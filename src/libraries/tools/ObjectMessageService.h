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

#include "MessageDispatcher_ABC.h"
#include <boost/function.hpp>
#include <map>

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
{
private:
    //! @name Types
    //@{
    typedef boost::function< void( const std::string&, const std::string& ) > T_Callback;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ObjectMessageService();
    virtual ~ObjectMessageService();
    //@}

    //! @name Operations
    //@{
    using MessageDispatcher_ABC::RegisterMessage;
    virtual ObjectMessageCallback_ABC* Retrieve( unsigned long id );
    virtual void Register( unsigned long id, std::auto_ptr< ObjectMessageCallback_ABC > callback );

    void OnMessage( const std::string& endpoint, Message& message );
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetNbMessagesReceived() const { return nbMessagesReceived_; }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectMessageService( const ObjectMessageService& );            //!< Copy constructor
    ObjectMessageService& operator=( const ObjectMessageService& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, ObjectMessageCallback_ABC* > T_Callbacks;
    typedef T_Callbacks::const_iterator                        CIT_Callbacks;
    //@}

private:
    //! @name Member data
    //@{
    T_Callbacks callbacks_;
    unsigned long nbMessagesReceived_;
    //@}
};

}

#endif // __ObjectMessageService_h_
