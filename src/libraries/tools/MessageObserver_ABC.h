// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_MessageObserver_ABC_h
#define tools_MessageObserver_ABC_h

#include <boost/noncopyable.hpp>

namespace tools
{
// =============================================================================
/** @class  MessageObserver_ABC
    @brief  Message observer definition
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Message >
class MessageObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageObserver_ABC() {}
    virtual ~MessageObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const Message& message, int context ) = 0;
    //@}
};

}

#endif // tools_MessageObserver_ABC_h
