// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ActionsFilter_ABC_h_
#define __ActionsFilter_ABC_h_

namespace actions
{
    class Action_ABC;

// =============================================================================
/** @class  ActionsFilter_ABC
    @brief  ActionsFilter_ABC
*/
// Created: SBO 2010-05-06
// =============================================================================
class ActionsFilter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsFilter_ABC() {}
    virtual ~ActionsFilter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Allows( const Action_ABC& action ) const = 0;
    //@}
};

}

#endif // __ActionsFilter_ABC_h_
