// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __SubObjectName_h_
#define __SubObjectName_h_

#include <boost/noncopyable.hpp>

namespace gui
{
// =============================================================================
/** @class  ObjectLevelScope
    @brief  ObjectLevelScope
*/
// Created: NPT 2013-03-12
// =============================================================================
class SubObjectName : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             SubObjectName( const QString& objectName );
    virtual ~SubObjectName();
    //@}
};

}

#endif // __SubObjectNames_h_
