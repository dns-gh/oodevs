// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-11-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AttritionInfos.h $
// $Author: Nld $
// $Modtime: 20/07/05 14:11 $
// $Revision: 13 $
// $Workfile: ADN_AttritionInfos.h $
//
// *****************************************************************************

#ifndef __ADN_AttritionInfos_h_
#define __ADN_AttritionInfos_h_

#include "ADN_ArmorInfos.h"
#include "ADN_Type_VectorFixed_ABC.h"

namespace helpers
{

class AttritionInfos
    : public ADN_Ref_ABC
    , public ADN_DataTreeNode_ABC
{
    MT_COPYNOTALLOWED( AttritionInfos );

public:
    AttritionInfos( ArmorInfos* ptr );

    virtual std::string GetNodeName();
    std::string GetItemName();

    void CopyFrom( AttritionInfos& attritions );

    void ReadArchive( xml::xistream& );
    void WriteArchive( xml::xostream&, const std::string& tag = "attrition" );

public:
    //! @name Operators
    //@{
    xml::xostream& operator<<( xml::xostream& xos );
    //@}

public:
    ADN_TypePtr_InVector_ABC< ArmorInfos > ptrArmor_;
    ADN_Type_Double                        rDestroy_;
    ADN_Type_Double                        rRepairWithEvac_;
    ADN_Type_Double                        rRepairNoEvac_;

public:
    typedef ArmorInfos  T_Item;

    class CmpRef : public std::unary_function< AttritionInfos* , bool >
    {
    public:
        CmpRef( ArmorInfos* val ) : val_(val){}
        bool operator()( AttritionInfos* tgtnfos ) const 
        {   return tgtnfos->ptrArmor_.GetData() == val_;}

    private:
        ArmorInfos* val_;
    };

    class Cmp : public std::unary_function< AttritionInfos* , bool >
    {
    public:
        Cmp( const std::string& name ) : name_(name) {}
        bool operator()( AttritionInfos* tgtnfos ) const 
        {   return tgtnfos->ptrArmor_.GetData()->strName_ == name_;}

    private:
        std::string name_;
    };
};

typedef ADN_Type_VectorFixed_ABC< AttritionInfos > T_AttritionInfos_Vector;
typedef T_AttritionInfos_Vector::iterator         IT_AttritionInfos_Vector;

}

#endif // __ADN_AttritionInfos_h_
