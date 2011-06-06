// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.h $
// $Author: RFT $
// $Modtime: 2/05/05 11:39 $
// $Revision: 4 $
// $Workfile: DEC_FireFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_MedicalTreatmentFunctions_h_
#define __DEC_MedicalTreatmentFunctions_h_

#include "Knowledge/DEC_Knowledge_Object.h"

class DEC_Decision_ABC;
class MIL_AgentPion;

// =============================================================================
// Created: RFT 2004-03-31
// =============================================================================
class DEC_MedicalTreatmentFunctions
{
public:
    //! @name Functions
    //@{
    static void                                      TakeCareOfThePatient ( const MIL_Agent_ABC&, const DEC_Decision_ABC* agent, boost::shared_ptr< DEC_Knowledge_Object > object );
    static boost::shared_ptr< DEC_Knowledge_Object > DetermineHospital    ( const MIL_Agent_ABC& caller, const DEC_Decision_ABC* agent, std::vector< boost::shared_ptr< DEC_Knowledge_Object > > knowledges  );
    static bool                                      CanHospitalTreatWound( const MIL_Agent_ABC&, const DEC_Decision_ABC* agent, boost::shared_ptr< DEC_Knowledge_Object > object );
    //@}
};

#endif // __DEC_MedicalTreatmentFunctions_h_
