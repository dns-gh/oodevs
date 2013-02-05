// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Selectable_ABC_h_
#define __Selectable_ABC_h_

class QPoint;
class QString;

namespace kernel
{
    class ActionController;

// =============================================================================
/** @class  Selectable_ABC
    @brief  Selectable extension definition
*/
// Created: JSR 2012-05-21
// =============================================================================
class Selectable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Selectable_ABC() {}
    virtual ~Selectable_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const = 0;
    virtual QString GetTooltip() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Select( ActionController& controller ) const = 0;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const Selectable_ABC* >& elements ) const = 0;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const = 0;
    virtual void Activate( ActionController& controller ) const = 0;
    virtual void OverFly( ActionController& controller ) const = 0;
    //@}

    //! @name Types
    //@{
    typedef std::vector< const Selectable_ABC* > T_Selectables;
    //@}
};

}

#endif // __Selectable_ABC_h_
