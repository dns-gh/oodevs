// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Missions_Data_h_
#define __ADN_Missions_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"

class ADN_XmlInput_Helper;

// =============================================================================
/** @class  ADN_Missions_Data
    @brief  ADN_Missions_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Missions_Data
: public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Missions_Data )

// =============================================================================
// Mission parameters
// =============================================================================
public:
    class MissionParameterValue : public ADN_Ref_ABC
                                , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( MissionParameterValue )
    public:
                 MissionParameterValue();
        virtual ~MissionParameterValue();

        std::string GetItemName();
        MissionParameterValue* CreateCopy();

        void ReadArchive ( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output, unsigned int id );

    public:
        ADN_Type_String name_;
    };

    typedef ADN_Type_Vector_ABC<MissionParameterValue>  T_MissionParameterValue_Vector;
    typedef T_MissionParameterValue_Vector::iterator   IT_MissionParameterValue_Vector;

public:
    class MissionParameter : public ADN_Ref_ABC
                           , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( MissionParameter )
    public:
                 MissionParameter();
        virtual ~MissionParameter();
        
        std::string GetItemName();
        MissionParameter* CreateCopy();

        void ReadArchive ( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_String                                                   strName_;
        ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType > type_;
        ADN_Type_Bool                                                     isOptional_;
        T_MissionParameterValue_Vector                                    values_;
    };

    typedef ADN_Type_Vector_ABC<MissionParameter>  T_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::iterator   IT_MissionParameter_Vector;

// =============================================================================
// Missions
// =============================================================================
public:
    class Mission : public ADN_Ref_ABC
                  , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( Mission )
    public:
        explicit Mission( bool isAutomat = false );
        virtual ~Mission();

        std::string GetItemName();
        Mission* CreateCopy();

        void ReadArchive ( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output, const std::string& type );

    public:
        ADN_Type_String           strName_;
        ADN_Type_Bool             isAutomat_;
        T_MissionParameter_Vector parameters_;
    };

    typedef ADN_Type_Vector_ABC<Mission>  T_Mission_Vector;
    typedef T_Mission_Vector::iterator   IT_Mission_Vector;

// =============================================================================
// Frag orders
// =============================================================================
public:
    class FragOrder : public ADN_Ref_ABC
                    , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( FragOrder )
    public:
                 FragOrder();
        virtual ~FragOrder();

        std::string GetItemName();
        FragOrder* CreateCopy();

        void ReadArchive ( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_String           strName_;
        T_MissionParameter_Vector parameters_;
        ADN_Type_Bool             isAvailableForAllMissions_;
        ADN_Type_Bool             isAvailableWithoutMission_;
    };

    typedef ADN_Type_Vector_ABC<FragOrder>  T_FragOrder_Vector;
    typedef T_FragOrder_Vector::iterator   IT_FragOrder_Vector;

// =============================================================================
// Main data
// =============================================================================
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Missions_Data();
    virtual ~ADN_Missions_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();
    
    T_FragOrder_Vector& GetFragOrders();
    T_Mission_Vector&   GetUnitMissions();
    T_Mission_Vector&   GetAutomatMissions();
    T_Mission_Vector&   GetPopulationMissions();
    FragOrder*          FindFragOrder( const std::string& strName );
    Mission*            FindMission( T_Mission_Vector& missions, const std::string& strName );
    //@}

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void ReadMiscMission( ADN_XmlInput_Helper& input, const std::string& name, ADN_TypePtr_InVector_ABC< Mission >& ptrMission );
    void WriteArchive( MT_OutputArchive_ABC& output );

public:
    T_Mission_Vector   unitMissions_;
    T_Mission_Vector   automatMissions_;
    T_Mission_Vector   populationMissions_;
    T_FragOrder_Vector fragOrders_;

    ADN_TypePtr_InVector_ABC< Mission > ptrSurrenderMission_;
    ADN_TypePtr_InVector_ABC< Mission > ptrGoToRefugeeCampMission_;
};

#endif // __ADN_Missions_Data_h_
