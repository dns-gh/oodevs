// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DelayAttribute_h_
#define __DelayAttribute_h_

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
/** @class  DelayAttribute
    @brief  DelayAttribute
*/
// Created: JSR 2010-07-06
// =============================================================================
class DelayAttribute : public kernel::DelayAttribute_ABC
                     , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DelayAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             DelayAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~DelayAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetDelay( unsigned int time );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DelayAttribute( const DelayAttribute& );            //!< Copy constructor
    DelayAttribute& operator=( const DelayAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< unsigned int > delay_;
    //@}
};

#endif // __DelayAttribute_h_
