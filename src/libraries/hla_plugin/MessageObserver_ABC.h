// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MessageObserver_ABC_h
#define plugins_hla_MessageObserver_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MessageObserver_ABC
    @brief  Message observer definition
*/
// Created: SLI 2011-06-24
// =============================================================================
template< typename Message >
class MessageObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageObserver_ABC() {}
    virtual ~MessageObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
        virtual void Notify( const Message& message ) = 0;
    //@}
};

}
}

#endif // plugins_hla_MessageObserver_ABC_h
