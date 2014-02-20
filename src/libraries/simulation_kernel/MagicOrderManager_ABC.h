// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MagicOrderManager_ABC_h_
#define __MagicOrderManager_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class MagicAction;
}

class MagicOrderManager_ABC : public boost::noncopyable
{
public:
    virtual ~MagicOrderManager_ABC() {}
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}

    virtual uint32_t Register( const sword::MagicAction& msg ) = 0;
    virtual uint32_t Register( const sword::UnitMagicAction& msg ) = 0;
    virtual uint32_t Register( const sword::ObjectMagicAction& msg ) = 0;
    virtual uint32_t Register( const sword::KnowledgeMagicAction& msg ) = 0;
    virtual uint32_t Register( const sword::SetAutomatMode& msg ) = 0;
    virtual void     Send    ( uint32_t id ) = 0;

    virtual void SendStateToNewClient() = 0;
};

#endif // __MagicOrderManager_ABC_h_
