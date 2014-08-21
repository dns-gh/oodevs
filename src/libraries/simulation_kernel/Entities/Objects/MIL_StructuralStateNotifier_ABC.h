// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_StructuralStateNotifier_ABC_h_
#define __MIL_StructuralStateNotifier_ABC_h_

class MIL_Object_ABC;

// =============================================================================
/** @class  MIL_StructuralStateNotifier_ABC
    @brief  MIL_StructuralStateNotifier_ABC
*/
// Created: JSR 2011-01-14
// =============================================================================
class MIL_StructuralStateNotifier_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_StructuralStateNotifier_ABC() {}
    virtual ~MIL_StructuralStateNotifier_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyStructuralStateChanged( float structuralState, const MIL_Object_ABC& object ) = 0;
    virtual void NotifyFired() = 0;
    //@}
};

#endif // __MIL_StructuralStateNotifier_ABC_h_
