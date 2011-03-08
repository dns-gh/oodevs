// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
class AutomatsLayer : public QObject
                    , public EntityLayer< kernel::Automat_ABC >
                    , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AutomatsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                            View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
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
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual void NotifySelected( const kernel::Automat_ABC* automat );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC&, kernel::ContextMenu& );
    virtual void Select( const kernel::Entity_ABC&, bool );
    virtual void ContextMenu( const kernel::Entity_ABC&, const geometry::Point2f&, const QPoint& );
    void Toggle( const kernel::Entity_ABC& entity, bool aggregate );
    bool IsAggregated( const kernel::Entity_ABC& entity ) const;
    bool HasAggregatedSubordinate( const kernel::Entity_ABC& entity ) const;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    //@}
};

}

#endif // __gui_AutomatsLayer_h_
