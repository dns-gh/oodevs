// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterIntelligence_h_
#define __ActionParameterIntelligence_h_

#include "Entity.h"
#include "clients_kernel/Intelligence_ABC.h"

namespace sword
{
    class Intelligence;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
    class Formation_ABC;
    class FormationLevels;
    class Intelligence_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Intelligence
    @brief  Intelligence
*/
// Created: SBO 2007-10-23
// =============================================================================
class Intelligence : public Entity< kernel::Intelligence_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Intelligence( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller );
             Intelligence( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis, const kernel::EntityResolver_ABC& formations
                         , const kernel::FormationLevels& levels, kernel::Controller& controller );
             Intelligence( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter
                         , const kernel::EntityResolver_ABC& formations, const kernel::FormationLevels& levels
                         , const sword::Intelligence& message, kernel::Controller& controller );
    virtual ~Intelligence();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MsgMissionParameter& message ) const;
    virtual void CommitTo( sword::MsgMissionParameter_Value& message ) const;
    void CommitTo( sword::Intelligence& message ) const;
    void Clean( sword::Intelligence& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    void CommitToChildren();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Intelligence( const Intelligence& );            //!< Copy constructor
    Intelligence& operator=( const Intelligence& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, const kernel::FormationLevels& levels );
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

    }
}

#endif // __ActionParameterIntelligence_h_
