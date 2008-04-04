// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Intelligence_h_
#define __Intelligence_h_

#include "game_asn/Simulation.h"
#include "Entity_ABC.h"

namespace dispatcher
{
    class Formation;
    class Model;

// =============================================================================
/** @class  Intelligence
    @brief  Intelligence
*/
// Created: SBO 2007-10-22
// =============================================================================
class Intelligence : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Intelligence( Model& model, const ASN1T_MsgIntelligenceCreation& message );
    virtual ~Intelligence();
    //@}

    //! @name Operations
    //@{
    using Entity_ABC::Update;
    void Update( const ASN1T_MsgIntelligenceUpdate& message );
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Intelligence( const Intelligence& );            //!< Copy constructor
    Intelligence& operator=( const Intelligence& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Formation& formation_;
    unsigned int id_;
    std::string name_;
    std::string nature_;
    bool embarked_;
    ASN1T_EnumNatureLevel level_;
    ASN1T_EnumDiplomacy diplomacy_;
    ASN1T_CoordLatLong position_;
    //@}
};

}

#endif // __Intelligence_h_
