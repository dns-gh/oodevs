// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_ModelAutomate.h $
// $Author: Mco $
// $Modtime: 11/02/05 16:14 $
// $Revision: 2 $
// $Workfile: DEC_ModelAutomate.h $
//
// *****************************************************************************

#ifndef __DEC_ModelAutomate_h_
#define __DEC_ModelAutomate_h_

#include "MIL.h"

#include "DEC_Model_ABC.h"

class MIL_AutomateMissionType;

// =============================================================================
// Created: NLD 2003-11-24
// =============================================================================
class DEC_ModelAutomate : public DEC_Model_ABC< MIL_AutomateMissionType >
{
    MT_COPYNOTALLOWED( DEC_ModelAutomate );

public:
    DEC_ModelAutomate( const DEC_Workspace& decWorkspace, const std::string& strName, MIL_InputArchive& input, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strArchivePath, const std::string& strSourcePath );
    virtual ~DEC_ModelAutomate();
};

#endif // __DEC_ModelAutomate_h_
