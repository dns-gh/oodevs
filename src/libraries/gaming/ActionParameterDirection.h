// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterDirection_h_
#define __ActionParameterDirection_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"

// =============================================================================
/** @class  ActionParameterDirection
    @brief  ActionParameterDirection
*/
// Created: SBO 2007-04-16
// =============================================================================
class ActionParameterDirection : public ActionParameter< float >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterDirection( const kernel::OrderParameter& parameter, const ASN1T_Direction& asn );
    virtual ~ActionParameterDirection();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterDirection( const ActionParameterDirection& );            //!< Copy constructor
    ActionParameterDirection& operator=( const ActionParameterDirection& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    geometry::Vector2f direction_;
    //@}
};

#endif // __ActionParameterDirection_h_
