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

#include "LocalizedString.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace tools
{
    class Path;
}

namespace kernel
{
    class Context;
    class LocalizedString;
    
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
    typedef boost::container::flat_map< std::string, boost::shared_ptr< Context > > T_Contexts;
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit XmlTranslations();
    virtual ~XmlTranslations();
    //@}

    //! @name TranslationQueries operations
    //@{
    void Purge();
    //@}

    //! @name Translations operations
    //@{
    bool ApplyOnTranslations( std::function< bool( LocalizedString& ) > function );
    void LoadTranslationFile( const tools::Path& xmlFile, const tools::Path& localesDirectory, const std::string& languageCode );
    void LoadTranslationXmlStream( xml::xistream& xis, const std::string& languageCode );
    void MergeDuplicateTranslations();
    void SaveTranslationFiles( const tools::Path& xmlFile, const tools::Path& localesDirectory, const tools::LanguagesVector& languages ) const;
    //@}

    //! @name Accessors
    //@{
    bool HasTranslations() const;
    bool HasDuplicateErrors() const;
    const boost::shared_ptr< Context >& GetContext( const std::string& context );
    const boost::shared_ptr< LocalizedString >& GetTranslation( const std::string& context, const std::string& key );
    //@}

private:
    //! @name Helpers
    //@{
    void SetTranslation( const std::string& context, const std::string& key, const std::string& languageCode, const std::string& translation, E_TranslationType type = kernel::eTranslationType_Unfinished );
    void CleanTranslations();

    void ReadContext( xml::xistream& xis, const std::string& languageCode );
    void ReadMessage( xml::xistream& xis, const std::string& languageCode, const std::string& translations );
    //@}

private:
    //! @name Member data
    //@{
    T_Contexts contexts_;
    bool hasTranslations_;
    //@}
};

} //! namespace kernel

#endif // __XmlTranslations_h_
