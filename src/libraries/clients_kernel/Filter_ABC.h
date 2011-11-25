// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Filter_ABC_h_
#define __Filter_ABC_h_

namespace kernel
{
    class Entity_ABC;
    class Profile_ABC;

// =============================================================================
/** @class  Filter_ABC
    @brief  Filter definition
*/
// Created: LGY 2011-11-24
// =============================================================================
class Filter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Filter_ABC() {}
    virtual ~Filter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QString GetFilter() const = 0;
    virtual void SetFilter( const kernel::Entity_ABC& entity ) = 0;
    virtual void SetFilter( const kernel::Profile_ABC& profile ) = 0;
    virtual void RemoveFilter() = 0;
    //@}
};

}

#endif // __Filter_ABC_h_
