// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Automata_Data_h_
#define __ADN_Automata_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Units_Data.h"

namespace xml { class xistream; }


// =============================================================================
/** @class  ADN_Automata_Data
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_Automata_Data : public ADN_Data_ABC
{

public:
// *****************************************************************************
    class UnitInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {

    public:
        UnitInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        UnitInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output, const ADN_TypePtr_InVector_ABC<ADN_Units_Data::UnitInfos>& pc );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Units_Data::UnitInfos> ptrUnit_;
        ADN_Type_Int                                     min_;
        ADN_Type_Int                                     max_;
    };

    typedef ADN_Type_Vector_ABC<UnitInfos> T_UnitInfosVector;
    typedef T_UnitInfosVector::iterator    IT_UnitInfosVector;


// *****************************************************************************
    class AutomatonInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {

    public:
        AutomatonInfos();
        ~AutomatonInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        AutomatonInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadUnit( xml::xistream& input );
        void WriteArchive( xml::xostream& output, int nMosId );

    public:
        ADN_Type_String                                       strName_;
        ADN_Type_Enum<E_AgentTypeAutomate,eNbrAgentTypeAutomate> nAgentType_;
        ADN_TypePtr_InVector_ABC<ADN_Models_Data::ModelInfos> ptrModel_;
        T_UnitInfosVector                                     vSubUnits_;
        ADN_TypePtr_InVector_ABC<ADN_Units_Data::UnitInfos>   ptrUnit_;
        ADN_Type_Bool                                         bStrengthRatioFeedbackTime_;
        ADN_Type_Time                                         strengthRatioFeedbackTime_;
    };

    typedef ADN_Type_Vector_ABC<AutomatonInfos>  T_AutomatonInfosVector;
    typedef T_AutomatonInfosVector::iterator      IT_AutomatonInfosVector;


// *****************************************************************************
public:
    ADN_Automata_Data();
    virtual ~ADN_Automata_Data();

    void                        FilesNeeded(T_StringList& l) const;
    void                        Reset();

    T_AutomatonInfosVector&  GetAutomata();
    AutomatonInfos*          FindAutomaton( const std::string& strName );

    QStringList GetAutomataThatUse( ADN_Units_Data::UnitInfos& unit );
    QStringList GetAutomataThatUse( ADN_Models_Data::ModelInfos& model );

private:
    void ReadArchive( xml::xistream& input );
    void ReadAutomat( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    T_AutomatonInfosVector  vAutomata_;
};


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::GetAutomata
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Automata_Data::T_AutomatonInfosVector& ADN_Automata_Data::GetAutomata()
{
    return vAutomata_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::FindAutomaton
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Automata_Data::AutomatonInfos* ADN_Automata_Data::FindAutomaton( const std::string& strName )
{
    IT_AutomatonInfosVector it = std::find_if( vAutomata_.begin(), vAutomata_.end(), ADN_Tools::NameCmp<AutomatonInfos>( strName ) );
    if( it == vAutomata_.end() )
        return 0;
    return *it;
}


#endif // __ADN_Automata_Data_h_
