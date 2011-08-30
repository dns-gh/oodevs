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
#include "IdentifierFactory.h"
#include "ADN_Drawings_Data.h"

namespace xml { class xistream; }

// =============================================================================
/** @class  ADN_Missions_Data
    @brief  ADN_Missions_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Missions_Data : public ADN_Data_ABC
{

// =============================================================================
// Mission parameters
// =============================================================================
public:
    class MissionParameterValue : public ADN_Ref_ABC
                                , public ADN_DataTreeNode_ABC
    {
    public:
                 MissionParameterValue();
        virtual ~MissionParameterValue();

        std::string GetItemName();
        MissionParameterValue* CreateCopy();

        void ReadArchive ( xml::xistream& input );
        void WriteArchive( xml::xostream& output, unsigned int id );

    public:
        ADN_Type_String name_;
    };
    class MissionType : public ADN_Ref_ABC
                      , public ADN_DataTreeNode_ABC
    {
    public:
                 MissionType();
        explicit MissionType( const std::string& name );
        virtual ~MissionType();

        std::string GetItemName();
        MissionType* CreateCopy();

        void WriteArchive( xml::xostream& output );
    public:
        std::string name_;
        std::string displayName_;
        ADN_Type_Bool isAllowed_;
    };

    typedef ADN_Type_Vector_ABC<MissionParameterValue>  T_MissionParameterValue_Vector;
    typedef T_MissionParameterValue_Vector::iterator   IT_MissionParameterValue_Vector;
    typedef ADN_Type_Vector_ABC<MissionType>            T_Choice_Vector;
    typedef T_Choice_Vector::iterator                  IT_Choice_Vector;

public:
    class MissionParameter : public ADN_Ref_ABC
                           , public ADN_DataTreeNode_ABC
    {
    public:
                 MissionParameter();
        virtual ~MissionParameter();

        std::string GetItemName();
        MissionParameter* CreateCopy();

        void ReadArchive ( xml::xistream& input );
        void ReadValue   ( xml::xistream& input );
        void ReadChoice  ( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    private:
        void FillChoices();

    public:
        ADN_Type_String                                                   strName_;
        ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType > type_;
        ADN_Type_Bool                                                     isOptional_;
        ADN_Type_Int                                                      min_;
        ADN_Type_Int                                                      max_;
        T_MissionParameterValue_Vector                                    values_;
        T_Choice_Vector                                                   choices_;

        ADN_Type_String                                                   diaName_;
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
    public:
                 Mission();
        explicit Mission( unsigned int id );
        virtual ~Mission();

        std::string GetItemName();
        Mission* CreateCopy();

        void ReadArchive ( xml::xistream& input, std::size_t contextLength );
        void ReadParameter( xml::xistream& input, std::size_t& index, std::size_t contextLength );
        void WriteArchive( xml::xostream& output, const std::string& type, const T_MissionParameter_Vector& context );

    public:
        ADN_Type_Int id_;
        ADN_Type_String strName_;
        T_MissionParameter_Vector parameters_;
        ADN_Type_String diaType_;
        ADN_Type_String diaBehavior_;
        ADN_Type_String cdtDiaBehavior_;
        ADN_Type_String mrtDiaBehavior_;
        ADN_Type_String doctrineDescription_;
        ADN_Type_String usageDescription_;
        ADN_Type_String strPackage_;
        ADN_TypePtr_InVector_ABC< ADN_Drawings_Data::DrawingInfo > symbol_;
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
    public:
                 FragOrder();
        explicit FragOrder( unsigned int id );
        virtual ~FragOrder();

        std::string GetItemName();
        FragOrder* CreateCopy();

        void ReadArchive ( xml::xistream& input );
        void ReadParameter( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Int              id_;
        ADN_Type_String           strName_;
        T_MissionParameter_Vector parameters_;
        ADN_Type_String           diaType_;
        ADN_Type_Bool             isAvailableWithoutMission_;
        ADN_Type_String           doctrineDescription_;
        ADN_Type_String           usageDescription_;

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
    virtual void FilesNeeded( T_StringList& vFiles ) const;
    virtual void Reset();

    T_FragOrder_Vector& GetFragOrders();
    T_Mission_Vector&   GetUnitMissions();
    T_Mission_Vector&   GetAutomatMissions();
    T_Mission_Vector&   GetPopulationMissions();
    FragOrder*          FindFragOrder( const std::string& strName );
    Mission*            FindMission( T_Mission_Vector& missions, const std::string& strName );
    virtual void Load( const tools::Loader_ABC& fileLoader );
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadFragOrder( xml::xistream& input );
    void ReadMission( xml::xistream& input, T_Mission_Vector& missions, std::size_t contextLength );
    void ReadContext( xml::xistream& input, T_MissionParameter_Vector& context );
    void ReadContextParameter( xml::xistream& input, T_MissionParameter_Vector& context );
    void WriteArchive( xml::xostream& output );

public:
    T_MissionParameter_Vector   unitContext_;
    T_Mission_Vector            unitMissions_;
    T_MissionParameter_Vector   automatContext_;
    T_Mission_Vector            automatMissions_;
    T_MissionParameter_Vector   populationContext_;
    T_Mission_Vector            populationMissions_;
    T_FragOrder_Vector          fragOrders_;

private:
    static IdentifierFactory idFactory_;
};

#endif // __ADN_Missions_Data_h_
