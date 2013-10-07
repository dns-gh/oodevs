// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __Language_h_
#define __Language_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{

// =============================================================================
/** @class  Language
    @brief  Language
*/
// Created: ABR 2013-07-09
// =============================================================================
class Language : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Language( xml::xistream& xis );
    virtual ~Language();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    const std::string& GetCode() const;
    //@}

    //! @name Static methods
    //@{
    static const std::string& Current();
    static void SetCurrent( const std::string& language );
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    const std::string code_;
    //@}

    //! @name Static member data
    //@{
    static std::string current_;
    //@}
};

} //! namespace kernel

#endif // __Language_h_
