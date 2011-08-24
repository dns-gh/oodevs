// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __TrafficabilityAttribute_h_
#define __TrafficabilityAttribute_h_

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
/** @class  TrafficabilityAttribute
    @brief  Trafficability attribute
*/
// Created: LGY 2011-08-23
// =============================================================================
class TrafficabilityAttribute : public kernel::TrafficabilityAttribute_ABC
                              , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TrafficabilityAttribute( kernel::PropertiesDictionary& dictionary, double value );
             TrafficabilityAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary );
    virtual ~TrafficabilityAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TrafficabilityAttribute( const TrafficabilityAttribute& );            //!< Copy constructor
    TrafficabilityAttribute& operator=( const TrafficabilityAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< double > max_;
    //@}
};

#endif // __TrafficabilityAttribute_h_
