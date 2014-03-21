// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Inhabitants_Data_h_
#define __ADN_Inhabitants_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Type_Repartition.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Crowds_Data.h"
#include "ADN_ResourceNetworks_Data.h"

// =============================================================================
/** @class  ADN_Inhabitants_Data
*/
// Created: SLG 2010-11-22
// =============================================================================
class ADN_Inhabitants_Data : public ADN_Data_ABC
{
public:
// *****************************************************************************
    class EventInfos : public ADN_Ref_ABC
    {
    public:
                 EventInfos();
        virtual ~EventInfos();

        EventInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Enum< E_Days, eNbrDays > day_;
        ADN_Type_String from_;
        ADN_Type_String to_;
        ADN_TypePtr_InVector_ABC< ADN_Urban_Data::AccommodationInfos > ptrAccommodation_;
    };
    typedef ADN_Type_Vector_ABC< EventInfos > T_EventInfosVector;

    class InhabitantsInfosConsumption : public ADN_CrossedRef< ADN_ResourceNetworks_Data::ResourceNetworkInfos >
    {
    public:
                 InhabitantsInfosConsumption();
        virtual ~InhabitantsInfosConsumption() {}
        std::string GetItemName();

        InhabitantsInfosConsumption* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Int consumption_;
    };
    typedef ADN_Type_Vector_ABC< InhabitantsInfosConsumption > T_InhabitantsInfosConsumptionVector;

// *****************************************************************************
    class InhabitantsInfos : public ADN_RefWithLocalizedName
    {
    public:
        InhabitantsInfos();
        InhabitantsInfos( unsigned int );
        ~InhabitantsInfos();

        InhabitantsInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );

    private:
        void ReadEvent( xml::xistream& input );
        void ReadConsumption( xml::xistream& input );
        void CheckErrors( ADN_ConsistencyChecker& checker ) const;

    public:
        ADN_Type_Int nId_;
        ADN_TypePtr_InVector_ABC< ADN_Crowds_Data::CrowdsInfos > ptrModel_;
        ADN_Type_String strAngryCrowdMission_;
        ADN_Type_Repartition repartition_;
        ADN_Type_Time transferTime_;
        ADN_Type_Double securityLossOnFire_;
        ADN_Type_Double securityGainPerHour_;
        ADN_Type_Int healthNeed_;
        T_EventInfosVector schedule_;
        T_InhabitantsInfosConsumptionVector consumptions_;
    };
    typedef ADN_Type_Vector_ABC< InhabitantsInfos > T_InhabitantsInfosVector;

// *****************************************************************************
public:
             ADN_Inhabitants_Data();
    virtual ~ADN_Inhabitants_Data();

    void FilesNeeded(tools::Path::T_Paths& l) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    T_InhabitantsInfosVector& GetInhabitants();
    InhabitantsInfos* FindInhabitant( const std::string& strName );

    QStringList GetInhabitantsThatUse( ADN_Crowds_Data::CrowdsInfos& population );
    QStringList GetInhabitantsThatUse( ADN_ResourceNetworks_Data::ResourceNetworkInfos& network );
    QStringList GetSchedulesThatUse( ADN_Urban_Data::AccommodationInfos& accomodations );

private:
    void ReadArchive( xml::xistream& input );
    void ReadPeople( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    T_InhabitantsInfosVector vInhabitants_;

private:
    static tools::IdManager idManager_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::GetPopulation
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
inline
ADN_Inhabitants_Data::T_InhabitantsInfosVector& ADN_Inhabitants_Data::GetInhabitants()
{
    return vInhabitants_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FindPopulation
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
inline
ADN_Inhabitants_Data::InhabitantsInfos* ADN_Inhabitants_Data::FindInhabitant( const std::string& strName )
{
    auto it = std::find_if( vInhabitants_.begin(), vInhabitants_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vInhabitants_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Inhabitants_Data_h_
