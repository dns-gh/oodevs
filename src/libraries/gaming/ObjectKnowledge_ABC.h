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

#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Team_ABC.h"

namespace kernel
{
    class Displayer_ABC;
}

// =============================================================================
/** @class  ObjectKnowledge_ABC
    @brief  ObjectKnowledge_ABC
*/
// Created: AGE 2006-10-16
// =============================================================================
class ObjectKnowledge_ABC : public kernel::Knowledge_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
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
    virtual const kernel::Team_ABC&   GetOwner() const = 0;
    //@}

    //! @name A dégager  // $$$$ AGE 2006-10-16: 
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const = 0;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}
};

#endif // __ObjectKnowledge_ABC_h_
