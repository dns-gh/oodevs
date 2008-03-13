// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_IntelligenceList_h_
#define __MissionParameter_IntelligenceList_h_

#include "MissionParameter_ABC.h"
#include "ModelsContainer.h"

namespace dispatcher
{
    class IntelligenceOrder;

// =============================================================================
/** @class  MissionParameter_IntelligenceList
    @brief  MissionParameter_IntelligenceList
*/
// Created: SBO 2008-03-04
// =============================================================================
class MissionParameter_IntelligenceList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_IntelligenceList( Model& model, const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_IntelligenceList();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_IntelligenceList( const MissionParameter_IntelligenceList& );            //!< Copy constructor
    MissionParameter_IntelligenceList& operator=( const MissionParameter_IntelligenceList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ModelsContainer< IntelligenceOrder >  intelligenceOrders_;
    //@}
};

}

#endif // __MissionParameter_IntelligenceList_h_
