// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_LocalizedType_h_
#define __ADN_LocalizedType_h_

#include "ADN_Type_ABC.h"
#include "clients_kernel/Language.h"
#include "clients_kernel/Translations.h"

// $$$$ ABR 2013-07-17: TODO: If someday we need to translate another type than std::string, template the following on the new type, and just implement a few ToString converters to make it build.

// =============================================================================
/** @class  ADN_LocalizedType
    @brief  ADN_LocalizedType
*/
// Created: ABR 2013-07-12
// =============================================================================
template< typename T >
class ADN_LocalizedType : public ADN_Type_ABC< T >
{

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

        virtual bool IsValid( const ADN_Type_ABC< T >& data ) const
        {
            return static_cast< const ADN_LocalizedType< T >& >( data ).GetType() != kernel::eTranslationType_Unfinished;
        }
    };
    //@}

    //! @name Constructors/Destructor
    //@{
             ADN_LocalizedType();
             ADN_LocalizedType( const T& val );
    virtual ~ADN_LocalizedType();
    //@}

    //! @name Operations
    //@{
    void Initialize( ADN_Connector_ABC& dest ) const;
    virtual void OnLanguageChanged( const std::string& language );
    virtual void OnTypeChanged( int );
    //@}

    //! @name Operators
    //@{
    ADN_LocalizedType& operator=( const T& val );
    virtual bool operator ==(const T& val) const;
    virtual bool operator !=(const T& val) const;
    //@}

    //! @name Accessors
    //@{
    virtual const T& GetData() const;
    virtual void SetData( const T& data );

    kernel::E_TranslationType GetType() const;
    void SetType( kernel::E_TranslationType type );

    const T& GetKey() const;
    void SetKey( const T& key );

    const kernel::Translation* GetTranslation() const;
    void SetTranslation( const std::string& key, kernel::Translation* translation );
    //@}

private:
    //! @name Helpers
    //@{
    bool HasTranslation() const;
    void AddTranslationChecker()
    {
        checkers_.push_back( new TranslationChecker( eWarning, tr( "Unfinished translation") ) );
    }
    //@}

private:
    //! @name Member data
    //@{
    std::string language_;
    kernel::Translation* translation_;
    //@}
};

#include "ADN_LocalizedType.inl"

#endif // __ADN_LocalizedType_h_
