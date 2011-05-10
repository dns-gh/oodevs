// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ExtensionDependency_h_
#define __ExtensionDependency_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  ExtensionDependency
    @brief  ExtensionDependency
*/
// Created: ABR 2011-05-03
// =============================================================================
class ExtensionDependency : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ExtensionDependency( xml::xistream& xis );
    virtual ~ExtensionDependency();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    bool Allow( const std::string& value ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string          name_;
    std::vector< std::string > values_;
    //@}
};

}

#endif // __ExtensionDependency_h_
