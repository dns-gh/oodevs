// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Supply_Data.h $
// $Author: Ape $
// $Modtime: 22/03/05 16:02 $
// $Revision: 1 $
// $Workfile: ADN_Supply_Data.h $
//
// *****************************************************************************

#ifndef __ADN_Supply_Data_h_
#define __ADN_Supply_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Units_Data.h"

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
    class ConvoySetupInfo
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ConvoySetupInfo )
    public:
        ConvoySetupInfo();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Int    nNbrTrucks_;
        ADN_Type_Double rSetupTime_;
    };

    typedef ADN_Type_Vector_ABC<ConvoySetupInfo>  T_ConvoySetupInfoVector;
    typedef T_ConvoySetupInfoVector::iterator    IT_ConvoySetupInfoVector;


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
    ADN_Type_Enum<E_UnitMission, eNbrUnitMission> nSupplyMission_;

    T_ConvoySetupInfoVector vConvoySetupInfos_;
};


#endif // __ADN_Supply_Data_h_
