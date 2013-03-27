// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __PopulationTreeView_h_
#define __PopulationTreeView_h_

#include "clients_gui/EntityTreeView.h"

// =============================================================================
/** @class  PopulationTreeView
    @brief  PopulationTreeView
*/
// Created: JSR 2012-09-21
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
    virtual void dragMoveEvent( QDragMoveEvent *pEvent );
    virtual Qt::ItemFlags ItemSpecificFlags( const kernel::Entity_ABC& entity ) const;
    virtual QStringList MimeTypes() const;
    virtual void Drop( const QString& mimeType, void* data, QStandardItem& target );
    //@}

private:
    //! @name Operations
    //@{
    virtual void keyPressEvent( QKeyEvent* event );
    //@}
};

#endif // __PopulationTreeView_h_
