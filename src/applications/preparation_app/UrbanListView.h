// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanListView_h_
#define __UrbanListView_h_

#include "clients_gui/EntityListView.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class UrbanObject_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class SymbolIcon;
    class SymbolIcons;
    class SearchListView_ABC;
}

class ModelBuilder;
class StaticModel;
class UrbanModel;

// =============================================================================
/** @class  UrbanListView
    @brief  UrbanListView
*/
// Created: JSR 2012-05-15
// =============================================================================
class UrbanListView : public gui::EntityListView
                    , public tools::ElementObserver_ABC< kernel::UrbanObject_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UrbanListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder,
                            gui::SymbolIcons& symbols, const StaticModel& staticModel, UrbanModel& urbanModel );
    virtual ~UrbanListView();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyCreated( const kernel::UrbanObject_ABC& object );
    virtual void NotifyUpdated( const kernel::UrbanObject_ABC& object );
    virtual void NotifyDeleted( const kernel::UrbanObject_ABC& object );
    virtual bool IsTypeRejected( const kernel::Entity_ABC& entity ) const;
    virtual void NotifyModeChanged( int newMode );
    virtual void OnContextMenuRequested( Q3ListViewItem* i, const QPoint& pos, int );
    virtual void keyPressEvent( QKeyEvent* key );
    virtual Q3DragObject* dragObject();
    virtual void viewportDropEvent( QDropEvent* pEvent );
    virtual void viewportDragMoveEvent( QDragMoveEvent *pEvent );
    virtual void viewportDragEnterEvent( QDragEnterEvent* pEvent );
    virtual void viewportResizeEvent( QResizeEvent* pEvent );
    virtual void setColumnWidth( int column, int w );
    virtual void CreateFilters( gui::SearchListView_ABC& searchListView );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateCity();
    void Update();
    //@}

private:
    //! @name Helpers
    //@{
    kernel::Entity_ABC* Drop( QDropEvent* pEvent ) const;
    void UpdatePixmap( const kernel::UrbanObject_ABC& object, gui::ValuedListItem* item );
    void UpdateItems( gui::ValuedListItem* root );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, gui::SymbolIcon >    T_InfrastructurePixmaps;
    typedef T_InfrastructurePixmaps::iterator          IT_InfrastructurePixmaps;
    typedef T_InfrastructurePixmaps::const_iterator   CIT_InfrastructurePixmaps;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                controllers_;
    ModelBuilder&                       modelBuilder_;
    gui::SymbolIcons&                   symbols_;
    const StaticModel&                  staticModel_;
    std::vector< kernel::Entity_ABC* >  dragged_;
    T_InfrastructurePixmaps             infrastructures_;
    QTimer*                             timer_;
    UrbanModel&                         urbanModel_;
    //@}
};

#endif // __UrbanListView_h_
