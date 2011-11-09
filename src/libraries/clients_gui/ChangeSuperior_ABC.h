// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ChangeSuperior_ABC_h_
#define __ChangeSuperior_ABC_h_

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  ChangeSuperior_ABC
    @brief  ChangeSuperior_ABC
*/
// Created: JSR 2011-11-08
// =============================================================================
class ChangeSuperior_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ChangeSuperior_ABC() {}
    virtual ~ChangeSuperior_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const = 0;
    virtual void DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior ) = 0;
    //@}
};

} // end namespace gui

#endif // __ChangeSuperior_ABC_h_
