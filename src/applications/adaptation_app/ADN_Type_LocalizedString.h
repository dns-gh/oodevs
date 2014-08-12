// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Type_LocalizedString_h_
#define __ADN_Type_LocalizedString_h_

#include "ADN_Type_ABC.h"
#include "clients_kernel/LocalizedString.h"
#include "tools/LanguageObserver_ABC.h"

namespace kernel
{
    class Context;
}

// =============================================================================
/** @class  ADN_Type_LocalizedString
    @brief  ADN_Type_LocalizedString
*/
// Created: ABR 2013-07-12
// =============================================================================
class ADN_Type_LocalizedString : public ADN_Type_ABC< std::string >
                               , public tools::Observer_ABC
                               , public tools::LanguageObserver_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    class TranslationChecker : public TypeChecker
    {
    public:
        TranslationChecker( ADN_ErrorStatus status, const QString& errorMsg )
            : TypeChecker( status, errorMsg )
        {}
        virtual ~TranslationChecker() {}

        virtual bool IsValid( const ADN_Type_ABC< std::string >& data ) const
        {
            return static_cast< const ADN_Type_LocalizedString& >( data ).GetType() != kernel::eTranslationType_Unfinished;
        }
    };
    class TranslationUniquenessChecker : public TypeChecker
    {
    public:
        TranslationUniquenessChecker( ADN_ErrorStatus status, const QString& errorMsg )
            : TypeChecker( status, errorMsg )
        {}
        virtual ~TranslationUniquenessChecker() {}

        virtual bool IsValid( const ADN_Type_ABC< std::string >& data ) const
        {
            return static_cast< const ADN_Type_LocalizedString& >( data ).CheckUniqueTranslation();
        }
    };
    //@}

    //! @name Constructors/Destructor
    //@{
             ADN_Type_LocalizedString();
             ADN_Type_LocalizedString( const std::string& val );
    virtual ~ADN_Type_LocalizedString();
    //@}

    //! @name Operations
    //@{
    void Initialize( ADN_Connector_ABC& dest ) const;
    bool CheckUniqueTranslation() const;
    void Purge();
    //@}

    //! @name Operators
    //@{
    ADN_Type_LocalizedString& operator=( const std::string& val );
    virtual bool operator==( const std::string& val ) const;
    virtual bool operator!=( const std::string& val ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const std::string& GetData() const;
    virtual void SetData( const std::string& data );

    const std::string& GetValue( const std::string& language ) const;
    void SetValue( const std::string& language, const std::string& data );

    kernel::E_TranslationType GetType() const;
    void SetType( kernel::E_TranslationType type );

    kernel::E_TranslationType GetType( const std::string& language ) const;
    void SetType( const std::string& language, kernel::E_TranslationType type );

    const std::string& GetKey() const;
    void SetKey( const std::string& key );

    void SetContext( boost::shared_ptr< kernel::Context > context );
    const boost::shared_ptr< kernel::LocalizedString >& GetTranslation() const;
    //@}

public slots:
    //! @name Slots
    //@{
    void OnTypeChanged( int );
    //@}

private:
    //! @name LanguageObserver_ABC implementation
    //@{
    virtual void OnLanguageChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize();
    void InitTranslation( const std::string& data );
    void AddTranslationCheckers();
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< kernel::LocalizedString > translation_;
    boost::shared_ptr< kernel::Context > context_;
    bool swappingLanguage_;
    //@}
};

std::ostream& operator<<( std::ostream& os, const ADN_Type_LocalizedString& type );
xml::xostream& operator<<( xml::xostream& xos, const ADN_Type_LocalizedString& type );
xml::xistream& operator>>( xml::xistream& xis, ADN_Type_LocalizedString& type );

#endif // __ADN_Type_LocalizedString_h_
