//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LineManager.h $
// $Author: Ape $
// $Modtime: 30/07/04 11:10 $
// $Revision: 5 $
// $Workfile: MOS_LineManager.h $
//
//*****************************************************************************

#ifndef __MOS_LineManager_h_
#define __MOS_LineManager_h_

#include "MOS_Lima.h"
#include "MOS_Limit.h"


// =============================================================================
/** @class  MOS_LineManager
    @brief  Manages the limits and limas.
*/
// Created: APE 2004-03-12
// =============================================================================
class MOS_LineManager
{
    MT_COPYNOTALLOWED( MOS_LineManager );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::set< MOS_TacticalLine_ABC* >         T_TacticalLineSet;
    typedef const T_TacticalLineSet                   CT_TacticalLineSet;
    typedef T_TacticalLineSet::iterator               IT_TacticalLineSet;
    typedef T_TacticalLineSet::const_iterator         CIT_TacticalLineSet;
    typedef T_TacticalLineSet::const_reverse_iterator RCIT_TacticalLineSet;
    //@}

public:
     MOS_LineManager();
    ~MOS_LineManager();
    
    //-------------------------------------------------------------------------
    /** @name  */
    //-------------------------------------------------------------------------
    //@{
    void RegisterLine( MOS_TacticalLine_ABC& line );
    void DeleteLine  ( MIL_LineID nLineID );
    void DeleteAll  ();
    void OnDeconnexion();

    MOS_TacticalLine_ABC* FindLine( MIL_LineID nLineID );
    CT_TacticalLineSet& GetLineList();

    bool UpdateToSim();
    //@}

    //! @name In/Out
    //@{
    void Read( MT_InputArchive_ABC& archive );
    void Write( MT_OutputArchive_ABC& archive ) const;
    //@}

private:
    T_TacticalLineSet lineSet_;
};

#   include "MOS_LineManager.inl"

#endif // __MOS_LineManager_h_
