// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Asa/MIL_AutomateTypeASA.h $
// $Author: Nld $
// $Modtime: 23/12/04 11:04 $
// $Revision: 2 $
// $Workfile: MIL_AutomateTypeASA.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateTypeASA_h_
#define __MIL_AutomateTypeASA_h_

#include "MIL.h"

#include "Entities/Automates/MIL_AutomateType.h"

// =============================================================================
// @class  MIL_AutomateType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateTypeASA : public MIL_AutomateType
{
    MT_COPYNOTALLOWED( MIL_AutomateTypeASA )

public:
    MIL_AutomateTypeASA( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AutomateTypeASA();

public:
    static const MIL_AutomateType* Create( const std::string& strName, MIL_InputArchive& archive );
};

#include "MIL_AutomateTypeASA.inl"

#endif // __MIL_AutomateTypeASA_h_
