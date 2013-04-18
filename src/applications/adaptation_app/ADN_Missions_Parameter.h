// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef ADAPTATIONS_ADN_MISSIONS_PARAMETER_H
#define ADAPTATIONS_ADN_MISSIONS_PARAMETER_H

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
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

class ADN_Missions_Parameter : public ADN_RefWithName
{
public:
    typedef ADN_Type_Vector_ABC<ADN_Missions_ParameterValue>  T_MissionParameterValue_Vector;
    typedef ADN_Type_Vector_ABC<ADN_Missions_Type>            T_Choice_Vector;

             ADN_Missions_Parameter();
    virtual ~ADN_Missions_Parameter();

    std::string GetItemName();
    ADN_Missions_Parameter* CreateCopy();

    void ReadArchive( xml::xistream& input );
    void ReadValue( xml::xistream& input );
    void ReadChoiceVector( xml::xistream& input, T_Choice_Vector& data );
    void ReadChoiceGenObjects( xml::xistream& input, helpers::T_MissionGenObjectTypes_Infos_Vector& data );
    void WriteArchive( xml::xostream& output );

private:
    void FillChoices();

public:
    ADN_Type_String                                                   strMission_;
    ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType > type_;
    ADN_Type_Bool                                                     isOptional_;
    ADN_Type_Int                                                      minOccurs_;
    ADN_Type_Int                                                      maxOccurs_;
    ADN_Type_Int                                                      minValue_;
    ADN_Type_Int                                                      maxValue_;
    T_MissionParameterValue_Vector                                    values_;
    T_Choice_Vector                                                   choices_;
    helpers::T_MissionGenObjectTypes_Infos_Vector                     genObjects_;
    helpers::T_MissionGenObjectTypes_Infos_Vector                     knowledgeObjects_;
    ADN_Type_String                                                   diaName_;
    bool                                                              isContext_;
    ADN_Type_String                                                   description_;
};

#endif // ADAPTATIONS_ADN_MISSIONS_PARAMETER_H