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
class ResourceNetworkType
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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkType( const ResourceNetworkType& );            //!< Copy constructor
    ResourceNetworkType& operator=( const ResourceNetworkType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    float red_, green_, blue_;
    //@}
};

}

#endif // __ResourceNetworkType_h_
