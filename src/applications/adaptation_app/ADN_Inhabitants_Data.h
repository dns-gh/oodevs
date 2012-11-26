// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_People_Data_h_
#define __ADN_People_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Type_Repartition.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Population_Data.h"
#include "ADN_ResourceNetworks_Data.h"
#include <map>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

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
        ~EventInfos();

        EventInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Enum< E_Days, eNbrDays > day_;
        ADN_Type_String from_;
        ADN_Type_String to_;
        ADN_TypePtr_InVector_ABC< ADN_Urban_Data::AccommodationInfos > ptrAccommodation_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< EventInfos >, EventInfosVector )

    class InhabitantsInfosConsumption : public ADN_Ref_ABC
    {

    public:
                 InhabitantsInfosConsumption();
        virtual ~InhabitantsInfosConsumption() {}
        std::string GetItemName();

        InhabitantsInfosConsumption* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_TypePtr_InVector_ABC< ADN_ResourceNetworks_Data::ResourceNetworkInfos > ptrResource_;
        ADN_Type_Int consumption_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< InhabitantsInfosConsumption >, InhabitantsInfosConsumptionVector )

// *****************************************************************************
    class InhabitantsInfos : public ADN_RefWithName
    {
    public:
        InhabitantsInfos();
        InhabitantsInfos( unsigned int );
        ~InhabitantsInfos();

        InhabitantsInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    private:
        void ReadEvent( xml::xistream& input );
        void ReadConsumption( xml::xistream& input );
        const std::string CheckErrors() const;

    public:
        ADN_Type_Int nId_;
        ADN_TypePtr_InVector_ABC<ADN_Population_Data::PopulationInfos> ptrModel_;
        ADN_Type_String strAngryCrowdMission_;
        ADN_Type_Repartition repartition_;
        ADN_Type_Time transferTime_;
        ADN_Type_Double securityLossOnFire_;
        ADN_Type_Double securityGainPerHour_;
        ADN_Type_Int healthNeed_;
        T_EventInfosVector schedule_;
        T_InhabitantsInfosConsumptionVector consumptions_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< InhabitantsInfos >, InhabitantsInfosVector )

// *****************************************************************************
public:
             ADN_Inhabitants_Data();
    virtual ~ADN_Inhabitants_Data();

    void Reset();
    void FilesNeeded(T_StringList& l) const;

    T_InhabitantsInfosVector& GetInhabitants();
    InhabitantsInfos* FindInhabitant( const std::string& strName );

    QStringList GetInhabitantsThatUse( ADN_Population_Data::PopulationInfos& population );
    QStringList GetInhabitantsThatUse( ADN_ResourceNetworks_Data::ResourceNetworkInfos& network );

private:
    void ReadArchive( xml::xistream& input );
    void ReadPeople( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    T_InhabitantsInfosVector vPeople_;

private:
    static tools::IdManager idManager_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::GetPopulation
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
inline
ADN_Inhabitants_Data::T_InhabitantsInfosVector& ADN_Inhabitants_Data::GetInhabitants()
{
    return vPeople_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FindPopulation
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
inline
ADN_Inhabitants_Data::InhabitantsInfos* ADN_Inhabitants_Data::FindInhabitant( const std::string& strName )
{
    IT_InhabitantsInfosVector it = std::find_if( vPeople_.begin(), vPeople_.end(), ADN_Tools::NameCmp< InhabitantsInfos >( strName ) );
    if( it == vPeople_.end() )
        return 0;
    return *it;
}

#endif // __ADN_People_Data_h_
