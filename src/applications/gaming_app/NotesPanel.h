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
#include "protocol/Publisher_ABC.h"
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

// =============================================================================
/** @class  NotesPanel
    @brief  NotesPanel
*/
// Created: HBD 2010-01-19
// =============================================================================
class NotesPanel : public QDockWindow
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< Note >
                 , public tools::ElementObserver_ABC< Simulation >{
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
    virtual void NotifyUpdated( const Simulation& simulation );

    void Display( const Note& note, QListViewItem* item );

    QListViewItem*  FindItem( unsigned int parent ) const;
    QListViewItem*  FindItem(const Note* element ) const; 
    //@}

    //! @name Types
    //@{
    typedef std::map< QListViewItem*, const Note*> T_Items;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    QListView* notes_;
    gui::ItemFactory_ABC& factory_;
    NoteDialog* noteDialog_;
    NotesModel& model_;
    Publisher_ABC &publisher_;
    T_Items itemsList_;
    //@}
};

#endif // __NotesPanel_h_
