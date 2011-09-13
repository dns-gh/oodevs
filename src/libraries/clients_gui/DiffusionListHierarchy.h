// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_DiffusionListHierarchy_h_
#define gui_DiffusionListHierarchy_h_

#include "HierarchyListView.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <boost/function.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class EntitySymbols;
    class ItemFactory_ABC;

// =============================================================================
/** @class  DiffusionListHierarchy
    @brief  DiffusionListHierarchy
*/
// Created: ABR 2011-04-29
// =============================================================================
class DiffusionListHierarchy : public HierarchyListView< kernel::TacticalHierarchies >
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum EFilter
    {
        EFilterFull = 0,
        EFilterRecipients = 1,
        EFilterRecipientsAndPotentials = 2
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DiffusionListHierarchy( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, const EntitySymbols& icons );
    virtual ~DiffusionListHierarchy();
    //@}

public:
    //! @name Operations
    //@{
    void Purge();
    void Initialize( kernel::SafePointer< kernel::Entity_ABC > selected, const QString& diffusionList );
    void UpdateDisplay( const QString& diffusionList );
    //@}

signals:
    //! @name Slots
    //@{
    void ListChanged( const QString& );
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnSelectionChange( Q3ListViewItem* item );
    virtual void OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int );
    void OnAddItem();
    void OnRemoveItem();
    void OnFilterChanged( int );
    void OnFilterChanged( const QString& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    bool ApplyAllFilters( ValuedListItem* item );
    bool ApplyFilterLine( ValuedListItem* item );
    bool ApplyFilterButtons( ValuedListItem* item );
    void ComputeAndEmitDiffusionList();
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    std::set< unsigned long >                 recipients_;
    std::set< unsigned long >                 potentials_;
    const QPixmap                             basePixmap_;
    const QPixmap                             selectedPixmap_;
    const QPixmap                             recipientsPixmap_;
    Q3PopupMenu                               popupMenu_;
    QString                                   filterLine_;
    std::vector< QColor >                     filters_;
    EFilter                                   filter_;
    //@}

public:
    //! @name Static member data
    //@{
    static const int     iconSize_;
    static const QColor  baseColor_;
    static const QColor  selectedColor_;
    static const QColor  recipientsColor_;
    static const QColor  potentialsColor_;
    static const QString diffusionSeparator_;
    static const QRegExp diffusionRegexp_;
    //@}
};

}

#endif // gui_DiffusionListHierarchy_h_
