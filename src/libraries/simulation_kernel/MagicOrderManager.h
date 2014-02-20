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

#include "MagicOrderManager_ABC.h"
#include "protocol/SimulationSenders.h"
#include <cstdint>
#include <map>

class MagicOrderManager : public MagicOrderManager_ABC
{
public:
             MagicOrderManager();
    virtual ~MagicOrderManager();

    template< typename Archive > void serialize( Archive& file, const unsigned int version );

    virtual uint32_t Register( const sword::MagicAction& msg );
    virtual uint32_t Register( const sword::UnitMagicAction& msg );
    virtual uint32_t Register( const sword::ObjectMagicAction& msg );
    virtual uint32_t Register( const sword::KnowledgeMagicAction& msg );
    virtual uint32_t Register( const sword::SetAutomatMode& msg );
    virtual void     Send    ( uint32_t id );

    virtual void SendStateToNewClient();

private:
    uint32_t Register( const sword::MagicOrder& msg );

private:
    uint32_t ids_;
    std::map< uint32_t, sword::MagicOrder > orders_;
};

BOOST_CLASS_EXPORT_KEY( MagicOrderManager )

#endif // __MagicOrderManager_h_
