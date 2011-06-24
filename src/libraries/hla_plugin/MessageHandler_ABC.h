// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MessageHandler_ABC_h
#define plugins_hla_MessageHandler_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MessageHandler_ABC
    @brief  Message handler definition
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Category >
class MessageHandler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageHandler_ABC() {}
    virtual ~MessageHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const Category& message ) = 0;
    //@}
};

}
}

#endif // plugins_hla_MessageHandler_ABC_h
