// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExtensionType_h_
#define __ExtensionType_h_

#include <tools/Resolver.h>

namespace xml
{
    class xistream;
}

namespace kernel
{
class AttributeType;

// =============================================================================
/** @class  ExtensionType
    @brief  ExtensionType
*/
// Created: JSR 2010-10-01
// =============================================================================
class ExtensionType : public tools::StringResolver< AttributeType >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ExtensionType( xml::xistream& xis );
    virtual ~ExtensionType();
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::AttributeType* > T_AttributesTypes;
    //@}

    //! @name Operations
    //@{
    void GetAttributeTypes( const std::string& type, T_AttributesTypes& attributeTypes );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExtensionType( const ExtensionType& );            //!< Copy constructor
    ExtensionType& operator=( const ExtensionType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAttribute( xml::xistream& xis );
    //@}
};

}

#endif // __ExtensionType_h_
