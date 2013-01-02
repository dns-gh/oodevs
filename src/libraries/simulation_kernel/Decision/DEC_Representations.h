// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Representations_h_
#define __DEC_Representations_h_

#include "MT_Tools/Role_ABC.h"
#include <boost/serialization/access.hpp>

class MIL_FragOrder;
class DEC_PathPoint;

// =============================================================================
/** @class  DEC_Representations
    @brief  DEC_Representations
*/
// Created: LDC 2009-04-03
// =============================================================================
class DEC_Representations : public tools::Role_ABC
{
public:
    typedef DEC_Representations RoleInterface;

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Representations();
    virtual ~DEC_Representations();
    //@}

    //! @name Operations
    //@{
    const std::vector< boost::shared_ptr< MIL_FragOrder > >& GetOrdersCategory();
    void AddToOrdersCategory( boost::shared_ptr< MIL_FragOrder > pObject );
    void RemoveFromOrdersCategory( boost::shared_ptr< MIL_FragOrder > pObject );
    void DeleteOrderRepresentation( boost::shared_ptr< MIL_FragOrder > pObject ); // $$$$ _RC_ SLI 2012-12-13: same as RemoveFromOrdersCategory, delete it?

    const std::vector< boost::shared_ptr< DEC_PathPoint > >& GetPointsCategory();
    void AddToPointsCategory( boost::shared_ptr< DEC_PathPoint > pObject );
    void RemoveFromPointsCategory( boost::shared_ptr< DEC_PathPoint > pObject );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Representations( const DEC_Representations& );            //!< Copy constructor
    DEC_Representations& operator=( const DEC_Representations& ); //!< Assignment operator
    //@}

    //! @name Serialization
    //@{
    friend class boost::serialization::access;
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< tools::Role_ABC >( *this );
    }
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< MIL_FragOrder > > T_OrdersRepresentationVector;
    typedef std::vector< boost::shared_ptr< DEC_PathPoint > > T_PointsRepresentationVector;
    //@}

private:
    //! @name Member data
    //@{
    T_OrdersRepresentationVector orderRepresentations_;
    T_PointsRepresentationVector pointRepresentations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Representations )

#endif // __DEC_Representations_h_
