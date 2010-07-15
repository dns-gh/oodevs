// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DEC_DIA3Model_h_
#define __DEC_DIA3Model_h_

#include "DEC_Model_ABC.h"

class DEC_DIAWorkspace ;

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  DEC_Model
    @brief  DEC_Model
*/
// Created: RDS 2008-05-14
// =============================================================================
class DEC_Model : public DEC_Model_ABC
{
    MT_COPYNOTALLOWED( DEC_Model );

public:
             DEC_Model( const std::string& strName, xml::xistream& xis, const std::string& strSourcePath, const std::string& strPrefix, const std::map< std::string, const MIL_MissionType_ABC*, sCaseInsensitiveLess >& missionTypes );
    virtual ~DEC_Model();
};

#endif // __DEC_Model_h_
