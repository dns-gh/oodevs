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

#include "ActionParameter.h"

// =============================================================================
/** @class  ActionParameterDirection
    @brief  ActionParameterDirection
*/
// Created: SBO 2007-04-16
// =============================================================================
class ActionParameterDirection : public ActionParameter< int >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterDirection( const kernel::OrderParameter& parameter, int value );
             ActionParameterDirection( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~ActionParameterDirection();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterDirection( const ActionParameterDirection& );            //!< Copy constructor
    ActionParameterDirection& operator=( const ActionParameterDirection& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    geometry::Vector2f direction_;
    //@}
};

#endif // __ActionParameterDirection_h_
