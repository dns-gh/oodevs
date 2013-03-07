// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BypassAttribute_h_
#define __BypassAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"

namespace kernel
{
    class Displayer_ABC;
    class NBCAgent;
    class PropertiesDictionary;
    class Entity_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  BypassAttribute
    @brief  BypassAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class BypassAttribute : public kernel::BypassAttribute_ABC
                      , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit BypassAttribute( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             BypassAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~BypassAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetBypassConstruction( int value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BypassAttribute( const BypassAttribute& );            //!< Copy constructor
    BypassAttribute& operator=( const BypassAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< int > rBypassConstructionPercentage_;
    //@}
};

#endif // __BypassAttribute_h_
