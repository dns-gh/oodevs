// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_NBC_Datas.h $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 9 $
// $Workfile: ADN_NBC_Datas.h $
//
// *****************************************************************************

#ifndef __ADN_NBC_Datas_h_
#define __ADN_NBC_Datas_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Enums.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_NBC_Datas
    @brief  ADN_NBC_Datas
*/
// Created: AGN 2004-05-06
// =============================================================================
class ADN_NBC_Datas : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_NBC_Datas );

public:
//*****************************************************************************
    class NbcAgentInfos
       : public ADN_Ref_ABC
       , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( NbcAgentInfos )

    public:
        NbcAgentInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        NbcAgentInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_String strName_;
        int nMosId_;

        ADN_Type_Double rNbAlivedHumans_;
        ADN_Type_Double rNbHurtedHumans1_;
        ADN_Type_Double rNbHurtedHumans2_;
        ADN_Type_Double rNbHurtedHumans3_;
        ADN_Type_Double rNbHurtedHumansE_;
        ADN_Type_Double rNbDeadHumans_;
        
        ADN_Type_Time   lifeTime_;
        ADN_Type_Double rSpreadAngle_;
    };

    typedef ADN_Type_Vector_ABC< NbcAgentInfos > T_NbcAgentInfos_Vector;


//*****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
     ADN_NBC_Datas();
    ~ADN_NBC_Datas();
    //@}

    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();

    T_NbcAgentInfos_Vector& GetNbcAgentVector();
    
    int GetNextId();

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

private:
    int nNextId_;

    T_NbcAgentInfos_Vector vNbcAgent_;
    ADN_Type_Double rNbcSuitMaxSpeedMultiplier_;
    ADN_Type_Double rNbcSuitReloadSpeedMultiplier_;
    ADN_Type_Double rWindSpeedLimitForSpreading_;
};


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Datas::GetNbcAgentVector
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
inline
ADN_NBC_Datas::T_NbcAgentInfos_Vector& ADN_NBC_Datas::GetNbcAgentVector()
{
    return vNbcAgent_;
}


#endif // __ADN_NBC_Datas_h_
