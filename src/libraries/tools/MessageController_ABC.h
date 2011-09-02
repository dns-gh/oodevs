// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_MessageController_ABC_h
#define tools_MessageController_ABC_h

#include <boost/noncopyable.hpp>

namespace tools
{
    template< typename Category > class MessageHandler_ABC;

// =============================================================================
/** @class  MessageController_ABC
    @brief  Message controller definition
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category >
class MessageController_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef Category category_type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MessageController_ABC() {}
    virtual ~MessageController_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( MessageHandler_ABC< Category >& handler ) = 0;
    virtual void Unregister( MessageHandler_ABC< Category >& handler ) = 0;
    virtual void Dispatch( const Category& message, int context ) = 0;
    //@}
};

}

#endif // tools_MessageController_ABC_h
