// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitFilter_h_
#define __UnitFilter_h_

#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Filter_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class TacticalHierarchies;
    class CommunicationHierarchies;
    class Knowledge_ABC;
}

// =============================================================================
/** @class  UnitFilter
    @brief  UnitFilter
*/
// Created: AGE 2006-11-29
// =============================================================================
class UnitFilter : public kernel::Profile_ABC
                 , public kernel::Filter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitFilter( kernel::Controllers& controllers, const kernel::Profile_ABC& forward );
    virtual ~UnitFilter();
    //@}

    //! @name Operations
    //@{
    virtual const QString& GetLogin() const;
    virtual bool IsVisible( const kernel::Entity_ABC& entity ) const;
    virtual bool IsKnowledgeVisible( const kernel::Knowledge_ABC& knowledge ) const;
    virtual bool IsKnowledgeVisibleNoSupervision( const kernel::Knowledge_ABC& knowledge ) const;
    virtual bool CanBeOrdered( const kernel::Entity_ABC& entity ) const;
    virtual bool CanDoMagic( const kernel::Entity_ABC& entity ) const;
    virtual bool IsSupervision() const;
    virtual bool HasTimeControl() const;
    virtual bool IsPerceived( const kernel::Entity_ABC& ) const;
    virtual bool IsFiltered() const;
    virtual QString GetFilter() const;
    virtual void SetFilter( const kernel::Entity_ABC& entity );
    virtual void SetFilter( const kernel::Profile_ABC& profile );
    virtual void RemoveFilter();
    const kernel::Entity_ABC* GetFilteredEntity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UnitFilter( const UnitFilter& );            //!< Copy constructor
    UnitFilter& operator=( const UnitFilter& ); //!< Assignment operator
   //@}

    //! @name Helpers
    //@{
    bool IsInHierarchy( const kernel::Entity_ABC& entity ) const;
    bool IsSubordinate( const kernel::Entity_ABC& entity ) const;
    bool IsInKnowledgeGroup( const kernel::Entity_ABC& entity ) const;
    bool IsObjectOfSameTeam( const kernel::Entity_ABC& entity ) const;
    template< typename D, typename U >
    bool IsChildSubordinateOf( const D& down, const U& up ) const;
    bool IsKnown( const kernel::TacticalHierarchies* t, const kernel::CommunicationHierarchies* c, const kernel::Entity_ABC& entity ) const;
    bool IsVisibleInHierarchy( const kernel::Knowledge_ABC& knowledge ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Profile_ABC& forward_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    const kernel::TacticalHierarchies* tHierarchies_;
    const kernel::CommunicationHierarchies* cHierarchies_;
    //@}
};

#endif // __UnitFilter_h_
