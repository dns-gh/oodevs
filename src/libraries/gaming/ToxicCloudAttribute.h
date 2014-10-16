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

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include <tools/Resolver_ABC.h>
#include <geometry/Types.h>
#include <limits>

namespace sword
{
    class ObjectUpdate;
    class ObjectCreation;
    class LocatedQuantityList;
}

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
                          , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            ToxicCloudAttribute( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ToxicCloudAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ToxicCloudAttribute( const ToxicCloudAttribute& );            //!< Copy constructor
    ToxicCloudAttribute& operator=( const ToxicCloudAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    void UpdateToxicCloud( const sword::LocatedQuantityList& cloud );
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
            if( first > quantity )
                first = quantity;
            if( second < quantity )
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
