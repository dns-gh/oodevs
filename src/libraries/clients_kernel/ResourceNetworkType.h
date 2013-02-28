// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __kernel_ResourceNetworkType_h_
#define __kernel_ResourceNetworkType_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  ResourceNetworkType
    @brief  ResourceNetworkType
*/
// Created: JSR 2010-09-10
// =============================================================================
class ResourceNetworkType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ResourceNetworkType( xml::xistream& xis );
    virtual ~ResourceNetworkType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    void GetColor( float& red, float& green, float& blue ) const;
    unsigned int GetDefaultProduction() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    float red_, green_, blue_;
    unsigned int defaultProduction_;
    //@}
};

}

#endif // __ResourceNetworkType_h_
