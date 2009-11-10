// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/MIL_AutomateTypeInjuredHuman.h $
// $Author: Rft $
// $Modtime: 27/12/04 16:34 $
// $Revision: 1 $
// $Workfile: MIL_AutomateTypeInjuredHuman.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateTypeInjuredHuman_h_
#define __MIL_AutomateTypeInjuredHuman_h_

#include "MIL.h"

#include "Entities/Automates/MIL_AutomateType.h"

namespace xml
{
    class xistream;
}

class AlgorithmsFactories;

// =============================================================================
// @class  MIL_AutomateType
// Created: RFT 2004-08-03
// =============================================================================
class MIL_AutomateTypeInjuredHuman : public MIL_AutomateType
{
    MT_COPYNOTALLOWED( MIL_AutomateTypeInjuredHuman )

public:
    MIL_AutomateTypeInjuredHuman( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AutomateTypeInjuredHuman();

    //! @name Accessors
    //@{
    virtual bool IsInjuredHuman() const;
    //@}

    //! @name Instanciation
    //@{
    virtual MIL_Automate& InstanciateAutomate( uint nID, MIL_Formation& parent, xml::xistream& xis, DEC_DataBase& database ) const;
    virtual MIL_Automate& InstanciateAutomate( uint nID, MIL_Automate&  parent, xml::xistream& xis, DEC_DataBase& database ) const;
    //@}

    //! @name Main
    //@{
    static const MIL_AutomateType* Create( const std::string& strName, xml::xistream& xis );
    //@}
};

#endif // __MIL_AutomateTypeInjuredHuman_h_
