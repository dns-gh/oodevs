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

#ifndef __MIL_AutomateTypeLOG_h_
#define __MIL_AutomateTypeLOG_h_

#include "MIL.h"

#include "Entities/Automates/MIL_AutomateType.h"

// =============================================================================
// @class  MIL_AutomateType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateTypeLOG : public MIL_AutomateType
{
    MT_COPYNOTALLOWED( MIL_AutomateTypeLOG )

public:
    MIL_AutomateTypeLOG( const std::string& strName, MIL_InputArchive& archive );
    virtual ~MIL_AutomateTypeLOG();

    //! @name Accessors
    //@{
    virtual bool IsLogistic() const;
    //@}

    //! @name Instanciation
    //@{
    virtual MIL_Automate& InstanciateAutomate( uint nID, MIL_Formation& formation, MIL_InputArchive& archive ) const;
    //@}

    //! @name Main
    //@{
    static const MIL_AutomateType* Create( const std::string& strName, MIL_InputArchive& archive );
    //@}
};

#include "MIL_AutomateTypeLOG.inl"

#endif // __MIL_AutomateTypeLOG_h_
