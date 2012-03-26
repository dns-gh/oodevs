// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/MIL_AutomateTypeLOG.h $
// $Author: Nld $
// $Modtime: 27/12/04 16:34 $
// $Revision: 1 $
// $Workfile: MIL_AutomateTypeLOG.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateTypeBaseLOG_h_
#define __MIL_AutomateTypeBaseLOG_h_

#include "MIL_AutomateTypeLOG.h"

// =============================================================================
// @class  MIL_AutomateTypeBaseLOG
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateTypeBaseLOG : public MIL_AutomateTypeLOG
{
public:
             MIL_AutomateTypeBaseLOG( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AutomateTypeBaseLOG();

    //! @name Accessors
    //@{
    virtual bool IsLogisticBase() const;
    //@}

    //! @name Main
    //@{
    static const MIL_AutomateType* Create( const std::string& strName, xml::xistream& xis );
    //@}
};

#endif // __MIL_AutomateTypeBaseLOG_h_
