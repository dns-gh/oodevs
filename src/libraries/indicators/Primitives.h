// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Primitives_h_
#define __Primitives_h_

#include "clients_kernel/Resolver.h"
#include <qstring.h>

namespace xml
{
    class xistream;
}

namespace indicators
{
    class DataTypeFactory;
    class Primitive;

// =============================================================================
/** @class  Primitives
    @brief  Primitives
*/
// Created: SBO 2009-04-06
// =============================================================================
class Primitives : public kernel::Resolver< Primitive, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             Primitives();
    virtual ~Primitives();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& file );
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Primitives( const Primitives& );            //!< Copy constructor
    Primitives& operator=( const Primitives& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPrimitive( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< DataTypeFactory > types_;
    //@}
};

}

#endif // __Primitives_h_
