// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttributeFactory_ABC_h_
#define __ObjectAttributeFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class PropertiesDictionary;
}

class ObjectAttributeFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectAttributeFactory_ABC() {}
    virtual ~ObjectAttributeFactory_ABC() {}
    //@}

    //! @name Register
    //@{
    virtual void Create( const std::string& attribute, kernel::Object_ABC& object, kernel::PropertiesDictionary& dico, xml::xistream& xis ) = 0;
    //@}
};

#endif // __ObjectAttributeFactory_ABC_h_