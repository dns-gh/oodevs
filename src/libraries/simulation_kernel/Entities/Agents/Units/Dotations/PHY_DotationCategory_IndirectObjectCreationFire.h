// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PHY_DotationCategory_IndirectObjectCreationFire_h_
#define __PHY_DotationCategory_IndirectObjectCreationFire_h_

#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"

// =============================================================================
/** @class  PHY_DotationCategory_IndirectObjectCreationFire
    @brief  PHY_DotationCategory_IndirectObjectCreationFire
*/
// Created: LDC 2009-12-30
// =============================================================================
class PHY_DotationCategory_IndirectObjectCreationFire : public PHY_DotationCategory_IndirectFire_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_DotationCategory_IndirectObjectCreationFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                              unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange );
             PHY_DotationCategory_IndirectObjectCreationFire( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, const std::string& objectType,
                                                              unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange );
    virtual ~PHY_DotationCategory_IndirectObjectCreationFire();
    //@}

    static PHY_DotationCategory_IndirectFire_ABC& Create( const PHY_IndirectFireDotationClass& type, const PHY_DotationCategory& dotationCategory, xml::xistream& xis,
                                                          unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange );

    //! @name Operations
    //@{
    virtual void ApplyEffect( const MIL_Agent_ABC* pFirer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, double rInterventionTypeFired, PHY_FireResults_ABC& fireResult ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_DotationCategory_IndirectObjectCreationFire( const PHY_DotationCategory_IndirectObjectCreationFire& );            //!< Copy constructor
    PHY_DotationCategory_IndirectObjectCreationFire& operator=( const PHY_DotationCategory_IndirectObjectCreationFire& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string objectType_;
    int   nLifeDuration_;
    //@}
};

#endif // __PHY_DotationCategory_IndirectObjectCreationFire_h_
