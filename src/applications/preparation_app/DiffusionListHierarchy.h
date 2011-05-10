// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DiffusionListHierarchy_h_
#define __DiffusionListHierarchy_h_

#include "clients_gui/HierarchyListView.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <boost/function.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class TacticalHierarchies;
}

namespace gui
{
    class EntitySymbols;
    class ItemFactory_ABC;
}

class AgentsModel;

// =============================================================================
/** @class  DiffusionListHierarchy
    @brief  DiffusionListHierarchy
*/
// Created: ABR 2011-04-29
// =============================================================================
class DiffusionListHierarchy : public gui::HierarchyListView< kernel::TacticalHierarchies >
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
             DiffusionListHierarchy( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons );
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
    virtual void OnSelectionChange( QListViewItem* item );
    virtual void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void OnAddItem();
    void OnRemoveItem();
    void OnFilterChanged( int );
    void OnFilterChanged( const QString& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    bool ApplyAllFilters( gui::ValuedListItem* item );
    bool ApplyFilterLine( gui::ValuedListItem* item );
    bool ApplyFilterButtons( gui::ValuedListItem* item );
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
    QPopupMenu                                popupMenu_;
    QString                                   filterLine_;
    std::vector< QColor >                     filters_;
    EFilter                                   filter_;
    //@}

public:
    //! @name Static member data
    //@{
    static const QColor  baseColor_;
    static const QColor  selectedColor_;
    static const QColor  recipientsColor_;
    static const QColor  potentialsColor_;
    static const int     iconSize_;
    static const QString diffusionSeparator_;
    static const QRegExp diffusionRegexp_;
    //@}
};

#endif // __DiffusionListHierarchy_h_
