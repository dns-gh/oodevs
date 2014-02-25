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
#include "clients_kernel/SubTypes.h"
#include "clients_kernel/Units.h"

namespace kernel
{
    class Displayer_ABC;
    class Entity_ABC;
}

namespace gui
{
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
    explicit TimeLimitedAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             TimeLimitedAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~TimeLimitedAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
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
    void CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Duration activityTime_;
    //@}
};

#endif // __TimeLimitedAttribute_h_
