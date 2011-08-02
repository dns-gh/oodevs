// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FileToolbar_h_
#define __FileToolbar_h_

#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

// =============================================================================
/** @class  FileToolbar
    @brief  FileToolbar
*/
// Created: SBO 2006-12-13
// =============================================================================
class FileToolbar : public QToolBar
                  , private boost::noncopyable
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                  , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             FileToolbar( QMainWindow* parent, kernel::Controllers& controllers );
    virtual ~FileToolbar();
    //@}

    //! @name Operations
    //@{
    void EnableSaveItem( bool status );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QToolButton* saveButton_;
    QToolButton* saveAsButton_;
    //@}
};

#endif // __FileToolbar_h_
