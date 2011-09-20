// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Supply_Data_h_
#define __ADN_Supply_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Units_Data.h"
#include "ADN_AvailabilityWarning.h"

namespace xml { class xistream; }

// =============================================================================
/** @class  ADN_Supply_Data
    @brief  ADN_Supply_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Supply_Data : public ADN_Data_ABC
{

public:
// *****************************************************************************
    template< typename T >
    class ConvoyInfo : public ADN_Ref_ABC
                     , public ADN_DataTreeNode_ABC
    {
    public:
        explicit ConvoyInfo( typename T::BaseType value );

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( const std::string& attribute, xml::xistream& input );
        void WriteArchive( const std::string& section, const std::string& attribute, xml::xostream& output );

    public:
        ADN_Type_Int  nNbrTrucks_;
        T             value_;
    };

    typedef ADN_Type_Vector_ABC< ConvoyInfo< ADN_Type_Time > >  T_ConvoyTimeInfoVector;
    typedef T_ConvoyTimeInfoVector::iterator                    IT_ConvoyTimeInfoVector;

    typedef ADN_Type_Vector_ABC< ConvoyInfo< ADN_Type_Double > > T_ConvoyDoubleInfoVector;
    typedef T_ConvoyDoubleInfoVector::iterator                   IT_ConvoyDoubleInfoVector;

    class SupplyDataInfos : public ADN_Ref_ABC
                          , public ADN_DataTreeNode_ABC
    {
    public:
                 SupplyDataInfos();
        virtual ~SupplyDataInfos();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );
        bool IsValidDatabase();
        void Reset();

    private:
        void ReadConstitutionTime( xml::xistream& input );
        void ReadLoadingTime( xml::xistream& input );
        void ReadUnloadingTime( xml::xistream& input );
        void ReadSpeedModifier( xml::xistream& input );
        void ReadResourceAvailability( xml::xistream& input );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Units_Data::UnitInfos> ptrUnit_;
        ADN_TypePtr_InVector_ABC<ADN_Missions_Data::Mission> ptrSupplyMission_;

        T_ConvoyTimeInfoVector   vConvoySetupInfos_;
        T_ConvoyTimeInfoVector   vConvoyLoadingInfos_;
        T_ConvoyTimeInfoVector   vConvoyUnloadingInfos_;
        T_ConvoyDoubleInfoVector vConvoySpeedModificatorInfos_;
        T_AvailabilityWarning_Vector vVectorWarnings_;
        ADN_Type_Enum<E_SupplyConvoyType, eNbrSupplyConvoy> dotationSupplyConvoyType_;
        ADN_Type_Enum<E_SupplyConvoyType, eNbrSupplyConvoy> stockSupplyConvoyType_;
    };

// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Supply_Data();
    virtual ~ADN_Supply_Data();
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( T_StringList& vFiles ) const;
    virtual void Reset();
    virtual bool IsValidDatabase();
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    SupplyDataInfos infos_;
};


#endif // __ADN_Supply_Data_h_
