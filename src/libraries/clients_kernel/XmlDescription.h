// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __XmlDescription_h_
#define __XmlDescription_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  XmlDescription
    @brief  XmlDescription
*/
// Created: ABR 2011-11-10
// =============================================================================
class XmlDescription : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             XmlDescription( const std::string& currentLanguage );
    explicit XmlDescription( xml::xistream& xis, const std::string& currentLanguage );
    virtual ~XmlDescription();
    //@}

    //! @name Operations
    //@{
    const std::string GetName() const;
    const std::string GetDescription() const;
    const std::string GetCurrentLanguage() const;
    void SetCurrentLanguage( const std::string& currentLanguage );
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
    std::string currentLanguage_;
    //@}
};

}

#endif // __XmlDescription_h_
