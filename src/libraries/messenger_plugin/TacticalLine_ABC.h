// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "Entity_ABC.h"
#include "game_asn/Messenger.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream ;
    class xostream ;
}

namespace messenger
{

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  TacticalLine_ABC
*/
// Created: SBO 2006-11-15
// =============================================================================
class TacticalLine_ABC : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( unsigned int id, const ASN1T_TacticalLine& asn );
             TacticalLine_ABC( unsigned int id, xml::xistream&, const ASN1T_TacticalLinesDiffusion& diffusion, const kernel::CoordinateConverter_ABC& converter );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Network
    //@{
    void Send( ASN1T_TacticalLine& asn ) const;
    void Send( ASN1T_Line& asn ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned int                         GetID() const;
    const ASN1T_TacticalLinesDiffusion&  GetDiffusion() const { return diffusion_; }
    //@}

protected:

    //! @name types
    //@{
    typedef std::vector< ASN1T_CoordLatLong > T_PositionVector;
    typedef T_PositionVector::const_iterator  CIT_PositionVector;
    //@}

    //! @name Network
    //@{
    void Update        ( const ASN1T_TacticalLine& asn );
    void UpdateGeometry( const ASN1T_Location& asn );
    //@}

    //! @name xml read / write
    //@{
            void ReadPoint ( xml::xistream& xis, T_PositionVector& points, const kernel::CoordinateConverter_ABC& converter ) const;
    virtual void Write     ( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const;
            void WritePoint( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter, const ASN1T_CoordLatLong& point ) const;
    //@}

private:

    //! @name Copy/Assignement
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );            //!< Copy constructor
    TacticalLine_ABC& operator=( const TacticalLine_ABC& ); //!< Assignement operator
    //@}

    //! @name Member data
    //@{
    const unsigned int                   id_;
          std::string                    strName_;
          T_PositionVector               geometry_;
          ASN1T_TacticalLinesDiffusion   diffusion_ ;
    //@}


};

}

#endif // __TacticalLine_ABC_h_
