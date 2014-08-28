// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __PopulationTreeView_h_
#define __PopulationTreeView_h_

#include "clients_gui/EntityTreeView.h"

// =============================================================================
/** @class  PopulationTreeView
    @brief  PopulationTreeView
*/
// Created: JSR 2013-01-18
// =============================================================================
class PopulationTreeView : public gui::PopulationTreeView
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, QWidget* parent = 0 );
    virtual ~PopulationTreeView();
    //@}

    //! @name Operations
    //@{
    virtual std::vector< const QPixmap* > GetEntityPixmap( const kernel::Entity_ABC& entity );
    virtual Qt::ItemFlags ItemSpecificFlags( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    QPixmap icon_user_;
    //@}
};

#endif // __PopulationTreeView_h_
