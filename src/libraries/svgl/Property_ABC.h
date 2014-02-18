// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Property_ABC_h_
#define __Property_ABC_h_

namespace svg
{
    class RenderingContext_ABC;

// =============================================================================
/** @class  Property_ABC
    @brief  Property definition
*/
// Created: AGE 2006-08-14
// =============================================================================
class Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Property_ABC() {};
    virtual ~Property_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void ReachTop( const RenderingContext_ABC& ) {};
    virtual void LeaveTop( const RenderingContext_ABC& ) {};
    virtual Property_ABC& Clone() const = 0;
    //@}
};

}

#endif // __Property_ABC_h_
