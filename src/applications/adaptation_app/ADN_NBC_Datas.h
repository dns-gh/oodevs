// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_NBC_Datas_h_
#define __ADN_NBC_Datas_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Enums.h"

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
    class NbcIntoxInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( NbcIntoxInfos )

    public:
        NbcIntoxInfos( const std::string& nodeName );
        virtual ~NbcIntoxInfos() {}

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( NbcIntoxInfos& infos );

        void ReadEffect( xml::xistream& input );
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );
        void WriteContent( xml::xostream& output );

    public:
        std::string     nodeName_;
        ADN_Type_Bool   bIntoxPresent_;
        ADN_Type_Double rNbAlivedHumans_;
        ADN_Type_Double rNbHurtedHumans1_;
        ADN_Type_Double rNbHurtedHumans2_;
        ADN_Type_Double rNbHurtedHumans3_;
        ADN_Type_Double rNbHurtedHumansE_;
        ADN_Type_Double rNbDeadHumans_;
        ADN_Type_Bool   bContaminationPresent_;
    };

    class NbcGazInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( NbcGazInfos )

    public:
        NbcGazInfos();
        virtual ~NbcGazInfos() {}

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( NbcGazInfos& infos );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        NbcIntoxInfos   intoxInfos_;
        ADN_Type_Time   lifeTime_;
        ADN_Type_Double rSpreadAngle_;
    };

    class NbcAgentInfos
       : public ADN_Ref_ABC
       , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( NbcAgentInfos )

    public:
        NbcAgentInfos();
        virtual ~NbcAgentInfos() {}

        virtual std::string GetNodeName();
        std::string GetItemName();

        NbcAgentInfos* CreateCopy();

        void ReadEffect( xml::xistream& input );
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String strName_;
        int             nMosId_;
        NbcIntoxInfos   liquidInfos_;
        ADN_Type_Bool   bGazPresent_;
        NbcGazInfos     gazInfos_;
    };

    typedef ADN_Type_Vector_ABC< NbcAgentInfos > T_NbcAgentInfos_Vector;


//*****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
     ADN_NBC_Datas();
    virtual ~ADN_NBC_Datas();
    //@}

    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();

    T_NbcAgentInfos_Vector& GetNbcAgentVector();
    
    int GetNextId();

private:
    void ReadAgent( xml::xistream& input );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

private:
    int nNextId_;

    T_NbcAgentInfos_Vector vNbcAgent_;
    ADN_Type_Double rNbcSuitMaxSpeedMultiplier_;
    ADN_Type_Double rNbcSuitReloadSpeedMultiplier_;
    ADN_Type_Double rWindSpeedLimitForSpreading_;
    ADN_Type_Double rContaminationDistance_;
    ADN_Type_Double rContaminationQuantityGiven_;
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
