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

namespace kernel
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
             Language();
             Language( const Language& );
    explicit Language( xml::xistream& xis );
    virtual ~Language();
    //@}

    //! @name Operations
    //@{
    void Read( xml::xistream& xis );
    void Write( xml::xostream& xos ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    const std::string& GetShortName() const;
    //@}

    //! @name Static methods
    //@{
    static const std::string& Current();
    static void SetCurrent( const std::string& language );
    static bool IsDefault( const std::string& language );
    static bool IsCurrentDefault();
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string shortname_;
    //@}

    //! @name Static member data
    //@{
public:
    static const std::string default_;
private:
    static std::string current_;
    //@}
};

xml::xostream& operator<<( xml::xostream& xos, const Language& language );
xml::xistream& operator>>( xml::xistream& xis, Language& language );

} //! namespace kernel

#endif // __Language_h_
