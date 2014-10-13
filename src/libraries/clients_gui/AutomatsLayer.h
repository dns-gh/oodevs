// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_AutomatsLayer_h_
#define __gui_AutomatsLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace gui
{
// =============================================================================
/** @class  AutomatsLayer
    @brief  AutomatsLayer
*/
// Created: SBO 2007-04-12
// =============================================================================
class AutomatsLayer : public EntityLayer< kernel::Automat_ABC >
                    , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AutomatsLayer( kernel::Controllers& controllers,
                            GLView_ABC& view,
                            ColorStrategy_ABC& strategy,
                            const kernel::Profile_ABC& profile );
    virtual ~AutomatsLayer();
    //@}

    //! @name Operations
    //@{
    void Aggregate( const kernel::Entity_ABC& automat );
    void Disaggregate( const kernel::Entity_ABC& automat );
    //@}

private slots:
    //! @name Slots
    //@{
    void Aggregate();
    void Disaggregate();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void NotifySelectionChanged( const std::vector< const kernel::Automat_ABC* >& elements );
    virtual void NotifyActivated( const kernel::Automat_ABC& automat );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC&, kernel::ContextMenu& );
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    void Toggle( const kernel::Entity_ABC& entity, bool aggregate );
    bool HasSubordinate( const kernel::Entity_ABC& entity, std::function< bool( const kernel::Entity_ABC& ) > fun ) const;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    //@}
};

}

#endif // __gui_AutomatsLayer_h_
