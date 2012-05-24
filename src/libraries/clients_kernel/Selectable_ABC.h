// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Selectable_ABC_h_
#define __Selectable_ABC_h_

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

    //! @name Operations
    //@{
    virtual void Select( ActionController& controller ) const = 0;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const Selectable_ABC* >& elements ) const = 0;
    //@}
};

}

#endif // __Selectable_ABC_h_
