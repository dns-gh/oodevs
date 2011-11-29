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

#include "clients_gui/ColorModifier_ABC.h"
#include "clients_gui/ColorEditor_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/optional.hpp>

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
class ColorController : public gui::ColorModifier_ABC
                      , public gui::ColorEditor_ABC
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ColorController( kernel::Controllers& controllers );
    virtual ~ColorController();
    //@}

    //! @name Operations
    //@{
    virtual QColor Apply( const kernel::Entity_ABC& entity, const QColor& base );
    virtual void Add( const kernel::Entity_ABC& entity, const QColor& newColor );
    virtual void Remove( const kernel::Entity_ABC& entity );

    virtual void NotifyCreated( const kernel::Entity_ABC& entity );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Helpers
    //@{
    void AddSubordinate( const kernel::Entity_ABC& entity, const QColor& newColor, const boost::optional< QColor >& oldColor = boost::none );
    void RemoveSubordinate( const kernel::Entity_ABC& entity, const QColor& color );
    void UpdateHierarchies( const kernel::Entity_ABC& entity );
    void UpdateLogisticBaseStates( const kernel::TacticalHierarchies& tactical );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, QColor > T_Colors;
    typedef T_Colors::const_iterator        CIT_Colors;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Colors colors_;
    //@}
};

#endif // _ColorController_h
