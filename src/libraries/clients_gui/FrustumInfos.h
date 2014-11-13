// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef FrustumInfos_h
#define FrustumInfos_h

#include "Tools.h"
#include <boost/optional.hpp>

namespace gui 
{

struct Frustum2D
{
    Frustum2D( geometry::Point2f center,
               float zoom )
        : center_( center )
        , zoom_( zoom )
    {
        // NOTHING
    }
    geometry::Point2f center_;
    float zoom_;
};

struct Frustum3D
{
    Frustum3D( geometry::Point3f center,
               geometry::Vector3f front,
               geometry::Vector3f right,
               geometry::Vector3f top )
        : center_( center )
        , front_( front )
        , right_( right )
        , top_( top )
    {
        // NOTHING
    }
    geometry::Point3f center_;
    geometry::Vector3f front_;
    geometry::Vector3f right_;
    geometry::Vector3f top_;
};

struct FrustumInfos
{
    //! @name Constructors/Destructor
    //@{
    FrustumInfos( const geometry::Point2f& center,
                  float zoom )
        : infos2D_( Frustum2D( center, zoom ) )
    {
        // NOTHING
    }
    FrustumInfos( const geometry::Point3f& center,
                  const geometry::Vector3f& front,
                  const geometry::Vector3f& right,
                  const geometry::Vector3f& top )
        : infos3D_( Frustum3D( center, front, right, top ) )
    {
        // NOTHING
    }
    FrustumInfos( QSettings& settings )
    {
        Load( settings );
    }
    //@}

    //! @name Operations
    //@{
    void Save( QSettings& settings ) const
    {
        if( infos2D_ )
        {
            settings.beginGroup( "2D" );
            settings.setValue( "Center", tools::Point2fToString( infos2D_->center_ ) );
            settings.setValue( "Zoom", infos2D_->zoom_ );
            settings.endGroup();
        }
        else if( infos3D_ )
        {
            settings.beginGroup( "3D" );
            settings.setValue( "Center", tools::Point3fToString( infos3D_->center_ ) );
            settings.setValue( "Front", tools::Vector3fToString( infos3D_->front_ ) );
            settings.setValue( "Right", tools::Vector3fToString( infos3D_->right_ ) );
            settings.setValue( "Top", tools::Vector3fToString( infos3D_->top_ ) );
            settings.endGroup();
        }
    }
    //@}

private:
    //! @name Helpers
    //@{
    void Load( QSettings& settings )
    {
        infos2D_ = boost::none;
        infos3D_ = boost::none;
        if( settings.contains( "2D/Center" ) )
        {
            settings.beginGroup( "2D" );
            infos2D_ = Frustum2D( tools::StringToPoint2f( settings.value( "Center" ).toString() ),
                                  settings.value( "Zoom" ).toFloat() );
            settings.endGroup();
        }
        else if( settings.contains( "3D/Center" ) )
        {
            settings.beginGroup( "3D" );
            infos3D_ = Frustum3D( tools::StringToPoint3f( settings.value( "Center" ).toString() ),
                                  tools::StringToVector3f( settings.value( "Front" ).toString() ),
                                  tools::StringToVector3f( settings.value( "Right" ).toString() ),
                                  tools::StringToVector3f( settings.value( "Top" ).toString() ) );
            settings.endGroup();
        }
    }
    //@}

public:
    //! @name Member data
    //@{
    boost::optional< Frustum2D > infos2D_;
    boost::optional< Frustum3D > infos3D_;
    //@}
};

} //! namespace gui

#endif // FrustumInfos_h
