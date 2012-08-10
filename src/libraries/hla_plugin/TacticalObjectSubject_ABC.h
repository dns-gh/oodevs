// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TacticalObjectSubject_ABC_h
#define plugins_hla_TacticalObjectSubject_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    class TacticalObjectListener_ABC;

// =============================================================================
/** @class  TacticalObjectSubject_ABC
    @brief  TacticalObjectSubject_ABC
*/
// Created: AHC 2012-08-08
// =============================================================================
class TacticalObjectSubject_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
            TacticalObjectSubject_ABC() {}
    virtual ~TacticalObjectSubject_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( TacticalObjectListener_ABC& listener ) = 0;
    virtual void Unregister( TacticalObjectListener_ABC& listener ) = 0;
    //@}
};

}
}

#endif // plugins_hla_TacticalObjectSubject_ABC_h
