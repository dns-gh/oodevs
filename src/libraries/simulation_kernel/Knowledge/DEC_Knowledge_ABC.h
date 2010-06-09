// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/knowledge/DEC_Knowledge_ABC.h $
// $Author: Jvt $
// $Modtime: 25/03/05 13:13 $
// $Revision: 2 $
// $Workfile: DEC_Knowledge_ABC.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ABC_h_
#define __DEC_Knowledge_ABC_h_

#include <boost/serialization/export.hpp>

// =============================================================================
/** @class  DEC_Knowledge_ABC
    @brief  The base class for a knowledge
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ABC();
    virtual ~DEC_Knowledge_ABC();
    //@}
    
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive& /*file*/, const unsigned int /*version*/ )
    {
        //NOTHING
    }
    //@}
    
protected:
    //! @name Tools
    //@{
    unsigned int GetCurrentTimeStep() const; 
    //@}

};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ABC )

#endif // __DEC_Knowledge_ABC_h_
