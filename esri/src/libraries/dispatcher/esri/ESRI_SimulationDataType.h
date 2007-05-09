// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EsriSimulationDataType_h_
#define __EsriSimulationDataType_h_

// =============================================================================
/** @class  EsriSimulationDataType
    @brief  EsriSimulationDataType    
*/
// Created: JCR 2007-05-02
// =============================================================================
class EsriSimulationDataType
{

public:
    //! @name Constructors/Destructor
    //@{
             EsriSimulationDataType();
    virtual ~EsriSimulationDataType();
    //@}


private:
    //! @name Copy/Assignement
    //@{
    EsriSimulationDataType( const EsriSimulationDataType& );            //!< Copy constructor
    EsriSimulationDataType& operator=( const EsriSimulationDataType& ); //!< Assignement operator
    //@}
    
private:
    //! @name Member data
    //@{
    //@}
};

#endif // __EsriSimulationDataType_h_
