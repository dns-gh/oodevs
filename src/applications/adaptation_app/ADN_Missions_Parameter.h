// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef ADAPTATIONS_ADN_MISSIONS_PARAMETER_H
#define ADAPTATIONS_ADN_MISSIONS_PARAMETER_H

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Drawings_Data.h"
#include "ADN_MissionGenObjectTypes_Infos.h"
#include "ADN_Missions_ParameterValue.h"
#include "ADN_Missions_Type.h"

enum E_EntityType;

namespace xml
{
    class xistream;
    class xostream;
}

class ADN_Missions_Parameter : public ADN_Ref_ABC
                             , public ADN_DataTreeNode_ABC
{
public:
    typedef ADN_Type_Vector_ABC<ADN_Missions_ParameterValue>  T_MissionParameterValue_Vector;
    typedef T_MissionParameterValue_Vector::iterator          IT_MissionParameterValue_Vector;
    typedef ADN_Type_Vector_ABC<ADN_Missions_Type>            T_Choice_Vector;
    typedef T_Choice_Vector::iterator                         IT_Choice_Vector;

             ADN_Missions_Parameter();
    virtual ~ADN_Missions_Parameter();

    std::string GetItemName();
    ADN_Missions_Parameter* CreateCopy();

    void ReadArchive ( xml::xistream& input );
    void ReadValue   ( xml::xistream& input );
    template< typename T >
    void ReadChoice  ( xml::xistream& input, T& data );
    void WriteArchive( xml::xostream& output );

private:
    void FillChoices();
    void FillGenObjects();
    bool HasGenObjects() const;

public:
    ADN_Type_String                                                   strMission_;
    ADN_Type_String                                                   strName_;
    ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType > type_;
    ADN_Type_Bool                                                     isOptional_;
    ADN_Type_Int                                                      minOccurs_;
    ADN_Type_Int                                                      maxOccurs_;
    ADN_Type_Int                                                      minValue_;
    ADN_Type_Int                                                      maxValue_;
    T_MissionParameterValue_Vector                                    values_;
    T_Choice_Vector                                                   choices_;
    helpers::T_MissionGenObjectTypes_Infos_Vector                     genObjects_;
    ADN_Type_String                                                   diaName_;
};

#endif // ADAPTATIONS_ADN_MISSIONS_PARAMETER_H
