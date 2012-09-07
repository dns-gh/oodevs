// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteTacticalObjectSubject_ABC_h
#define plugins_hla_RemoteTacticalObjectSubject_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    class ClassListener_ABC;

// =============================================================================
/** @class  plugins_hla_RemoteTacticalObjectSubject_ABC_h
    @brief  plugins_hla_RemoteTacticalObjectSubject_ABC_h
*/
// Created: AHC 2012-09-07
// =============================================================================
class RemoteTacticalObjectSubject_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~RemoteTacticalObjectSubject_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void RegisterTactical( ClassListener_ABC& listener ) = 0;
    virtual void UnregisterTactical( ClassListener_ABC& listener ) = 0;
    //@}
};

}
}

#endif // plugins_hla_RemoteTacticalObjectSubject_ABC_h
