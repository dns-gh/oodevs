// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanTemplateType_h_
#define __UrbanTemplateType_h_

#include "UrbanColor_ABC.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
class ObjectTypes;
class UrbanObject_ABC;

// =============================================================================
/** @class  UrbanTemplateType
    @brief  UrbanTemplateType
*/
// Created: JSR 2012-06-11
// =============================================================================
class UrbanTemplateType : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanTemplateType( xml::xistream& xis );
    virtual ~UrbanTemplateType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    void Fill( kernel::UrbanObject_ABC& urbanObject, const ObjectTypes& objectTypes ) const;
    bool Matches( const kernel::UrbanObject_ABC& urbanObject ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadUsage( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, float > T_Usages;
    typedef T_Usages::const_iterator     CIT_Usages;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string roofShape_;
    std::string material_;
    unsigned int height_;
    unsigned int floorNumber_;
    float occupation_;
    float trafficability_;
    unsigned int parkingFloors_;
    T_Usages usages_;
    UrbanBlockColor color_;
    //@}
};

}

#endif // __UrbanTemplateType_h_
