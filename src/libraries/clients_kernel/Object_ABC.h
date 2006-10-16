// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Object_ABC_h_
#define __Object_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
    class Team_ABC;
    class ObjectType;
    class ActionController;
    class Displayer_ABC;

// =============================================================================
/** @class  Object_ABC
    @brief  Object_ABC
*/
// Created: SBO 2006-08-03
// =============================================================================
class Object_ABC : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Object_ABC();
    virtual ~Object_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Display( Displayer_ABC& displayer ) const = 0;
    virtual void DisplayInTooltip( Displayer_ABC& displayer ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual ObjectType& GetType() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}
};

}

#endif // __Object_ABC_h_
