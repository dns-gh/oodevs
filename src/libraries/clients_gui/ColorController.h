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

#include "ColorModifier_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Entity_ABC;
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
                      , public gui::ColorModifier_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ColorController();
    virtual ~ColorController();
    //@}

    //! @name Operations
    //@{
    virtual QColor Apply( const kernel::Entity_ABC& entity, const QColor& base ) const;
    virtual float Apply( const kernel::Entity_ABC& entity, float alpha ) const;

    virtual void NotifyCreated( const kernel::Entity_ABC& entity );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ColorController( const ColorController& );            //!< Copy constructor
    ColorController& operator=( const ColorController& ); //!< Assignment operator
    //@}

protected:
    //! @name Helpers
    //@{
    void ChangeColor( const kernel::Entity_ABC& entity );
    virtual void UpdateHierarchies( const kernel::Entity_ABC& entity ) = 0;
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< unsigned long, QColor > T_Colors;
    typedef T_Colors::const_iterator        CIT_Colors;
    //@}

protected:
    //! @name Member data
    //@{
    T_Colors colors_;
    //@}
};
}

#endif // __clients_gui_ColorController_h_
