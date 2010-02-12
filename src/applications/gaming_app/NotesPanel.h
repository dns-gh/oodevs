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
#include "clients_gui/ListDisplayer.h"
#include "game_asn/Publisher_ABC.h"
#include "NoteDialog.h"
#include <qpushbutton.h>
#include <qlayout.h>
#include <qdialog.h>

namespace gui
{
    class ItemFactory_ABC;
}

class Note;
class NotesModel;
class Publisher_ABC;

typedef std::map< unsigned int, gui::ValuedListItem* >::iterator itemIterator;

// =============================================================================
/** @class  NotesPanel
    @brief  NotesPanel
*/
// Created: HBD 2010-01-19
// =============================================================================
class NotesPanel : public QDockWindow
    , public tools::Observer_ABC
    , public tools::ElementObserver_ABC< Note >
{
   Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             NotesPanel( QMainWindow* mainWindow, kernel::Controller& controllers, gui::ItemFactory_ABC& factory, NotesModel& model, Publisher_ABC &publisher );
    virtual ~NotesPanel();
    //@}

    //! @name Operations
    //@{
    //@}

 private slots:
    //! @name Slots
    //@{
     void OnContextMenu( QListViewItem* item, const QPoint& point, int);
     void ConfirmDeleteAllTreeNote();
     void ConfirmDeleteNote();
     void PreCreationProcess();
     void UpdateNote();
     //@}

private:
    //! @name Copy/Assignment
    //@{
    NotesPanel( const NotesPanel& );            //!< Copy constructor
    NotesPanel& operator=( const NotesPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Note& element );
    virtual void NotifyUpdated( const Note& element );
    virtual void NotifyDeleted( const Note& element );
    void Display( const Note& note, gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    gui::ListDisplayer< NotesPanel >* notes_;
    gui::ItemFactory_ABC& factory_;
    NoteDialog* noteDialog_;
    NotesModel& model_;
    Publisher_ABC &publisher_;
    std::map< unsigned int, gui::ValuedListItem* > itemsList_;
    //@}
};

#endif // __NotesPanel_h_
