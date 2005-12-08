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

#include "MOS_Types.h"
#include "MOS_TacticalLine_ABC.h"

// =============================================================================
// Created: APE 2004-03-12
// =============================================================================
class MOS_LineManager
{
    MT_COPYNOTALLOWED( MOS_LineManager );

public:
    //! @name Types
    //@{
    typedef std::set< MOS_TacticalLine_ABC* >         T_TacticalLineSet;
    typedef const T_TacticalLineSet                   CT_TacticalLineSet;
    typedef T_TacticalLineSet::iterator               IT_TacticalLineSet;
    typedef T_TacticalLineSet::const_iterator         CIT_TacticalLineSet;
    typedef T_TacticalLineSet::const_reverse_iterator RCIT_TacticalLineSet;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
     MOS_LineManager();
    ~MOS_LineManager();
    //@}
    
    //! @name Operations
    //@{
    void RegisterLine( MOS_TacticalLine_ABC& line );
    void DeleteLine  ( MIL_LineID nLineID );
    void OnDeconnexion();

    MOS_TacticalLine_ABC* FindLine   ( MIL_LineID nLineID );
    CT_TacticalLineSet&   GetLineList();

    bool UpdateToSim        ();
    void UseSimTacticalLines();
    bool NeedSaving         ();
    //@}

    //! @name In/Out
    //@{
    void Read ( MT_InputArchive_ABC& archive );
    void Write( MT_OutputArchive_ABC& archive ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void DeleteAll();
    //@}

private:
    T_TacticalLineSet lineSet_;
    bool              bUseSimTacticalLines_;
};

#   include "MOS_LineManager.inl"

#endif // __MOS_LineManager_h_
