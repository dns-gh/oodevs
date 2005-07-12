//*****************************************************************************
//
// $Created: JDY 03-07-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Launchers_Data.h $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 7 $
// $Workfile: ADN_Launchers_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Launchers_Data_h_
#define __ADN_Launchers_Data_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h" 
#include "ADN_Type_Tab_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Tools.h"

class ADN_XmlInput_Helper;


//*****************************************************************************
// Created: JDY 03-07-11
//*****************************************************************************
class ADN_Launchers_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Launchers_Data )

public:
//*****************************************************************************
    typedef ADN_Type_Double ModifPhsInfos;
    

//*****************************************************************************
    class LauncherInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( LauncherInfos )

    public:
        LauncherInfos();
        ~LauncherInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        LauncherInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_String                           strName_;
        ADN_Type_Bool                             bDirect_;
        ADN_Type_Bool                             bIndirect_;

        // Data for direct
        ADN_Type_Tab_ABC<ModifPhsInfos>           tabModifs_;
    };

    typedef ADN_Type_Vector_ABC<LauncherInfos>          T_LauncherInfos_Vector;
    typedef T_LauncherInfos_Vector::iterator            IT_LauncherInfos_Vector;


//*****************************************************************************
public:
    explicit ADN_Launchers_Data();
    virtual ~ADN_Launchers_Data();

    void FilesNeeded(T_StringList& l) const;
    void Reset();
    
    T_LauncherInfos_Vector& GetLaunchersInfos();
    LauncherInfos*          FindLauncher( const std::string& strName );

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

private:
    T_LauncherInfos_Vector      vLaunchers_;
};


//-----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::GetLaunchersInfos
// Created: JDY 03-07-15
//-----------------------------------------------------------------------------
inline
ADN_Launchers_Data::T_LauncherInfos_Vector& ADN_Launchers_Data::GetLaunchersInfos()
{
    return vLaunchers_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Launchers_Data::FindLauncher
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
inline
ADN_Launchers_Data::LauncherInfos* ADN_Launchers_Data::FindLauncher( const std::string& strName )
{
    IT_LauncherInfos_Vector it = std::find_if( vLaunchers_.begin(), vLaunchers_.end(), ADN_Tools::NameCmp<ADN_Launchers_Data::LauncherInfos>( strName ) );
    if( it == vLaunchers_.end() )
        return 0;
    return *it;
}


#endif // __ADN_Launchers_Data_h_