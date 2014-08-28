// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __LocalizedString_h_
#define __LocalizedString_h_

#include "tools/Language.h"
#include <boost/container/flat_map.hpp>
#include <boost/noncopyable.hpp>

namespace kernel
{

enum E_TranslationType
{
    eTranslationType_None           = 0,
    eTranslationType_Unfinished     = 1,
    eNbrTranslationType             = 2
};
xml::xostream& operator<<( xml::xostream& xos, E_TranslationType type );
xml::xistream& operator>>( xml::xistream& xis, E_TranslationType& type );

// =============================================================================
/** @class  LocalizedString
    @brief  LocalizedString
*/
// Created: ABR 2013-08-22
// =============================================================================
class LocalizedString
{
    //! @name Types
    //@{
    struct TranslationText
    {
        TranslationText() : type_( eTranslationType_Unfinished ), value_() {}
        TranslationText( const std::string& value, E_TranslationType type = eTranslationType_Unfinished )
            : type_( type )
            , value_( value )
        {}
        virtual ~TranslationText() {}
        bool operator==( const TranslationText& other ) const
        {
            return type_ == other.type_ && value_ == other.value_;
        }

        E_TranslationType type_;
        std::string value_;
    };
    typedef boost::container::flat_map< std::string, TranslationText > T_Texts;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             LocalizedString();
    explicit LocalizedString( const std::string& key );
    virtual ~LocalizedString();
    //@}

    //! @name Operations
    //@{
    void CopyValues( const LocalizedString& other );
    bool IsUnfinished( const std::string& language ) const;
    bool Initialize( const tools::LanguagesVector& languages );
    //@}

    //! @name Operators
    //@{
    bool operator==( const LocalizedString& other ) const;
    bool operator!=( const LocalizedString& other ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string& Key() const;
    const std::string& Value( const std::string& language ) const;
    E_TranslationType Type( const std::string& language ) const;
    const std::string& Translate() const;
    const std::string& Translate( const std::string& language ) const;

    void SetValue( const std::string& language, const std::string& value );
    void SetType( const std::string& language, E_TranslationType type );
    //@}

private:
    //! @name Helpers
    //@{
    friend class Context;
    void SetKey( const std::string& key );
    bool SwapKey( const std::string& oldKey, const std::string& newKey );
    void CheckLanguageValidity( const std::string& language ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string key_;
    T_Texts values_;
    //@}
};

} //! Namespace kernel

#endif // __LocalizedString_h_
