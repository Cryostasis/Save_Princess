#include "control_panel.h"

#include <iostream>
#include <fstream>

#define GRID_CELL_SIZE 15

#define wxEVT_GRID_CELL_CHANGE wxEVT_GRID_CELL_CHANGED 

ControlPanel::ControlPanel(wxWindow* parent, GameInterface& gameInterface, wxWindowID id, const wxString& title,
	const wxPoint& pos, const wxSize& size, long style) :
	wxFrame(parent, id, title, pos, size, style), _gameInterface(gameInterface)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer(0, 3, 10, 10);
	fgSizer1->AddGrowableCol(0);
	fgSizer1->SetFlexibleDirection(wxBOTH);
	fgSizer1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxBoxSizer* FieldSizer;
	FieldSizer = new wxBoxSizer(wxVERTICAL);

	SpinSize = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
		wxDefaultSize, wxSP_ARROW_KEYS, 3, 25, 25);
	FieldSizer->Add(SpinSize, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	BtnSize = new wxButton(this, wxID_ANY, wxT("Set size"), wxDefaultPosition, wxDefaultSize, 0);
	FieldSizer->Add(BtnSize, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	FieldGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);

	// Grid
	FieldGrid->CreateGrid(25, 25);
	FieldGrid->EnableEditing(true);
	FieldGrid->EnableGridLines(true);
	FieldGrid->EnableDragGridSize(false);
	FieldGrid->SetMargins(0, 0);

	// Columns
	for (int i = 0; i < 25; i++)
		FieldGrid->SetColSize(i, GRID_CELL_SIZE);
	FieldGrid->EnableDragColMove(false);
	FieldGrid->EnableDragColSize(true);
	FieldGrid->SetColLabelSize(0);
	FieldGrid->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

	// Rows
	for (int i = 0; i < 25; i++)
		FieldGrid->SetRowSize(i, GRID_CELL_SIZE);
	FieldGrid->EnableDragRowSize(true);
	FieldGrid->SetRowLabelSize(0);
	FieldGrid->SetRowLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

	// Label Appearance

	// Cell Defaults

	int sz = FieldGrid->GetNumberCols();
	for (int i = 0; i < sz; i++)
		for (int j = 0; j < sz; j++)
			FieldGrid->SetCellValue({ i, j }, CELL_EMPTY);
	fix_cell_alignment();

	FieldGrid->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_TOP);
	FieldSizer->Add(FieldGrid, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);


	fgSizer1->Add(FieldSizer, 1, wxEXPAND, 5);

	wxBoxSizer* PrinceSizer;
	PrinceSizer = new wxBoxSizer(wxVERTICAL);

	LabelPrinceHP = new wxStaticText(this, wxID_ANY, wxT("Prince max HP"), wxDefaultPosition, wxDefaultSize, 0);
	LabelPrinceHP->Wrap(-1);
	PrinceSizer->Add(LabelPrinceHP, 0, wxALL, 5);

	SpinPrinceHP = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 20);
	PrinceSizer->Add(SpinPrinceHP, 0, wxALL, 5);

	LabelPrinceStrength = new wxStaticText(this, wxID_ANY, wxT("Prince max strength"), wxDefaultPosition, wxDefaultSize, 0);
	LabelPrinceStrength->Wrap(-1);
	PrinceSizer->Add(LabelPrinceStrength, 0, wxALL, 5);

	SpinPrinceST = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 20);
	PrinceSizer->Add(SpinPrinceST, 0, wxALL, 5);

	LabelPrinceHPregen = new wxStaticText(this, wxID_ANY, wxT("Prince HP regen"), wxDefaultPosition, wxDefaultSize, 0);
	LabelPrinceHPregen->Wrap(-1);
	PrinceSizer->Add(LabelPrinceHPregen, 0, wxALL, 5);

	SpinPrinceHPregen = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 1);
	PrinceSizer->Add(SpinPrinceHPregen, 0, wxALL, 5);

	LabelPrinceSTregen = new wxStaticText(this, wxID_ANY, wxT("Prince ST regen"), wxDefaultPosition, wxDefaultSize, 0);
	LabelPrinceSTregen->Wrap(-1);
	PrinceSizer->Add(LabelPrinceSTregen, 0, wxALL, 5);

	SpinPrinceSTregen = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 1);
	PrinceSizer->Add(SpinPrinceSTregen, 0, wxALL, 5);

	m_staticText5 = new wxStaticText(this, wxID_ANY, wxT("Prince attack power"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5->Wrap(-1);
	PrinceSizer->Add(m_staticText5, 0, wxALL, 5);

	SpinPrinceAttack = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 4);
	PrinceSizer->Add(SpinPrinceAttack, 0, wxALL, 5);

	m_staticText6 = new wxStaticText(this, wxID_ANY, wxT("Prince ST per attack"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText6->Wrap(-1);
	PrinceSizer->Add(m_staticText6, 0, wxALL, 5);

	SpinPrinceSTperAttack = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 2);
	PrinceSizer->Add(SpinPrinceSTperAttack, 0, wxALL, 5);


	fgSizer1->Add(PrinceSizer, 1, wxEXPAND, 5);

	wxBoxSizer* MonstersSizer;
	MonstersSizer = new wxBoxSizer(wxVERTICAL);

	m_staticText7 = new wxStaticText(this, wxID_ANY, wxT("Zombie attack"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText7->Wrap(-1);
	MonstersSizer->Add(m_staticText7, 0, wxALL, 5);

	SpinZombieAttack = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 1);
	MonstersSizer->Add(SpinZombieAttack, 0, wxALL, 5);

	m_staticText8 = new wxStaticText(this, wxID_ANY, wxT("Zombie HP"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText8->Wrap(-1);
	MonstersSizer->Add(m_staticText8, 0, wxALL, 5);

	SpinZombieHP = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 1);
	MonstersSizer->Add(SpinZombieHP, 0, wxALL, 5);

	m_staticText9 = new wxStaticText(this, wxID_ANY, wxT("Dragon attack"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText9->Wrap(-1);
	MonstersSizer->Add(m_staticText9, 0, wxALL, 5);

	SpinDragonAttack = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 5);
	MonstersSizer->Add(SpinDragonAttack, 0, wxALL, 5);

	m_staticText10 = new wxStaticText(this, wxID_ANY, wxT("Dragon HP"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText10->Wrap(-1);
	MonstersSizer->Add(m_staticText10, 0, wxALL, 5);

	SpinDragonHP = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 10);
	MonstersSizer->Add(SpinDragonHP, 0, wxALL, 5);


	fgSizer1->Add(MonstersSizer, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer(wxVERTICAL);

	FieldPicker = new wxFilePickerCtrl(this, wxID_ANY, wxT("Fields/input.txt"), wxT("Select field file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE);
	bSizer71->Add(FieldPicker, 0, wxALL | wxEXPAND, 10);


	bSizer5->Add(bSizer71, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxHORIZONTAL);


	bSizer8->Add(0, 0, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxHORIZONTAL);

	BtnLoad = new wxButton(this, wxID_ANY, wxT("Load"), wxDefaultPosition, wxSize(75, -1), 0);
	bSizer9->Add(BtnLoad, 0, wxALL, 5);

	BtnSave = new wxButton(this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxSize(75, -1), 0);
	bSizer9->Add(BtnSave, 0, wxALL, 5);


	bSizer8->Add(bSizer9, 1, wxEXPAND, 5);


	bSizer8->Add(0, 0, 1, wxEXPAND, 5);


	bSizer5->Add(bSizer8, 1, wxEXPAND, 5);


	fgSizer1->Add(bSizer5, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxVERTICAL);


	fgSizer1->Add(bSizer6, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxVERTICAL);

	BtnApply = new wxButton(this, wxID_ANY, wxT("RUN"), wxDefaultPosition, wxDefaultSize, 0);
	BtnApply->Enable(false);

	bSizer7->Add(BtnApply, 0, wxALL | wxEXPAND, 8);

	fgSizer1->Add(bSizer7, 1, wxEXPAND, 5);

	this->SetSizer(fgSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	BtnSize->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::BtnSizeOnButtonClick), NULL, this);
	FieldGrid->Connect(wxEVT_GRID_CELL_CHANGE, wxGridEventHandler(ControlPanel::FieldGridCellChange), NULL, this);
	BtnLoad->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::BtnLoadClick), NULL, this);
	BtnSave->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::BtnSaveClick), NULL, this);
	BtnApply->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::BtnApplyClick), NULL, this);

	check_field();
	//file_check();
	this->SetPosition(wxPoint(170, 70));
	this->SetSize(820, 580);

}

bool flagLoad = false;

ControlPanel::~ControlPanel()
{
	// Disconnect Events
	BtnSize->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::BtnSizeOnButtonClick), NULL, this);
	FieldGrid->Disconnect(wxEVT_GRID_CELL_CHANGE, wxGridEventHandler(ControlPanel::FieldGridCellChange), NULL, this);
	BtnLoad->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::BtnLoadClick), NULL, this);
	BtnSave->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::BtnSaveClick), NULL, this);
	BtnApply->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ControlPanel::BtnApplyClick), NULL, this);
}

