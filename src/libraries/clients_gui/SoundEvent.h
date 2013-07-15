// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __SoundEvent_h_
#define __SoundEvent_h_

#include <boost/noncopyable.hpp>
// =============================================================================
/** @class  SoundEvent
    @brief  SoundEvent
*/
// Created: NPT 2013-07-15
// =============================================================================

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{

class SoundEvent : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             SoundEvent( const kernel::Entity_ABC* entity, const std::string& soundType, int simulationTick );
    virtual ~SoundEvent();
    //@}

    //! @name Operations
    //@{
    const kernel::Entity_ABC* GetEntity() const;
    const std::string& GetSoundType() const;
    int GetSoundTick() const;
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* entity_;
    const std::string& soundType_;
    int simulationTick_;
    //@}
};

}

#endif // __SoundEvent_h_
