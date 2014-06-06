// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ChatProfiles_h_
#define __ChatProfiles_h_

#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class UserProfile_ABC;
}

// =============================================================================
/** @class  ChatProfiles
    @brief  ChatProfiles
*/
// Created: SBO 2008-06-11
// =============================================================================
class ChatProfiles : public QListWidget
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< kernel::UserProfile_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ChatProfiles( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ChatProfiles();
    //@}

signals:
    //! @name Signals
    //@{
    void Selected( const kernel::UserProfile_ABC& profile );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelected( QListWidgetItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::UserProfile_ABC& profile );
    virtual void NotifyDeleted( const kernel::UserProfile_ABC& profile );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __ChatProfiles_h_
