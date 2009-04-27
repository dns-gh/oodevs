// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScorePanel_h_
#define __ScorePanel_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_gui/ListDisplayer.h"
#include <QDockWindow.h>

namespace gui
{
    class ItemFactory_ABC;
}

namespace kernel
{
    class Controllers;
    class ModelUnLoaded;
}

class Score;

// =============================================================================
/** @class  ScorePanel
    @brief  ScorePanel
*/
// Created: SBO 2009-03-12
// =============================================================================
class ScorePanel : public QDockWindow
                 , public kernel::Observer_ABC
                 , public kernel::ElementObserver_ABC< Score >
                 , public kernel::ElementObserver_ABC< kernel::ModelUnLoaded >
{

public:
    //! @name Constructors/Destructor
    //@{
             ScorePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ScorePanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScorePanel( const ScorePanel& );            //!< Copy constructor
    ScorePanel& operator=( const ScorePanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Score& element );
    virtual void NotifyUpdated( const Score& element );
    virtual void NotifyDeleted( const Score& element );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    void Display( const Score& score, gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    gui::ListDisplayer< ScorePanel >* scores_;
    //@}
};

#endif // __ScorePanel_h_
