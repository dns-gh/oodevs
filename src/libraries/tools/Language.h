// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _tools_Language_h_
#define _tools_Language_h_

#include <boost/noncopyable.hpp>
#include <vector>

namespace xml
{
    class xistream;
    class xostream;
}

namespace tools
{

// =============================================================================
/** @class  Language
    @brief  Language
*/
// Created: ABR 2013-07-09
// =============================================================================
class Language
{

public:
    //! @name Constructors/Destructor
    //@{
             Language( const Language& );
    explicit Language( xml::xistream& xis );
    virtual ~Language();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    const std::string& GetEnglishName() const;
    const std::string& GetCode() const;
    bool IsSupported() const;
    //@}

    //! @name Static methods
    //@{
    static const std::string& Current();
    static void SetCurrent( const std::string& language );
    static void InitFromRegistry();
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string englishName_;
    std::string code_;
    bool supported_;
    //@}

    //! @name Static member data
    //@{
    static std::string current_;
    //@}
};

typedef std::vector< tools::Language > LanguagesVector;

} //! namespace tools

#endif // _tools_Language_h_
