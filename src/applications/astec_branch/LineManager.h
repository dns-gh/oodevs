//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LineManager.h $
// $Author: Ape $
// $Modtime: 30/07/04 11:10 $
// $Revision: 5 $
// $Workfile: LineManager.h $
//
//*****************************************************************************

#ifndef __LineManager_h_
#define __LineManager_h_

#include "Types.h"
#include "TacticalLine_ABC.h"

// =============================================================================
// Created: APE 2004-03-12
// =============================================================================
class LineManager
{
    MT_COPYNOTALLOWED( LineManager );

public:
    //! @name Types
    //@{
    typedef std::set< TacticalLine_ABC* >         T_TacticalLineSet;
    typedef const T_TacticalLineSet                   CT_TacticalLineSet;
    typedef T_TacticalLineSet::iterator               IT_TacticalLineSet;
    typedef T_TacticalLineSet::const_iterator         CIT_TacticalLineSet;
    typedef T_TacticalLineSet::const_reverse_iterator RCIT_TacticalLineSet;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
     LineManager();
    ~LineManager();
    //@}
    
    //! @name Operations
    //@{
    void RegisterLine( TacticalLine_ABC& line );
    void DeleteLine  ( MIL_LineID nLineID );
    void OnDeconnexion();

    TacticalLine_ABC* FindLine   ( MIL_LineID nLineID );
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

#   include "LineManager.inl"

#endif // __LineManager_h_
