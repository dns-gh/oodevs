// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireResultListView_h_
#define __FireResultListView_h_

#include "clients_kernel/SafePointer.h"
#include <tools/Observer_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace gui
{
    class DisplayExtractor;
}

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class Entity_ABC;
    class KnowledgeConverter_ABC;
}

class Explosions;
class PopulationFireResult;
class AgentFireResult;
class Equipment;
class Casualties;

// =============================================================================
/** @class  FireResultListView
    @brief  FireResultListView
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireResultListView : public QTreeView
                         , public tools::Observer_ABC
                         , public tools::SelectionObserver< kernel::Entity_ABC >
                         , public tools::ElementObserver_ABC< Explosions >
                         , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FireResultListView( QWidget* parent, kernel::Controllers& controllers,
                 gui::DisplayExtractor& extractor, const kernel::KnowledgeConverter_ABC& converter );
    virtual ~FireResultListView();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    //@}

private:
    //! @name Helpers
    //@{
    QStandardItem* AddRoot( int row,
                            int col,
                            const QString& text,
                            const kernel::Entity_ABC* entity = 0 );
    QStandardItem* AddChild( QStandardItem& root,
                             int row,
                             int col,
                             const QString& text,
                             bool bold = false );

    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyUpdated( const Explosions& results );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    void UpdateDisplay();
    void DisplaySelection();
    void Display( const AgentFireResult& result );
    void Display( const PopulationFireResult& result );
    void Display( const Equipment& equipment, QStandardItem& parent );
    void Display( const Casualties& casualties, QStandardItem& parent );
    QString GetFirerName( const kernel::Entity_ABC* firer ) const;
    QString GetDisplayName( const kernel::Agent_ABC& agent ) const;
    //@}

private:
    //! @name Member data
    //@{
    QStandardItemModel model_;
    QSortFilterProxyModel proxy_;
    kernel::Controllers& controllers_;
    gui::DisplayExtractor& extractor_;
    const kernel::KnowledgeConverter_ABC& converter_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    std::vector< const Explosions* > explosions_;
    //@}
};

#endif // __FireResultListView_h_
