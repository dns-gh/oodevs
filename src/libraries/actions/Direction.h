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

#include "Parameter.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Direction
    @brief  Direction
*/
// Created: SBO 2007-04-16
// =============================================================================
class Direction : public Parameter< int >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Direction( const kernel::OrderParameter& parameter );
             Direction( const kernel::OrderParameter& parameter, int value );
             Direction( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~Direction();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    geometry::Vector2f direction_;
    //@}
};

}
}

#endif // __ActionParameterDirection_h_
