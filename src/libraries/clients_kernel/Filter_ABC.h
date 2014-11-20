// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef gui__Filter_ABC_h_
#define gui__Filter_ABC_h_

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
    virtual void SetFilter( const kernel::Profile_ABC& profile, bool update = true ) = 0;
    virtual void RemoveFilter( bool update = true ) = 0;
    virtual const kernel::Entity_ABC* GetFilteredEntity() const = 0;
    virtual const kernel::Profile_ABC* GetFilteredProfile() const = 0;
    //@}
};

} //! namespace kernel

#endif // gui__Filter_ABC_h_
