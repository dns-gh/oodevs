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

#include "game_asn/Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"

// =============================================================================
/** @class  DecisionalStates
    @brief  Decisional states
*/
// Created: AGE 2007-05-31
// =============================================================================
class DecisionalStates : public kernel::Extension_ABC
                       , public kernel::Updatable_ABC< ASN1T_MsgDecisionalState >
                       , public kernel::Drawable_ABC
                       , public kernel::Displayable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DecisionalStates();
    virtual ~DecisionalStates();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgDecisionalState& message );
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DecisionalStates( const DecisionalStates& );            //!< Copy constructor
    DecisionalStates& operator=( const DecisionalStates& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, QString > T_Values;
    typedef T_Values::const_iterator   CIT_Values;
    //@}

private:
    //! @name Member data
    //@{
    T_Values values_;
    bool drawSauvegarde_;
    bool draw1stEchelon_;
    //@}
};

#endif // __DecisionalStates_h_
