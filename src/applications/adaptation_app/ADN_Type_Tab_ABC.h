// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Type_Tab_ABC_h_
#define __ADN_Type_Tab_ABC_h_

#include "ADN_Type_Vector_ABC.h"

template < class T >
class ADN_Type_Line_ABC : public ADN_Type_Vector_ABC< T >
{
public:
    explicit ADN_Type_Line_ABC( int w = 0 );
    virtual ~ADN_Type_Line_ABC();

    std::string GetItemName();
};

template < class T >
class ADN_Type_Tab_ABC : public ADN_Type_Vector_ABC< ADN_Type_Line_ABC< T > >
{
public:
    explicit ADN_Type_Tab_ABC( int w = 0, int h = 0 );
    virtual ~ADN_Type_Tab_ABC();

    T& Get( int i, int j );
    const T& Get( int i, int j ) const;

};

#include "ADN_Type_Tab_ABC.inl"

#endif // __ADN_Type_Tab_ABC_h_