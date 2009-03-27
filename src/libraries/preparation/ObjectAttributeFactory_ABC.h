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

namespace xml
{
    class xistream;
}

namespace tools
{
    template< typename T > class SortedInterfaceContainer;
}

namespace kernel
{
    class Extension_ABC;
    class PropertiesDictionary;
}


#include <boost/function.hpp>

class ObjectAttributeFactory_ABC
{
public:	
	//! @name Constructors/Destructor
    //@{
			ObjectAttributeFactory_ABC() {}
	virtual ~ObjectAttributeFactory_ABC() {}
	//@}	

	//! @name Registrer
    //@{	
    virtual void Create( const std::string& attribute, tools::SortedInterfaceContainer< kernel::Extension_ABC >& result, kernel::PropertiesDictionary& dico, xml::xistream& xis ) = 0;
	//@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectAttributeFactory_ABC( const ObjectAttributeFactory_ABC& );            //!< Copy constructor
    ObjectAttributeFactory_ABC& operator=( const ObjectAttributeFactory_ABC& ); //!< Assignment operator
    //@}
};

#endif // __ObjectAttributeFactory_ABC_h_