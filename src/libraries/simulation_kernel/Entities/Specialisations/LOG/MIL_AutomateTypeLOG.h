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

#include "Entities/Automates/MIL_AutomateType.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AutomateType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateTypeLOG : public MIL_AutomateType
{

public:
             MIL_AutomateTypeLOG( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AutomateTypeLOG();

    //! @name Accessors
    //@{
    virtual bool IsLogistic() const;
    //@}

    //! @name Instanciation
    //@{
    virtual MIL_Automate& InstanciateAutomate( uint nID, MIL_Formation& parent, xml::xistream& xis, DEC_DataBase& database ) const;
    virtual MIL_Automate& InstanciateAutomate( uint nID, MIL_Automate&  parent, xml::xistream& xis, DEC_DataBase& database ) const;
    //@}

    //! @name Instanciation
    //@{
    virtual void RegisterFunctions( directia::Brain& brain, MIL_Automate& automat ) const;
    //@}

    //! @name Main
    //@{
    static const MIL_AutomateType* Create( const std::string& strName, xml::xistream& xis );
    //@}
};

#endif // __MIL_AutomateTypeLOG_h_
