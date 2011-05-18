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
/** @class  AltitudeModifierAttribute
    @brief  AltitudeModifierAttribute
*/
// Created: JSR 2011-05-17
// =============================================================================
class AltitudeModifierAttribute : public kernel::AltitudeModifierAttribute_ABC
                                , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AltitudeModifierAttribute( kernel::PropertiesDictionary& dico );
             AltitudeModifierAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico );
    virtual ~AltitudeModifierAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
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
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< unsigned int > height_;
    //@}
};

#endif // __AltitudeModifierAttribute_h_
