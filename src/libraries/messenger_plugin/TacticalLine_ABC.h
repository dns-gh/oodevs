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
#include "protocol/protocol.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace plugins
{
namespace messenger
{
// =============================================================================
/** @class  TacticalLine_ABC
    @brief  Tactical line declaration
*/
// Created: SBO 2006-11-15
// =============================================================================
class TacticalLine_ABC : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( unsigned int id, const Common::MsgTacticalLine& asn );
             TacticalLine_ABC( unsigned int id, xml::xistream&, const Common::MsgTacticalLine_Diffusion& diffusion, const kernel::CoordinateConverter_ABC& converter );
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Network
    //@{
    void Send( Common::MsgTacticalLine& asn ) const;
    void Send( Common::MsgLocation& asn ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    const Common::MsgTacticalLine_Diffusion&  GetDiffusion() const { return diffusion_; }
    //@}

protected:

    //! @name types
    //@{
    typedef std::vector< Common::MsgCoordLatLong > T_PositionVector;
    typedef T_PositionVector::const_iterator     CIT_PositionVector;
    //@}

    //! @name Network
    //@{
    void Update        ( const Common::MsgTacticalLine& asn );
    void UpdateGeometry( const Common::MsgLocation& asn );
    //@}

    //! @name xml read / write
    //@{
            void ReadPoint ( xml::xistream& xis, T_PositionVector& points, const kernel::CoordinateConverter_ABC& converter ) const;
    virtual void Write     ( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const;
            void WritePoint( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter, const Common::MsgCoordLatLong& point ) const;
    //@}

private:

    //! @name Copy/Assignment
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );            //!< Copy constructor
    TacticalLine_ABC& operator=( const TacticalLine_ABC& ); //!< Assignment operator
    //@}

    //! @name Member data
    //@{
    const unsigned int id_;
    std::string strName_;
    T_PositionVector geometry_;
    Common::MsgTacticalLine_Diffusion diffusion_;
    //@}


};

}
}

#endif // __TacticalLine_ABC_h_
