// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterDescription_h_
#define __FilterDescription_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  FilterDescription
    @brief  FilterDescription
*/
// Created: ABR 2011-09-29
// =============================================================================
class FilterDescription : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             FilterDescription();
    explicit FilterDescription( xml::xistream& xis );
    virtual ~FilterDescription();
    //@}

    //! @name Operations
    //@{
    const std::string GetName() const;
    const std::string GetDescription() const;
    const std::string GetCurrentLanguage() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDescriptions( xml::xistream& xis );
    void ReadDescription( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::pair< std::string, std::string > > T_Descriptions;
    //@}

private:
    //! @name Member data
    //@{
    T_Descriptions    descriptions_;
    const std::string currentLanguage_;
    //@}
};

#endif // __FilterDescription_h_
