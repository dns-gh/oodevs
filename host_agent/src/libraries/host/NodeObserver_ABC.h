// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_OBSERVER_ABC_H
#define NODE_OBSERVER_ABC_H

namespace host
{
    struct Node_ABC;
}

namespace host
{
struct NodeObserver_ABC
{
    //! @name Constructors/Destructors
    //@{
    NodeObserver_ABC() {}
    virtual ~NodeObserver_ABC() {}
    //@}

    //! @name Public methods
    //@{
    virtual void Notify( const Node_ABC& node ) const = 0;
    //@}
};
}

#endif // NODE_OBSERVER_ABC_H
