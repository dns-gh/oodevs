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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_TypeAutomate.h $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: MOS_TypeAutomate.h $
//
// *****************************************************************************

#ifndef __MOS_TypeAutomate_h_
#define __MOS_TypeAutomate_h_

#include "MOS_Types.h"

class MOS_AgentModel;
class MOS_TypePion;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class MOS_TypeAutomate
{
    MT_COPYNOTALLOWED( MOS_TypeAutomate );

public:
     MOS_TypeAutomate( const std::string& strName, MOS_InputArchive& archive );
    ~MOS_TypeAutomate();

    //! @name Accessors
    //@{
    const std::string&    GetName  () const;
          uint            GetID    () const;
    const MOS_AgentModel& GetModel () const;
    const MOS_TypePion&   GetTypePC() const;
    const std::string 	  GetType  () const;
    //@}

private:
    const std::string     strName_;
	std::string	          strType_;
    uint                  nID_;
    const MOS_AgentModel* pModel_;
    const MOS_TypePion*   pTypePC_;
};

#include "MOS_TypeAutomate.inl"

#endif // __MOS_TypeAutomate_h_
