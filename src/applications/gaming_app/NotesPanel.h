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
#include "protocol/ServerPublisher_ABC.h"
#include "NoteDialog.h"

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
class NotesPanel : public QDockWidget
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

 private slots:
    //! @name Slots
    //@{
     void OnContextMenu( Q3ListViewItem* item, const QPoint& point, int);
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

    void Display( const Note& note, Q3ListViewItem* item );

    Q3ListViewItem*  FindItem( unsigned int parent ) const;
    Q3ListViewItem*  FindItem(const Note* element ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< Q3ListViewItem*, const Note*> T_Items;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Q3ListView* notes_;
    gui::ItemFactory_ABC& factory_;
    NoteDialog* noteDialog_;
    NotesModel& model_;
    Publisher_ABC &publisher_;
    T_Items itemsList_;
    //@}
};

#endif // __NotesPanel_h_
