// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Constant_h_
#define __Constant_h_

#include "ModelFunction.h"
#include "Values.h"
#include <xeumeuleu/xml.h>
#include <boost/lexical_cast.hpp>

// =============================================================================
/** @class  Constant
    @brief  Constant
*/
// Created: AGE 2007-10-08
// =============================================================================
template< typename T >
class Constant : public ModelFunction< ConstantValue< T > >
{

public:
    //! @name Constructors/Destructor
    //@{
    Constant( xml::xistream& xis, ValueHandler_ABC< Return_Type >& handler )
        : ModelFunction< ConstantValue< T > >( handler, ConstantValue< T >( ReadValue< T >( xis ) ) )
        {}
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        ModelFunction< ConstantValue< T > >::BeginTick();
    }
    virtual void EndTick()
    {
        ModelFunction< ConstantValue< T > >::EndTick();
    }
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    inline
    static T ReadValue( xml::xistream& xis )
    {
        return boost::lexical_cast< T >( xml::attribute< std::string >( xis, "value" )  );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Constant( const Constant& );            //!< Copy constructor
    Constant& operator=( const Constant& ); //!< Assignment operator
    //@}
};

#endif // __Constant_h_
