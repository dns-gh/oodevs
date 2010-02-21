// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLocationList_h_
#define __ActionParameterLocationList_h_

#include "Location.h"
#include "Parameter.h"
#include "ParameterVisitor_ABC.h"

namespace Common
{
    class MsgLocationList;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  LocationList
    @brief  LocationList
*/
// Created: SBO 2007-04-20
// =============================================================================
class LocationList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationList( const kernel::OrderParameter& parameter );
             template< typename List >
             LocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const List& message );
             LocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Common::MsgLocationList& message );
             LocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~LocationList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void Clean( Common::MsgMissionParameter& message ) const;
    template< typename Message >
    void CommitTo( Message& message ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocationList( const LocationList& );            //!< Copy constructor
    LocationList& operator=( const LocationList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadLocation( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}

    //! @name Types
    //@{
    template< typename Message >
    struct MessageSerializer : public ParameterVisitor_ABC
    {
        explicit MessageSerializer( Message& message );
        virtual void Visit( const Location& param );
        Message* message_;
    };
    //@}
};

// -----------------------------------------------------------------------------
// Name: LocationList constructor
// Created: SBO 2009-10-30
// -----------------------------------------------------------------------------
template< typename List >
LocationList::LocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const List& message )
    : Parameter< QString >( parameter )
{
    for( int i = 0; i < message.elem_size(); ++i )
        AddParameter( *new Location( kernel::OrderParameter( tools::translate( "Parameter", "Location %1" ).arg( i ).ascii(), "location", false ), converter, message.elem( i ).location() ) );
}

// -----------------------------------------------------------------------------
// Name: LocationList::MessageSerializer constructor
// Created: FHD 2009-09-30
// -----------------------------------------------------------------------------
template< typename Message >
LocationList::MessageSerializer< Message >::MessageSerializer( Message& message )
    : message_( &message )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocationList::MessageSerializer::Visit
// Created: FHD 2009-09-30
// -----------------------------------------------------------------------------
template< typename Message >
void LocationList::MessageSerializer< Message >::Visit( const Location& param )
{
    param.CommitTo( *message_->add_elem()->mutable_location() );
}

// -----------------------------------------------------------------------------
// Name: LocationList::CommitTo
// Created: FHD 2009-09-30
// -----------------------------------------------------------------------------
template< typename Message >
void LocationList::CommitTo( Message& message ) const
{
    MessageSerializer< Message > serializer( message );
    Accept( serializer );
}

    }
}

#endif // __ActionParameterLocationList_h_
