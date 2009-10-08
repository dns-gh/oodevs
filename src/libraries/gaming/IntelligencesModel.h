// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligencesModel_h_
#define __IntelligencesModel_h_

#include "IntelligenceFactory.h"
#include "tools/Resolver.h"
#include "clients_kernel/Intelligence_ABC.h"

class IntelligenceFactory;

// =============================================================================
/** @class  IntelligencesModel
    @brief  IntelligencesModel
*/
// Created: SBO 2007-10-17
// =============================================================================
class IntelligencesModel : public tools::Resolver< kernel::Intelligence_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IntelligencesModel( IntelligenceFactory& factory );
    virtual ~IntelligencesModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Create( const ASN1T_MsgIntelligenceCreation& message );
    void Delete( const ASN1T_MsgIntelligenceDestruction& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligencesModel( const IntelligencesModel& );            //!< Copy constructor
    IntelligencesModel& operator=( const IntelligencesModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    IntelligenceFactory& factory_;
    //@}
};

#endif // __IntelligencesModel_h_
