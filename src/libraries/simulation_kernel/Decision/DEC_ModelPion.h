// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_ModelPion.h $
// $Author: Mco $
// $Modtime: 11/02/05 16:14 $
// $Revision: 2 $
// $Workfile: DEC_ModelPion.h $
//
// *****************************************************************************

#ifndef __DEC_ModelPion_h_
#define __DEC_ModelPion_h_

#include "MIL.h"

#include "DEC_Model_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_PionMissionType;

// =============================================================================
// Created: NLD 2003-11-24
// =============================================================================
class DEC_ModelPion : public DEC_Model_ABC< MIL_PionMissionType >
{
    MT_COPYNOTALLOWED( DEC_ModelPion );

public:
    DEC_ModelPion( const DEC_Workspace& decWorkspace, const std::string& strName, xml::xistream& xis, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strArchivePath );
    virtual ~DEC_ModelPion();
};

#endif // __DEC_ModelPion_h_
