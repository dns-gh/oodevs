// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AltitudeModifierAttribute_h_
#define __AltitudeModifierAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class Displayer_ABC;
    class Object_ABC;
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
/** @class  AltitudeModifierAttribute
    @brief  AltitudeModifierAttribute
*/
// Created: JSR 2011-05-17
// =============================================================================
class AltitudeModifierAttribute : public kernel::AltitudeModifierAttribute_ABC
                                , public kernel::Serializable_ABC
                                , public tools::Observer_ABC
                                , public tools::ElementObserver_ABC< AltitudeModifierAttribute >
                                , public tools::ElementObserver_ABC< kernel::Object_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             AltitudeModifierAttribute( gui::PropertiesDictionary& dictionary, kernel::DetectionMap& detection,
                                        const kernel::Object_ABC& object, kernel::Controllers& controllers, const kernel::Entity_ABC& entity );
             AltitudeModifierAttribute( xml::xistream& xis, kernel::DetectionMap& detection, const kernel::Object_ABC& object,
                                        gui::PropertiesDictionary& dictionary, kernel::Controllers& controllers, const kernel::Entity_ABC& entity );
    virtual ~AltitudeModifierAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    virtual void NotifyUpdated( const AltitudeModifierAttribute& attribute );
    virtual void NotifyDeleted( const kernel::Object_ABC& object );
    //@}

    //! @name Modifiers
    //@{
    void SetHeight( unsigned int height );
    //@}

    //! @name Operations
    //@{
    void BeginDrag() const;
    void EndDrag() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AltitudeModifierAttribute( const AltitudeModifierAttribute& );            //!< Copy constructor
    AltitudeModifierAttribute& operator=( const AltitudeModifierAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::DetectionMap& detection_;
    const kernel::Object_ABC& object_;
    kernel::UnitedValue< unsigned int > height_;
    mutable T_PointVector dragLocation_;
    //@}
};

#endif // __AltitudeModifierAttribute_h_
