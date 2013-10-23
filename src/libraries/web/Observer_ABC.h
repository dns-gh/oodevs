// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef OBSERVER_ABC_H
#define OBSERVER_ABC_H

#include <boost/noncopyable.hpp>
#include <map>
#include <string>

namespace web
{
    struct Request_ABC;
    struct Reply_ABC;

// =============================================================================
/** @class  Observer_ABC
    @brief  Web observer definition
*/
// Created: BAX 2012-02-28
// =============================================================================
struct Observer_ABC : public boost::noncopyable
{
    //! @name Constructor/Destructor
             Observer_ABC() {}
    virtual ~Observer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void DoGet ( Reply_ABC& reply, Request_ABC& request ) = 0;
    virtual void DoPost( Reply_ABC& reply, Request_ABC& request ) = 0;
    //@}
};
}

#endif // OBSERVER_ABC_H
