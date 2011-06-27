// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ColorController_h_
#define __ColorController_h_

#include "clients_gui/ColorModifier_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Agent_ABC;
    class Formation_ABC;
    class Team_ABC;
}

// =============================================================================
/** @class  ColorController
    @brief  Color controller
*/
// Created: LGY 2011-06-27
// =============================================================================
class ColorController : public gui::ColorModifier_ABC
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< kernel::Agent_ABC >
                      , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                      , public tools::ElementObserver_ABC< kernel::Team_ABC >

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

    virtual void NotifyCreated( const kernel::Agent_ABC& agent );
    virtual void NotifyDeleted( const kernel::Agent_ABC& agent );
    virtual void NotifyCreated( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Helpers
    //@{
    void ChangeColor( const kernel::Entity_ABC& entity );
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

#endif // __ColorController_h_
