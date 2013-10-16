// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __NotesPanel_h_
#define __NotesPanel_h_

#include "clients_gui/RichDockWidget.h"
#include "NoteDialog.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
}

class Note;
class NotesModel;
class Publisher_ABC;

// =============================================================================
/** @class  NotesPanel
    @brief  NotesPanel
*/
// Created: HBD 2010-01-19
// =============================================================================
class NotesPanel : public gui::RichDockWidget
                 , public tools::ElementObserver_ABC< Note >
                 , public tools::ElementObserver_ABC< Simulation >
{
   Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             NotesPanel( QMainWindow* mainWindow, kernel::Controllers& controller, NotesModel& model, Publisher_ABC &publisher );
    virtual ~NotesPanel();
    //@}

 private slots:
    //! @name Slots
    //@{
     void OnContextMenu( const QPoint& point );
     void ConfirmDeleteNote();
     void AddSubNote();
     void AddRootNote();
     void UpdateNote();
     //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Note& element );
    virtual void NotifyUpdated( const Note& element );
    virtual void NotifyDeleted( const Note& element );
    virtual void NotifyUpdated( const Simulation& simulation );

    void AddNoteInfo( const Note& note, QStandardItem* parent );

    QStandardItem*  FindItem( const Note* element ) const;
    QStandardItem*  FindItem( unsigned int id, QStandardItem* parent = 0 ) const;
    //@}

private:
    //! @name Member data
    //@{
    QTreeView* notes_;
    QStandardItemModel noteModel_;
    NoteDialog* noteDialog_;
    NotesModel& model_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __NotesPanel_h_
