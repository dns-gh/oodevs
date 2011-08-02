// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DiplomacyDialog_ABC_h_
#define __DiplomacyDialog_ABC_h_

#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Karma;
    class Profile_ABC;
    class Team_ABC;
}

namespace gui
{

// =============================================================================
/** @class  DiplomacyDialog_ABC
    @brief  DiplomacyDialog_ABC
*/
// Created: SBO 2008-12-09
// =============================================================================
class DiplomacyDialog_ABC : public QDialog
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< kernel::Team_ABC >
                          , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DiplomacyDialog_ABC( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile );
    virtual ~DiplomacyDialog_ABC();
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DiplomacyDialog_ABC( const DiplomacyDialog_ABC& );            //!< Copy constructor
    DiplomacyDialog_ABC& operator=( const DiplomacyDialog_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void SetDiplomacy( const kernel::Team_ABC& team1, const kernel::Team_ABC& team2, const kernel::Karma& diplomacy ) const = 0;

    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu );

    virtual void showEvent( QShowEvent* );
    virtual QSize sizeHint();
    void UpdateTable();
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Team_ABC* > T_Teams;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Profile_ABC& profile_;
    Q3Table* table_;
    T_Teams teams_;
    //@}
};

}

#endif // __DiplomacyDialog_ABC_h_
