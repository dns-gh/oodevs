// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanHierarchies_h_
#define __UrbanHierarchies_h_

#include "clients_kernel/UrbanTypes.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_gui/EntityHierarchies.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  UrbanHierarchies
    @brief  UrbanHierarchies
*/
// Created: JSR 2012-05-14
// =============================================================================
class UrbanHierarchies : public gui::EntityHierarchies< kernel::Hierarchies >
                       , public kernel::Serializable_ABC
                       , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, kernel::Entity_ABC* superior );
    virtual ~UrbanHierarchies();
    //@}

    //! @name Operations
    //@{
    EUrbanLevel GetLevel() const;
    void ChangeSuperior( kernel::Entity_ABC& superior );
    void ForceEntityNewId( unsigned long newId );
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    void RemoveSubordinateByRef( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    EUrbanLevel level_;
    //@}
};

#endif // __UrbanHierarchies_h_
