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

#include <tools/Resolver.h>
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )
#include <boost/noncopyable.hpp>

namespace xml
{
    class xisubstream;
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
class Primitives : public tools::Resolver< Primitive, QString >,
                   private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Primitives();
    virtual ~Primitives();
    //@}

    //! @name Operations
    //@{
    void Load( xml::xisubstream xis );
    void Purge();
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< DataTypeFactory > types_;
    //@}
};

}

#endif // __Primitives_h_
