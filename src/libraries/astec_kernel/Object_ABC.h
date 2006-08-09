// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_ABC_h_
#define __Object_ABC_h_

#include "Entity_ABC.h"

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
             Object_ABC() {}
    virtual ~Object_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Display( Displayer_ABC& displayer ) const = 0;
    virtual void DisplayInTooltip( Displayer_ABC& displayer ) const = 0;
    virtual void Select( ActionController& controller ) const = 0;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const = 0;
    virtual void Activate( ActionController& controller ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual bool IsInTeam( const Team_ABC& team ) const = 0;
    virtual unsigned long GetId() const = 0;
    virtual std::string GetName() const = 0;
    virtual const Team_ABC& GetTeam() const = 0;
    virtual ObjectType& GetType() const = 0;
    //@}
};

#endif // __Object_ABC_h_
