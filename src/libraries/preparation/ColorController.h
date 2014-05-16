// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _ColorController_h
#define _ColorController_h

#include "clients_gui/ColorEditor_ABC.h"
#include "clients_gui/ColorController.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/optional/optional_fwd.hpp>

namespace kernel
{
    class Controllers;
    class TacticalHierarchies;
}

// =============================================================================
/** @class  ColorController
    @brief  Color controller
*/
// Created: LGY 2011-06-23
// =============================================================================
class ColorController : public gui::ColorEditor_ABC
                      , public gui::ColorController
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ColorController( kernel::Controllers& controllers );
    virtual ~ColorController();
    //@}

    //! @name Operations
    //@{
    virtual void Add( const kernel::Entity_ABC& entity, const QColor& newColor );
    virtual void Remove( const kernel::Entity_ABC& entity );
    virtual void Reset( const kernel::Entity_ABC& entity, const QColor& newColor );
    //@}

private:
    //! @name Helpers
    //@{
    void AddSubordinate( const kernel::Entity_ABC& entity, const QColor& newColor, const boost::optional< QColor >& oldColor );
    void AddObjects( const kernel::Entity_ABC& entity, const QColor& newColor );
    void AddColor( const kernel::Entity_ABC& entity, const QColor& color );
    void ClearColor( const kernel::Entity_ABC& entity );
    void RemoveSubordinate( const kernel::Entity_ABC& entity, const QColor& color );
    void RemoveObjects( const kernel::Entity_ABC& entity );
    virtual void UpdateHierarchies( const kernel::Entity_ABC& entity );
    void UpdateLogisticBaseStates( const kernel::TacticalHierarchies& tactical );
    void ResetSubordinate( const kernel::Entity_ABC& entity, const QColor& newColor );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // _ColorController_h
