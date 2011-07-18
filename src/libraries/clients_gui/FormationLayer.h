// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_FormationLayer_h_
#define __gui_FormationLayer_h_

#include "EntityLayer.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/Formation_ABC.h"
#include <boost/function.hpp>
#include <set>

namespace kernel
{
    class Formation_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;

// =============================================================================
/** @class  FormationLayer
    @brief  Layer to display entities which have no real position
*/
// Created: AGE 2007-05-31
// =============================================================================
class FormationLayer : public QObject
                     , public EntityLayer< kernel::Formation_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view,
                             const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
    virtual ~FormationLayer();
    //@}

public:
    //! @name Operations
    //@{
    void Aggregate( const kernel::Entity_ABC& formation );
    void Disaggregate( const kernel::Entity_ABC& formation );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Formation_ABC* formation );
    virtual void NotifyActivated( const kernel::Formation_ABC& formation );
    //@}

private slots:
    //! @name Slots
    //@{
    void Aggregate();
    void Disaggregate();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    virtual void Select( const kernel::Entity_ABC&, bool );
    virtual void ContextMenu( const kernel::Entity_ABC&, const geometry::Point2f&, const QPoint& );
    void Toggle( const kernel::Entity_ABC& entity, bool aggregate );
    bool IsAggregated( const kernel::Entity_ABC& entity ) const;
    bool HasSubordinate( const kernel::Entity_ABC& entity, boost::function< bool( const kernel::Entity_ABC& ) > fun ) const;
    bool IsAgent( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    ColorStrategy_ABC& strategy_;
    kernel::SafePointer< kernel::Formation_ABC > selected_;
    //@}
};

}

#endif // __gui_FormationLayer_h_
