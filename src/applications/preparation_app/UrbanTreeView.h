// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanTreeView_h_
#define __UrbanTreeView_h_

#include "clients_gui/EntityTreeView_ABC.h"

namespace kernel
{
    class UrbanObject_ABC;
}

namespace gui
{
    class SymbolIcons;
}

class StaticModel;

// =============================================================================
/** @class  UrbanTreeView
    @brief  UrbanTreeView
*/
// Created: JSR 2012-09-14
// =============================================================================
class UrbanTreeView: public gui::EntityTreeView_ABC
                   , public tools::ElementObserver_ABC< kernel::UrbanObject_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
    UrbanTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, gui::SymbolIcons& symbols, const StaticModel& staticModel, QWidget* parent = 0 );
    virtual ~UrbanTreeView();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::UrbanObject_ABC& object );
    virtual void NotifyUpdated( const kernel::UrbanObject_ABC& object );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyModeChanged( E_Modes newMode );
    //@}

protected slots:
    //! @name Slots
    //@{
    void OnCreateCity();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void CreateFilters( gui::RichView_ABC& richView );
    virtual void ContextMenuRequested( const QPoint& where );
    virtual bool IsTypeRejected( const kernel::Entity_ABC& entity ) const;
    std::vector< const QPixmap* > GetEntityPixmap( const kernel::Entity_ABC& entity );
    virtual QStringList MimeTypes() const;
    virtual QMimeData* MimeData( const QModelIndexList& indexes, bool& overriden ) const;
    virtual void dragMoveEvent( QDragMoveEvent *pEvent );
    virtual void Drop( const QString& mimeType, void* data, QStandardItem& target );
    virtual void keyPressEvent( QKeyEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    gui::SymbolIcons& symbols_;
    const StaticModel& staticModel_;
    static const QString mimeType_;
    //@}
};

#endif // __UrbanTreeView_h_
