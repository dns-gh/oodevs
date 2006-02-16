// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-05 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/InfoPanel_ABC.h $
// $Author: Age $
// $Modtime: 6/04/05 11:56 $
// $Revision: 2 $
// $Workfile: InfoPanel_ABC.h $
//
// *****************************************************************************

#ifndef __InfoPanel_ABC_h_
#define __InfoPanel_ABC_h_

// =============================================================================
/** @class  InfoPanel_ABC
    @brief  InfoPanel_ABC
*/
// Created: AGE 2005-04-05
// =============================================================================
class InfoPanel_ABC : public QScrollView
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit InfoPanel_ABC( QWidget* pParent );
    virtual ~InfoPanel_ABC();
    //@}

    virtual void insertChild ( QObject* pObj );
    virtual QLayout* layout();

private:
    //! @name Copy/Assignement
    //@{
    InfoPanel_ABC( const InfoPanel_ABC& );            //!< Copy constructor
    InfoPanel_ABC& operator=( const InfoPanel_ABC& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{
    QVBox* pBox_;
    //@}
};

#endif // __InfoPanel_ABC_h_
