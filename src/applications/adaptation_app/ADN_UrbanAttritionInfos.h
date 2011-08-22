// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_UrbanAttritionInfos_h_
#define __ADN_UrbanAttritionInfos_h_

#include "ADN_Urban_Data.h"

namespace helpers
{

// =============================================================================
/** @class  ADN_UrbanAttritionInfos
    @brief  ADN_UrbanAttritionInfos
*/
// Created: JSR 2010-12-02
// =============================================================================
class ADN_UrbanAttritionInfos : public ADN_Ref_ABC
                              , public ADN_DataTreeNode_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_UrbanAttritionInfos( ADN_Urban_Data::UrbanMaterialInfos* ptr );
    virtual ~ADN_UrbanAttritionInfos() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetNodeName();
    virtual std::string GetItemName();
    virtual void CopyFrom( ADN_UrbanAttritionInfos& attritions );
    virtual void ReadArchive( xml::xistream& );
    virtual void WriteArchive( xml::xostream&, const std::string& tag = "urban-modifier" );
    //@}

public:
    //! @name Member data
    //@{
    ADN_TypePtr_InVector_ABC< ADN_Urban_Data::UrbanMaterialInfos > ptrMaterial_;
    ADN_Type_Double rCoeff_;
    ADN_Type_String& strName_;
    //@}

public:
    typedef ADN_Urban_Data::UrbanMaterialInfos T_Item;

    class CmpRef : public std::unary_function< ADN_UrbanAttritionInfos*, bool >
    {
    public:
        CmpRef( ADN_Urban_Data::UrbanMaterialInfos* val ) : val_( val ) {}

        bool operator()( ADN_UrbanAttritionInfos* tgtnfos ) const
        {
            return tgtnfos->ptrMaterial_.GetData() == val_;
        }
    private:
        ADN_Urban_Data::UrbanMaterialInfos* val_;
    };

    class Cmp : public std::unary_function< ADN_UrbanAttritionInfos*, bool >
    {
    public:
        Cmp(const std::string& val) : val_( val ) {}
        ~Cmp() {}

        bool operator()( ADN_UrbanAttritionInfos* tgtnfos ) const
        {
            return tgtnfos->ptrMaterial_.GetData() && tgtnfos->ptrMaterial_.GetData()->strName_ == val_;
        }
    private:
        std::string val_;
    };
};

TYPEDEF_FULL_DECLARATION( ADN_Type_VectorFixed_ABC< ADN_UrbanAttritionInfos >, UrbanAttritionInfos_Vector );

} // end namespace helpers

#endif // __ADN_UrbanAttritionInfos_h_
