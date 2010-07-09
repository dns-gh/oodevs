// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLima_h_
#define __ActionParameterLima_h_

#include "Parameter.h"

namespace Common
{
    class MsgLimaOrder;
}

namespace kernel
{
    class Location_ABC;
    class CoordinateConverter_ABC;
}

namespace actions {
    namespace parameters {

class Location;

// =============================================================================
/** @class  Lima
    @brief  Lima
*/
// Created: SBO 2007-04-26
// =============================================================================
class Lima : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             Lima( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             Lima( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Common::MsgLimaOrder& message );
             Lima( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~Lima();
    //@}

    //! @name Operations
    //@{
    void AddFunction( unsigned int i );
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;

    void CommitTo( Common::MsgLimaOrder& message ) const;
    void Clean( Common::MsgLimaOrder& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Lima( const Lima& );            //!< Copy constructor
    Lima& operator=( const Lima& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    void ReadParameter( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

    }
}

#endif // __ActionParameterLima_h_
