// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_AttritionInfos_h_
#define __ADN_AttritionInfos_h_

#include "ADN_Armors_Data.h"
#include "ADN_CrossedRef.h"
#include "ADN_Type_VectorFixed_ABC.h"

namespace helpers
{

class AttritionInfos : public ADN_CrossedRef< ADN_Armors_Data::ArmorInfos >
{

public:
    explicit AttritionInfos( ADN_Armors_Data::ArmorInfos* ptr );
    virtual ~AttritionInfos() {}

    void CopyFrom( AttritionInfos& attritions );
    void ReadArchive( xml::xistream& xis );
    void WriteArchive( xml::xostream& xos, const std::string& tag = "attrition" );

    AttritionInfos* CreateCopy();

public:
    ADN_Type_Double rDestroy_;
    ADN_Type_Double rRepairWithEvac_;
    ADN_Type_Double rRepairNoEvac_;

public:
    typedef ADN_Armors_Data::ArmorInfos  T_Item;

    class CmpRef : public std::unary_function< AttritionInfos*, bool >
    {
    public:
        CmpRef( ADN_Armors_Data::ArmorInfos* val ) : val_( val ) {}
        bool operator()( AttritionInfos* tgtnfos ) const
        {
            return tgtnfos->GetCrossedElement() == val_;
        }

    private:
        ADN_Armors_Data::ArmorInfos* val_;
    };

    class Cmp : public std::unary_function< AttritionInfos*, bool >
    {
    public:
        Cmp( const std::string& name ) : name_( name ) {}
        bool operator()( AttritionInfos* tgtnfos ) const
        {
            return tgtnfos->GetCrossedElement() ? tgtnfos->GetCrossedElement()->strName_ == name_ : false;
        }

    private:
        std::string name_;
    };
};
typedef ADN_Type_VectorFixed_ABC< AttritionInfos > T_AttritionInfos_Vector;

}

#endif // __ADN_AttritionInfos_h_
