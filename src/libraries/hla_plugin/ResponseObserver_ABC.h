// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef ResponseObserver_ABC_h
#define ResponseObserver_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ResponseObserver_ABC
    @brief  Response observer definition
*/
// Created: SLI 2011-09-09
// =============================================================================
template< typename MessageResponse >
class ResponseObserver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ResponseObserver_ABC() {}
    virtual ~ResponseObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const MessageResponse& message, const std::string& identifier ) = 0;
    //@}
};

}
}

#endif // ResponseObserver_ABC_h
