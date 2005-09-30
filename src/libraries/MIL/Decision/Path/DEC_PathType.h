//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathType.h $
// $Author: Nld $
// $Modtime: 21/07/05 16:33 $
// $Revision: 5 $
// $Workfile: DEC_PathType.h $
//
//*****************************************************************************

#ifndef __DEC_PathType_h_
#define __DEC_PathType_h_

#include "MIL.h"

class TerrainRule_ABC;
class DEC_Path;
class DEC_PathClass;

//*****************************************************************************
// Created: JDY 03-04-10
//*****************************************************************************
class DEC_PathType
{
public:
    //! @name Types
    //@{
    enum E_PathType
    {
        eInfoMovement = 0,
        eInfoRecon,
        eInfoInfiltration,
        eInfoAssault,
        eInfoRetreat,
        eInfoBackup,
        eInfoMineClearance,
        eNbrPathType
    };
    //@}    

public:
     DEC_PathType( E_PathType nType );
    ~DEC_PathType();

    //! @name Operations
    //@{
    TerrainRule_ABC& CreateRule( const DEC_Path& path, const MT_Vector2D& from, const MT_Vector2D& to ) const;

    std::string ConvertPathTypeToString() const;

    static void InitializeRules( MIL_InputArchive& archive );
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< bool, bool >                 T_BooleanPair;
    typedef std::pair< std::string, T_BooleanPair > T_RuleType;
    typedef std::map< T_RuleType, DEC_PathClass* >  T_Rules;
    //@}

    //! @name Helpers
    //@{
    static void CheckRulesExistence();
    //@}

private:
    static T_Rules rules_;
    E_PathType nPathType_;
};

#   include "DEC_PathType.inl"

#endif // __DEC_PathType_h_
