// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DEC_ResourceNetwork_h_
#define __DEC_ResourceNetwork_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class ResourceNetworkElement;
}

// =============================================================================
/** @class  DEC_ResourceNetwork
    @brief  DEC_ResourceNetwork
*/
// Created: JSR 2011-05-03
// =============================================================================
class DEC_ResourceNetwork : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_ResourceNetwork( const sword::ResourceNetworkElement& message );
    virtual ~DEC_ResourceNetwork();
    //@}

    //! @name Operations
    //@{
    void Serialize( sword::ResourceNetworkElement& message ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetObjectId() const;
    const std::string& GetResource() const;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int objectId_;
    const std::string resource_;
    //@}
};

#endif // __DEC_ResourceNetwork_h_
