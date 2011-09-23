// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef InteractionSender_ABC_h
#define InteractionSender_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  InteractionSender_ABC
    @brief  Interaction sender definition
*/
// Created: SLI 2011-09-23
// =============================================================================
template< typename Interaction >
class InteractionSender_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InteractionSender_ABC() {}
    virtual ~InteractionSender_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( const Interaction& interaction ) = 0;
    //@}
};

}
}

#endif // InteractionSender_ABC_h