void ControlPanel::BtnSizeOnButtonClick(wxCommandEvent & event)
{
	grid_resize();
}

void ControlPanel::FieldGridCellChange(wxGridEvent & event)
{
	grid_check_cell(event.GetRow(), event.GetCol());
}

void ControlPanel::BtnApplyClick(wxCommandEvent & event)
{
	int sz = FieldGrid->GetNumberCols();
	vector<string> V(sz);
	for (int i = 0; i < sz; i++)
		for (int j = 0; j < sz; j++)
			V[i].push_back(FieldGrid->GetCellValue(i, j)[0]);
	
	STRENGTH_PER_ATTACK = SpinPrinceSTperAttack->GetValue();
	STRENGTH_REGEN	    = SpinPrinceSTregen->GetValue();
	HP_REGEN		    = SpinPrinceHPregen->GetValue();
	
	KNIGHT_MAX_STRENGTH = SpinPrinceST->GetValue();
	KNIGHT_MAX_HP	    = SpinPrinceHP->GetValue();
	KNIGHT_ATTAK_POWER  = SpinPrinceAttack->GetValue();
	
	//PRINCESS_MAX_HP	    = SpinPrinceSTperAttack->GetValue();
	
	ZOMBIE_MAX_HP	    = SpinZombieHP->GetValue();
	ZOMBIE_ATTAK_POWER  = SpinZombieAttack->GetValue();
	 
	DRAGON_MAX_HP	    = SpinDragonHP->GetValue();
	DRAGON_ATTAK_POWER  = SpinDragonAttack->GetValue();

	_gameInterface.form_callback(V);
}

