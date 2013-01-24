// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DecisionalStates_h_
#define __DecisionalStates_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include <boost/noncopyable.hpp>

namespace sword
{
    class DecisionalState;
    class UnitAttributes;
}

// =============================================================================
/** @class  DecisionalStates
    @brief  Decisional states
*/
// Created: AGE 2007-05-31
// =============================================================================
class DecisionalStates : public kernel::Extension_ABC
                       , public kernel::Updatable_ABC< sword::DecisionalState >
                       , public kernel::Updatable_ABC< sword::UnitAttributes >
                       , public kernel::Displayable_ABC
                       , public kernel::Aggregatable_ABC
                       , public gui::Drawable_ABC
                       , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DecisionalStates();
    virtual ~DecisionalStates();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::DecisionalState& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< QString, QString > T_Values;
    typedef T_Values::const_iterator   CIT_Values;
    //@}

    //! @name Helpers
    //@{
    virtual void Aggregate( const bool& );
    //@}

private:
    //! @name Member data
    //@{
    T_Values values_;
    bool drawSauvegarde_;
    bool draw1stEchelon_;
    bool drawEclairage_;
    bool drawEtatOps_;
    float ratio_;
    //@}
};

#endif // __DecisionalStates_h_
