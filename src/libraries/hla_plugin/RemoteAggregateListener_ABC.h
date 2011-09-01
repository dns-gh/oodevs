// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAggregateListener_ABC_h
#define plugins_hla_RemoteAggregateListener_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RemoteAggregateListener_ABC
    @brief  Remote aggregate listener definition
*/
// Created: SLI 2011-07-28
// =============================================================================
class RemoteAggregateListener_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAggregateListener_ABC() {}
    virtual ~RemoteAggregateListener_ABC() {}
    //@}

    //! @name Operations
    //@{

    //@}
};

}
}

#endif // plugins_hla_RemoteAggregateListener_ABC_h
