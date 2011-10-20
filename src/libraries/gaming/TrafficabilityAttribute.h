// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TrafficabilityAttribute_h_
#define __TrafficabilityAttribute_h_

#include "clients_kernel/ObjectExtensions.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  TrafficabilityAttribute
    @brief  TrafficabilityAttribute
*/
// Created: CMA 2011-09-06
// =============================================================================
class TrafficabilityAttribute : public kernel::TrafficabilityAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TrafficabilityAttribute( kernel::Controller& controller );
    virtual ~TrafficabilityAttribute();
    //@}

    //! @name Accessors
    //@{
    double GetMaxValue() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TrafficabilityAttribute( const TrafficabilityAttribute& );            //!< Copy constructor
    TrafficabilityAttribute& operator=( const TrafficabilityAttribute& ); //!< Assignment operator
    //@}

    //! @name Updatable_ABC
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    double max_;
    //@}
};

#endif // __TrafficabilityAttribute_h_
