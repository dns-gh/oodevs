// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-13 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_AddElementCommand.h $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_AddElementCommand.h $
//
// *****************************************************************************

#ifndef __ADN_AddElementCommand_h_
#define __ADN_AddElementCommand_h_

#include "qtundo.h"
#include "ADN_Type_Vector_ABC.h"

// =============================================================================
/** @class  ADN_AddElementCommand
    @brief  ADN_AddElementCommand
    @par    Using example
    @code
    ADN_AddElementCommand;
    @endcode
*/
// Created: AGN 2004-05-13
// =============================================================================
template< class ObjectType >
class ADN_AddElementCommand
: public QtCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_AddElementCommand( ADN_Type_Vector_ABC< ObjectType >& modifiedData, ObjectType* pAddedObject );
    virtual ~ADN_AddElementCommand();
    //@}

    //! @name Operations
    //@{
    virtual void undo();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_AddElementCommand( const ADN_AddElementCommand& );            //!< Copy constructor
    ADN_AddElementCommand& operator=( const ADN_AddElementCommand& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Type_Vector_ABC< ObjectType >& modifiedData_;
    ObjectType* pAddedObject_;
    //@}
};

#include "ADN_Type_ABC.h"

// =============================================================================
// Specialization for ADN_Type_ABC< std::string >
// =============================================================================
template<>
class ADN_AddElementCommand< ADN_Type_ABC< std::string > >
: public QtCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_AddElementCommand( ADN_Type_Vector_ABC< ADN_Type_ABC< std::string > >& modifiedData, ADN_Type_ABC< std::string >* pAddedObject );
    virtual ~ADN_AddElementCommand();
    //@}

    //! @name Operations
    //@{
    virtual void undo();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_AddElementCommand( const ADN_AddElementCommand& );            //!< Copy constructor
    ADN_AddElementCommand& operator=( const ADN_AddElementCommand& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Type_Vector_ABC< ADN_Type_ABC< std::string > >& modifiedData_;
    ADN_Type_ABC< std::string >* pAddedObject_;
    //@}
};

// =============================================================================
// Specialization for ADN_Type_ABC< double >
// =============================================================================
template<>
class ADN_AddElementCommand< ADN_Type_ABC< double > >
: public QtCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_AddElementCommand( ADN_Type_Vector_ABC< ADN_Type_ABC< double > >& modifiedData, ADN_Type_ABC< double >* pAddedObject );
    virtual ~ADN_AddElementCommand();
    //@}

    //! @name Operations
    //@{
    virtual void undo();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_AddElementCommand( const ADN_AddElementCommand& );            //!< Copy constructor
    ADN_AddElementCommand& operator=( const ADN_AddElementCommand& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Type_Vector_ABC< ADN_Type_ABC< double > >& modifiedData_;
    ADN_Type_ABC< double >* pAddedObject_;
    //@}
};
#include "ADN_AddElementCommand.inl"

#endif // __ADN_AddElementCommand_h_
