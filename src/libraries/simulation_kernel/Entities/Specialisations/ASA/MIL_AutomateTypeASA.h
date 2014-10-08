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

#include "Entities/Automates/MIL_AutomateType.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AutomateType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateTypeASA : public MIL_AutomateType
{
public:
             MIL_AutomateTypeASA( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AutomateTypeASA();

    virtual std::string GetArchetypeName() const;
    virtual void RegisterFunctions( sword::Brain& brain, MIL_Automate& automat ) const;

    static const MIL_AutomateType* Create( const std::string& strName, xml::xistream& xis );
};

#endif // __MIL_AutomateTypeASA_h_
