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

#include "tools/ElementObserver_ABC.h"
#include "NoteDialog.h"

namespace kernel
{
    class Controller;
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
class NotesPanel : public QDockWidget
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< Note >
                 , public tools::ElementObserver_ABC< Simulation >
{
   Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             NotesPanel( QMainWindow* mainWindow, kernel::Controller& controller, NotesModel& model, Publisher_ABC &publisher );
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
    kernel::Controller& controller_;
    QTreeView* notes_;
    QStandardItemModel noteModel_;
    NoteDialog* noteDialog_;
    NotesModel& model_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __NotesPanel_h_
