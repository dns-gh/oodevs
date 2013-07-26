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
#include "Translations.h"

namespace tools
{
    class Path;
}

namespace kernel
{

class Language;
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
    typedef std::vector< Language >                 T_Languages;
    typedef std::vector< TranslationQuery >         T_TranslationQueries;
    typedef std::map< std::string, Translations >   T_Contexts;
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
    void LoadTranslationFile( const tools::Path& xmlFile, const tools::Path& localesDirectory, const std::string& language = "" );
    void LoadTranslationFiles( const tools::Path& xmlFile, const tools::Path& localesDirectory, const T_Languages& languages );
    void SaveTranslationFiles( const tools::Path& xmlFile, const tools::Path& localesDirectory, const T_Languages& languages ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string Translate( const std::string& key, const std::string& context = "", const std::string& language = "" ) const;

    void SetTranslation( const std::string& context, const std::string& key, const std::string& language, const std::string& translation, E_TranslationType type = kernel::eTranslationType_Unfinished );
    Translation* GetTranslation( const std::string& context, const std::string& key );
    //@}

private:
    //! @name Helpers
    //@{
    void CopyAndAddTranslationQueries( const std::string& name, xml::xistream& xis, xml::xostream& xos ) const;

    void ReadTranslationQueries( const std::string& name, xml::xistream& xis, int depthMax );
    void ReadTranslationQuery( xml::xistream& xis );
    void ReadContext( xml::xistream& xis, const std::string& language );
    void ReadMessage( xml::xistream& xis, const std::string& language, const std::string& translations );
    //@}

private:
    //! @name Member data
    //@{
    T_TranslationQueries queries_;
    T_Contexts contexts_; // translation = contexts_[ context ][ sourceText ][ language ]
    bool loadAllLanguages_;
    std::string currentLanguage_;
    //@}
};

} //! namespace kernel

#endif // __XmlTranslations_h_
