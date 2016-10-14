#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4996)
#include <wx/wx.h>

#include "winfuncs.h"
#include "dispatcher.h"
#include "camera.h"
#include "gl_vars.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/spinctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/filepicker.h>

#include <set>

using namespace std;



///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////

class GameInterface;

class ControlPanel : public wxFrame
{
private:

protected:
	wxSpinCtrl* SpinSize;
	wxButton* BtnSize;
	wxGrid* FieldGrid;
	wxStaticText* LabelPrinceHP;
	wxSpinCtrl* SpinPrinceHP;
	wxStaticText* LabelPrinceStrength;
	wxSpinCtrl* SpinPrinceST;
	wxStaticText* LabelPrinceHPregen;
	wxSpinCtrl* SpinPrinceHPregen;
	wxStaticText* LabelPrinceSTregen;
	wxSpinCtrl* SpinPrinceSTregen;
	wxStaticText* m_staticText5;
	wxSpinCtrl* SpinPrinceAttack;
	wxStaticText* m_staticText6;
	wxSpinCtrl* SpinPrinceSTperAttack;
	wxStaticText* m_staticText7;
	wxSpinCtrl* SpinZombieAttack;
	wxStaticText* m_staticText8;
	wxSpinCtrl* SpinZombieHP;
	wxStaticText* m_staticText9;
	wxSpinCtrl* SpinDragonAttack;
	wxStaticText* m_staticText10;
	wxSpinCtrl* SpinDragonHP;
	wxButton* BtnApply;
	wxFilePickerCtrl* FieldPicker;
	wxButton* BtnLoad;
	wxButton* BtnSave;

	// Virtual event handlers, overide them in your derived class
	virtual void BtnSizeOnButtonClick(wxCommandEvent& event);
	virtual void FieldGridCellChange(wxGridEvent& event);
	virtual void BtnApplyClick(wxCommandEvent& event);
	virtual void BtnLoadClick(wxCommandEvent& event);
	virtual void BtnSaveClick(wxCommandEvent& event);

	void grid_resize();
	void grid_check_cell(int i, int j);
	void fix_cell_alignment();
	void file_check();
	bool check_field();

	GameInterface& _gameInterface;
public:

	ControlPanel(wxWindow* parent, GameInterface& gameInterface, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(820, 503), 
		long style = wxCAPTION | wxTAB_TRAVERSAL);

	~ControlPanel();
};

class ControlForm : public wxApp
{
public:
	ControlForm(GameInterface& gameInterface) : _gameInterface(gameInterface){};
private:
	GameInterface& _gameInterface;
	ControlPanel* _control;
	bool OnInit()
	{
		_control = new ControlPanel(NULL, _gameInterface, -1, "Conrol panel");
		_control->Show();
		glutMainLoop();
		return true;
	}
};

class GameInterface
{
public:
	GameInterface(GameTextures& textures) : _textures(textures) {};
	void create_form()
	{
		wxApp* pApp = new ControlForm(*this);
		wxApp::SetInstance(pApp);
		wxEntry(0, NULL);
	}

	void go_up(const bool attack) { _dispatcher->go_up(attack); };
	void go_down(const bool attack) { _dispatcher->go_down(attack); };
	void go_right(const bool attack) { _dispatcher->go_right(attack); };
	void go_left(const bool attack) { _dispatcher->go_left(attack); };

	void form_callback(vector<string> V);
	void render(GLuint program, Camera camera)
	{
		_dispatcher->render(program, camera);
	}
private:
	GameTextures& _textures;
	ControlForm* _form;
	GameDispatcher* _dispatcher;
};
