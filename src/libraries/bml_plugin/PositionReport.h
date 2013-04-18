// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PositionReport_h_
#define __PositionReport_h_

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
}

namespace plugins
{
namespace bml
{
    class Publisher_ABC;

// =============================================================================
/** @class  PositionReport
    @brief  PositionReport
    // $$$$ SBO 2008-07-15: Rename, StatusReport
*/
// Created: SBO 2008-05-22
// =============================================================================
class PositionReport
{
public:
    //! @name Constructors/Destructor
    //@{
             PositionReport( const dispatcher::Agent_ABC& entity, const sword::UnitAttributes& attributes );
    virtual ~PositionReport();
    //@}

    //! @name Operations
    //@{
    void Send( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PositionReport( const PositionReport& );            //!< Copy constructor
    PositionReport& operator=( const PositionReport& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SendEquipmentStatus( xml::xostream& xos ) const;
    void SendStatus( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent_ABC& entity_;
    const sword::UnitAttributes& attributes_;
    //@}
};

}
}

#endif // __PositionReport_h_