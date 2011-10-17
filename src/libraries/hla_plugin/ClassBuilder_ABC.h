// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ClassBuilder_ABC_h
#define plugins_hla_ClassBuilder_ABC_h

#include <boost/noncopyable.hpp>

namespace hla
{
    template< typename T > class Class;
}

namespace plugins
{
namespace hla
{
    class HlaObject_ABC;
    class Federate_ABC;

// =============================================================================
/** @class  ClassBuilder_ABC
    @brief  Class builder definition
*/
// Created: SLI 2011-07-26
// =============================================================================
class ClassBuilder_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassBuilder_ABC() {}
    virtual ~ClassBuilder_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Build( Federate_ABC& federate, ::hla::Class< HlaObject_ABC >& hlaClass ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_ClassBuilder_ABC_h
