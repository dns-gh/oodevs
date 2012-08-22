// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __TimeLimitedAttribute_h_
#define __TimeLimitedAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"

namespace kernel
{
    class Displayer_ABC;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  TimeLimitedAttribute
    @brief  TimeLimitedAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class TimeLimitedAttribute : public kernel::TimeLimitedAttribute_ABC
                           , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TimeLimitedAttribute( kernel::PropertiesDictionary& dictionary );
             TimeLimitedAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary );
    virtual ~TimeLimitedAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetActivityTime( unsigned int time );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimeLimitedAttribute( const TimeLimitedAttribute& );            //!< Copy constructor
    TimeLimitedAttribute& operator=( const TimeLimitedAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary );
    //@}

private:
    //! @name Member data
    //@{
    QTime activityTime_;
    //@}
};

#endif // __TimeLimitedAttribute_h_
