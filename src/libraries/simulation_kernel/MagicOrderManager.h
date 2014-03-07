// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __MagicOrderManager_h_
#define __MagicOrderManager_h_

#include <cstdint>
#include <map>

namespace sword
{
    class KnowledgeMagicAction;
    class MagicAction;
    class MagicOrder;
    class ObjectMagicAction;
    class SetAutomatMode;
    class UnitMagicAction;
}

// MagicOrderManager stores all magic orders and assign to them a unique id
// When a new client connects, it will receive all previously stored magic orders.
class MagicOrderManager
{
public:
             MagicOrderManager();
    virtual ~MagicOrderManager();

    template< typename Archive > void serialize( Archive& file, const unsigned int version );

    uint32_t Register( const sword::MagicAction& msg );
    uint32_t Register( const sword::UnitMagicAction& msg );
    uint32_t Register( const sword::ObjectMagicAction& msg );
    uint32_t Register( const sword::KnowledgeMagicAction& msg );
    uint32_t Register( const sword::SetAutomatMode& msg );
    void     Send    ( uint32_t id, int32_t code, const std::string& msg );

    virtual void SendStateToNewClient();

private:
    uint32_t Register( const sword::MagicOrder& msg );

private:
    uint32_t ids_;
    std::map< uint32_t, sword::MagicOrder > orders_;
};

BOOST_CLASS_EXPORT_KEY( MagicOrderManager )

#endif // __MagicOrderManager_h_
