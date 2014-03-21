// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_NBC_Data_h_
#define __ADN_NBC_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "tools/IdManager.h"

// =============================================================================
/** @class  ADN_NBC_Data
    @brief  ADN_NBC_Data
*/
// Created: AGN 2004-05-06
// =============================================================================
class ADN_NBC_Data : public ADN_Data_ABC
{

public:
//*****************************************************************************
    class NbcIntoxInfos : public ADN_Ref_ABC
    {
    public:
        explicit NbcIntoxInfos();
        virtual ~NbcIntoxInfos() {}

        void CopyFrom( NbcIntoxInfos& infos );
        void ReadEffect( xml::xistream& input );
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        void WriteContent( xml::xostream& output ) const;
        void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;

    public:
        std::string parentName_;
        ADN_Type_Bool bIntoxPresent_;
        ADN_Type_Double rNbAlivedHumans_;
        ADN_Type_Double rNbHurtedHumans1_;
        ADN_Type_Double rNbHurtedHumans2_;
        ADN_Type_Double rNbHurtedHumans3_;
        ADN_Type_Double rNbHurtedHumansE_;
        ADN_Type_Double rNbDeadHumans_;
        ADN_Type_Bool bContaminationPresent_;
    };

    class NbcGazInfos : public ADN_Ref_ABC
    {
    public:
                 NbcGazInfos();
        virtual ~NbcGazInfos() {}

        void CopyFrom( NbcGazInfos& infos );
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        NbcIntoxInfos intoxInfos_;
        ADN_Type_Time lifeTime_;
        ADN_Type_Double rSpreadAngle_;
    };

    class NbcAgentInfos : public ADN_RefWithLocalizedName
    {
    public:
                 NbcAgentInfos();
                 NbcAgentInfos( unsigned int id );
        virtual ~NbcAgentInfos() {}

        NbcAgentInfos* CreateCopy();
        void ReadEffect( xml::xistream& input );
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Int nId_;
        ADN_Type_String category_;
        NbcIntoxInfos liquidInfos_;
        ADN_Type_Bool bGazPresent_;
        ADN_Type_Bool bLiquidPresent_;
        NbcGazInfos gazInfos_;
    };
    typedef ADN_Type_Vector_ABC< NbcAgentInfos > T_NbcAgentInfos_Vector;

//*****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_NBC_Data();
    virtual ~ADN_NBC_Data();
    //@}

    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;

    T_NbcAgentInfos_Vector& GetNbcAgentVector();

public:
    ADN_Type_Double rNbcSuitMaxSpeedMultiplier_;
    ADN_Type_Double rNbcSuitReloadSpeedMultiplier_;
    ADN_Type_Double rWindSpeedLimitForSpreading_;
    ADN_Type_Double rContaminationDistance_;
    ADN_Type_Double rContaminationQuantityGiven_;

private:
    void ReadAgent( xml::xistream& input );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;

private:
    T_NbcAgentInfos_Vector vNbcAgent_;

private:
    static tools::IdManager idManager_;
};

// -----------------------------------------------------------------------------
// Name: ADN_NBC_Data::GetNbcAgentVector
// Created: AGN 2004-05-06
// -----------------------------------------------------------------------------
inline
ADN_NBC_Data::T_NbcAgentInfos_Vector& ADN_NBC_Data::GetNbcAgentVector()
{
    return vNbcAgent_;
}

#endif // __ADN_NBC_Data_h_
