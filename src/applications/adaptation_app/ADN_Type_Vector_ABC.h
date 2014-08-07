// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Type_Vector_ABC_h_
#define __ADN_Type_Vector_ABC_h_

#include "ADN_Connector_Vector_ABC.h"
#include "ADN_Checker.h"
#include "ADN_Type_ABC.h"

//*****************************************************************************
// Created: JDY 03-06-26
//*****************************************************************************
template < class T >
class ADN_Type_Vector_ABC : public ADN_Connector_Vector_ABC
                          , public std::vector< T* >
{
public:
    typedef std::vector< T* > T_PtrVector;

    typedef ADN_Type_ABC< std::string >& ( *T_Extractor )( T& );
    class UniquenessChecker : public ADN_Checker
    {
    public:
        UniquenessChecker( ADN_ErrorStatus type, const QString& errorMsg, T_Extractor extractor )
            : ADN_Checker( type, errorMsg )
            , extractor_( extractor )
        {
            // NOTHING
        }
        virtual ~UniquenessChecker() {}

        bool IsValid( T& lhs, T& rhs ) const
        {
            return extractor_( lhs ).GetData() != extractor_( rhs ).GetData();
        }

        void InvalidData( T& lhs, T& rhs ) const
        {
            extractor_( lhs ).SetErrorStatus( std::max< ADN_ErrorStatus >( status_, extractor_( lhs ).GetErrorStatus() ), msg_ );
            extractor_( rhs ).SetErrorStatus( std::max< ADN_ErrorStatus >( status_, extractor_( rhs ).GetErrorStatus() ), msg_ );
            lhs.SetErrorStatus( std::max< ADN_ErrorStatus >( status_, lhs.GetErrorStatus() ), msg_ );
            rhs.SetErrorStatus( std::max< ADN_ErrorStatus >( status_, rhs.GetErrorStatus() ), msg_ );
        }

    private:
        T_Extractor extractor_;
    };
    typedef std::vector< UniquenessChecker* > T_Checkers;

public:
    explicit ADN_Type_Vector_ABC( bool bAutoRef = true, bool owner = true );
    virtual ~ADN_Type_Vector_ABC();

    virtual void Initialize( ADN_Connector_Vector_ABC& dest ) const;

    virtual void CheckValidity();
    void AddUniquenessChecker( ADN_ErrorStatus errorType, const QString& errorMsg, T_Extractor extractor );
    virtual std::vector< T* > FindElements( std::function< bool( const T* ) > func ) const;

protected:
    virtual void SetDataPrivate( void* pData );
    virtual bool AddItemPrivate( void* pObj );
    virtual bool RemItemPrivate( void* pObj );
    virtual void ClearPrivate( bool bInConnection = false );
    virtual void InvalidatePrivate( void* pObj, bool bDel = false );

public:
    //! @name Assignment/Copy constructor
    //@{
    ADN_Type_Vector_ABC< T >( const ADN_Type_Vector_ABC< T >& o );
    ADN_Type_Vector_ABC< T >& operator=( const ADN_Type_Vector_ABC< T >& o );
    //@}

public:
    bool bAutoRef_;
    bool owner_;
    T_Checkers checkers_;
};

#include "ADN_Type_Vector_ABC.inl"

#endif // __ADN_Type_Vector_ABC_h_
