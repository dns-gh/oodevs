// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-22 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Composantes/PHY_ComposanteHLA.h $
// $Author: Age $
// $Modtime: 13/12/04 10:34 $
// $Revision: 3 $
// $Workfile: PHY_ComposanteHLA.h $
//
// *****************************************************************************

#ifndef __PHY_ComposanteHLA_h_
#define __PHY_ComposanteHLA_h_

#include "PHY_Composante_ABC.h"

// =============================================================================
/** @class  PHY_ComposanteHLA
    @brief  PHY composante HLA
*/
// Created: AGE 2004-11-22
// =============================================================================
class PHY_ComposanteHLA : public PHY_Composante_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_ComposanteHLA( const PHY_ComposanteType_ABC& type, unsigned int nElements );
    virtual ~PHY_ComposanteHLA();
    //@}

    //! @name Operations
    //@{
    void Update( unsigned int nElements );
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_ComposanteType_ABC& GetType            () const;
    virtual bool                          CanFire            () const;
    virtual bool                          CanFireWhenUnloaded() const;
    virtual unsigned int                          GetMajorScore      () const;
    virtual float                GetIdentificationMaxRange  () const { return 0; };
    virtual float                GetReconnoissanceMaxRange  () const { return 0; };

    bool IsActive() const; //!< Test whether the composante should be taken into account in various algorithms

    unsigned int CurrentElements() const;
    unsigned int MaxElements();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_ComposanteHLA( const PHY_ComposanteHLA& );            //!< Copy constructor
    PHY_ComposanteHLA& operator=( const PHY_ComposanteHLA& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const PHY_ComposanteType_ABC& type_;
    unsigned int nCurrentElements_;
    unsigned int nMaxElements_;
    //@}
};

#endif // __PHY_ComposanteHLA_h_
