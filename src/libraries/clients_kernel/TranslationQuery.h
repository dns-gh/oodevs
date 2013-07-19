// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __kernel_TranslationQuery_h_
#define __kernel_TranslationQuery_h_

namespace tools
{
    class Path;
}

namespace kernel
{

// =============================================================================
/** @class  TranslationQuery
    @brief  TranslationQuery
*/
// Created: ABR 2013-07-09
// =============================================================================
class TranslationQuery
{

public:
    //! @name Constructors/Destructor
    //@{
             TranslationQuery();
             TranslationQuery( const TranslationQuery& );
    explicit TranslationQuery( xml::xistream& xis );
    virtual ~TranslationQuery();
    //@}

    //! @name Operations
    //@{
    void Read( xml::xistream& xis );
    void Write( xml::xostream& xos ) const;

    QStringList Evaluate( const tools::Path& path ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetContext() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string query_;
    std::string context_;
    std::string prefix_;
    //@}
};

xml::xistream& operator>>( xml::xistream& xis, TranslationQuery& query );
xml::xostream& operator<<( xml::xostream& xos, const TranslationQuery& query );

} //! namespace kernel

#endif // __kernel_TranslationQuery_h_
