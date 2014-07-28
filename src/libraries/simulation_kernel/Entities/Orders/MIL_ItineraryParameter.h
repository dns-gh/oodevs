// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ItineraryParameter_h_
#define __MIL_ItineraryParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_ItineraryParameter
    @brief  MIL_ItineraryParameter
*/
// =============================================================================
class MIL_ItineraryParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ItineraryParameter( const sword::Pathfind& );
    virtual ~MIL_ItineraryParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& path ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    virtual bool ToItinerary( sword::Pathfind& ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template< typename Archive >        void load( Archive&, const unsigned int );
    template< typename Archive >        void save( Archive&, const unsigned int ) const;
    template< typename Archive > friend void save_construct_data( Archive&, const MIL_ItineraryParameter*, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive&, MIL_ItineraryParameter*, const unsigned int /*version*/ );
    //@

private:
    //! @name Member data
    //@{
    sword::Pathfind message_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ItineraryParameter )

#endif // __MIL_ItineraryParameter_h_
