// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_InteractionSender_h
#define plugins_hla_InteractionSender_h

#include "InteractionSender_ABC.h"
#include "InteractionBuilder.h"
#include <memory>

namespace hla
{
    template <typename T> class InteractionNotification_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  InteractionSender
    @brief  Interaction sender
*/
// Created: SLI 2011-11-07
// =============================================================================
template< typename T >
class InteractionSender : public InteractionSender_ABC< T >
{
public:
    //! @name Constructors/Destructor
    //@{
    InteractionSender( ::hla::InteractionNotification_ABC< T >& receiver, const InteractionBuilder& builder )
        : interaction_( new ::hla::Interaction< T >( receiver ) )
    {
        if(! builder.Build( *interaction_ ) )
        {
            interaction_.reset();
        }
    }

    virtual ~InteractionSender() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( const T& interaction )
    {
        if (IsSupported()) interaction_->Send( interaction );
    }

    virtual bool IsSupported() const
    {
        return interaction_.get() != NULL;
    }
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< ::hla::Interaction< T > > interaction_;
    //@}
};

}
}

#endif // InteractionSender_h
