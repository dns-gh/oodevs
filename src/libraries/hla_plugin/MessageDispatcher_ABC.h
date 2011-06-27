// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MessageDispatcher_ABC_h
#define plugins_hla_MessageDispatcher_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    template< typename Category > class MessageHandler_ABC;

// =============================================================================
/** @class  MessageDispatcher_ABC
    @brief  Message dispatcher definition
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category >
class MessageDispatcher_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef Category category_type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MessageDispatcher_ABC() {}
    virtual ~MessageDispatcher_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( MessageHandler_ABC< Category >& handler ) = 0;
    virtual void Unregister( MessageHandler_ABC< Category >& handler ) = 0;
    virtual void Dispatch( const Category& message ) = 0;
    //@}
};

}
}

#endif // plugins_hla_MessageDispatcher_ABC_h
