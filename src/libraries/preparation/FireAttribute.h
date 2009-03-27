// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireAttribute_h_
#define __FireAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class FireClass;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  FireAttribute
    @brief  FireAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class FireAttribute : public kernel::FireAttribute_ABC
                    , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FireAttribute( kernel::PropertiesDictionary& dico );
             FireAttribute( xml::xistream& xis, const kernel::Resolver_ABC< kernel::FireClass, std::string >& FireClasses, kernel::PropertiesDictionary& dico );
    virtual ~FireAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetClass( const kernel::FireClass& fireClass );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FireAttribute( const FireAttribute& );            //!< Copy constructor
    FireAttribute& operator=( const FireAttribute& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

public:
    //! @name Member data
    //@{
    kernel::FireClass* fireClass_;
    //@}
};

#endif // __FireAttribute_h_
