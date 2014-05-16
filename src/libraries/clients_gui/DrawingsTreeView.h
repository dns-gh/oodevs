// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui_DrawingsTreeView_h_
#define __gui_DrawingsTreeView_h_

#include "EntityTreeView_ABC.h"

namespace kernel
{
    class TacticalLine_ABC;
    class Drawing_ABC;
}

namespace gui
{
    class ParametersLayer;
// =============================================================================
/** @class  DrawingsTreeView
    @brief  DrawingsTreeView
*/
// Created: LGY 2014-05-07
// =============================================================================
class DrawingsTreeView : public EntityTreeView_ABC
                       , public tools::ElementObserver_ABC< kernel::Drawing_ABC >
                       , public tools::ElementObserver_ABC< kernel::TacticalLine_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawingsTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile,
                               ModelObserver_ABC& modelObserver, ParametersLayer& paramLayer, QWidget* parent = 0 );
    virtual ~DrawingsTreeView();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Purge();

    virtual void NotifyCreated( const kernel::TacticalLine_ABC& line );
    virtual void NotifyCreated( const kernel::Drawing_ABC& drawing );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual bool ApplyProfileFilter( QStandardItem& item ) const;
    virtual void keyPressEvent( QKeyEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    ParametersLayer& paramLayer_;
    QStandardItem* drawingsItem_;
    QStandardItem* limitsItem_;
    QStandardItem* phaseLinesItem_;
    //@}
};

}

#endif // __gui_DrawingsTreeView_h_
