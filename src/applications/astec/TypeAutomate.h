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
// $Archive: /MVW_v10/Build/SDK/Light2/src/TypeAutomate.h $
// $Author: Ape $
// $Modtime: 16/02/05 10:48 $
// $Revision: 1 $
// $Workfile: TypeAutomate.h $
//
// *****************************************************************************

#ifndef __TypeAutomate_h_
#define __TypeAutomate_h_

#include "Types.h"

class AgentModel;
class TypePion;

// =============================================================================
// Created: NLD 2004-03-18
// =============================================================================
class TypeAutomate
{
    MT_COPYNOTALLOWED( TypeAutomate );

public:
     TypeAutomate( const std::string& strName, InputArchive& archive );
    ~TypeAutomate();

    //! @name Accessors
    //@{
    const std::string&    GetName  () const;
          uint            GetID    () const;
    const AgentModel& GetModel () const;
    const TypePion&   GetTypePC() const;
    const std::string 	  GetType  () const;
    //@}

private:
    const std::string     strName_;
	std::string	          strType_;
    uint                  nID_;
    const AgentModel* pModel_;
    const TypePion*   pTypePC_;
};

#include "TypeAutomate.inl"

#endif // __TypeAutomate_h_
