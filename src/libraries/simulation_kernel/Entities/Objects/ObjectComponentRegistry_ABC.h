// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectComponentRegistry_ABC_h_
#define __ObjectComponentRegistry_ABC_h_

namespace xml
{
    class xistream;
}

#include <boost/function.hpp>

template< typename Component >
class ObjectComponentRegistry_ABC
{
public:
    //! @name Types
    //@{
    typedef boost::function2< void, Component&, xml::xistream& > T_CallBack;
    //@}

public:	
	//! @name Constructors/Destructor
    //@{
			ObjectComponentRegistry_ABC() {}
	virtual ~ObjectComponentRegistry_ABC() {}
	//@}	

	//! @name Registrer
    //@{
	virtual void Register( const std::string& component, const T_CallBack& callback ) = 0;
	//@}

protected:    
	//! @name Type
    //@{
	typedef std::map< std::string, T_CallBack >     T_CallBacks;
    typedef typename T_CallBacks::const_iterator    CIT_Callbacks;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectComponentRegistry_ABC( const ObjectComponentRegistry_ABC& );            //!< Copy constructor
    ObjectComponentRegistry_ABC& operator=( const ObjectComponentRegistry_ABC& ); //!< Assignment operator
    //@}
};

#endif