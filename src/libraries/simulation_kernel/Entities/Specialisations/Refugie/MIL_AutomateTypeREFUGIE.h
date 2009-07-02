// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Refugie/MIL_AutomateTypeREFUGIE.h $
// $Author: Nld $
// $Modtime: 10/03/05 16:07 $
// $Revision: 1 $
// $Workfile: MIL_AutomateTypeREFUGIE.h $
//
// *****************************************************************************

#ifndef __MIL_AutomateTypeREFUGIE_h_
#define __MIL_AutomateTypeREFUGIE_h_

#include "Entities/Automates/MIL_AutomateType.h"

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  MIL_AutomateType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AutomateTypeREFUGIE : public MIL_AutomateType
{

public:
             MIL_AutomateTypeREFUGIE( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_AutomateTypeREFUGIE();

    //! @name Accessors
    //@{
    virtual bool IsRefugee() const;
    //@}

    //! @name Main
    //@{
    static const MIL_AutomateType* Create( const std::string& strName, xml::xistream& xis );
    //@}
};

#endif // __MIL_AutomateTypeREFUGIE_h_
