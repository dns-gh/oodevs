//*****************************************************************************
//
// $Created: JDY 03-08-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ComboBox_Enum.h $
// $Author: Ape $
// $Modtime: 18/02/05 11:54 $
// $Revision: 2 $
// $Workfile: ADN_ComboBox_Enum.h $
//
//*****************************************************************************

#ifndef __ADN_ComboBox_Enum_h_
#define __ADN_ComboBox_Enum_h_


#include "ADN_Tools.h"
#include "ADN_ComboBox.h"
#include "ENT/ENT_Tr_ABC.h"

//*****************************************************************************
// Created: JDY 03-08-29
//*****************************************************************************
template <class EnumType>
class ADN_ComboBox_Enum
:   public ADN_ComboBox
{
public:

    typedef const std::string& (*T_Converter)( EnumType, ENT_Tr_ABC::E_Conversion );
    
    explicit ADN_ComboBox_Enum(T_Converter cv,QWidget * parent = 0, const char * name = 0);
    virtual ~ADN_ComboBox_Enum();

protected:
    void UpdateEnableState();

private:
    
    class ADN_CCB
    : public ADN_Connector_EnumComboBox
    {
    public:
        ADN_CCB(ADN_ComboBox_Enum<EnumType>& combo)
        :   ADN_Connector_EnumComboBox(&combo)
        {}

        virtual ~ADN_CCB()
        {}

        std::string GetItem(void * obj)
        {   // return indirect ammo type
            return static_cast<ADN_ComboBox_Enum<EnumType>*>(pGfx_)->converter_(*(EnumType*)obj,ENT_Tr_ABC::eToTr); 
        }
    };

    friend class    ADN_ComboBox_Enum::ADN_CCB;
    T_Converter     converter_;
};

#include "ADN_ComboBox_Enum.inl"

#endif // __ADN_ComboBox_Enum_h_