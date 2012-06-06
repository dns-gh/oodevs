// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanObject_ABC_h_
#define __UrbanObject_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  UrbanObject_ABC
    @brief  Urban Object
*/
// Created: SLG 2009-02-10
// =============================================================================
class UrbanObject_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             UrbanObject_ABC();
    virtual ~UrbanObject_ABC();
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const Selectable_ABC* >& elements ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    //@}
};

}

#endif // __UrbanObject_ABC_h_
