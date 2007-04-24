// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLimit_h_
#define __ActionParameterLimit_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  ActionParameterLimit
    @brief  ActionParameterLimit
*/
// Created: SBO 2007-04-13
// =============================================================================
class ActionParameterLimit : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterLimit( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Line& line );
             ActionParameterLimit( const QString& name, const T_PointVector& points );
    virtual ~ActionParameterLimit();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLimit( const ActionParameterLimit& );            //!< Copy constructor
    ActionParameterLimit& operator=( const ActionParameterLimit& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_PointVector points_;
    geometry::Rectangle2f boundingBox_;
    //@}
};

#endif // __ActionParameterLimit_h_
