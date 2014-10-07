// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_MessageHandler_h
#define tools_MessageHandler_h

#include "MessageHandler_ABC.h"
#include "MessageObserver_ABC.h"
#include "MessageController_ABC.h"

namespace tools
{
// =============================================================================
/** @class  MessageHandler
    @brief  Message handler
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category, typename Message >
class MessageHandler : public MessageHandler_ABC< Category >
{
public:
    //! @name Types
    //@{
    typedef bool( Category::*T_Checker )() const;
    typedef const Message&( Category::*T_Retriever )() const;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    MessageHandler( MessageController_ABC< Category >& controller, MessageObserver_ABC< Message >& observer,
                    T_Checker checker, T_Retriever retriever )
        : controller_( controller )
        , observer_  ( observer )
        , checker_   ( checker )
        , retriever_ ( retriever )
    {
        controller_.Register( *this );
    }
    virtual ~MessageHandler()
    {
        controller_.Unregister( *this );
    }
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const Category& message, int context )
    {
        if( ( message.*checker_ )() )
            observer_.Notify( ( message.*retriever_ )(), context );
    }
    //@}

private:
    //! @name Member data
    //@{
    MessageController_ABC< Category >& controller_;
    MessageObserver_ABC< Message >& observer_;
    T_Checker checker_;
    T_Retriever retriever_;
    //@}
};

}

#endif // tools_MessageHandler_h
