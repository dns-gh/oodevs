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

#include "SelectedElement.h"

class Agent;
class Population;
class Agent_ABC;

// =============================================================================
/** @class  InfoPanel_ABC
    @brief  InfoPanel_ABC
*/
// Created: AGE 2005-04-05
// =============================================================================
class InfoPanel_ABC : public QScrollView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit InfoPanel_ABC( QWidget* pParent );
    virtual ~InfoPanel_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void SetSelection( SelectedElement& item );
    virtual void insertChild ( QObject*             pObj );
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnUpdate           ();
    virtual void OnClearSelection   () = 0;
    virtual void OnAgentUpdated     ( Agent&      agent  );
    virtual void OnObjectUpdated    ( Object_ABC& object );
    virtual void OnPopulationUpdated( Population& population );

    virtual void showEvent( QShowEvent* );
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename Cont1 >
    void Suit( Cont1& destCont, unsigned int nSize, QListView* pView )
    {
        if( ! nSize )
        {
            pView->hide();
            return;
        }
        pView->show();

        while( destCont.size() < nSize )
        {
            destCont.push_back( new MT_ListViewItem( pView ) );
            destCont.back()->SetBackground( destCont.size() % 2 == 0 ? QColor( 255, 255, 255 ) : QColor( 230, 230, 255 ) );
        }
        while( destCont.size() > nSize )
        {
            delete destCont.back();
            destCont.pop_back();
        }
    }

    bool     ShouldDisplay( const Agent&      agent      ) const;
    bool     ShouldDisplay( const Population& population ) const;
    bool     ShouldDisplay( const Object_ABC& object     ) const;
    QLayout* layout       ();
    //@}

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
    SelectedElement selectedItem_;
    //bool bVisible_;
    //@}
};

#endif // __InfoPanel_ABC_h_