void ControlPanel::BtnLoadClick(wxCommandEvent & event)
{
	file_check();
}

void ControlPanel::BtnSaveClick(wxCommandEvent & event)
{
	int n = FieldGrid->GetNumberCols();
	ofstream fout;
	fout.open((string)FieldPicker->GetPath());
	fout << n << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			fout << (char)FieldGrid->GetCellValue(i, j)[0];
		fout << endl;
	}
}

void ControlPanel::grid_resize()
{
	int olD = FieldGrid->GetNumberCols();
	int neW = SpinSize->GetValue();
	if (olD == neW)
		return;
	if (olD < neW)
	{
		FieldGrid->InsertCols(olD, neW - olD, false);
		FieldGrid->InsertRows(olD, neW - olD, false);
		for (int i = 0; i < neW; i++)
			for (int j = olD; j < neW; j++)
				FieldGrid->SetCellValue({ i, j }, CELL_EMPTY);
		for (int i = olD; i < neW; i++)
			for (int j = 0; j < neW; j++)
				FieldGrid->SetCellValue({ i, j }, CELL_EMPTY);
	}
	else
	{
		FieldGrid->DeleteCols(neW, olD - neW, false);
		FieldGrid->DeleteRows(neW, olD - neW, false);
	}
	for (int i = 0; i < neW; i++)
	{
		FieldGrid->SetColSize(i, GRID_CELL_SIZE);
		FieldGrid->SetRowSize(i, GRID_CELL_SIZE);
	}
	fix_cell_alignment();
	check_field();
}

void ControlPanel::grid_check_cell(int x, int y)
{
	if (flagLoad)
		return;
	if (FieldGrid->GetCellValue(x, y).length() > 1)
		FieldGrid->SetCellValue(x, y, FieldGrid->GetCellValue(x, y)[0]);
	if (FieldGrid->GetCellValue(x, y).length() == 0 ||
		USED_SYMBOLS.find(FieldGrid->GetCellValue(x, y)[0]) == USED_SYMBOLS.end())
		FieldGrid->SetCellValue(x, y, CELL_EMPTY);
	check_field();
}

void ControlPanel::fix_cell_alignment()
{
	int sz = FieldGrid->GetNumberCols();
	for (int i = 0; i < sz; i++)
		for (int j = 0; j < sz; j++)
			FieldGrid->SetCellAlignment(i, j, wxALIGN_CENTRE, wxALIGN_CENTRE);
}

void ControlPanel::file_check()
{
	flagLoad = true;
	ifstream fin;
	fin.open((string)FieldPicker->GetPath());
	int n;
	fin >> n;
	SpinSize->SetValue(n);
	grid_resize();
	char c;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			fin >> c;
			FieldGrid->SetCellValue(i, j, c);
			grid_check_cell(i, j);
		}
	flagLoad = false;
	check_field();
}

bool ControlPanel::check_field()
{
	int knightCnt = 0, princessCnt = 0;
	int sz = FieldGrid->GetNumberCols();
	bool flagError = false;
	for (int i = 0; i < sz; i++)
		for (int j = 0; j < sz; j++)
		{
			char c = FieldGrid->GetCellValue(i, j)[0];
			flagError = flagError || (USED_SYMBOLS.find(c) == USED_SYMBOLS.end());
			knightCnt += (c == CELL_KNIGHT) ? 1 : 0;
			princessCnt += (c == CELL_PRINCESS) ? 1 : 0;
		}


	flagError = flagError || knightCnt != 1 || princessCnt != 1;
	if (flagError)
		BtnApply->Disable();
	else
		BtnApply->Enable();
	return flagError;
}

//=============================GAME INTERFACE==============================================

void GameInterface::form_callback(vector<string> V)
{
	//if (_dispatcher != nullptr)
	//	delete _dispatcher;
	_dispatcher = new GameDispatcher(_textures, WND_RES[0] - 2 * BORDER_W_PX, 0, 0, V.size(), V);
	flagGameStarted = true;
	glutShowWindow();
}
