// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ModelObserver_ABC_h_
#define __ModelObserver_ABC_h_

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  ModelObserver_ABC
    @brief  ModelObserver_ABC
*/
// Created: JSR 2012-08-31
// =============================================================================
class ModelObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelObserver_ABC() {}
    virtual ~ModelObserver_ABC() {}
    //@}

public:
    //! @name Operations
    //@{
    virtual void OnRename( kernel::Entity_ABC& entity, const QString& newName ) = 0;
    //@}
};

} //! namespace gui

#endif // __ModelObserver_ABC_h_
