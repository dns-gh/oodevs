// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalStates_h_
#define __MedicalStates_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Controller;

// =============================================================================
/** @class  MedicalStates
    @brief  MedicalStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class MedicalStates : public Extension_ABC
                    , public Updatable_ABC< ASN1T_MsgLogSanteEtat >
{

public:
    //! @name Constructors/Destructor
    //@{
             MedicalStates( Controller& controller );
    virtual ~MedicalStates();
    //@}

    //! @name Operations
    //@{
    
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MedicalStates( const MedicalStates& );            //!< Copy constructor
    MedicalStates& operator=( const MedicalStates& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgLogSanteEtat& message );
    //@}

    //! @name Types
    //@{
    // $$$$ AGE 2006-02-14: resolve
    typedef std::vector< unsigned >                        T_Priorities;
    typedef std::vector< std::pair< unsigned, unsigned > > T_LogisticAvailabilities;
    //@}

private:
    //! @name Member data
    //@{
    Controller&              controller_;

    bool                     bChainEnabled_;
    unsigned                 nTempsBordee_;
    T_LogisticAvailabilities dispoReleveAmbulances_;
    T_LogisticAvailabilities dispoRamassageAmbulances_;  
    T_LogisticAvailabilities dispoDoctors_;
    T_Priorities             priorities_;
    T_Priorities             tacticalPriorities_;
    //@}
};

#endif // __MedicalStates_h_
