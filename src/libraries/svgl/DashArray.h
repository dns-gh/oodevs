// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DashArray_h_
#define __DashArray_h_

#include "Property_ABC.h"

namespace svg
{

// =============================================================================
/** @class  DashArray
    @brief  DashArray
    // $$$$ AGE 2006-09-04: dashoffset...
*/
// Created: AGE 2006-09-04
// =============================================================================
class DashArray : public Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DashArray( const std::string& array );
    virtual ~DashArray();
    //@}

    //! @name Operations
    //@{
    virtual DashArray& Clone() const;
    virtual void ReachTop( const RenderingContext_ABC& );
    //@}

private:
    //! @name Helpers
    //@{
    void Parse( const std::string& array );
    void GenerateMask( const std::vector< float >& values );
    void SetBit( unsigned index );
    //@}

private:
    //! @name Member data
    //@{
    bool disable_; // $$$$ AGE 2006-09-04: Faire un DashArrayNone.
    bool pixels_;
    unsigned short pattern_;
    float bitLength_;
    //@}
};

}

#endif // __DashArray_h_
