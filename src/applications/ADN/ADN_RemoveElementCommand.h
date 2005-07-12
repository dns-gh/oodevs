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
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_RemoveElementCommand.h $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_RemoveElementCommand.h $
//
// *****************************************************************************

#ifndef __ADN_RemoveElementCommand_h_
#define __ADN_RemoveElementCommand_h_

#include "qtundo.h"
#include "ADN_Type_Vector_ABC.h"

// =============================================================================
/** @class  ADN_RemoveElementCommand
    @brief  ADN_RemoveElementCommand
    @par    Using example
    @code
    ADN_RemoveElementCommand;
    @endcode
*/
// Created: AGN 2004-05-13
// =============================================================================
template< typename ObjectType >
class ADN_RemoveElementCommand
: public QtCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_RemoveElementCommand( ADN_Type_Vector_ABC< ObjectType >& modifiedData, ObjectType* pRemovedObject );
    virtual ~ADN_RemoveElementCommand();
    //@}

    //! @name Operations
    //@{
    virtual void undo();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_RemoveElementCommand( const ADN_RemoveElementCommand& );            //!< Copy constructor
    ADN_RemoveElementCommand& operator=( const ADN_RemoveElementCommand& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Type_Vector_ABC< ObjectType >& modifiedData_;
    ObjectType* pRemovedObject_;
    //@}
};

#include "ADN_Type_ABC.h"

// =============================================================================
// Specialization for ADN_Type_ABC< std::string >
// =============================================================================
template<>
class ADN_RemoveElementCommand< ADN_Type_ABC< std::string > >
: public QtCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_RemoveElementCommand( ADN_Type_Vector_ABC< ADN_Type_ABC< std::string > >& modifiedData, ADN_Type_ABC< std::string >* pRemovedObject );
    virtual ~ADN_RemoveElementCommand();
    //@}

    //! @name Operations
    //@{
    virtual void undo();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_RemoveElementCommand( const ADN_RemoveElementCommand& );            //!< Copy constructor
    ADN_RemoveElementCommand& operator=( const ADN_RemoveElementCommand& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Type_Vector_ABC< ADN_Type_ABC< std::string > >& modifiedData_;
    ADN_Type_ABC< std::string >* pRemovedObject_;
    //@}
};

// =============================================================================
// Specialization for ADN_Type_ABC< double >
// =============================================================================
template<>
class ADN_RemoveElementCommand< ADN_Type_ABC< double > >
: public QtCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_RemoveElementCommand( ADN_Type_Vector_ABC< ADN_Type_ABC< double > >& modifiedData, ADN_Type_ABC< double >* pRemovedObject );
    virtual ~ADN_RemoveElementCommand();
    //@}

    //! @name Operations
    //@{
    virtual void undo();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_RemoveElementCommand( const ADN_RemoveElementCommand& );            //!< Copy constructor
    ADN_RemoveElementCommand& operator=( const ADN_RemoveElementCommand& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void UpdateDescription();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Type_Vector_ABC< ADN_Type_ABC< double > >& modifiedData_;
    ADN_Type_ABC< double >* pRemovedObject_;
    //@}
};

#include "ADN_RemoveElementCommand.inl"

#endif // __ADN_RemoveElementCommand_h_
