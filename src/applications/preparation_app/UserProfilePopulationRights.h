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
#include "clients_gui/EntityTreeView.h"
#include "clients_gui/DummyModelObserver.h"

// =============================================================================
/** @class  UserProfilePopulationRights
    @brief  UserProfilePopulationRights
*/
// Created: SBO 2007-01-18
// =============================================================================
class UserProfilePopulationRights : public gui::PopulationTreeView
                                  , public UserProfileRights_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    UserProfilePopulationRights( const QString& objectName, QWidget* pParent, kernel::Controllers& controllers, const QString& name );
    virtual ~UserProfilePopulationRights();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* GetWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( const QModelIndex& index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    gui::DummyModelObserver observer_;
    //@}
};

#endif // __UserProfilePopulationRights_h_
