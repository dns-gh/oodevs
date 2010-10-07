// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExtensionTypes_h_
#define __ExtensionTypes_h_

#include "tools/Resolver.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
class DictionaryType;
class ExtensionType;

// =============================================================================
/** @class  ExtensionTypes
    @brief  ExtensionTypes
*/
// Created: JSR 2010-10-01
// =============================================================================
class ExtensionTypes : public tools::StringResolver< DictionaryType >
                     , public tools::StringResolver< ExtensionType >
{
public:
    //! @name Constructors/Destructor
    //@{
             ExtensionTypes();
    virtual ~ExtensionTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& file );
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExtensionTypes( const ExtensionTypes& );            //!< Copy constructor
    ExtensionTypes& operator=( const ExtensionTypes& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadElement( const std::string& name, xml::xistream& xis );
    void ReadDictionary( xml::xistream& xis );
    //@}
};

}

#endif // __ExtensionTypes_h_
