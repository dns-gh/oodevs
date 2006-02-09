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
// $Archive: /MVW_v10/Build/SDK/Light2/src/TypePion.h $
// $Author: Age $
// $Modtime: 14/04/05 11:03 $
// $Revision: 3 $
// $Workfile: TypePion.h $
//
// *****************************************************************************

#ifndef __TypePion_h_
#define __TypePion_h_

#include "Types.h"

class AgentModel;
class Nature;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class TypePion
{
    MT_COPYNOTALLOWED( TypePion );

public:
    typedef std::map< MIL_AgentID, uint >       T_ResourceQty_Map;

public:
    //! @name Constructor/Destructor
    //@{
     TypePion( const std::string& strName, const Nature& nature );
     TypePion( const std::string& strName, InputArchive& archive );
    ~TypePion();
    //@}

    //! @name Accessors
    //@{
    const std::string&       GetName       () const;
          uint               GetID         () const;
    const AgentModel&    GetModel      () const;
    char                     GetSymbol     () const;
    const T_ResourceQty_Map& GetStocks     () const;
    char                     GetLevelSymbol() const;
          bool               IsLogistic    () const;
          bool               HasMaintenance() const;
          bool               HasMedical    () const;
          bool               HasSupply     () const;
    //@}

private:
    const std::string     strName_;
    uint                  nID_;
    const AgentModel* pModel_;
    Nature*           pNature_;
    bool                  bHasMaintenance_;
    bool                  bHasMedical_;
    bool                  bHasSupply_;
    T_ResourceQty_Map     stocks_;
};

#include "TypePion.inl"

#endif // __TypePion_h_
