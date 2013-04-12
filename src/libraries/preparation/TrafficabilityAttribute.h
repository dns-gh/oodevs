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
             TrafficabilityAttribute( gui::PropertiesDictionary& dictionary, double value, const kernel::Entity_ABC& entity );
             TrafficabilityAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~TrafficabilityAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TrafficabilityAttribute( const TrafficabilityAttribute& );            //!< Copy constructor
    TrafficabilityAttribute& operator=( const TrafficabilityAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< double > max_;
    //@}
};

#endif // __TrafficabilityAttribute_h_
