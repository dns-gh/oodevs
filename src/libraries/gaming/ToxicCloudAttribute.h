// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ToxicCloudAttribute_h_
#define __ToxicCloudAttribute_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "tools/Resolver_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "geometry/Types.h"
#include <limits>

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  ToxicCloudAttribute
    @brief  ToxicCloudAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class ToxicCloudAttribute : public kernel::ToxicCloudAttribute_ABC                        
                         , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                         , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
                         , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
            ToxicCloudAttribute( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ToxicCloudAttribute();
    //@}

    //! @name Operations
    //@{    
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ToxicCloudAttribute( const ToxicCloudAttribute& );            //!< Copy constructor
    ToxicCloudAttribute& operator=( const ToxicCloudAttribute& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{    
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    void UpdateToxicCloud( const ASN1T_LocatedQuantityList& cloud );
    void CreateTexture();
    void UpdateTexture() const;
    //@}

private:
    //! @name 
    //@{
    struct QuantityBoundaries : public std::pair< double, double >
    {
        QuantityBoundaries()
            : std::pair< double, double >( std::numeric_limits< double >::max(), std::numeric_limits< double >::min() )
        {
            // NOTHING
        }

        void Incorporate( double quantity )
        {
            if ( first > quantity )
                first = quantity;
            if ( second < quantity )
                second = quantity;
        }
    };
    
    typedef std::vector< std::pair< geometry::Point2f, double > >   T_QuantityCloud;
    typedef T_QuantityCloud::const_iterator                         CIT_QuantityCloud;
    //@}



public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Rectangle2f   boundingBox_;
    QuantityBoundaries      boundaries_;
    T_QuantityCloud         cloud_;
    unsigned int            texture_;
    //@}
};

#endif // __ToxicCloudAttribute_h_
