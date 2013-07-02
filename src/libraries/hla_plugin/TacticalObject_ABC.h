// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TacticalObject_ABC_h
#define plugins_hla_TacticalObject_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    struct TacticalObjectEventListener_ABC;

// =============================================================================
/** @class  TacticalObject_ABC
    @brief  TacticalObject_ABC definition
*/
// Created: AHC 2012-08-08
// =============================================================================
class TacticalObject_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
            TacticalObject_ABC() {}
    virtual ~TacticalObject_ABC() {}
    //@}

    //! @name Registration
    //@{
    virtual void Register( TacticalObjectEventListener_ABC& listener ) = 0;
    virtual void Unregister( TacticalObjectEventListener_ABC& listener ) = 0;
    //@}
};

}
}

#endif // plugins_hla_TacticalObject_ABC_h
