// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Menu_h_
#define __Menu_h_

#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>
#include <qmenubar.h>

class QMainWindow;
class QDialog;

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
    class ItemFactory_ABC;
    class HelpSystem;
}

// =============================================================================
/** @class  Menu
    @brief  Menu
*/
// Created: SBO 2006-04-28
// =============================================================================
class Menu : public QMenuBar
           , private boost::noncopyable
           , public tools::Observer_ABC
           , public tools::ElementObserver_ABC< kernel::ModelLoaded >
           , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Menu( QMainWindow* pParent, kernel::Controllers& controllers
                 , QDialog& prefDialog, QDialog& profileDialog, QDialog& profileWizardDialog, QDialog& importDialog, QDialog& exportDialog, QDialog& scoreDialog, QDialog& successFactorDialog, QDialog& exerciseDialog
                 , gui::ItemFactory_ABC& factory, const QString& license, const gui::HelpSystem& help );
    virtual ~Menu();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    void EnableSaveItem( bool status );
    //@}

private:
    //! @name Helpers
    //@{
    void EnableItems( bool status );
    void Wrap( int item );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    std::vector< int > exerciseItems_;
    int saveItem_;
    //@}
};

#endif // __Menu_h_
