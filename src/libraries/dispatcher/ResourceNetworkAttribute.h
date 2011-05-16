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

namespace sword
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
            unsigned int objectId_;
            int capacity_;
            unsigned int flow_;
        };
        std::string resource_;
        std::vector< Link > links_;
        bool enabled_;
        unsigned int maxStock_;
        unsigned int stock_;
        unsigned int production_;
        unsigned int consumption_;
        unsigned int maxConsumption_;
        unsigned int currentConsumption_;
        bool critical_;
        float functionalState_;
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ResourceNetworkAttribute( const sword::UrbanAttributes& message );
    explicit ResourceNetworkAttribute( const sword::ObjectAttributes& message );
    virtual ~ResourceNetworkAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::UrbanAttributes& message );
    virtual void Send  (       sword::UrbanAttributes& message ) const;
    virtual void Update( const sword::ObjectAttributes& message );
    virtual void Send  (       sword::ObjectAttributes& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const sword::ResourceNetwork& message );
    void Send( sword::ResourceNetwork& message, const ResourceNetwork& network ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::map< std::string, ResourceNetwork > resourceMap_;
    //@}
};

}

#endif // __ResourceNetworkAttribute_h_
