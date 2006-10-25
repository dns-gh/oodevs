// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/MIL_AgentType_ABC.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:47 $
// $Revision: 7 $
// $Workfile: MIL_AgentType_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_AgentType_ABC_h_
#define __MIL_AgentType_ABC_h_

#include "MIL.h"

class PHY_NatureLevel;
class PHY_NatureAtlas;

// =============================================================================
// @class  MIL_AgentType_ABC
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentType_ABC
{
    MT_COPYNOTALLOWED( MIL_AgentType_ABC )

public:
    MIL_AgentType_ABC( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AgentType_ABC();

    //! @name Types
    //@{
          uint             GetID         () const;
    const std::string&     GetName       () const;
    const PHY_NatureLevel& GetNatureLevel() const;
    const PHY_NatureAtlas& GetNatureAtlas() const;

    virtual bool           IsMilitia     () const;
    virtual bool           IsRefugee     () const;
    virtual bool           IsTerrorist   () const;
    //@}

private:
    //! @name Init
    //@{
    void InitializeNature( MIL_InputArchive& archive );
    //@}

private:
          uint             nID_;
    const std::string      strName_;
    const PHY_NatureLevel* pNatureLevel_;
    const PHY_NatureAtlas* pNatureAtlas_;
};

#include "MIL_AgentType_ABC.inl"

#endif // __MIL_AgentType_ABC_h_
