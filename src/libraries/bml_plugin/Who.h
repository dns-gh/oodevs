// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Who_h_
#define __Who_h_

namespace sword
{
    class UnitAttributes;
}

namespace xml
{
    class xostream;
}

namespace dispatcher
{
    class Agent_ABC;
    class Automat_ABC;
}

namespace plugins
{
namespace bml
{

// =============================================================================
/** @class  Who
    @brief  Who
*/
// Created: SBO 2008-05-22
// =============================================================================
class Who
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Who( const dispatcher::Agent_ABC& entity );
    explicit Who( const dispatcher::Automat_ABC& entity );
             Who( const dispatcher::Agent_ABC& entity, const sword::UnitAttributes& attributes );
             Who( const dispatcher::Agent_ABC& entity, int detectionLevel );
    virtual ~Who();
    //@}

    //! @name Operations
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const Who& who );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Who( const Who& );            //!< Copy constructor
    Who& operator=( const Who& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SendEquipmentStatus( xml::xostream& xos ) const;
    void SendStatus( xml::xostream& xos ) const;

    std::string GetFilterHostility() const;
    std::string GetFilterOperationalState() const;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent_ABC* agent_;
    const dispatcher::Automat_ABC* automat_;
    const sword::UnitAttributes* attributes_;
    int level_;
    //@}
};

}
}

#endif // __Who_h_
