// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesDictionary_h_
#define __PropertiesDictionary_h_

#include "Extension_ABC.h"
#include "Property_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class Controller;

// =============================================================================
/** @class  PropertiesDictionary
    @brief  PropertiesDictionary
    // $$$$ SBO 2006-10-17: DataDictionary... factor...
*/
// Created: SBO 2006-10-17
// =============================================================================
class PropertiesDictionary : public Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertiesDictionary( Controller& controller );
    virtual ~PropertiesDictionary();
    //@}

    //! @name Operations
    //@{
    template< typename T, typename Owner >
    void Register( const Owner& owner, const QString& name, T& value )
    {
        Property_ABC*& property = properties_[ name ];
        delete property;
        property = new Property< T, Owner >( controller_, owner, value );
    }

    void Display( Displayer_ABC& displayer );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PropertiesDictionary( const PropertiesDictionary& );            //!< Copy constructor
    PropertiesDictionary& operator=( const PropertiesDictionary& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    typedef std::map< QString, Property_ABC* >  T_Properties;
    typedef T_Properties::iterator             IT_Properties;
    typedef T_Properties::const_iterator      CIT_Properties;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    T_Properties properties_;
    //@}
};

}

#endif // __PropertiesDictionary_h_
