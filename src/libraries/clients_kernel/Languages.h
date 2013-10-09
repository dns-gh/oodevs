// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef _kernel_Languages_h
#define _kernel_Languages_h

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace tools
{
    class Path;
}

namespace kernel
{
    class Language;

// =============================================================================
/** @class  Languages
    @brief  Languages
*/
// Created: ABR 2013-09-30
// =============================================================================
class Languages : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< kernel::Language > > T_Languages;
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit Languages( const tools::Path& xmlFile );
    explicit Languages( xml::xistream& xis );
    virtual ~Languages();
    //@}

    //! @name Operations
    //@{
    const boost::shared_ptr< Language >& Get( const std::string& code ) const;
    const T_Languages& GetLanguages() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_Languages languages_;
    //@}
};

} //! namespace kernel

#endif // _kernel_Languages_h
