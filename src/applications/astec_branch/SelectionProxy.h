// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SelectionProxy_h_
#define __SelectionProxy_h_

class SelectionLayer_ABC;

// =============================================================================
/** @class  SelectionProxy
    @brief  Selection proxy
*/
// Created: SBO 2006-03-16
// =============================================================================
class SelectionProxy
{

public:
    //! @name Constructors/Destructor
    //@{
             SelectionProxy(); // $$$$ SBO 2006-03-16: SelectionPolicy?
    virtual ~SelectionProxy();
    //@}

    //! @name Operations
    //@{
    void Register( SelectionLayer_ABC& layer );
    void Remove  ( SelectionLayer_ABC& layer );

    void NotifyFocusLost( SelectionLayer_ABC& layer );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SelectionProxy( const SelectionProxy& );            //!< Copy constructor
    SelectionProxy& operator=( const SelectionProxy& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< SelectionLayer_ABC* > T_Layers;
    typedef T_Layers::iterator                IT_Layers;
    //@}

private:
    //! @name Member data
    //@{
    T_Layers  layers_;
    unsigned  focused_;
    //@}
};

#endif // __SelectionProxy_h_
