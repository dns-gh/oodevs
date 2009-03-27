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
#include "pathfind/SpatialContainer.h"
#include <boost/shared_ptr.hpp>

class QuantityTraits;
class MIL_NbcAgentType;

// =============================================================================
/** @class  InputToxicCloudAttribute
    @brief  InputToxicCloudAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class InputToxicCloudAttribute 
    : public ToxicAttribute_ABC
    , public UpdatableAttribute_ABC
{ 
public:
    //! @name 
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
            if ( lhs < rhs )
                return -1;
            if ( lhs > rhs )
                return 1;
            return 0;
        }
    };

    typedef pathfind::SpatialContainer< T_Quantity, QuantityTraits, double > T_Quantities; 
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
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name 
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name 
    //@{
    MIL_ToxicEffectManipulator GetContaminationEffect( const NBCAttribute& nbc, const MT_Vector2D& position ) const;    
    //@}

    //! @name 
    //@{    
    bool Update( uint time, TER_Polygon& shape );
    //@}

    //! @name Operations
    //@{
    InputToxicCloudAttribute& operator=( const InputToxicCloudAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, std::string >           T_Schedule;
    typedef T_Schedule::const_iterator              CIT_Schedule;
    
    typedef std::vector< T_Quantity >               T_QuantityContainer;
    typedef T_QuantityContainer::const_iterator     CIT_QuantityContainer;
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
    std::string     filename_;
    std::string     field_;
    std::string     dataField_;
    bool            bExport_;
    boost::shared_ptr< T_Schedule >     schedule_;
    geometry::Rectangle2d               extent_;
    boost::shared_ptr< T_Quantities >   quantities_;
    T_QuantityContainer                 export_;    
    //@}
};

#endif // __InputToxicCloudAttribute_h_
