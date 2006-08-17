//*****************************************************************************
//
// $Created: JDY 03-08-28 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Type_Tab_ABC.h $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_Type_Tab_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_Type_Tab_ABC_h_
#define __ADN_Type_Tab_ABC_h_

#include "ADN_Type_Vector_ABC.h"



template < class T >
class ADN_Type_Line_ABC
: public ADN_Type_Vector_ABC< T >
{
    MT_COPYNOTALLOWED(ADN_Type_Line_ABC);
public:
    ADN_Type_Line_ABC(int w=0);
    virtual ~ADN_Type_Line_ABC();
    
    std::string GetItemName();
};





template < class T >
class ADN_Type_Tab_ABC 
: public ADN_Type_Vector_ABC< ADN_Type_Line_ABC< T > >
{
    MT_COPYNOTALLOWED(ADN_Type_Tab_ABC);

public:

    ADN_Type_Tab_ABC(int w=0,int h=0);
    virtual ~ADN_Type_Tab_ABC();
    
    T& Get(int i,int j);
    
};

#include "ADN_Type_Tab_ABC.inl"


#endif // __ADN_Type_Tab_ABC_h_