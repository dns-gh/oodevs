// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Type_Enum_h_
#define __ADN_Type_Enum_h_

#include "ADN_Connector_Vector_ABC.h"
#include "ENT/ENT_Tr.h"
#include "ADN_Tr.h"
#include "ADN_ComboBox.h"

//*****************************************************************************
// Created: JDY 03-07-22
//*****************************************************************************
template< class T, int nb >
class ADN_Type_Enum : public ADN_Connector_ABC
{
public:
    typedef const std::string& (*T_Converter)( T, ENT_Tr::E_Conversion );

    explicit ADN_Type_Enum();
    explicit ADN_Type_Enum( const T& value );
    virtual ~ADN_Type_Enum();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const T GetData() const;
    virtual void Initialize( ADN_Connector_ABC& dest ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators*/
    //-------------------------------------------------------------------------
    //@{
    ADN_Type_Enum& operator =(const T& val);
    ADN_Type_Enum& operator =(const ADN_Type_Enum& e);
    bool          operator ==(const ADN_Type_Enum& o) const;
    bool          operator ==(const T& val) const;
    bool          operator !=(const ADN_Type_Enum& o) const;
    bool          operator !=(const T& val) const;
    //@}

    void SetAlphabeticalSort( bool sort );
    static void SetConverter( typename ADN_Type_Enum< T,nb >::T_Converter enumConverter );
    const std::string& Convert( ENT_Tr::E_Conversion nConversion = ENT_Tr::eToSim ) const;

protected:
    virtual void SetDataPrivate( void *data );
            void SetDataPrivate( const T& value );

private:
    explicit ADN_Type_Enum(const ADN_Type_Enum& e);

private:
    bool sortAlphabetically_;
    T data_;
    static T_Converter converter_;
};

#include "ADN_Type_Enum.inl"

#endif // __ADN_Type_Enum_h_
