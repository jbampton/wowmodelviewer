/*
* CharDetailsCustomizationChoice.cpp
*
*  Created on: 27 sep. 2020
*      Author: Jeromnimo
*/

#include "CharDetailsCustomizationChoice.h"

#include "CharDetails.h"
#include "CharDetailsEvent.h"

#include "logger/Logger.h"

IMPLEMENT_CLASS(CharDetailsCustomizationChoice, wxWindow)

BEGIN_EVENT_TABLE(CharDetailsCustomizationChoice, wxWindow)
  EVT_CHOICE(wxID_ANY, CharDetailsCustomizationChoice::onChoice)
END_EVENT_TABLE()


CharDetailsCustomizationChoice::CharDetailsCustomizationChoice(wxWindow* parent, CharDetails & details, CharDetails::CustomizationType type)
: wxWindow(parent, wxID_ANY),  m_type(type), m_details(details)
{
  wxFlexGridSizer *top = new wxFlexGridSizer(2, 0, 5);
  top->AddGrowableCol(2);

  m_params = m_details.getParams(m_type);
  m_details.attach(this);

  top->Add(new wxStaticText(this, wxID_ANY, m_params.name.toStdWString()),
           wxSizerFlags().Align(wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT, 5));

  m_choice = new wxChoice(this, wxID_ANY);

  top->Add(m_choice, wxSizerFlags().Align(wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL).Border(wxRIGHT, 5));

  SetAutoLayout(true);
  top->SetSizeHints(this);
  SetSizer(top);

  buildList();
  refresh();

  Layout();
}

void CharDetailsCustomizationChoice::onChoice(wxCommandEvent& event)
{
  m_details.set(m_type, m_params.possibleValues[event.GetSelection()]);
}

void CharDetailsCustomizationChoice::onEvent(Event * e)
{
  // update params for dynamically changing customization stuff (ie face type depends on skin color)
  if ((e->type() == CharDetailsEvent::SKIN_COLOR_CHANGED && m_type == CharDetails::FACE) ||
      (e->type() == CharDetailsEvent::FACE_CHANGED && m_type == CharDetails::SKIN_COLOR))
  {
    buildList();
    refresh();
  }

  // refesh only if event corresponds to current customization type (avoid flicker effect)
  if((e->type() == CharDetailsEvent::SKIN_COLOR_CHANGED && m_type == CharDetails::SKIN_COLOR) ||
     (e->type() == CharDetailsEvent::FACE_CHANGED && m_type == CharDetails::FACE) ||
     (e->type() == CharDetailsEvent::FACIAL_CUSTOMIZATION_STYLE_CHANGED && m_type == CharDetails::FACIAL_CUSTOMIZATION_STYLE) ||
     (e->type() == CharDetailsEvent::FACIAL_CUSTOMIZATION_COLOR_CHANGED && m_type == CharDetails::FACIAL_CUSTOMIZATION_COLOR) ||
     (e->type() == CharDetailsEvent::ADDITIONAL_FACIAL_CUSTOMIZATION_CHANGED && m_type == CharDetails::ADDITIONAL_FACIAL_CUSTOMIZATION) ||
     (e->type() == CharDetailsEvent::CUSTOM1_STYLE_CHANGED && m_type == CharDetails::CUSTOM1_STYLE) ||
     (e->type() == CharDetailsEvent::CUSTOM1_COLOR_CHANGED && m_type == CharDetails::CUSTOM1_COLOR) ||
     (e->type() == CharDetailsEvent::CUSTOM2_STYLE_CHANGED && m_type == CharDetails::CUSTOM2_STYLE) ||
     (e->type() == CharDetailsEvent::CUSTOM3_STYLE_CHANGED && m_type == CharDetails::CUSTOM3_STYLE))
  {
    refresh();
  }

}

void CharDetailsCustomizationChoice::buildList()
{
  if (m_choice)
  {
    m_params = m_details.getParams(m_type);
    // clear list and re add possible choices
    m_choice->Clear();
    for (auto c : m_params.possibleValues)
      m_choice->Append(wxString::Format(wxT(" %i "), c));
  }
}


void CharDetailsCustomizationChoice::refresh()
{
  if (m_choice)
  {
    uint pos = 0;

    const int currentValue = static_cast<int>(m_details.get(m_type));

    for (; pos < m_params.possibleValues.size(); pos++)
      if (m_params.possibleValues[pos] == currentValue) break;

    m_choice->SetSelection(pos);
  }
}


