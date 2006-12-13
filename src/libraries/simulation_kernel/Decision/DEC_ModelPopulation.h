// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_ModelPopulation.h $
// $Author: Mco $
// $Modtime: 11/02/05 16:11 $
// $Revision: 2 $
// $Workfile: DEC_ModelPopulation.h $
//
// *****************************************************************************

#ifndef __DEC_ModelPopulation_h_
#define __DEC_ModelPopulation_h_

#include "MIL.h"

#include "DEC_Model_ABC.h"

class MIL_PopulationMissionType;

// =============================================================================
// Created: NLD 2003-11-24
// =============================================================================
class DEC_ModelPopulation : public DEC_Model_ABC< MIL_PopulationMissionType >
{
    MT_COPYNOTALLOWED( DEC_ModelPopulation );

public:
    DEC_ModelPopulation( const DEC_Workspace& decWorkspace, const std::string& strName, MIL_InputArchive& input, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strArchivePath, const std::string& strSourcePath );
    virtual ~DEC_ModelPopulation();
};

#endif // __DEC_ModelPopulation_h_
