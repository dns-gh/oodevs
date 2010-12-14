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

#include "Entity_ABC.h"
#include "protocol/Protocol.h"

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace plugins
{
namespace messenger
{

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
             Intelligence( unsigned int id, const sword::IntelligenceCreationRequest& message );
             Intelligence( unsigned int id, xml::xistream&, const sword::FormationId& formation, const kernel::CoordinateConverter_ABC& converter );
    virtual ~Intelligence();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const  { return id_ ; } 
    const sword::FormationId& GetFormation() const { return formation_ ; } 
    //@}

    //! @name Network
    //@{
    void Update( const sword::IntelligenceUpdateRequest& message );
    virtual void SendCreation   ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendUpdate     ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendFullState  ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name xml read / write
    //@{
    virtual void Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const;
    //@}


private:
    //! @name Copy/Assignment
    //@{
    Intelligence( const Intelligence& );            //!< Copy constructor
    Intelligence& operator=( const Intelligence& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPosition ( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter ) ;
    void WritePosition( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const;
    //@}

private:
    //! @name Member data
    //@{
    sword::FormationId formation_;
    unsigned int id_;
    std::string name_;
    std::string nature_;
    bool embarked_;
    sword::EnumNatureLevel level_;
    sword::EnumDiplomacy diplomacy_;
    sword::MsgCoordLatLong position_;
    //@}
};

}
}

#endif // __Intelligence_h_
