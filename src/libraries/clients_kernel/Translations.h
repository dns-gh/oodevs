// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __Translations_h_
#define __Translations_h_

namespace kernel
{
    enum E_TranslationType
    {
        eTranslationType_None           = 0,
        eTranslationType_Unfinished     = 1,
        eNbrTranslationType             = 2
    };
    xml::xostream& operator<<( xml::xostream& xos, const E_TranslationType& language );
    xml::xistream& operator>>( xml::xistream& xis, E_TranslationType& language );

    struct TranslationText
    {
        TranslationText() : type_( eTranslationType_Unfinished ), value_() {}
        TranslationText( const std::string& value, E_TranslationType type = eTranslationType_Unfinished )
            : type_( type )
            , value_( value )
        {}
        virtual ~TranslationText() {}

        E_TranslationType type_;
        std::string value_;
    };
    typedef std::map< std::string, TranslationText > T_LocalizedTexts;

    struct Translation
    {
        Translation() :  key_() {}
        Translation( const std::string& key )
            : key_( key )
        {}
        virtual ~Translation() {}

        std::string key_;
        T_LocalizedTexts values_;
    };

// =============================================================================
/** @class  Translations
    @brief  Translations
*/
// Created: ABR 2013-07-15
// =============================================================================
class Translations : public std::vector< Translation >
{

public:
    //! @name Constructors/Destructor
    //@{
             Translations();
    virtual ~Translations();
    //@}

    //! @name Operations
    //@{
    std::vector< Translation >::iterator find( const std::string& key );
    const std::vector< Translation >::const_iterator find( const std::string& key ) const;

    T_LocalizedTexts& operator[] ( const std::string& key );
    const T_LocalizedTexts& operator[] ( const std::string& key ) const;

    const T_LocalizedTexts& at( const std::string& key ) const;
    //@}
};

} //! Namespace kernel

#endif // __Translations_h_
