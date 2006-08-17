//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Types.h $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_Types.h $
//
//*****************************************************************************

#ifndef __ADN_Types_h_
#define __ADN_Types_h_

#include "ADN_Type_ABC.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_TypePtr_InVector_ABC.h"
#include "ADN_Type_Enum.h"

class QWidget;

typedef ADN_Type_ABC<bool>          ADN_Type_Bool;
typedef ADN_Type_ABC<int>           ADN_Type_Int;
typedef ADN_Type_ABC<double>        ADN_Type_Double;
typedef ADN_Type_ABC<std::string>   ADN_Type_String;
typedef ADN_Type_ABC<std::string>   ADN_Type_Time;

template<>
class ADN_ChangeValueCommand_Chooser< bool >
{
public:
    static
    QtCommand* CreateCommand( ADN_Type_ABC< bool >& data, bool oVal , bool nVal )
    {
        return ADN_CreateCommand( data, oVal, nVal );
    };
};

template<>
class ADN_ChangeValueCommand_Chooser< double >
{
public:
    static
    QtCommand* CreateCommand( ADN_Type_ABC< double >& data, double oVal, double nVal )
    {
        return ADN_CreateCommand( data, oVal, nVal );
    };
};

template<>
class ADN_ChangeValueCommand_Chooser< int >
{
public:
    static
    QtCommand* CreateCommand( ADN_Type_ABC< int >& data, int oVal , int nVal )
    {
        return ADN_CreateCommand( data, oVal, nVal );
    };
};

template<>
class ADN_ChangeValueCommand_Chooser< std::string >
{
public:
    static
    QtCommand* CreateCommand( ADN_Type_ABC< std::string >& data, const std::string& oVal, const std::string& nVal )
    {
        return ADN_CreateCommand( data, oVal, nVal );
    };
};

typedef std::vector<QWidget*>                   T_PtrWidgetVector;
typedef T_PtrWidgetVector::iterator             IT_PtrWidgetVector;
typedef T_PtrWidgetVector::const_iterator       CIT_PtrWidgetVector;

typedef std::list<std::string>                  T_StringList;
typedef T_StringList::iterator                  IT_StringList;
typedef T_StringList::const_iterator            CIT_StringList;

#endif // __ADN_Types_h_