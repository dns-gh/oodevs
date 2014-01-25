// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Type_ABC_h_
#define __ADN_Type_ABC_h_

#include "ADN_Connector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-02
//*****************************************************************************
template <class T >
class ADN_Type_ABC : public ADN_Connector_ABC
{

public:
    //! @name Types
    //@{
    typedef typename T BaseType;

    class TypeChecker : public ADN_Checker
    {
    public:
        TypeChecker( ADN_ErrorStatus status, const QString& errorMsg )
            : ADN_Checker( status, errorMsg )
        {}
        virtual ~TypeChecker() {}

        virtual bool IsValid( const ADN_Type_ABC< BaseType >& ) const
        {
            return true;
        }
    };

    class MinMaxChecker : public TypeChecker
    {
    public:
        MinMaxChecker( ADN_ErrorStatus status, const QString& errorMsg, BaseType min, BaseType max )
            : TypeChecker( status, errorMsg )
            , min_( min )
            , max_( max )
        {}
        virtual ~MinMaxChecker() {}

        virtual bool IsValid( const ADN_Type_ABC< BaseType >& data ) const
        {
            return data.GetData() >= min_ && data.GetData() <= max_;
        }

    private:
        const BaseType min_;
        const BaseType max_;
    };

    class RegExpChecker : public TypeChecker
    {
    public:
        RegExpChecker( ADN_ErrorStatus type, const QString& errorMsg, const QRegExp& regExp )
            : TypeChecker( type, errorMsg )
            , regExp_( regExp )
        {}
        virtual ~RegExpChecker() {}

        virtual bool IsValid( const ADN_Type_ABC< BaseType >& data ) const
        {
            return regExp_.exactMatch( data.GetData().c_str() );
        }

    private:
        const QRegExp regExp_;
    };

    typedef std::vector< TypeChecker* > T_Checkers;
    //@}

    //! @name Constructors/Destructor
    //@{
             ADN_Type_ABC();
             ADN_Type_ABC( const T& val );
    virtual ~ADN_Type_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const T& GetData() const;
    virtual void Initialize( ADN_Connector_ABC& dest ) const;
    //@}

    //! @name Validity
    //@{
    void AddMinMaxChecker( ADN_ErrorStatus errorType, const QString& errorMsg, const T& min, const T& max );
    void AddRegExpChecker( ADN_ErrorStatus errorType, const QString& errorMsg, const QRegExp& regExp );
    virtual void CheckValidity();
    //@}

    //! @name Operators
    //@{
    ADN_Type_ABC& operator =(const T& val);
    virtual bool operator ==(const T& val) const;
    virtual bool operator !=(const T& val) const;
    //@}

public:
    //! @name Compare operator
    //@{
    class Cmp : public std::unary_function< ADN_Type_ABC* , bool >
    {
    public:
        Cmp(const T& val) : val_(val) {}
        ~Cmp() {}

        bool operator()( ADN_Type_ABC* adn_type ) const { return adn_type->GetData()==val_; }

    private:
        T val_;
    };
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void SetData( const T& data );
    void SetDataPrivate( void *data );
    //@}

protected:
    //! @name Member data
    //@{
    T val_;
    T_Checkers checkers_;
    //@}
};

// -----------------------------------------------------------------------------
// Stream operators
// -----------------------------------------------------------------------------
template< typename T >
std::ostream& operator<<( std::ostream& os, const ADN_Type_ABC< T >& type )
{
    return os << type.GetData();
}

template< typename T >
xml::xostream& operator<<( xml::xostream& xos, const ADN_Type_ABC< T >& type )
{
    return xos << type.GetData();
}
template< typename T >
xml::xistream& operator>>( xml::xistream& xis, ADN_Type_ABC< T >& type )
{
    T value;
    xis >> value;
    type = value;
    return xis;
}

#include "ADN_Type_ABC.inl"

#endif // __ADN_Type_ABC_h_
