// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RcEntityResolver_ABC_h_
#define __RcEntityResolver_ABC_h_

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  RcEntityResolver_ABC
    @brief  Rc entity resolver declaration
*/
// Created: SBO 2006-09-18
// =============================================================================
class RcEntityResolver_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             RcEntityResolver_ABC() {}
    virtual ~RcEntityResolver_ABC() {}
    //@}

    // The entity is an optional context in which to resolve the id,
    // for instance if type is a knowledge then the resolution will
    // depend on the knowledge group to which the entity belongs.
    virtual QString CreateLink( const kernel::Entity_ABC* entity, const std::string& type, unsigned long id ) const = 0;
};

#endif // __RcEntityResolver_ABC_h_
