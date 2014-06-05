// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ProfileFilter_h_
#define __ProfileFilter_h_

#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Filter_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <boost/noncopyable.hpp>

class UnitFilter;

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  ProfileFilter
    @brief  Profile filter
*/
// Created: LGY 2011-11-24
// =============================================================================
class ProfileFilter : public kernel::Profile_ABC
                    , public kernel::Filter_ABC
                    , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ProfileFilter( kernel::Controllers& controllers, const kernel::Profile_ABC& forward );
    virtual ~ProfileFilter();
    //@}

    //! @name Operations
    //@{
    virtual QString GetLogin() const;
    virtual bool IsVisible( const kernel::Entity_ABC& entity ) const;
    virtual bool IsKnowledgeVisible( const kernel::Knowledge_ABC& knowledge ) const;
    virtual bool CanBeOrdered( const kernel::Entity_ABC& entity ) const;
    virtual bool CanDoMagic( const kernel::Entity_ABC& entity ) const;
    virtual bool IsPerceived( const kernel::Entity_ABC& entity ) const;
    virtual bool IsSupervision() const;
    virtual bool HasTimeControl() const;

    virtual QString GetFilter() const;
    virtual void SetFilter( const kernel::Entity_ABC& entity );
    virtual void SetFilter( const kernel::Profile_ABC& profile );
    virtual void RemoveFilter();
    virtual const kernel::Entity_ABC* GetFilteredEntity() const;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< UnitFilter > pUnitFilter_;
    kernel::Controller& controller_;
    kernel::SafePointer< kernel::Profile_ABC > profile_;
    //@}
};

#endif // __ProfileFilter_h_
