// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfilePopulationRights_h_
#define __UserProfilePopulationRights_h_

#include "UserProfileRights_ABC.h"
#include "clients_gui/DummyModelObserver.h"
#include "clients_gui/EntityTreeView.h"

namespace kernel
{
    class Profile_ABC;
}

// =============================================================================
/** @class  UserProfilePopulationRights
    @brief  UserProfilePopulationRights
*/
// Created: SBO 2007-01-18
// =============================================================================
class UserProfilePopulationRights : public gui::PopulationTreeView
                                  , public UserProfileRights_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfilePopulationRights( QWidget* pParent, kernel::Controllers& controllers, const QString& name, const kernel::Profile_ABC& profile );
    virtual ~UserProfilePopulationRights();
    //@}

    //! @name Operations
    //@{
    virtual bool NeedsCommit() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( const QModelIndex& index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* event );
    virtual void hideEvent( QHideEvent* event );
    virtual void contextMenuEvent( QContextMenuEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    gui::DummyModelObserver observer_;
    //@}
};

#endif // __UserProfilePopulationRights_h_
