/////////////////////////////////////////////////////////////////////////////
// Name:        xh_listb.cpp
// Purpose:     XML resource for wxListBox
// Author:      Bob Mitchell & Vaclav Slavik
// Created:     2000/07/29
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
 
#ifdef __GNUG__
#pragma implementation "xh_listb.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/xml/xh_listb.h"
#include "wx/listbox.h"

wxListBoxXmlHandler::wxListBoxXmlHandler() 
: wxXmlResourceHandler() , m_insideBox(FALSE)
{
    ADD_STYLE(wxLB_SINGLE);
    ADD_STYLE(wxLB_MULTIPLE);
    ADD_STYLE(wxLB_EXTENDED);
    ADD_STYLE(wxLB_HSCROLL);
    ADD_STYLE(wxLB_ALWAYS_SB);
    ADD_STYLE(wxLB_NEEDED_SB);
    ADD_STYLE(wxLB_SORT);
    AddWindowStyles();
}

wxObject *wxListBoxXmlHandler::DoCreateResource()
{ 
    if( m_class == wxT("wxListBox"))
    {
        // find the selection
        long selection = GetLong( wxT("selection"), -1 );

        // need to build the list of strings from children
        m_insideBox = TRUE;
        CreateChildrenPrivately( NULL, GetParamNode(wxT("content")));
        wxString *strings = (wxString *) NULL;
        if( strList.GetCount() > 0 )
        {
            strings = new wxString[strList.GetCount()];
            int count = strList.GetCount();
            for( int i = 0; i < count; i++ )
                strings[i]=strList[i];
        }


        wxListBox *control = new wxListBox(m_parentAsWindow,
                                    GetID(),
                                    GetPosition(), GetSize(),
                                    strList.GetCount(),
                                    strings,
                                    GetStyle(),
                                    wxDefaultValidator,
                                    GetName()
                                    );

        if( selection != -1 )
            control->SetSelection( selection );

        SetupWindow(control);

        if( strings != NULL )
            delete [] strings;
        strList.Clear();    // dump the strings   

        return control;
    }
    else
    {
        // on the inside now.
        // handle <item>Label</item>
        
        // add to the list
        strList.Add( GetNodeContent(m_node) );

        return NULL;
    }

}



bool wxListBoxXmlHandler::CanHandle(wxXmlNode *node)
{
    return (IsOfClass(node, wxT("wxListBox")) ||
           (m_insideBox && node->GetName() == wxT("item"))
           );
}

