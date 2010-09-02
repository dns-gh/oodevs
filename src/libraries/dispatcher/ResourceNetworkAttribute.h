// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkAttribute_h_
#define __ResourceNetworkAttribute_h_

#include "UrbanObjectAttribute_ABC.h"
#include "ObjectAttribute_ABC.h"

namespace Common
{
    class ResourceNetwork;
}

namespace dispatcher
{

// =============================================================================
/** @class  ResourceNetworkAttribute
    @brief  ResourceNetworkAttribute
*/
// Created: JSR 2010-08-17
// =============================================================================
class ResourceNetworkAttribute : public UrbanObjectAttribute_ABC
                               , public ObjectAttribute_ABC
{
    struct ResourceNetwork
    {
        struct Link
        {
            int kind_;
            unsigned int target_;
            int capacity_;
            unsigned int flow_;
        };
        unsigned int type_;
        std::vector< Link > links_;
        bool enabled_;
        unsigned int maxStock_;
        unsigned int stock_;
        unsigned int production_;
        unsigned int consumption_;
        bool critical_;
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ResourceNetworkAttribute( const MsgsSimToClient::MsgUrbanAttributes& message );
    explicit ResourceNetworkAttribute( const Common::MsgObjectAttributes& message );
    virtual ~ResourceNetworkAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual void Send  (       MsgsSimToClient::MsgUrbanAttributes& message ) const;
    virtual void Update( const Common::MsgObjectAttributes& message );
    virtual void Send  (       Common::MsgObjectAttributes& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const Common::ResourceNetwork& message );
    void Send( Common::ResourceNetwork& message, const ResourceNetwork& network ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned int, ResourceNetwork > resourceMap_;
    //@}
};

}

#endif // __ResourceNetworkAttribute_h_
