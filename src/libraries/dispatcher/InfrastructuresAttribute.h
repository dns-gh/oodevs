// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __InfrastructuresAttribute_h_
#define __InfrastructuresAttribute_h_

#include "UrbanObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  InfrastructuresAttribute
    @brief  InfrastructuresAttribute
*/
// Created: JSR 2010-08-17
// =============================================================================
class InfrastructuresAttribute : public UrbanObjectAttribute_ABC
{
    struct ResourceNetwork
    {
        struct Link
        {
            int kind_;
            unsigned int target_;
            int capacity_;
            int flow_;
        };
        bool producer_;
        unsigned int production_;
        unsigned int type_;
        bool enabled_;
        bool stockActive_;
        unsigned int stock_;
        std::vector< Link > links_;
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit InfrastructuresAttribute( const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual ~InfrastructuresAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const MsgsSimToClient::MsgUrbanAttributes& message );
    virtual void Send  (       MsgsSimToClient::MsgUrbanAttributes& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned int, ResourceNetwork > resourceMap_;
    //@}
};

}

#endif // __InfrastructuresAttribute_h_
