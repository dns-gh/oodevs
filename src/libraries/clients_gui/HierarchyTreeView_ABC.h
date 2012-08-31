// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __HierarchyTreeView_ABC_h_
#define __HierarchyTreeView_ABC_h_

#include "EntityTreeView_ABC.h"

namespace kernel
{
    class Controllers;
    class Hierarchies;
    class Profile_ABC;
}

namespace gui
{
    class EntitySymbols;

// =============================================================================
/** @class  HierarchyTreeView_ABC
    @brief  HierarchyTreeView_ABC
*/
// Created: ABR 2012-08-10
// =============================================================================
class HierarchyTreeView_ABC : public EntityTreeView_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             HierarchyTreeView_ABC( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, const EntitySymbols& symbols, QWidget* parent = 0 );
    virtual ~HierarchyTreeView_ABC();
    //@}

    //! @name Operations
    //@{
    //@}

protected:
    //! @name Helpers
    //@{
    void InternalNotifyCreated( const kernel::Hierarchies& hierarchy );
    void InternalNotifyUpdated( const kernel::Hierarchies& hierarchy );
    void UpdateBackgroundColor( QStandardItem& entityItem, const kernel::Entity_ABC& entity );
    void UpdateSymbol( QStandardItem& entityItem, const kernel::Entity_ABC& entity );
    //@}

protected slots:
    //! @name Slots
    //@{
    void OnTimeOut();
    //@}

private:
    //! @name Member data
    //@{
    const EntitySymbols& symbols_;
    QTimer* timer_;
    std::set< const kernel::Entity_ABC* > waitingSymbols_;
    //@}
};

} //! namespace gui

#endif // __HierarchyTreeView_ABC_h_
