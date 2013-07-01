// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MissionGenObjectTypes_Infos_h_
#define __ADN_MissionGenObjectTypes_Infos_h_

#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_Type_VectorFixed_ABC.h"

namespace helpers
{
    // $$$$ ABR 2013-01-08: Rename this class, not only about gen objects, that can be about object knowledge too

// =============================================================================
/** @class  ADN_MissionGenObjectTypes_Infos
    @brief  ADN_MissionGenObjectTypes_Infos
*/
// Created: LGY 2012-04-18
// =============================================================================
class ADN_MissionGenObjectTypes_Infos : public ADN_Ref_ABC
                                      , public ADN_DataTreeNode_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_MissionGenObjectTypes_Infos( ADN_Objects_Data_ObjectInfos* ptr );
             ADN_MissionGenObjectTypes_Infos();
    virtual ~ADN_MissionGenObjectTypes_Infos();
    //@}

    //! @name Operations
    //@{
    std::string GetItemName();
    const std::string& GetInputName() const;
    virtual void WriteArchive( xml::xostream& );
    ADN_MissionGenObjectTypes_Infos* CreateCopy();
    //@}

public:
    //! @name Member data
    //@{
    ADN_TypePtr_InVector_ABC< ADN_Objects_Data_ObjectInfos > ptrObject_;
    ADN_Type_Bool isAllowed_;
    ADN_Type_String& name_;
    ADN_Type_String& type_;
    //@}

public:

    typedef ADN_Objects_Data_ObjectInfos T_Item;

    class CmpRef : public std::unary_function< ADN_MissionGenObjectTypes_Infos*, bool >
    {
    public:
        CmpRef( ADN_Objects_Data_ObjectInfos* val ) : val_( val ) {}

        bool operator()( ADN_MissionGenObjectTypes_Infos* tgtnfos ) const
        {
            return tgtnfos->ptrObject_.GetData() == val_;
        }
    private:
        ADN_Objects_Data_ObjectInfos* val_;
    };
};

TYPEDEF_FULL_DECLARATION( ADN_Type_VectorFixed_ABC< ADN_MissionGenObjectTypes_Infos >, MissionGenObjectTypes_Infos_Vector );

} // end namespace helpers

#endif // __ADN_MissionGenObjectTypes_Infos_h_
