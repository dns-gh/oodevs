// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __clients_gui_ColorController_h_
#define __clients_gui_ColorController_h_

#include "ColorController_ABC.h"
#include "ColorModifier_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/optional/optional_fwd.hpp>

namespace kernel
{
    class Entity_ABC;
    class Color_ABC;
    class Controllers;
    class TacticalHierarchies;
}

class QColor;

namespace gui
{
// =============================================================================
/** @class  ColorController
    @brief  ColorController
*/
// Created: LDC 2012-05-04
// =============================================================================
class ColorController : public tools::ElementObserver_ABC< kernel::Entity_ABC >
                      , public tools::Observer_ABC
                      , public ColorModifier_ABC
                      , public ColorController_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ColorController( kernel::Controllers& controllers );
    virtual ~ColorController();
    //@}

    //! @name Operations
    //@{
    virtual void Add( const kernel::Entity_ABC& entity, const QColor& newColor, bool applyToSubordinates = true, bool force = false );
    virtual void Remove( const kernel::Entity_ABC& entity, bool applyToSubordinates = true, bool force = false );
    virtual void Reset( const kernel::Entity_ABC& entity, const QColor& newColor );

    virtual QColor Apply( const kernel::Entity_ABC& entity, const QColor& base ) const;
    virtual float Apply( const kernel::Entity_ABC& entity, float alpha ) const;

    virtual void NotifyCreated( const kernel::Entity_ABC& entity );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

protected:
    void AddColor( const kernel::Entity_ABC& entity, const QColor& color );
    void ClearColor( const kernel::Entity_ABC& entity );

private:
    //! @name Helpers
    //@{
    void AddSubordinate( const kernel::Entity_ABC& entity, const QColor& newColor, const boost::optional< QColor >& oldColor, bool applyToSubordinates, bool force );
    void RemoveSubordinate( const kernel::Entity_ABC& entity, const QColor& color, bool applyToSubordinates, bool force );
    void UpdateHierarchies( const kernel::Entity_ABC& entity );
    void UpdateLogisticBaseStates( const kernel::TacticalHierarchies& tactical );
    void ResetSubordinate( const kernel::Entity_ABC& entity, const QColor& newColor );
    //@}

private:
    virtual bool ApplyColor( const kernel::Color_ABC& color ) = 0;

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    std::map< unsigned long, QColor > colors_;
    //@}
};
}

#endif // __clients_gui_ColorController_h_
