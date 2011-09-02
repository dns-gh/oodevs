// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __rpr_Coordinates_h_
#define __rpr_Coordinates_h_

#include <string>

namespace rpr
{
// =============================================================================
/** @class  WorldLocation
    @brief  World location
*/
// Created: AGE 2008-02-21
// =============================================================================
class WorldLocation
{
public:
    //! @name Constructors/Destructor
    //@{
             WorldLocation();
             WorldLocation( const std::string& mgrs, float altitude );
             WorldLocation( double latitude, double longitude, float altitude );
    virtual ~WorldLocation();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << x_ << y_ << z_;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> x_ >> y_ >> z_;
    }
    //@}

    //! @name Accessors
    //@{
    double X() const { return x_; };
    double Y() const { return y_; };
    double Z() const { return z_; };
    double Latitude () const;
    double Longitude() const;
    double Altitude()  const;
    //@}

private:
    //! @name Member data
    //@{
    double x_, y_, z_;
    //@}
};

// =============================================================================
/** @class  VelocityVector
    @brief  Velocity vector
*/
// Created: AGE 2008-02-21
// =============================================================================
class VelocityVector
{
public:
    //! @name Constructors/Destructor
    //@{
             VelocityVector();
             VelocityVector( const WorldLocation& location, float speed, float heading );
    virtual ~VelocityVector();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << xv_ << yv_ << zv_;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
       archive >> xv_ >> yv_ >> zv_;
    }
    //@}

    //! @name Accessors
    //@{
    double X() const { return xn_; };
    double Y() const { return yn_; };
    double Z() const { return zn_; };
    double VX() const { return xv_; };
    double VY() const { return yv_; };
    double VZ() const { return zv_; };
    //@}

private:
    //! @name Member data
    //@{
    float xn_, yn_, zn_; // normalized
    float xv_, yv_, zv_;
    //@}
};

// =============================================================================
/** @class  Orientation
    @brief  Orientation
*/
// Created: AGE 2008-02-21
// =============================================================================
class Orientation
{
public:
    //! @name Constructors/Destructor
    //@{
             Orientation();
             Orientation( const WorldLocation& location, const VelocityVector& velocity );
    virtual ~Orientation();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << psi_ << theta_ << phi_;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> psi_ >> theta_ >> phi_;
    }
    //@}

    //! @name Accessors
    //@{
    double Psi() const   { return psi_; };
    double Theta() const { return theta_; };
    double Phi() const   { return phi_; };
    //@}

private:
    //! @name Member data
    //@{
    float psi_, theta_, phi_;
    //@}
};

}

#endif // __rpr_Coordinates_h_
