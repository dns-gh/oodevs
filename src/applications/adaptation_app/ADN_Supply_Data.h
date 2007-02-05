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

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Supply_Data
    @brief  ADN_Supply_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Supply_Data
    : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Supply_Data )

public:
// *****************************************************************************
    template< typename T >
    class ConvoyInfo
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ConvoyInfo )
    public:
        ConvoyInfo( typename T::BaseType value );

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( const std::string& strSection, ADN_XmlInput_Helper& input );
        void WriteArchive( const std::string& strSection, MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Int  nNbrTrucks_;
        T             value_;
    };

    typedef ADN_Type_Vector_ABC< ConvoyInfo< ADN_Type_Time > >  T_ConvoyTimeInfoVector;
    typedef T_ConvoyTimeInfoVector::iterator                    IT_ConvoyTimeInfoVector;

    typedef ADN_Type_Vector_ABC< ConvoyInfo< ADN_Type_Double > > T_ConvoyDoubleInfoVector;
    typedef T_ConvoyDoubleInfoVector::iterator                   IT_ConvoyDoubleInfoVector;

// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
     ADN_Supply_Data();
    ~ADN_Supply_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();
    //@}

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

public:
    ADN_TypePtr_InVector_ABC<ADN_Units_Data::UnitInfos> ptrUnit_;
    ADN_TypePtr_InVector_ABC<ADN_Missions_Data::Mission> ptrSupplyMission_;

    T_ConvoyTimeInfoVector   vConvoySetupInfos_;
    T_ConvoyTimeInfoVector   vConvoyLoadingInfos_;
    T_ConvoyTimeInfoVector   vConvoyUnloadingInfos_;
    T_ConvoyDoubleInfoVector vConvoySpeedModificatorInfos_;
    T_AvailabilityWarning_Vector vVectorWarnings_;
};


#endif // __ADN_Supply_Data_h_
