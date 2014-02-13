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

namespace sword
{
    class DateTime;
}

namespace actions
{
namespace parameters
{

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
             DateTime( const kernel::OrderParameter& parameter );
             DateTime( const kernel::OrderParameter& parameter, const sword::DateTime& date );
             DateTime( const kernel::OrderParameter& parameter, const QDateTime& date );
    virtual ~DateTime();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    void CommitTo( sword::DateTime& message ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    const std::string& GetTime() const;
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
    std::string time_;
    //@}
};

}
}

#endif // __ActionParameterDateTime_h_
