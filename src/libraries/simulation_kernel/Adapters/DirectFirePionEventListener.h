// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_DIRECT_FIRE_PION_EVENT_LISTENER_H
#define SWORD_DIRECT_FIRE_PION_EVENT_LISTENER_H

#include <core/EventListener_ABC.h>
#include <map>

class PHY_FireResults_Pion;

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  DirectFirePionEventListener
    @brief  Direct fire pion event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class DirectFirePionEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit DirectFirePionEventListener( core::Facade& facade );
    virtual ~DirectFirePionEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, PHY_FireResults_Pion* > T_Results;
    typedef T_Results::iterator                            IT_Results;
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const core::Model& event );
    void Remove( const core::Model& event );
    //@}

private:
    //! @name Member data
    //@{
    core::Facade& facade_;
    T_Results results_;
    //@}
};

}

#endif // SWORD_DIRECT_FIRE_PION_EVENT_LISTENER_H
