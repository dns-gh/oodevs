// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Filter_ABC_h_
#define __Filter_ABC_h_

namespace kernel
{
    class Entity_ABC;
    class Profile_ABC;
}

namespace kernel
{
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
    virtual void SetFilter( const kernel::Entity_ABC& entity, bool update = true ) = 0;
    virtual void SetFilter( const kernel::Profile_ABC& profile ) = 0;
    virtual void RemoveFilter() = 0;
    virtual const kernel::Entity_ABC* GetFilteredEntity() const = 0;
    //@}
};

} //! namespace kernel

#endif // __Filter_ABC_h_
