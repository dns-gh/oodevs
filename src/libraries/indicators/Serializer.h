// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Serializer_h_
#define __Serializer_h_

#include "GrammarHandler_ABC.h"
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xostream;
}

namespace indicators
{
    class ElementFactory_ABC;
    class Element_ABC;

// =============================================================================
/** @class  GrammarHandler
    @brief  GrammarHandler
*/
// Created: SBO 2009-03-16
// =============================================================================
class Serializer : public GrammarHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Serializer( const ElementFactory_ABC& factory );
    virtual ~Serializer();
    //@}

    //! @name Operations
    //@{
    virtual void HandleNumber( double value );
    virtual void HandleString( const std::string& value );
    virtual void HandleVariable( const std::string& name );
    virtual void HandleFunctionCall( const std::string& name, unsigned int parameters );
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Serializer( const Serializer& );            //!< Copy constructor
    Serializer& operator=( const Serializer& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const ElementFactory_ABC& factory_;
    std::vector< boost::shared_ptr< Element_ABC > > stack_;
    //@}
};

}

#endif // __Serializer_h_
