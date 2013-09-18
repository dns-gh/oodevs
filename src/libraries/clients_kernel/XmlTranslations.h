// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __XmlTranslations_h_
#define __XmlTranslations_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "LocalizedString.h"

namespace tools
{
    class Path;
}

namespace kernel
{
    class Context;
    class Language;
    class LocalizedString;
    class TranslationQuery;

// =============================================================================
/** @class  XmlTranslations
    @brief  XmlTranslations
*/
// Created: ABR 2013-07-09
// =============================================================================
class XmlTranslations : private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::vector< Language >                               T_Languages;
    typedef std::vector< TranslationQuery >                       T_TranslationQueries;
    typedef std::map< std::string, boost::shared_ptr< Context > > T_Contexts;
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit XmlTranslations();
    virtual ~XmlTranslations();
    //@}

    //! @name TranslationQueries operations
    //@{
    void Purge();
    bool LoadTranslationQueries( const tools::Path& xmlFile );
    void EvaluateTranslationQueries( const tools::Path& xmlFile, const T_Languages& languages );
    void SaveTranslationQueries( const tools::Path& xmlFile ) const;
    void SaveTranslationQueries( xml::xostream& xos ) const;
    //@}

    //! @name Translations operations
    //@{
    void LoadTranslationFile( const tools::Path& xmlFile, const tools::Path& localesDirectory, const std::string& language );
    void LoadTranslationFiles( const tools::Path& xmlFile, const tools::Path& localesDirectory, const T_Languages& languages );
    void MergeDuplicateTranslations();
    void SaveTranslationFiles( const tools::Path& xmlFile, const tools::Path& localesDirectory, const T_Languages& languages ) const;
    //@}

    //! @name Accessors
    //@{
    bool HasDuplicateErrors() const;

    const std::string Translate( const std::string& key, const std::string& context = "", const std::string& language = "" ) const;
    const boost::shared_ptr< Context >& GetContext( const std::string& context );
    const boost::shared_ptr< LocalizedString >& GetTranslation( const std::string& context, const std::string& key ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void SetTranslation( const std::string& context, const std::string& key, const std::string& language, const std::string& translation, E_TranslationType type = kernel::eTranslationType_Unfinished );
    void CopyAndAddTranslationQueries( const std::string& name, xml::xistream& xis, xml::xostream& xos ) const;
    void CleanTranslations();

    void ReadTranslationQueries( const std::string& name, xml::xistream& xis, int depthMax );
    void ReadTranslationQuery( xml::xistream& xis );
    void ReadContext( xml::xistream& xis, const std::string& language );
    void ReadMessage( xml::xistream& xis, const std::string& language, const std::string& translations );
    //@}

private:
    //! @name Member data
    //@{
    T_TranslationQueries queries_;
    T_Contexts contexts_;
    bool loadAllLanguages_;
    std::string currentLanguage_;
    //@}
};

} //! namespace kernel

#endif // __XmlTranslations_h_
