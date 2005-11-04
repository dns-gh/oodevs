// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TypePion.h $
// $Author: Age $
// $Modtime: 14/04/05 11:03 $
// $Revision: 3 $
// $Workfile: MOS_TypePion.h $
//
// *****************************************************************************

#ifndef __MOS_TypePion_h_
#define __MOS_TypePion_h_

#include "MOS_Types.h"

class MOS_AgentModel;
class MOS_Nature;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class MOS_TypePion
{
    MT_COPYNOTALLOWED( MOS_TypePion );
public:
         typedef std::map< MIL_AgentID, uint >       T_ResourceQty_Map;
public:
              MOS_TypePion( const std::string& strName, const MOS_Nature& nature );
              MOS_TypePion( const std::string& strName, MOS_InputArchive& archive );
             ~MOS_TypePion();

    //! @name Accessors
    //@{
    const std::string&    GetName () const;
          uint            GetID   () const;
    const MOS_AgentModel& GetModel() const;
    char                  GetSymbol() const;
    T_ResourceQty_Map     GetStocks() const;
    char                  GetLevelSymbol() const;
          bool            IsLogistic    () const;
          bool            HasMaintenance() const;
          bool            HasMedical    () const;
          bool            HasSupply     () const;
    //@}

private:
    const std::string     strName_;
    uint                  nID_;
    const MOS_AgentModel* pModel_;
    MOS_Nature*           pNature_;
    bool                  bHasMaintenance_;
    bool                  bHasMedical_;
    bool                  bHasSupply_;
    T_ResourceQty_Map     stocks_;
};

#include "MOS_TypePion.inl"

#endif // __MOS_TypePion_h_
