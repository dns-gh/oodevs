// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef gui__VisibilityFilter_ABC_h_
#define gui__VisibilityFilter_ABC_h_

namespace kernel
{
    class Entity_ABC;
    class Profile_ABC;
}

namespace gui
{
class GLOptions;

class VisibilityFilter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             VisibilityFilter_ABC() {}
    virtual ~VisibilityFilter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QString GetFilter() const = 0;
    virtual void SetFilter( const kernel::Entity_ABC& entity, bool update = true ) = 0;
    virtual void SetFilter( const kernel::Profile_ABC& profile, bool update = true ) = 0;
    virtual void SetFilter( const GLOptions& options, bool update = true ) = 0;
    virtual void RemoveFilter( bool update = true ) = 0;
    virtual const kernel::Entity_ABC* GetFilteredEntity() const = 0;
    virtual const kernel::Profile_ABC* GetFilteredProfile() const = 0;
    //@}
};

} //! namespace kernel

#endif // gui__VisibilityFilter_ABC_h_
