// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_IndirectFireSender_h
#define plugins_hla_IndirectFireSender_h

#include "tools/MessageObserver.h"
#include <map>

namespace sword
{
    class SimToClient_Content;
    class StartUnitFire;
    class StopUnitFire;
}

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace plugins
{
namespace hla
{
    template< typename Interaction > class InteractionSender_ABC;
    class MunitionTypeResolver_ABC;

namespace interactions
{
    struct MunitionDetonation;
}
// =============================================================================
/** @class  IndirectFireSender
    @brief  Indirect fire sender
*/
// Created: SLI 2011-09-23
// =============================================================================
class IndirectFireSender : private tools::MessageObserver< sword::StartUnitFire >
                         , private tools::MessageObserver< sword::StopUnitFire >
{
public:
    //! @name Constructors/Destructor
    //@{
             IndirectFireSender( InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender,
                                 tools::MessageController_ABC< sword::SimToClient_Content >& controller, const std::string& federateName,
                                 MunitionTypeResolver_ABC& munitionTypeResolver );
    virtual ~IndirectFireSender();
    //@}

private:
    //! @name Messages
    //@{
    virtual void Notify( const sword::StartUnitFire& message, int context );
    virtual void Notify( const sword::StopUnitFire& message, int context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, sword::StartUnitFire > T_Fires;
    //@}

private:
    //! @name Member data
    //@{
    InteractionSender_ABC< interactions::MunitionDetonation >& interactionSender_;
    const std::string federateName_;
    const MunitionTypeResolver_ABC& munitionTypeResolver_;
    T_Fires fires_;
    //@}
};

}
}

#endif // plugins_hla_IndirectFireSender_h
