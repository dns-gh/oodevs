// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ObjectTreeView_h_
#define __ObjectTreeView_h_

#include "clients_gui/ObjectTreeView.h"

// =============================================================================
/** @class  ObjectTreeView
    @brief  ObjectTreeView
*/
// Created: JSR 2012-09-20
// =============================================================================
class ObjectTreeView : public gui::ObjectTreeView
{

public:
    //! @name Constructors/Destructor
    //@{
    ObjectTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, QWidget* parent = 0 );
    virtual ~ObjectTreeView();
    //@}

    //! @name Operations
    //@{
    virtual QStringList MimeTypes() const;
    virtual Qt::ItemFlags ItemSpecificFlags( const kernel::Entity_ABC& entity ) const;
    virtual QMimeData* MimeData( const QModelIndexList& indexes, bool& overriden ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void keyPressEvent( QKeyEvent* event );
    //@}
};

#endif // __ObjectTreeView_h_
