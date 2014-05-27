// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ActionManager_h_
#define __ActionManager_h_

#include <cstdint>
#include <map>

namespace sword
{
    class Action;
    class AutomatOrder;
    class CrowdOrder;
    class FragOrder;
    class KnowledgeMagicAction;
    class MagicAction;
    class ObjectMagicAction;
    class SetAutomatMode;
    class UnitMagicAction;
    class UnitOrder;
}

// ActionManager stores all orders and magic actions and assign to them a
// unique id When a new client connects, it will receive all previously
// stored magic orders.
class ActionManager
{
public:
             ActionManager();
    virtual ~ActionManager();

    template< typename Archive > void serialize( Archive& file, const unsigned int version );

    uint32_t Register( const sword::AutomatOrder& msg );
    uint32_t Register( const sword::CrowdOrder& msg );
    uint32_t Register( const sword::FragOrder& msg );
    uint32_t Register( const sword::KnowledgeMagicAction& msg );
    uint32_t Register( const sword::MagicAction& msg );
    uint32_t Register( const sword::ObjectMagicAction& msg );
    uint32_t Register( const sword::SetAutomatMode& msg );
    uint32_t Register( const sword::UnitMagicAction& msg );
    uint32_t Register( const sword::UnitOrder& msg );
    void     Send    ( uint32_t id, int32_t code, const std::string& msg );

    virtual void SendStateToNewClient();

private:
    uint32_t Register( const sword::Action& msg );

private:
    uint32_t ids_;
    std::map< uint32_t, sword::Action > actions_;
};

BOOST_CLASS_EXPORT_KEY( ActionManager )

#endif // __ActionManager_h_
