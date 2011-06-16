// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __InputToxicCloudAttribute_h_
#define __InputToxicCloudAttribute_h_

#include "MIL.h"
#include "ToxicAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <spatialcontainer/SpatialContainer.h>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

class QuantityTraits;
class MIL_NbcAgentType;
class TER_Polygon;

// =============================================================================
/** @class  InputToxicCloudAttribute
    @brief  InputToxicCloudAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class InputToxicCloudAttribute : public ToxicAttribute_ABC
                               , public UpdatableAttribute_ABC
{
public:
    //! @name Types
    //@{
    typedef std::pair< MT_Vector2D, double >  T_Quantity; // Lat/Long/quantity
    struct QuantityTraits
    {
        int CompareOnX( double value, const T_Quantity& quantity ) const
        {
            return Compare( quantity.first.rX_, value );
        }

        int CompareOnY( double value, const T_Quantity& quantity ) const
        {
            return Compare( quantity.first.rY_, value );
        }

        int Compare( double lhs, double rhs ) const
        {
            if( lhs < rhs )
                return -1;
            if( lhs > rhs )
                return 1;
            return 0;
        }
    };

    typedef spatialcontainer::SpatialContainer< T_Quantity, QuantityTraits, double > T_Quantities;
    //@}
public:
    //! @name Constructors/Destructor
    //@{
             InputToxicCloudAttribute();
    explicit InputToxicCloudAttribute( xml::xistream& xis );
    virtual ~InputToxicCloudAttribute();
    //@}

     //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void Register( MIL_Object_ABC& object ) const;
    void SendFullState( sword::ObjectAttributes& asn ) const;
    void SendUpdate( sword::ObjectAttributes& asn ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name
    //@{
    MIL_ToxicEffectManipulator GetContaminationEffect( const NBCAttribute& nbc, const MT_Vector2D& position ) const;
    //@}

    //! @name
    //@{
    bool Update( unsigned int time, TER_Polygon& shape );
    //@}

    //! @name Operations
    //@{
    InputToxicCloudAttribute& operator=( const InputToxicCloudAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, std::string > T_Schedule;
    typedef T_Schedule::const_iterator          CIT_Schedule;

    typedef std::vector< T_Quantity >             T_QuantityContainer;
    typedef T_QuantityContainer::const_iterator CIT_QuantityContainer;
    //@}

private:
    //! @name
    //@{
    void Insert( T_Quantity& quantity );
    void LoadConfig();
    void ReadFiles( xml::xistream& xis );
    //@}

    //! @name
    //@{
    void LoadShape( const std::string& name );
    void BuildConvexHull( TER_Polygon& polygon ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string filename_;
    std::string field_;
    std::string dataField_;
    bool bExport_;
    boost::shared_ptr< T_Schedule > schedule_;
    geometry::Rectangle2d extent_;
    boost::shared_ptr< T_Quantities > quantities_;
    T_QuantityContainer export_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( InputToxicCloudAttribute )

#endif // __InputToxicCloudAttribute_h_
