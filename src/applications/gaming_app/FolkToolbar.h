// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FolkToolbar_h_
#define __FolkToolbar_h_

#include "clients_kernel/ElementObserver_ABC.h"

class FolkModel;

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  FolkToolbar
    @brief  FolkToolbar
*/
// Created: AGE 2007-09-05
// =============================================================================
class FolkToolbar : public QToolBar
                  , public tools::Observer_ABC
                  , public kernel::ElementObserver_ABC< FolkModel >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             FolkToolbar( QMainWindow* pParent, kernel::Controllers& controllers, FolkModel& model );
    virtual ~FolkToolbar();
    //@}

    //! @name Operations
    //@{
    //@}

private slots:
    //! @name Slots
    //@{
    void SomethingChanged();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FolkToolbar( const FolkToolbar& );            //!< Copy constructor
    FolkToolbar& operator=( const FolkToolbar& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const FolkModel& );
    void Fill( QComboBox* box, const std::vector< QString >& names );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    FolkModel& model_;
    QComboBox* container_;
    QComboBox* profile_;
    QComboBox* activity_;
    //@}
};

#endif // __FolkToolbar_h_
