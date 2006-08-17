// *****************************************************************************
//
// $Created: JVT 2005-04-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/CheckPoints/DIA_Serializer.h $
// $Author: Jvt $
// $Modtime: 6/04/05 16:39 $
// $Revision: 1 $
// $Workfile: DIA_Serializer.h $
//
// *****************************************************************************

#ifndef __DIA_Serializer_h_
#define __DIA_Serializer_h_

#include "MIL.h"

class DIA_Motivation_Part;

// =============================================================================
// Created: JVT 2005-04-06
// =============================================================================
class DIA_Serializer
{

public:
     DIA_Serializer( DIA_Motivation_Part& motivationPart );
    ~DIA_Serializer();
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    template< typename Archive > void load( Archive&, const uint );
    template< typename Archive > void save( Archive&, const uint ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DIA_Serializer( const DIA_Serializer& );            //!< Copy constructor
    DIA_Serializer& operator=( const DIA_Serializer& ); //!< Assignement operator
    //@}


private:
    DIA_Motivation_Part& motivationPart_;
};

#include "DIA_Serializer.inl"

#endif // __DIA_Serializer_h_
