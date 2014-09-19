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
#include "Displayable_ABC.h"

namespace kernel
{
    class Team_ABC;
    class ObjectType;
    class ActionController;

// =============================================================================
/** @class  Object_ABC
    @brief  Object definition
*/
// Created: SBO 2006-08-03
// =============================================================================
class Object_ABC : public Entity_ABC, public Displayable_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Object_ABC();
    virtual ~Object_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Display( Displayer_ABC& displayer ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const ObjectType& GetType() const = 0;
    virtual std::string GetSymbol() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;
    virtual void Select( ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const;
    virtual void Activate( ActionController& controller ) const;
    void Finalize() const;
    //@}
};

}

#endif // __Object_ABC_h_
