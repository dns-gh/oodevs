// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterDateTime_h_
#define __ActionParameterDateTime_h_

#include "Parameter.h"

namespace Common
{
    class MsgDateTime;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  DateTime
    @brief  DateTime
*/
// Created: SBO 2007-05-15
// =============================================================================
class DateTime : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             DateTime( const kernel::OrderParameter& parameter, xml::xistream& xis );
             DateTime( const kernel::OrderParameter& parameter, const Common::MsgDateTime& date );
             DateTime( const kernel::OrderParameter& parameter, const QDateTime& date );
    virtual ~DateTime();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    void CommitTo( Common::MsgDateTime& message ) const;
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void Clean( Common::MsgMissionParameter& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DateTime( const DateTime& );            //!< Copy constructor
    DateTime& operator=( const DateTime& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string time_;
    //@}
};
    }
}

#endif // __ActionParameterDateTime_h_
