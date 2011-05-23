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
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class Displayer_ABC;
    class Object_ABC;
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
             AltitudeModifierAttribute( kernel::PropertiesDictionary& dico, kernel::DetectionMap& detection, const kernel::Object_ABC& object, kernel::Controllers& controllers );
             AltitudeModifierAttribute( xml::xistream& xis, kernel::DetectionMap& detection, const kernel::Object_ABC& object, kernel::PropertiesDictionary& dico, kernel::Controllers& controllers );
    virtual ~AltitudeModifierAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void NotifyUpdated( const AltitudeModifierAttribute& attribute );
    virtual void NotifyDeleted( const kernel::Object_ABC& object );
    //@}

    //! @name Modifiers
    //@{
    void SetHeight( unsigned int height );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AltitudeModifierAttribute( const AltitudeModifierAttribute& );            //!< Copy constructor
    AltitudeModifierAttribute& operator=( const AltitudeModifierAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    void ModifyAltitude( int heightOffset );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::DetectionMap& detection_;
    const kernel::Object_ABC& object_;
    kernel::UnitedValue< unsigned int > height_;
    unsigned int oldHeight_;
    //@}
};

#endif // __AltitudeModifierAttribute_h_
