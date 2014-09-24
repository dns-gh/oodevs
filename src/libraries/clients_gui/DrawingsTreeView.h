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
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class Drawing_ABC;
    class Pathfind_ABC;
    class TacticalLine_ABC;
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
                       , public tools::ElementObserver_ABC< kernel::Pathfind_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DrawingsTreeView( const QString& objectName,
                               kernel::Controllers& controllers,
                               const kernel::Profile_ABC& profile,
                               ModelObserver_ABC& modelObserver,
                               const std::shared_ptr< ParametersLayer >& paramLayer,
                               QWidget* parent = 0 );
    virtual ~DrawingsTreeView();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Purge();

    virtual void NotifyCreated( const kernel::TacticalLine_ABC& line );
    virtual void NotifyCreated( const kernel::Drawing_ABC& drawing );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyCreated( const kernel::Pathfind_ABC& pathfind );
    virtual bool ApplyProfileFilter( QStandardItem& item ) const;
    virtual void keyPressEvent( QKeyEvent* event );
    //@}

private:
    //! @name Helpers
    //@{
    void AddCommonMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    std::shared_ptr< ParametersLayer > paramLayer_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    QStandardItem* drawingsItem_;
    QStandardItem* limitsItem_;
    QStandardItem* phaseLinesItem_;
    QStandardItem* itinerariesItem_;
    //@}
};

}

#endif // __gui_DrawingsTreeView_h_
