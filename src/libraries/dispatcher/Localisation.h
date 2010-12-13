 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Localisation_h_
#define __Localisation_h_

namespace sword
{
    class MsgLocation;
    class MsgCoordLatLong;
    enum MsgLocation_Geometry;
}

namespace dispatcher
{
    class Automat;
    class Model;

// =============================================================================
/** @class  Localisation
    @brief  Localisation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Localisation
{
public:
    //! @name Constructors/Destructor
    //@{
             Localisation();
    explicit Localisation( const sword::MsgLocation& msg );
    virtual ~Localisation();
    //@}

    //! @name Main
    //@{
    void Update( const sword::MsgLocation& msg );
    void Send( sword::MsgLocation& msg ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< sword::MsgCoordLatLong > T_PositionVector;
    //@}

private:
    sword::MsgLocation_Geometry nType_;
    T_PositionVector points_;
};

}

#endif // __Localisation_h_
