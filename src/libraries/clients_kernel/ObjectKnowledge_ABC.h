// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledge_ABC_h_
#define __ObjectKnowledge_ABC_h_

#include "Knowledge_ABC.h"
#include "Object_ABC.h"
#include "Team_ABC.h"

namespace kernel
{
    class Displayer_ABC;

// =============================================================================
/** @class  ObjectKnowledge_ABC
    @brief  ObjectKnowledge_ABC
*/
// Created: AGE 2006-10-16
// =============================================================================
class ObjectKnowledge_ABC : public Knowledge_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledge_ABC();
    virtual ~ObjectKnowledge_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Object_ABC* GetEntity() const = 0;
    virtual const kernel::Entity_ABC&   GetOwner() const = 0;

    virtual void Display( kernel::Displayer_ABC& displayer ) const = 0;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where, QObject* emitter ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}
};

}

#endif // __ObjectKnowledge_ABC_h_
