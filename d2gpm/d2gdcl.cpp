/////////////////////////////////////////////////////////////////////////////
// Name:        d2gdcl.cpp
// Purpose:     D2gmain functions not part of the GUI itself
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     30/11/2009
// RCS-ID:      
// Copyright:   (c) Copyright 2009 Omni Systems, Inc.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "d2guiapp.h"
#include "wx/bookctrl.h"
#include "wx/msgdlg.h"

#include "configstart.h"
#include "d2gmain.h"

#pragma warning( disable : 4189 )


#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dclog.h"
#include "dcfili.h"
#include "dcfilp.h"

#include "d2gutil.h"



// D2gmain::functions


// do any inits for d2gdcl, like opening log file
void D2gmain::InitializeDCL(void)
{
	d2gpmVersion = 0;
#ifdef DITA
	UMXL = false;
#endif
#ifdef UDOC
	UMXL = true;
#endif

	CurrProjectID = NULL;
	NextProjectID = 0;
	NewProjectID = NULL;
	CurrProject = NULL;
	ActiveProjects = NULL;
	DeletedProjects = NULL;

	Creating = false;
	Modifying = false;
	Running = false;

	ProjectsChanged = true;
	CurrMProjectChanged = true;
	CurrDProjectChanged = true;
	CurrRProjectChanged = true;
	RunProjectChanged = true;
	ActiveMProjectsChanged = true;
	ActiveDProjectsChanged = true;
	ActiveRunProjectsChanged = true;
	DeletedProjectsChanged = true;

	CurrSourceDir = NULL;
	CurrSourceMap = NULL;
	CurrOutputDir = NULL;
	CurrWrapDir = NULL;

	FormatsChanged = true;
	MFormatsChanged = true;
	CurrFormatChanged = true;
	MCurrFormatChanged = true;
	CurrFormat = NULL;
	OutputFormats = NULL;
	CurrType = NULL;
	CurrIni = NULL;
	NewIniPath = NULL;
	OutputInis = NULL;
	SetCreateProjIni = false;

	Editor = NULL;
	CMStart = NULL;
	EditCommand = NULL;
	CMCommand = NULL;
	EditDocCommand = NULL;

	RunSourceMap = NULL;
	ShowLogCommand = NULL;
	ViewOutputCommand = NULL;
	LogFileName = NULL;
	FullLogName = NULL;
	HistoryFileName = NULL;
	FullHistoryName = NULL;
	OutputExt = NULL;
	//UseLog = true;
	ShowLog = false;
	LogIniChains = false;
	ShowConsole = false;
	ShowDelInCreate = false;
	WrapAndShip = false;

	CreateExistsText = NULL;
	CreateNothingText = NULL;
	CreatedProjectText = NULL;
	ModifiedProjectText = NULL;
	DeletedProjectText = NULL;
	RestoredProjectText = NULL;
	RunningText = NULL;
	RunningDoneText = NULL;
	RunningErrorText = NULL;
	RunningStopText = NULL;
	RunningAsychText = NULL;
	NoIniStart = NULL;
	NoIniMessage = NULL;
	NoIniTitle = NULL;


	SetUpSysini();
	SetUpProjects();
	GetDeletedProjects();
	GetActiveProjects();
	GetAllFormats();
}


// open system.ini and set it up to read settings
void D2gmain::SetUpSysini(void)
{
	// see if %omsyshome% is set
	if (!getenv("omsyshome")) {
		wxMessageDialog dialog (NULL,
		  wxT("System environment variable\n%omsyshome% not defined."),
#ifdef DITA
		  wxT("DITA2GO"), wxOK);
#endif
#ifdef UDOC
		  wxT("UDOC2GO"), wxOK);
#endif
		dialog.ShowModal();
		exit(1);
	}

	// determine local name for tooltips and labels file
#ifdef DITA
	char *fname = "%omsyshome%\\d2g\\system\\system.ini";
#endif
#ifdef UDOC
	char *fname = "%omsyshome%\\x2g\\system\\system.ini";
#endif
	wxString *str = NULL;

	SysIni = new DCirfile(fname);
	if (SysIni->FErr() == fok)
		SystemFileName = NewName(SysIni->getFilePath());
	else {
		delete SysIni;
		SysIni = NULL;
		SystemFileName = NULL;
	}

	// set up logging
	if (SystemFileName
	 && SysIni
	 && SysIni->Section("Logging")) {

		SetPMFlag();

		if (SysIni->Find("UseLog"))
			UseLog = SysIni->BoolRight();
		if (SysIni->Find("ShowLog"))
			ShowLog = SysIni->BoolRight();
		if (SysIni->Find("LogIniChains"))
			LogIniChains = SysIni->BoolRight();

		LogLevels[0] = 0;
		if (SysIni->Find("LogErrors"))
			LogLevels[1] = (unc) SysIni->IntRight();
		else
			LogLevels[1] = 1;
		if (SysIni->Find("LogWarnings"))
			LogLevels[2] = (unc) SysIni->IntRight();
		else
			LogLevels[2] = 1;
		if (SysIni->Find("LogQuerys"))
			LogLevels[3] = (unc) SysIni->IntRight();
		else
			LogLevels[3] = 1;
		if (SysIni->Find("LogInfo"))
			LogLevels[4] = (unc) SysIni->IntRight();
		else
			LogLevels[4] = 1;
		if (SysIni->Find("LogWarnings"))
			LogLevels[5] = (unc) SysIni->IntRight();
		else
			LogLevels[5] = 1;

		if ((str = GetSysIni("LogFileName")) != NULL)
			LogFileName = str;
		if ((str = GetSysIni("HistoryFileName")) != NULL)
			HistoryFileName = str;
	}
}


void D2gmain::SetUpLogging(void)
{
	if (!FullLogName)
		return;

	if (UseLog) {
		if (FullHistoryName
		 && DCrfile::FileExists((char *) FullLogName->c_str())) {  // append to history
			char *cmd = NewName("type ", FullLogName->Len() + 
				                           FullHistoryName->Len() + 8);
			strcat(cmd, FullLogName->c_str());
			strcat(cmd, " >>");
			strcat(cmd, FullHistoryName->c_str());
			RunDOSCommand(cmd, NULL, true, false, true);
			strcpy(cmd, "del /Q ");
			strcat(cmd, FullLogName->c_str());
			RunDOSCommand(cmd, NULL, true, false, true);
			DeleteName(cmd);
		}

#ifdef DITA
		Log = new DClog(FullLogName->c_str(), "d2gpm", "p004", LogLevels);
#endif
#ifdef UDOC
		Log = new DClog(FullLogName->c_str(), "x2gpm", "p001", LogLevels);
#endif
		if (Log->FErr() != fok)
			UseLog = false;
		D2gmain::LogEventFunc = LogEvent;
		::LogFileName = NewName(FullLogName->c_str());
	}
	DCirfile::LogEventFunc = (logfunc) LogEvent;
	DCirfile::LogIniChain = LogIniChains;
}


// open projects.ini and set it up for r/w of settings
void D2gmain::SetUpProjects(void)
{
	// determine local name
#ifdef DITA
	char *fname = "%omsyshome%\\d2g\\system\\projects.ini";
#endif
#ifdef UDOC
	char *fname = "%omsyshome%\\x2g\\system\\projects.ini";
#endif
	DCfile *fp = new DCfile(fname);
	ProjectFileName = NewName(fp->getFilePath());
	if (fp->FErr() == fno) {
		delete fp;
		DCwfile wf(ProjectFileName, ftxt);
		DCblock *blk = new DCblock();
		blk->wrap(ProjectsIni, strlen(ProjectsIni));
		wf = *blk;
		wf.CloseFile();
		blk->clear();
		delete blk;
	}
	else {
		fp->CloseFile();
		delete fp;
	}

	DCpfile::psetup(ProjectFileName);

	CurrFormat = new wxString("OmniHelp");

	// set globals per [Options]
#ifdef DITA
	d2gpmVersion = GetProjInt("Options", "d2gpmVersion");
#endif
#ifdef UDOC
	d2gpmVersion = GetProjInt("Options", "x2gpmVersion");
#endif

	long posx = GetProjInt("Options", "PosX");
	if ((posx < 1500) || (posx > 3000))
		posx = -1;
	long posy = GetProjInt("Options", "PosY");
	if ((posy < 1) || (posy > 1000))
		posy = -1;
	InitPos = new wxPoint(posx, posy);

	long sizex = GetProjInt("Options", "SizeX");
	if (sizex < 575)
		sizex = 575;
	long sizey = GetProjInt("Options", "SizeY");
	if (sizey < 375)
		sizey = 375;
	InitSize = new wxSize(sizex, sizey);

	GetOptionsSettings();

	// set globals per [Projects]
	NextProjectID = GetProjInt("Projects", "NextID");
}


// Setup for UI elements


void D2gmain::SetAllToolTips(void)
{
	if (!D2gmain::ShowToolTips()
	 || !SystemFileName
	 || !SysIni
	 || !SysIni->Section("Tooltips"))
		return;

	wxString *str = NULL;

	if ((str = GetSysIni("ProjectTop")) != NULL)
		SetToolTip(*str);

	if ((str = GetSysIni("ProjectName")) != NULL)
		m_projectname->SetToolTip(*str);
	if ((str = GetSysIni("ProjectSelect")) != NULL)
		m_projectselect->SetToolTip(*str);
	if ((str = GetSysIni("SourceDir")) != NULL)
		m_sourcedir->SetToolTip(*str);
	if ((str = GetSysIni("SrcDirSelect")) != NULL)
		m_srcdirselect->SetToolTip(*str);
	if ((str = GetSysIni("SourceMap")) != NULL)
		m_sourcemap->SetToolTip(*str);
	if ((str = GetSysIni("SrcMapSelect")) != NULL)
		m_srcmapselect->SetToolTip(*str);
	if ((str = GetSysIni("FormatName")) != NULL)
		m_formatname->SetToolTip(*str);
	if ((str = GetSysIni("FormatSelect")) != NULL)
		m_formatselect->SetToolTip(*str);
	if ((str = GetSysIni("OutputDir")) != NULL)
		m_outputdir->SetToolTip(*str);
	if ((str = GetSysIni("OutDirSelect")) != NULL)
		m_outdirselect->SetToolTip(*str);
	if ((str = GetSysIni("CreateProject")) != NULL)
		m_createproject->SetToolTip(*str);
	if ((str = GetSysIni("IniConfig")) != NULL)
		m_iniconfig->SetToolTip(*str);
	if ((str = GetSysIni("EditIni")) != NULL)
		m_editini->SetToolTip(*str);
	if ((str = GetSysIni("CreateCM")) != NULL)
		m_createcm->SetToolTip(*str);
	if ((str = GetSysIni("Result")) != NULL)
		m_result->SetToolTip(*str);
	if ((str = GetSysIni("ResultText")) != NULL)
		m_resulttext->SetToolTip(*str);

	if ((str = GetSysIni("MProjectName")) != NULL)
		m_mprojectname->SetToolTip(*str);
	if ((str = GetSysIni("MProjectSelect")) != NULL)
		m_mprojectselect->SetToolTip(*str);
	if ((str = GetSysIni("MSourceDir")) != NULL)
		m_msourcedir->SetToolTip(*str);
	if ((str = GetSysIni("MSrcDirSelect")) != NULL)
		m_msrcdirselect->SetToolTip(*str);
	if ((str = GetSysIni("MSourceMap")) != NULL)
		m_msourcemap->SetToolTip(*str);
	if ((str = GetSysIni("MSrcMapSelect")) != NULL)
		m_msrcmapselect->SetToolTip(*str);
	if ((str = GetSysIni("MFormatName")) != NULL)
		m_mformatname->SetToolTip(*str);
	if ((str = GetSysIni("MFormatSelect")) != NULL)
		m_mformatselect->SetToolTip(*str);
	if ((str = GetSysIni("MOutputDir")) != NULL)
		m_moutputdir->SetToolTip(*str);
	if ((str = GetSysIni("MOutDirSelect")) != NULL)
		m_moutdirselect->SetToolTip(*str);
	if ((str = GetSysIni("ModifyProject")) != NULL)
		m_modifyproject->SetToolTip(*str);
	if ((str = GetSysIni("MIniConfig")) != NULL)
		m_miniconfig->SetToolTip(*str);
	if ((str = GetSysIni("MEditIni")) != NULL)
		m_meditini->SetToolTip(*str);
	if ((str = GetSysIni("ModifyCM")) != NULL)
		m_modifycm->SetToolTip(*str);
	if ((str = GetSysIni("MResult")) != NULL)
		m_mresult->SetToolTip(*str);
	if ((str = GetSysIni("MResultText")) != NULL)
		m_mresulttext->SetToolTip(*str);

	if ((str = GetSysIni("DProjectName")) != NULL)
		m_dprojectname->SetToolTip(*str);
	if ((str = GetSysIni("DProjectSelect")) != NULL)
		m_dprojectselect->SetToolTip(*str);
	if ((str = GetSysIni("DSourceDir")) != NULL)
		m_dsourcedir->SetToolTip(*str);
	if ((str = GetSysIni("DSrcDirText")) != NULL)
		m_dsrcdirtext->SetToolTip(*str);
	if ((str = GetSysIni("DSourceMap")) != NULL)
		m_dsourcemap->SetToolTip(*str);
	if ((str = GetSysIni("DSrcMapText")) != NULL)
		m_dsrcmaptext->SetToolTip(*str);
	if ((str = GetSysIni("DFormatName")) != NULL)
		m_dformatname->SetToolTip(*str);
	if ((str = GetSysIni("DFormatText")) != NULL)
		m_dformattext->SetToolTip(*str);
	if ((str = GetSysIni("DOutputDir")) != NULL)
		m_doutputdir->SetToolTip(*str);
	if ((str = GetSysIni("DOutDirText")) != NULL)
		m_doutdirtext->SetToolTip(*str);
	if ((str = GetSysIni("DeleteProject")) != NULL)
		m_deleteproject->SetToolTip(*str);
	if ((str = GetSysIni("DResult")) != NULL)
		m_dresult->SetToolTip(*str);
	if ((str = GetSysIni("DResultText")) != NULL)
		m_dresulttext->SetToolTip(*str);

	if ((str = GetSysIni("RProjectName")) != NULL)
		m_rprojectname->SetToolTip(*str);
	if ((str = GetSysIni("RProjectSelect")) != NULL)
		m_rprojectselect->SetToolTip(*str);
	if ((str = GetSysIni("RSourceDir")) != NULL)
		m_rsourcedir->SetToolTip(*str);
	if ((str = GetSysIni("RSrcDirText")) != NULL)
		m_rsrcdirtext->SetToolTip(*str);
	if ((str = GetSysIni("RSourceMap")) != NULL)
		m_rsourcemap->SetToolTip(*str);
	if ((str = GetSysIni("RSrcMapText")) != NULL)
		m_rsrcmaptext->SetToolTip(*str);
	if ((str = GetSysIni("RFormatName")) != NULL)
		m_rformatname->SetToolTip(*str);
	if ((str = GetSysIni("RFormatText")) != NULL)
		m_rformattext->SetToolTip(*str);
	if ((str = GetSysIni("ROutputDir")) != NULL)
		m_routputdir->SetToolTip(*str);
	if ((str = GetSysIni("ROutDirText")) != NULL)
		m_routdirtext->SetToolTip(*str);
	if ((str = GetSysIni("RestoreProject")) != NULL)
		m_restoreproject->SetToolTip(*str);
	if ((str = GetSysIni("RResult")) != NULL)
		m_rresult->SetToolTip(*str);
	if ((str = GetSysIni("RResultText")) != NULL)
		m_rresulttext->SetToolTip(*str);

	if ((str = GetSysIni("RunProject")) != NULL)
		m_runproject->SetToolTip(*str);
	if ((str = GetSysIni("RunProjectSel")) != NULL)
		m_runprojectsel->SetToolTip(*str);
	if ((str = GetSysIni("RunSrcDir")) != NULL)
		m_runsrcdir->SetToolTip(*str);
	if ((str = GetSysIni("RunSrcDirText")) != NULL)
		m_runsrcdirtext->SetToolTip(*str);
	if ((str = GetSysIni("RunSrcMap")) != NULL)
		m_runsrcmap->SetToolTip(*str);
	if ((str = GetSysIni("RunSrcMapSel")) != NULL)
		m_runsrcmapsel->SetToolTip(*str);
	if ((str = GetSysIni("RunFormat")) != NULL)
		m_runformat->SetToolTip(*str);
	if ((str = GetSysIni("RunFormatText")) != NULL)
		m_runformattext->SetToolTip(*str);
	if ((str = GetSysIni("RunOutputDir")) != NULL)
		m_runoutputdir->SetToolTip(*str);
	if ((str = GetSysIni("RunOutDirText")) != NULL)
		m_runoutdirtext->SetToolTip(*str);
	if ((str = GetSysIni("RunStart")) != NULL)
		m_runstart->SetToolTip(*str);
	if ((str = GetSysIni("RunStop")) != NULL)
		m_runstop->SetToolTip(*str);
	if ((str = GetSysIni("RunEditIni")) != NULL)
		m_runeditini->SetToolTip(*str);
	if ((str = GetSysIni("RunCM")) != NULL)
		m_runcm->SetToolTip(*str);
	if ((str = GetSysIni("RunShowLog")) != NULL)
		m_runshowlog->SetToolTip(*str);
	if ((str = GetSysIni("RunViewOutput")) != NULL)
		m_runviewout->SetToolTip(*str);
	if ((str = GetSysIni("RunResult")) != NULL)
		m_runresult->SetToolTip(*str);
	if ((str = GetSysIni("RunResultText")) != NULL)
		m_runresulttext->SetToolTip(*str);

	if ((str = GetSysIni("RunOptAuto")) != NULL)
		m_runoptauto->SetToolTip(*str);
	if ((str = GetSysIni("RunWas")) != NULL)
		m_runwas->SetToolTip(*str);
	if ((str = GetSysIni("RunWasYes")) != NULL)
		m_runwasyes->SetToolTip(*str);
	if ((str = GetSysIni("RunWasNo")) != NULL)
		m_runwasno->SetToolTip(*str);
	if ((str = GetSysIni("RunComp")) != NULL)
		m_runcomp->SetToolTip(*str);
	if ((str = GetSysIni("RunCompYes")) != NULL)
		m_runcompyes->SetToolTip(*str);
	if ((str = GetSysIni("RunCompNo")) != NULL)
		m_runcompno->SetToolTip(*str);
	if ((str = GetSysIni("RunCwin")) != NULL)
		m_runcwin->SetToolTip(*str);
	if ((str = GetSysIni("RunCwinYes")) != NULL)
		m_runcwinyes->SetToolTip(*str);
	if ((str = GetSysIni("RunCwinNo")) != NULL)
		m_runcwinno->SetToolTip(*str);
	if ((str = GetSysIni("RunFts")) != NULL)
		m_runfts->SetToolTip(*str);
	if ((str = GetSysIni("RunFtsYes")) != NULL)
		m_runftsyes->SetToolTip(*str);
	if ((str = GetSysIni("RunFtsNo")) != NULL)
		m_runftsno->SetToolTip(*str);
	if ((str = GetSysIni("RunJar")) != NULL)
		m_runjar->SetToolTip(*str);
	if ((str = GetSysIni("RunJarYes")) != NULL)
		m_runjaryes->SetToolTip(*str);
	if ((str = GetSysIni("RunJarNo")) != NULL)
		m_runjarno->SetToolTip(*str);
	if ((str = GetSysIni("RunArch")) != NULL)
		m_runarch->SetToolTip(*str);
	if ((str = GetSysIni("RunArchYes")) != NULL)
		m_runarchyes->SetToolTip(*str);
	if ((str = GetSysIni("RunArchNo")) != NULL)
		m_runarchno->SetToolTip(*str);
	if ((str = GetSysIni("RunShip")) != NULL)
		m_runship->SetToolTip(*str);
	if ((str = GetSysIni("RunShipYes")) != NULL)
		m_runshipyes->SetToolTip(*str);
	if ((str = GetSysIni("RunShipNo")) != NULL)
		m_runshipno->SetToolTip(*str);
	if ((str = GetSysIni("RunOptAdv")) != NULL)
		m_runoptadv->SetToolTip(*str);
	if ((str = GetSysIni("RunDcl")) != NULL)
		m_rundcl->SetToolTip(*str);
	if ((str = GetSysIni("RunDclSrc")) != NULL)
		m_rundclsrc->SetToolTip(*str);
	if ((str = GetSysIni("RunDclOut")) != NULL)
		m_rundclout->SetToolTip(*str);
	if ((str = GetSysIni("RunDclNo")) != NULL)
		m_rundclno->SetToolTip(*str);
	if ((str = GetSysIni("RunDclYes")) != NULL)
		m_rundclyes->SetToolTip(*str);
	if ((str = GetSysIni("RunOnly")) != NULL)
		m_runonly->SetToolTip(*str);
	if ((str = GetSysIni("RunOnlyYes")) != NULL)
		m_runonlyyes->SetToolTip(*str);
	if ((str = GetSysIni("RunOnlyNo")) != NULL)
		m_runonlyno->SetToolTip(*str);

	if ((str = GetSysIni("OptEditor")) != NULL)
		m_opteditor->SetToolTip(*str);
	if ((str = GetSysIni("OptEditorText")) != NULL)
		m_opteditortext->SetToolTip(*str);
	if ((str = GetSysIni("OptShowDel")) != NULL)
		m_optshdel->SetToolTip(*str);
	if ((str = GetSysIni("OptShowDelYes")) != NULL)
		m_optshdelyes->SetToolTip(*str);
	if ((str = GetSysIni("OptShowDelNo")) != NULL)
		m_optshdelno->SetToolTip(*str);
	if ((str = GetSysIni("OptShow")) != NULL)
		m_optshow->SetToolTip(*str);
	if ((str = GetSysIni("OptShowLog")) != NULL)
		m_optshowlog->SetToolTip(*str);
	if ((str = GetSysIni("OptShowCon")) != NULL)
		m_optshowcon->SetToolTip(*str);
	if ((str = GetSysIni("OptNewIni")) != NULL)
		m_optnewini->SetToolTip(*str);
	if ((str = GetSysIni("OptNewIniSel")) != NULL)
		m_optnewinisel->SetToolTip(*str);
	if ((str = GetSysIni("OptLogFile")) != NULL)
		m_optlogfile->SetToolTip(*str);
	if ((str = GetSysIni("OptLogFileText")) != NULL)
		m_optlogfiletext->SetToolTip(*str);
	if ((str = GetSysIni("OptProjIni")) != NULL)
		m_optprojini->SetToolTip(*str);
	if ((str = GetSysIni("OptProjIniD2GBut")) != NULL)
		m_optprojinid2gbut->SetToolTip(*str);
	if ((str = GetSysIni("OptProjIniLocBut")) != NULL)
		m_optprojinilocbut->SetToolTip(*str);
	if ((str = GetSysIni("OptProjIniD2G")) != NULL)
		m_optprojinid2g->SetToolTip(*str);
	if ((str = GetSysIni("OptProjIniD2GSel")) != NULL)
		m_optprojinid2gsel->SetToolTip(*str);
	if ((str = GetSysIni("OptProjIniLoc")) != NULL)
		m_optprojiniloc->SetToolTip(*str);
	if ((str = GetSysIni("OptProjIniLocText")) != NULL)
		m_optprojiniloctext->SetToolTip(*str);
	if ((str = GetSysIni("OptSet")) != NULL)
		m_optset->SetToolTip(*str);
	if ((str = GetSysIni("OptReset")) != NULL)
		m_optreset->SetToolTip(*str);
}


void D2gmain::SetAllLabels(void)
{
	if (!SystemFileName
	 || !SysIni
	 || !SysIni->Section("Labels"))
		return;

	wxString *str = NULL;

	if ((str = GetSysIni("ProjectTop")) != NULL)
		SetTitle(*str);

	if ((str = GetSysIni("ProjectName")) != NULL)
		m_projectname->SetLabel(*str);
	if ((str = GetSysIni("SourceDir")) != NULL)
		m_sourcedir->SetLabel(*str);
	if ((str = GetSysIni("SourceMap")) != NULL)
		m_sourcemap->SetLabel(*str);
	if ((str = GetSysIni("FormatName")) != NULL)
		m_formatname->SetLabel(*str);
	if ((str = GetSysIni("OutputDir")) != NULL)
		m_outputdir->SetLabel(*str);
	if ((str = GetSysIni("CreateProject")) != NULL)
		m_createproject->SetLabel(*str);
	if ((str = GetSysIni("IniConfig")) != NULL)
		m_iniconfig->SetLabel(*str);
	if ((str = GetSysIni("EditIni")) != NULL)
		m_editini->SetLabel(*str);
	if ((str = GetSysIni("CreateCM")) != NULL)
		m_createcm->SetLabel(*str);
	if ((str = GetSysIni("Result")) != NULL)
		m_result->SetLabel(*str);

	if ((str = GetSysIni("MProjectName")) != NULL)
		m_mprojectname->SetLabel(*str);
	if ((str = GetSysIni("MSourceDir")) != NULL)
		m_msourcedir->SetLabel(*str);
	if ((str = GetSysIni("MSourceMap")) != NULL)
		m_msourcemap->SetLabel(*str);
	if ((str = GetSysIni("MFormatName")) != NULL)
		m_mformatname->SetLabel(*str);
	if ((str = GetSysIni("MOutputDir")) != NULL)
		m_moutputdir->SetLabel(*str);
	if ((str = GetSysIni("ModifyProject")) != NULL)
		m_modifyproject->SetLabel(*str);
	if ((str = GetSysIni("MIniConfig")) != NULL)
		m_miniconfig->SetLabel(*str);
	if ((str = GetSysIni("MEditIni")) != NULL)
		m_meditini->SetLabel(*str);
	if ((str = GetSysIni("ModifyCM")) != NULL)
		m_modifycm->SetLabel(*str);
	if ((str = GetSysIni("MResult")) != NULL)
		m_mresult->SetLabel(*str);

	if ((str = GetSysIni("DProjectName")) != NULL)
		m_dprojectname->SetLabel(*str);
	if ((str = GetSysIni("DSourceDir")) != NULL)
		m_dsourcedir->SetLabel(*str);
	if ((str = GetSysIni("DSourceMap")) != NULL)
		m_dsourcemap->SetLabel(*str);
	if ((str = GetSysIni("DFormatName")) != NULL)
		m_dformatname->SetLabel(*str);
	if ((str = GetSysIni("DOutputDir")) != NULL)
		m_doutputdir->SetLabel(*str);
	if ((str = GetSysIni("DeleteProject")) != NULL)
		m_deleteproject->SetLabel(*str);
	if ((str = GetSysIni("DResult")) != NULL)
		m_dresult->SetLabel(*str);

	if ((str = GetSysIni("RProjectName")) != NULL)
		m_rprojectname->SetLabel(*str);
	if ((str = GetSysIni("RSourceDir")) != NULL)
		m_rsourcedir->SetLabel(*str);
	if ((str = GetSysIni("RSourceMap")) != NULL)
		m_rsourcemap->SetLabel(*str);
	if ((str = GetSysIni("RFormatName")) != NULL)
		m_rformatname->SetLabel(*str);
	if ((str = GetSysIni("ROutputDir")) != NULL)
		m_routputdir->SetLabel(*str);
	if ((str = GetSysIni("RestoreProject")) != NULL)
		m_restoreproject->SetLabel(*str);
	if ((str = GetSysIni("RResult")) != NULL)
		m_rresult->SetLabel(*str);

	if ((str = GetSysIni("RunProject")) != NULL)
		m_runproject->SetLabel(*str);
	if ((str = GetSysIni("RunSrcDir")) != NULL)
		m_runsrcdir->SetLabel(*str);
	if ((str = GetSysIni("RunSrcMap")) != NULL)
		m_runsrcmap->SetLabel(*str);
	if ((str = GetSysIni("RunFormat")) != NULL)
		m_runformat->SetLabel(*str);
	if ((str = GetSysIni("RunOutputDir")) != NULL)
		m_runoutputdir->SetLabel(*str);
	if ((str = GetSysIni("RunStart")) != NULL)
		m_runstart->SetLabel(*str);
	if ((str = GetSysIni("RunStop")) != NULL)
		m_runstop->SetLabel(*str);
	if ((str = GetSysIni("RunEditIni")) != NULL)
		m_runeditini->SetLabel(*str);
	if ((str = GetSysIni("RunCM")) != NULL)
		m_runcm->SetLabel(*str);
	if ((str = GetSysIni("RunShowLog")) != NULL)
		m_runshowlog->SetLabel(*str);
	if ((str = GetSysIni("RunViewOutput")) != NULL)
		m_runviewout->SetLabel(*str);
	if ((str = GetSysIni("RunResult")) != NULL)
		m_runresult->SetLabel(*str);

	if ((str = GetSysIni("RunOptAuto")) != NULL)
		m_runoptauto->SetLabel(*str);
	if ((str = GetSysIni("RunWas")) != NULL)
		m_runwas->SetLabel(*str);
	if ((str = GetSysIni("RunWasYes")) != NULL)
		m_runwasyes->SetLabel(*str);
	if ((str = GetSysIni("RunWasNo")) != NULL)
		m_runwasno->SetLabel(*str);
	if ((str = GetSysIni("RunComp")) != NULL)
		m_runcomp->SetLabel(*str);
	if ((str = GetSysIni("RunCompYes")) != NULL)
		m_runcompyes->SetLabel(*str);
	if ((str = GetSysIni("RunCompNo")) != NULL)
		m_runcompno->SetLabel(*str);
	if ((str = GetSysIni("RunCwin")) != NULL)
		m_runcwin->SetLabel(*str);
	if ((str = GetSysIni("RunCwinYes")) != NULL)
		m_runcwinyes->SetLabel(*str);
	if ((str = GetSysIni("RunCwinNo")) != NULL)
		m_runcwinno->SetLabel(*str);
	if ((str = GetSysIni("RunFts")) != NULL)
		m_runfts->SetLabel(*str);
	if ((str = GetSysIni("RunFtsYes")) != NULL)
		m_runftsyes->SetLabel(*str);
	if ((str = GetSysIni("RunFtsNo")) != NULL)
		m_runftsno->SetLabel(*str);
	if ((str = GetSysIni("RunJar")) != NULL)
		m_runjar->SetLabel(*str);
	if ((str = GetSysIni("RunJarYes")) != NULL)
		m_runjaryes->SetLabel(*str);
	if ((str = GetSysIni("RunJarNo")) != NULL)
		m_runjarno->SetLabel(*str);
	if ((str = GetSysIni("RunArch")) != NULL)
		m_runarch->SetLabel(*str);
	if ((str = GetSysIni("RunArchYes")) != NULL)
		m_runarchyes->SetLabel(*str);
	if ((str = GetSysIni("RunArchNo")) != NULL)
		m_runarchno->SetLabel(*str);
	if ((str = GetSysIni("RunShip")) != NULL)
		m_runship->SetLabel(*str);
	if ((str = GetSysIni("RunShipYes")) != NULL)
		m_runshipyes->SetLabel(*str);
	if ((str = GetSysIni("RunShipNo")) != NULL)
		m_runshipno->SetLabel(*str);
	if ((str = GetSysIni("RunOptAdv")) != NULL)
		m_runoptadv->SetLabel(*str);
	if ((str = GetSysIni("RunDcl")) != NULL)
		m_rundcl->SetLabel(*str);
	if ((str = GetSysIni("RunDclSrc")) != NULL)
		m_rundclsrc->SetLabel(*str);
	if ((str = GetSysIni("RunDclOut")) != NULL)
		m_rundclout->SetLabel(*str);
	if ((str = GetSysIni("RunDclNo")) != NULL)
		m_rundclno->SetLabel(*str);
	if ((str = GetSysIni("RunDclYes")) != NULL)
		m_rundclyes->SetLabel(*str);
	if ((str = GetSysIni("RunOnly")) != NULL)
		m_runonly->SetLabel(*str);
	if ((str = GetSysIni("RunOnlyYes")) != NULL)
		m_runonlyyes->SetLabel(*str);
	if ((str = GetSysIni("RunOnlyNo")) != NULL)
		m_runonlyno->SetLabel(*str);

	if ((str = GetSysIni("OptEditor")) != NULL)
		m_opteditor->SetLabel(*str);
	if ((str = GetSysIni("OptShowDel")) != NULL)
		m_optshdel->SetLabel(*str);
	if ((str = GetSysIni("OptShowDelYes")) != NULL)
		m_optshdelyes->SetLabel(*str);
	if ((str = GetSysIni("OptShowDelNo")) != NULL)
		m_optshdelno->SetLabel(*str);
	if ((str = GetSysIni("OptShow")) != NULL)
		m_optshow->SetLabel(*str);
	if ((str = GetSysIni("OptShowLog")) != NULL)
		m_optshowlog->SetLabel(*str);
	if ((str = GetSysIni("OptShowCon")) != NULL)
		m_optshowcon->SetLabel(*str);
	if ((str = GetSysIni("OptNewIni")) != NULL)
		m_optnewini->SetLabel(*str);
	if ((str = GetSysIni("OptLogFile")) != NULL)
		m_optlogfile->SetLabel(*str);
	if ((str = GetSysIni("OptProjIni")) != NULL)
		m_optprojini->SetLabel(*str);
	if ((str = GetSysIni("OptProjIniD2GBut")) != NULL)
		m_optprojinid2gbut->SetLabel(*str);
	if ((str = GetSysIni("OptProjIniLocBut")) != NULL)
		m_optprojinilocbut->SetLabel(*str);
	if ((str = GetSysIni("OptProjIniD2G")) != NULL)
		m_optprojinid2g->SetLabel(*str);
	if ((str = GetSysIni("OptProjIniLoc")) != NULL)
		m_optprojiniloc->SetLabel(*str);
	if ((str = GetSysIni("OptSet")) != NULL)
		m_optset->SetLabel(*str);
	if ((str = GetSysIni("OptReset")) != NULL)
		m_optreset->SetLabel(*str);
}


void D2gmain::SetAllResults(void)
{
	if (!SystemFileName
	 || !SysIni
	 || !SysIni->Section("Results"))
		return;

	wxString *str = NULL;

	if ((str = GetSysIni("CreateExistsText")) != NULL)
		CreateExistsText = str;
	else
		CreateExistsText = new wxString("Project exists, use a different name.");

	if ((str = GetSysIni("CreateNothingText")) != NULL)
		CreateNothingText = str;
	else
		CreateNothingText = new wxString("Project name is required.");

	if ((str = GetSysIni("CreatedProjectText")) != NULL)
		CreatedProjectText = str;
	else
		CreatedProjectText = new wxString("Created new project ");

	if ((str = GetSysIni("ModifiedProjectText")) != NULL)
		ModifiedProjectText = str;
	else
		ModifiedProjectText = new wxString("Modified project ");

	if ((str = GetSysIni("DeletedProjectText")) != NULL)
		DeletedProjectText = str;
	else
		DeletedProjectText = new wxString("Deleted project ");

	if ((str = GetSysIni("RestoredProjectText")) != NULL)
		RestoredProjectText = str;
	else
		RestoredProjectText = new wxString("Restored project ");

	if ((str = GetSysIni("RunningText")) != NULL)
		RunningText = str;
	else
		RunningText = new wxString("Running project ");

	if ((str = GetSysIni("RunningDoneText")) != NULL)
		RunningDoneText = str;
	else
		RunningDoneText = new wxString("Run completed for project ");

	if ((str = GetSysIni("RunningErrorText")) != NULL)
		RunningErrorText = str;
	else
		RunningErrorText = new wxString("Run failed for project ");

	if ((str = GetSysIni("RunningStopText")) != NULL)
		RunningStopText = str;
	else
		RunningStopText = new wxString("Run stopped for project ");

	if ((str = GetSysIni("RunningAsychText")) != NULL)
		RunningAsychText = str;
	else
		RunningAsychText = new wxString(" Wait for prompt, then Stop or close Console.");

	if ((str = GetSysIni("NoIniStart")) != NULL)
		NoIniStart = str;
	else
		NoIniStart = new wxString("Cannot find new .ini file: ");

	if ((str = GetSysIni("NoIniMessage")) != NULL)
		NoIniMessage = str;
	else
		NoIniMessage = new wxString("\nCorrect path in Options, then Modify Project to install it.");

	if ((str = GetSysIni("NoIniTitle")) != NULL)
		NoIniTitle = str;
	else
		NoIniTitle = new wxString("New .ini file missing");
}

wxArrayString *D2gmain::GetSysIniSect(const char *sect)
{
	if (!SystemFileName
	 || !SysIni
	 || !SysIni->Section(sect))
		return NULL;
	
	wxString *str = NULL;
	wxArrayString *arr = NULL;

	while (SysIni->NextItem()) {
		str = new wxString(SysIni->StrLeft());
		if (!arr)
			arr = new wxArrayString();
		arr->Add(*str);
	}

	return arr;
}

wxString *D2gmain::GetSysIni(const char *key)
{
	if (!SysIni->Find(key))
		return NULL;
	char *chp = SysIni->StrRight();
	if (!chp)
		return NULL;
	return new wxString((const wxChar*) Unescape(chp));
}


// OptRest requires clones of all functs

void OptRest::SetAllToolTips(void)
{
	if (!OptRest::ShowToolTips()
	 || !SystemFileName
	 || !SysIni
	 || !SysIni->Section("Tooltips"))
		return;

	wxString *str = NULL;

	if ((str = GetSysIni("OptRestTop")) != NULL)
		SetToolTip(*str);

	if ((str = GetSysIni("OptRestDlg")) != NULL)
		m_optrestdlg->SetToolTip(*str);
	if ((str = GetSysIni("OptRestSet")) != NULL)
		m_optrestset->SetToolTip(*str);
	if ((str = GetSysIni("OptRestStart")) != NULL)
		m_optreststart->SetToolTip(*str);
	if ((str = GetSysIni("OptRestDef")) != NULL)
		m_optrestdef->SetToolTip(*str);
}


void OptRest::SetAllLabels(void)
{
	if (!SystemFileName
	 || !SysIni
	 || !SysIni->Section("Labels"))
		return;

	wxString *str = NULL;

	if ((str = GetSysIni("OptRestTop")) != NULL)
		SetTitle(*str);

	if ((str = GetSysIni("OptRestDlg")) != NULL)
		m_optrestdlg->SetLabel(*str);
	if ((str = GetSysIni("OptRestSet")) != NULL)
		m_optrestset->SetLabel(*str);
	if ((str = GetSysIni("OptRestStart")) != NULL)
		m_optreststart->SetLabel(*str);
	if ((str = GetSysIni("OptRestDef")) != NULL)
		m_optrestdef->SetLabel(*str);
}


wxString *OptRest::GetSysIni(const char *key)
{
	if (!SysIni->Find(key))
		return NULL;
	char *chp = SysIni->StrRight();
	if (!chp)
		return NULL;
	return new wxString((const wxChar*) Unescape(chp));
}


// IniToEdit requires clones of all functs

void IniToEdit::SetAllToolTips(void)
{
	if (!IniToEdit::ShowToolTips()
	 || !SystemFileName
	 || !SysIni
	 || !SysIni->Section("Tooltips"))
		return;

	wxString *str = NULL;

	if ((str = GetSysIni("I2ETop")) != NULL)
		SetToolTip(*str);

	if ((str = GetSysIni("I2EDlg")) != NULL)
		m_i2edit->SetToolTip(*str);
	if ((str = GetSysIni("I2EStart")) != NULL)
		m_i2estart->SetToolTip(*str);
	if ((str = GetSysIni("I2EStartName")) != NULL)
		m_i2estartname->SetToolTip(*str);
	if ((str = GetSysIni("I2EDoc")) != NULL)
		m_i2edoc->SetToolTip(*str);
	if ((str = GetSysIni("I2EDocName")) != NULL)
		m_i2edocname->SetToolTip(*str);
}


void IniToEdit::SetAllLabels(void)
{
	if (!SystemFileName
	 || !SysIni
	 || !SysIni->Section("Labels"))
		return;

	wxString *str = NULL;

	if ((str = GetSysIni("I2ETop")) != NULL)
		SetTitle(*str);

	if ((str = GetSysIni("I2EDlg")) != NULL)
		m_i2edit->SetLabel(*str);
	if ((str = GetSysIni("I2EStart")) != NULL)
		m_i2estart->SetLabel(*str);
	if ((str = GetSysIni("I2EDoc")) != NULL)
		m_i2edoc->SetLabel(*str);
}


wxString *IniToEdit::GetSysIni(const char *key)
{
	if (!SysIni->Find(key))
		return NULL;
	char *chp = SysIni->StrRight();
	if (!chp)
		return NULL;
	return new wxString((const wxChar*) Unescape(chp));
}


// Configstart requires clones of all functs

void Configstart::SetAllToolTips(void)
{
	if (!Configstart::ShowToolTips()
	 || !SystemFileName
	 || !SysIni
	 || !SysIni->Section("Tooltips"))
		return;

	wxString *str = NULL;

	if ((str = GetSysIni("ConfigTop")) != NULL)
		SetToolTip(*str);

	if ((str = GetSysIni("ConfigAll")) != NULL)
		m_wizardpage1->m_configall->SetToolTip(*str);
	if ((str = GetSysIni("ConfigStart1")) != NULL)
		m_wizardpage1->m_configstart1->SetToolTip(*str);
	if ((str = GetSysIni("ConfigStart2")) != NULL)
		m_wizardpage1->m_configstart2->SetToolTip(*str);
	if ((str = GetSysIni("ConfigIniSel")) != NULL)
		m_wizardpage1->m_configinisel->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWAS")) != NULL)
		m_wizardpage1->m_configwas->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWAS2")) != NULL)
		m_wizardpage1->m_configwas2->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWASYes")) != NULL)
		m_wizardpage1->m_configwasyes->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWASNo")) != NULL)
		m_wizardpage1->m_configwasno->SetToolTip(*str);

	if ((str = GetSysIni("ConfigCond")) != NULL)
		m_wizardpage1a->m_configcond->SetToolTip(*str);
	if ((str = GetSysIni("ConfigStart3")) != NULL)
		m_wizardpage1a->m_configstart3->SetToolTip(*str);
	if ((str = GetSysIni("ConfigDvalFile")) != NULL)
		m_wizardpage1a->m_configdvalfile->SetToolTip(*str);
	if ((str = GetSysIni("ConfigDvalSel")) != NULL)
		m_wizardpage1a->m_configdvalsel->SetToolTip(*str);
	if ((str = GetSysIni("ConfigDvalNo")) != NULL)
		m_wizardpage1a->m_configdvalno->SetToolTip(*str);

	if ((str = GetSysIni("ConfigLocMsg")) != NULL)
		m_wizardpage1->m_configlocmsg->SetToolTip(*str);
	if ((str = GetSysIni("ConfigLocApp")) != NULL) {
		m_wizardpage1a->m_configdvalapp->SetToolTip(*str);
		m_wizardpage2->m_configanameapp->SetToolTip(*str);
		m_wizardpage2->m_configaverapp->SetToolTip(*str);
		m_wizardpage2a->m_configgrdapp->SetToolTip(*str);
		m_wizardpage2a->m_configgrfapp->SetToolTip(*str);
	}
	if ((str = GetSysIni("ConfigLocDoc")) != NULL) {
		m_wizardpage1a->m_configdvaldoc->SetToolTip(*str);
		m_wizardpage2->m_configanamedoc->SetToolTip(*str);
		m_wizardpage2->m_configaverdoc->SetToolTip(*str);
		m_wizardpage2a->m_configgrddoc->SetToolTip(*str);
		m_wizardpage2a->m_configgrfdoc->SetToolTip(*str);
	}
	if ((str = GetSysIni("ConfigLocStart")) != NULL) {
		m_wizardpage1a->m_configdvalstart->SetToolTip(*str);
		m_wizardpage2->m_configanamestart->SetToolTip(*str);
		m_wizardpage2->m_configaverstart->SetToolTip(*str);
		m_wizardpage2a->m_configgrdstart->SetToolTip(*str);
		m_wizardpage2a->m_configgrfstart->SetToolTip(*str);
	}

	if ((str = GetSysIni("ConfigArchive")) != NULL)
		m_wizardpage2->m_configarchive->SetToolTip(*str);
	if ((str = GetSysIni("ConfigAName")) != NULL)
		m_wizardpage2->m_configaname->SetToolTip(*str);
	if ((str = GetSysIni("ConfigANameTxt")) != NULL)
		m_wizardpage2->m_configanametxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigAVer")) != NULL)
		m_wizardpage2->m_configaver->SetToolTip(*str);
	if ((str = GetSysIni("ConfigAVerTxt")) != NULL)
		m_wizardpage2->m_configavertxt->SetToolTip(*str);

	if ((str = GetSysIni("ConfigGraph")) != NULL)
		m_wizardpage2a->m_configgraph->SetToolTip(*str);
	if ((str = GetSysIni("ConfigGrDir")) != NULL)
		m_wizardpage2a->m_configgrdir->SetToolTip(*str);
	if ((str = GetSysIni("ConfigGrSel")) != NULL)
		m_wizardpage2a->m_configgrsel->SetToolTip(*str);
	if ((str = GetSysIni("ConfigGrFiles")) != NULL)
		m_wizardpage2a->m_configgrfiles->SetToolTip(*str);
	if ((str = GetSysIni("ConfigGrTxt")) != NULL)
		m_wizardpage2a->m_configgrtxt->SetToolTip(*str);

	if ((str = GetSysIni("ConfigHTML")) != NULL)
		m_wizardpage3->m_confightml->SetToolTip(*str);
	if ((str = GetSysIni("ConfigTitle")) != NULL)
		m_wizardpage3->m_configtitle->SetToolTip(*str);
	if ((str = GetSysIni("ConfigTitleTxt")) != NULL)
		m_wizardpage3->m_configtitletxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigHP")) != NULL)
		m_wizardpage3->m_confighp->SetToolTip(*str);
	if ((str = GetSysIni("ConfigHPTxt")) != NULL)
		m_wizardpage3->m_confighpftxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigHTitle")) != NULL)
		m_wizardpage3->m_confightitle->SetToolTip(*str);
	if ((str = GetSysIni("ConfigHTitleTxt")) != NULL)
		m_wizardpage3->m_confightitletxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigHTopic")) != NULL)
		m_wizardpage3->m_confightopic->SetToolTip(*str);
	if ((str = GetSysIni("ConfigHTopicTxt")) != NULL)
		m_wizardpage3->m_confightopictxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigOHTL")) != NULL)
		m_wizardpage3->m_configohtl->SetToolTip(*str);
	if ((str = GetSysIni("ConfigOHTLTxt")) != NULL)
		m_wizardpage3->m_configohtltxt->SetToolTip(*str);

	if ((str = GetSysIni("ConfigEH")) != NULL)
		m_wizardpage4->m_configeh->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHTitle")) != NULL)
		m_wizardpage4->m_configehtitle->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHTitleTxt")) != NULL)
		m_wizardpage4->m_configehtitletxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHPName")) != NULL)
		m_wizardpage4->m_configehpn->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHPNameTxt")) != NULL)
		m_wizardpage4->m_configehpntxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHPID")) != NULL)
		m_wizardpage4->m_configehpid->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHPIDTxt")) != NULL)
		m_wizardpage4->m_configehpidtxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHPProvider")) != NULL)
		m_wizardpage4->m_configehprov->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHPProviderTxt")) != NULL)
		m_wizardpage4->m_configehprovtxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHTopic")) != NULL)
		m_wizardpage4->m_configehtopic->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEHTopicTxt")) != NULL)
		m_wizardpage4->m_configehtopictxt->SetToolTip(*str);

	if ((str = GetSysIni("ConfigRtf")) != NULL)
		m_wizardpage5->m_configrtf->SetToolTip(*str);
	if ((str = GetSysIni("ConfigRtfID")) != NULL)
		m_wizardpage5->m_configrtfid->SetToolTip(*str);
	if ((str = GetSysIni("ConfigRtfIDTxt")) != NULL)
		m_wizardpage5->m_configrtfidtxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWV")) != NULL)
		m_wizardpage5->m_configwv->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWV97")) != NULL)
		m_wizardpage5->m_configwv97->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWV2K")) != NULL)
		m_wizardpage5->m_configwv2k->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWVXP")) != NULL)
		m_wizardpage5->m_configwvxp->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWV2003")) != NULL)
		m_wizardpage5->m_configwv2003->SetToolTip(*str);
	if ((str = GetSysIni("ConfigHPJ")) != NULL)
		m_wizardpage5->m_confighpj->SetToolTip(*str);
	if ((str = GetSysIni("ConfigHPJTxt")) != NULL)
		m_wizardpage5->m_confighpjtxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigCNTTitle")) != NULL)
		m_wizardpage5->m_configcnttitle->SetToolTip(*str);
	if ((str = GetSysIni("ConfigCNTTitleTxt")) != NULL)
		m_wizardpage5->m_configcnttitletxt->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWHTopic")) != NULL)
		m_wizardpage5->m_configwhtopic->SetToolTip(*str);
	if ((str = GetSysIni("ConfigWHTopicTxt")) != NULL)
		m_wizardpage5->m_configwhtopictxt->SetToolTip(*str);

	if ((str = GetSysIni("ConfigReview")) != NULL)
		m_wizardpageend->m_configreview->SetToolTip(*str);
	if ((str = GetSysIni("ConfigEnd")) != NULL)
		m_wizardpageend->m_configend->SetToolTip(*str);
	if ((str = GetSysIni("ConfigRevText")) != NULL)
		m_wizardpageend->m_configrevtext->SetToolTip(*str);
}


void Configstart::SetAllLabels(void)
{
	if (!SystemFileName
	 || !SysIni
	 || !SysIni->Section("Labels"))
		return;

	wxString *str = NULL;

	if ((str = GetSysIni("ConfigTop")) != NULL)
		SetTitle(*str);

	if ((str = GetSysIni("ConfigAll")) != NULL)
		m_wizardpage1->m_configall->SetLabel(*str);
	if ((str = GetSysIni("ConfigStart1")) != NULL)
		m_wizardpage1->m_configstart1->SetLabel(*str);
	if ((str = GetSysIni("ConfigStart2")) != NULL)
		m_wizardpage1->m_configstart2->SetLabel(*str);
	if ((str = GetSysIni("ConfigWAS")) != NULL)
		m_wizardpage1->m_configwas->SetLabel(*str);
	if ((str = GetSysIni("ConfigWAS2")) != NULL)
		m_wizardpage1->m_configwas2->SetLabel(*str);
	if ((str = GetSysIni("ConfigWASYes")) != NULL)
		m_wizardpage1->m_configwasyes->SetLabel(*str);
	if ((str = GetSysIni("ConfigWASNo")) != NULL)
		m_wizardpage1->m_configwasno->SetLabel(*str);

	if ((str = GetSysIni("ConfigCond")) != NULL)
		m_wizardpage1a->m_configcond->SetLabel(*str);
	if ((str = GetSysIni("ConfigStart3")) != NULL)
		m_wizardpage1a->m_configstart3->SetLabel(*str);
	if ((str = GetSysIni("ConfigDvalFile")) != NULL)
		m_wizardpage1a->m_configdvalfile->SetLabel(*str);
	if ((str = GetSysIni("ConfigDvalNo")) != NULL)
		m_wizardpage1a->m_configdvalno->SetLabel(*str);

	if ((str = GetSysIni("ConfigLocMsg")) != NULL)
		m_wizardpage1->m_configlocmsg->SetLabel(*str);
	if ((str = GetSysIni("ConfigLocApp")) != NULL) {
		m_wizardpage1a->m_configdvalapp->SetLabel(*str);
		m_wizardpage2->m_configanameapp->SetLabel(*str);
		m_wizardpage2->m_configaverapp->SetLabel(*str);
		m_wizardpage2a->m_configgrdapp->SetLabel(*str);
		m_wizardpage2a->m_configgrfapp->SetLabel(*str);
	}
	if ((str = GetSysIni("ConfigLocDoc")) != NULL) {
		m_wizardpage1a->m_configdvaldoc->SetLabel(*str);
		m_wizardpage2->m_configanamedoc->SetLabel(*str);
		m_wizardpage2->m_configaverdoc->SetLabel(*str);
		m_wizardpage2a->m_configgrddoc->SetLabel(*str);
		m_wizardpage2a->m_configgrfdoc->SetLabel(*str);
	}
	if ((str = GetSysIni("ConfigLocStart")) != NULL) {
		m_wizardpage1a->m_configdvalstart->SetLabel(*str);
		m_wizardpage2->m_configanamestart->SetLabel(*str);
		m_wizardpage2->m_configaverstart->SetLabel(*str);
		m_wizardpage2a->m_configgrdstart->SetLabel(*str);
		m_wizardpage2a->m_configgrfstart->SetLabel(*str);
	}

	if ((str = GetSysIni("ConfigArchive")) != NULL)
		m_wizardpage2->m_configarchive->SetLabel(*str);
	if ((str = GetSysIni("ConfigAName")) != NULL)
		m_wizardpage2->m_configaname->SetLabel(*str);
	if ((str = GetSysIni("ConfigAVer")) != NULL)
		m_wizardpage2->m_configaver->SetLabel(*str);

	if ((str = GetSysIni("ConfigGraph")) != NULL)
		m_wizardpage2a->m_configgraph->SetLabel(*str);
	if ((str = GetSysIni("ConfigGrDir")) != NULL)
		m_wizardpage2a->m_configgrdir->SetLabel(*str);
	if ((str = GetSysIni("ConfigGrFiles")) != NULL)
		m_wizardpage2a->m_configgrfiles->SetLabel(*str);

	if ((str = GetSysIni("ConfigHTML")) != NULL)
		m_wizardpage3->m_confightml->SetLabel(*str);
	if ((str = GetSysIni("ConfigTitle")) != NULL)
		m_wizardpage3->m_configtitle->SetLabel(*str);
	if ((str = GetSysIni("ConfigHP")) != NULL)
		m_wizardpage3->m_confighp->SetLabel(*str);
	if ((str = GetSysIni("ConfigHTitle")) != NULL)
		m_wizardpage3->m_confightitle->SetLabel(*str);
	if ((str = GetSysIni("ConfigHTopic")) != NULL)
		m_wizardpage3->m_confightopic->SetLabel(*str);
	if ((str = GetSysIni("ConfigOHTL")) != NULL)
		m_wizardpage3->m_configohtl->SetLabel(*str);

	if ((str = GetSysIni("ConfigEH")) != NULL)
		m_wizardpage4->m_configeh->SetLabel(*str);
	if ((str = GetSysIni("ConfigEHTitle")) != NULL)
		m_wizardpage4->m_configehtitle->SetLabel(*str);
	if ((str = GetSysIni("ConfigEHPName")) != NULL)
		m_wizardpage4->m_configehpn->SetLabel(*str);
	if ((str = GetSysIni("ConfigEHPID")) != NULL)
		m_wizardpage4->m_configehpid->SetLabel(*str);
	if ((str = GetSysIni("ConfigEHPProvider")) != NULL)
		m_wizardpage4->m_configehprov->SetLabel(*str);
	if ((str = GetSysIni("ConfigEHTopic")) != NULL)
		m_wizardpage4->m_configehtopic->SetLabel(*str);

	if ((str = GetSysIni("ConfigRtf")) != NULL)
		m_wizardpage5->m_configrtf->SetLabel(*str);
	if ((str = GetSysIni("ConfigRtfID")) != NULL)
		m_wizardpage5->m_configrtfid->SetLabel(*str);
	if ((str = GetSysIni("ConfigWV")) != NULL)
		m_wizardpage5->m_configwv->SetLabel(*str);
	if ((str = GetSysIni("ConfigWV97")) != NULL)
		m_wizardpage5->m_configwv97->SetLabel(*str);
	if ((str = GetSysIni("ConfigWV2K")) != NULL)
		m_wizardpage5->m_configwv2k->SetLabel(*str);
	if ((str = GetSysIni("ConfigWVXP")) != NULL)
		m_wizardpage5->m_configwvxp->SetLabel(*str);
	if ((str = GetSysIni("ConfigWV2003")) != NULL)
		m_wizardpage5->m_configwv2003->SetLabel(*str);
	if ((str = GetSysIni("ConfigHPJ")) != NULL)
		m_wizardpage5->m_confighpj->SetLabel(*str);
	if ((str = GetSysIni("ConfigCNTTitle")) != NULL)
		m_wizardpage5->m_configcnttitle->SetLabel(*str);
	if ((str = GetSysIni("ConfigWHTopic")) != NULL)
		m_wizardpage5->m_configwhtopic->SetLabel(*str);

	if ((str = GetSysIni("ConfigReview")) != NULL)
		m_wizardpageend->m_configreview->SetLabel(*str);
	if ((str = GetSysIni("ConfigEnd")) != NULL)
		m_wizardpageend->m_configend->SetLabel(*str);
}


wxString *Configstart::GetSysIni(const char *key)
{
	if (!SysIni->Find(key))
		return NULL;
	char *chp = SysIni->StrRight();
	if (!chp)
		return NULL;
	return new wxString((const wxChar*) Unescape(chp));
}


// Options setting processing


void D2gmain::GetOptionsSettings(void)
{
	Editor = GetProjStr("Options", "Editor");
	if (!Editor) {
		Editor = new wxString();
		*Editor = _("notepad");
	}
  InitEditor = NewName(Editor->c_str());

	CMStart = new wxString();
	*CMStart = _("d2gcm -s ");

	InitShowDelInCreate = ShowDelInCreate = GetProjBool("Options", "ShowDelInCreate");
	InitShowLog = ShowLog = GetProjBool("Options", "ShowLog");
	InitShowConsole = ShowConsole = GetProjBool("Options", "ShowConsole");

	NewIniPath = FixEnvPath(GetProjStr("Options", "NewIniPath"));
  InitNewIniPath = NewName(NewIniPath->c_str());

	LogFileName = GetProjStr("Options", "LogFileName");
	if (!LogFileName) {
		LogFileName = new wxString();
#ifdef DITA
		*LogFileName = _("_d2g_log.txt");
#endif
#ifdef UDOC
		*LogFileName = _("_x2g_log.txt");
#endif
	}
	InitLogFileName = NewName(LogFileName->c_str());

	InitProjDirLocal = ProjDirLocal = GetProjBool("Options", "LocalProjDir");
	ProjDirD2GPar = FixEnvPath(GetProjStr("Options", "CentralProjDir"));
	InitProjDirD2GPar = new wxString(NewName(ProjDirD2GPar->c_str()));
	ProjDirLocRel = GetProjStr("Options", "LocalProjDirRel");
	InitProjDirLocRel = new wxString(NewName(ProjDirLocRel->c_str()));

	SetCreateProjIni = true;
}


void D2gmain::GetOptionsValues(void)
{
	wxString wxs = m_opteditortext->GetValue();
	Editor = new wxString(wxs);

	ShowDelInCreate = m_optshdelyes->GetValue();
	ShowLog = m_optshowlog->GetValue();
	ShowConsole = m_optshowcon->GetValue();

	NewIniPath = new wxString(m_optnewinisel->GetPath());

	wxString wxs2 = m_optlogfiletext->GetValue();
	LogFileName = new wxString(wxs2);

	//ProjDirUsed = !m_optprojininobut->GetValue();
	ProjDirLocal = m_optprojinilocbut->GetValue();
	ProjDirD2GPar = new wxString(m_optprojinid2gsel->GetPath());
	wxString wxs3 = m_optprojiniloctext->GetValue();
	ProjDirLocRel = new wxString(wxs3);
}


void D2gmain::GetStartOptions(void)
{
	Editor->Empty();
	Editor->Append(InitEditor);

	ShowDelInCreate = InitShowDelInCreate;
	ShowLog = InitShowLog;
	ShowConsole = InitShowConsole;

	NewIniPath->Empty();
	NewIniPath->Append(InitNewIniPath);

	LogFileName->Empty();
	LogFileName->Append(InitLogFileName);

	//ProjDirUsed = InitProjDirUsed;
	ProjDirLocal = InitProjDirLocal;
	ProjDirD2GPar->Empty();
	ProjDirD2GPar->Append(*InitProjDirD2GPar);
	ProjDirLocRel->Empty();
	ProjDirLocRel->Append(*InitProjDirLocRel);
}


void D2gmain::GetDefOptions(void)
{
	Editor->Empty();
	Editor->Append((const wxChar *) "notepad");

	ShowDelInCreate = false;
	ShowLog = false;
	ShowConsole = false;

	NewIniPath->Empty();
#ifdef DITA
	NewIniPath->Append((const wxChar *) "%omsyshome%\\d2g\\local\\starts");
#endif
#ifdef UDOC
	NewIniPath->Append((const wxChar *) "%omsyshome%\\x2g\\local\\starts");
#endif

	LogFileName->Empty();
#ifdef DITA
	LogFileName->Append((const wxChar *) "_d2g_log.txt");
#endif
#ifdef UDOC
	LogFileName->Append((const wxChar *) "_x2g_log.txt");
#endif

	//ProjDirUsed = true;
	ProjDirLocal = false;
	ProjDirD2GPar->Empty();
#ifdef DITA
	ProjDirD2GPar->Append((const wxChar *) "%omsyshome%\\d2g\\documents");
#endif
#ifdef UDOC
	ProjDirD2GPar->Append((const wxChar *) "%omsyshome%\\x2g\\documents");
#endif
	ProjDirLocRel->Empty();
	ProjDirLocRel->Append((const wxChar *) "..\\_config");
}


void D2gmain::SetOptionsValues(void)
{
	m_opteditortext->SetValue(*Editor);

	if (ShowDelInCreate)
		m_optshdelyes->SetValue(true);
	else
		m_optshdelno->SetValue(true);

	m_optshowlog->SetValue(ShowLog);
	m_optshowcon->SetValue(ShowConsole);

	m_optnewinisel->SetPath(*NewIniPath);

	m_optlogfiletext->SetValue(*LogFileName);

	//if (!ProjDirUsed)
	//	m_optprojininobut->SetValue(true);
	//else 
	if (ProjDirLocal)
		m_optprojinilocbut->SetValue(true);
	else
		m_optprojinid2gbut->SetValue(true);

	m_optprojinid2gsel->SetPath(*ProjDirD2GPar);
	m_optprojiniloctext->SetValue(*ProjDirLocRel);
}


void D2gmain::UpdateOptions(void)
{
#ifdef DITA
	SetProjInt("Options", "d2gpmVersion", d2gpmVersion);
#endif
#ifdef UDOC
	SetProjInt("Options", "x2gpmVersion", d2gpmVersion);
#endif

	InitPos = new wxPoint(GetScreenPosition());
	SetProjInt("Options", "PosX", InitPos->x);
	SetProjInt("Options", "PosY", InitPos->y);

	InitSize = new wxSize(GetSize());
	SetProjInt("Options", "SizeX", InitSize->GetWidth());
	SetProjInt("Options", "SizeY", InitSize->GetHeight());

	SetProjStr("Options", "Editor", Editor->c_str());

	SetProjBool("Options", "ShowDelInCreate", ShowDelInCreate);

	SetProjBool("Options", "ShowLog", ShowLog);
	SetProjBool("Options", "ShowConsole", ShowConsole);

	SetProjStr("Options", "NewIniPath", NewIniPath->c_str());

	SetProjStr("Options", "LogFileName", LogFileName->c_str());

	//SetProjBool("Options", "ProjDirUsed", ProjDirUsed);
	SetProjBool("Options", "LocalProjDir", ProjDirLocal);
	SetProjStr("Options", "CentralProjDir", ProjDirD2GPar->c_str());
	SetProjStr("Options", "LocalProjDirRel", ProjDirLocRel->c_str());

	UpdateProjectsFile();
}


void D2gmain::UpdateProjectsFile(void)
{
	DCpfile::pclose(ProjectFileName);  // was pupdate
}


// update projects.ini and close it
void D2gmain::CloseProjectsFile(void)
{
	UpdateOptions();
  DCpfile::pflushall();
}


// Project file reading

wxString *D2gmain::GetProjStr(const char *sect, const char *key)
{
	unl len = DCpfile::getval((char *) sect, (char *) key, "", ProjBuf, PBSIZE,
												    ProjectFileName);
	return (len ? new wxString((const wxChar*) ProjBuf, len) : NULL);
}

wxArrayString *D2gmain::GetProjSect(const char *sect)
{
	unl len = DCpfile::getval((char *) sect, NULL, "", ProjBuf, PBSIZE,
	                          ProjectFileName);
	unl nm = 0;
	wxString *str = NULL;
	wxArrayString *arr = NULL;
	for (unl i = 0; i < len; i += (nm + 1)) {
		if ((nm = strlen(ProjBuf + i)) == 0)
			break;
		str = new wxString((const wxChar*)ProjBuf + i, nm);
		if (!arr)
			arr = new wxArrayString();
		arr->Add(*str);
	}
	return arr;
}

long D2gmain::GetProjInt(const char *sect, const char *key)
{
	DCpfile::getval((char *) sect, (char *) key, "0", ProjBuf, PBSIZE,
												ProjectFileName);
	return atol(ProjBuf);
}

bool D2gmain::GetProjBool(const char *sect, const char *key)
{
	DCpfile::getval((char *) sect, (char *) key, "", ProjBuf, PBSIZE,
												ProjectFileName);

	if (!stricmp(ProjBuf, "yes")
	 || *ProjBuf == '1')
		return true;

	if (!stricmp(ProjBuf, "no")
	 || *ProjBuf == '0')
		return false;

	return false;
}

// project writing

void D2gmain::SetProjStr(const char *sect, const char *key, const char *val)
{
	DCpfile::putval((char *) sect, (char *) key, (char *) val, ProjectFileName);
}

void D2gmain::SetProjInt(const char *sect, const char *key, long num)
{
	char val[34];
	_ltoa(num, val, 10);
	DCpfile::putval((char *) sect, (char *) key, val, ProjectFileName);
}

void D2gmain::SetProjBool(const char *sect, const char *key, bool val)
{
	DCpfile::putval((char *) sect, (char *) key, (val ? "Yes" : "No"), ProjectFileName);
}


// ini file reading

wxString *D2gmain::GetIniStr(const char *sect, const char *key, char *ini)
{
	unl len = DCpfile::getval((char *) sect, (char *) key, "", ProjBuf, PBSIZE, ini);
	return (len ? new wxString((const wxChar*) ProjBuf, len) : NULL);
}

bool D2gmain::GetIniBool(const char *sect, const char *key, char *ini)
{
	DCpfile::getval((char *) sect, (char *) key, "", ProjBuf, PBSIZE, ini);

	if (!stricmp(ProjBuf, "yes")
	 || *ProjBuf == '1')
		return true;

	if (!stricmp(ProjBuf, "no")
	 || *ProjBuf == '0')
		return false;

	return false;
}

// ini file writing

void D2gmain::SetIniStr(const char *sect, const char *key, const char *val, char *ini)
{
	DCpfile::putval((char *) sect, (char *) key, (char *) val, ini);
}

void D2gmain::SetIniBool(const char *sect, const char *key, bool val, char *ini)
{
	DCpfile::putval((char *) sect, (char *) key, (val ? "Yes" : "No"), ini);
}



// determining matching projects


char *D2gmain::GetSharedProjectName(void)
{
	// called after Create, or Modify if a new starting .ini is needed


	if (CurrProjFile)  // already selected one
		return NULL;

	SharedSrcDir.clear();
	SharedSrcMap.clear();
	SharedSrcID.clear();
	char *name = NULL;

	// scan active projects for any with same source dir or map
	int act = ActiveProjects->GetCount();

	if (act == 1)  // no other projects to match
		return NULL;

	wxString *prj = NULL;
	wxString *str = NULL;
	wxString *dir = NULL;
	wxString *map = NULL;
	wxString *pdir = NULL;

	for (int i = 0; i < act; i++) {
		prj = new wxString((ActiveProjects->Item(i)).c_str());
		str = GetProjStr("Active", prj->c_str());
		if (!str->Cmp(*CurrProjectID))
			continue;
		dir = GetProjStr("SourceDir", str->c_str());
		if (!dir->Cmp(*CurrSourceDir)) {
			int v = atoi(str->c_str());
			SharedSrcID.addnew(str->c_str(), v);
			map = GetProjStr("SourceMap", str->c_str());
			//UMXL = map->Matches(_("*.mx?"));
			if (!map->Cmp(*CurrSourceMap))
				SharedSrcMap.addnew(prj->c_str(), v);
			else
				SharedSrcDir.addnew(prj->c_str(), v);
		}
	}

	if (SharedSrcMap.count()) {
		DCnlist *mp = SharedSrcMap.next;
		while (mp) {
			pdir = GetProjStr("ProjDir", SharedSrcID.find(mp->id));
			if (pdir)
				break;  // project dir exists, use it
			mp = mp->next;
		}
		// if some with same map, use map name as dir name for central
		if (!pdir)
			name = NewName(CurrSourceDir->BeforeLast('.').c_str());
	}
	if (!name
	 && !pdir
	 && SharedSrcDir.count()) {
		DCnlist *md = SharedSrcDir.next;
		while (md) {
			pdir = GetProjStr("ProjDir", SharedSrcID.find(md->id));
			if (pdir)
				break;  // project dir exists, use it
			md = md->next;
		}
		// if only same dir, use that dir name for central unless generic
		if (!pdir) {
			name = NewName(CurrSourceDir->AfterLast('\\').c_str());
			// otherwise see if project names have common beginning, use that
			// as last resort, use "projectN" where N increments
			if (!name) {
				NextProjDirNum = GetProjInt("Projects", "NextDirNum");
				if (!NextProjDirNum)
					NextProjDirNum = 1;
				name = NewName(wxString::Format("Project%d", NextProjDirNum).c_str());
				SetProjInt("Projects", "NextDirNum", ++NextProjDirNum);
			}
		}
	}

	if (pdir) {  // use existing project dir
		CurrProjDir = pdir;
		return NULL;
	}

	// if no such matches, return NULL
	return name;
}



// getting list control data

// read projects.ini and populate active project list
void D2gmain::GetActiveProjects(void)
{
	// set CurrProject per [Projects]LastActive
	CurrProject = GetProjStr("Projects", "LastActive");
	if (CurrProject)
		CurrProjectID = GetProjStr("Active", (char *) CurrProject->c_str());
  else
    CurrProjectID = new wxString((const wxChar*) "0", 1);

	// set ActiveProjects from ini section
	if (ActiveProjects)
		delete ActiveProjects;
	ActiveProjects = GetProjSect("Active");
}



// read projects.ini and populate deleted project list
void D2gmain::GetDeletedProjects(void)
{
	// set CurrProjectID per [Projects]LastDeleted
	CurrDelProject = GetProjStr("Projects", "LastDeleted");
	if (CurrDelProject)
		CurrDelProjectID = GetProjStr("Deleted", (char *) CurrDelProject->c_str());
  else
    CurrDelProjectID = new wxString((const wxChar*) "0", 1);

	// set DeletedProjects from ini section
	if (DeletedProjects)
		delete DeletedProjects;
	DeletedProjects = GetProjSect("Deleted");
}


// read projects.ini and populate output types list
void D2gmain::GetAllFormats(void)
{
	if (!SystemFileName
	 || !SysIni)
		return;

	// set OutputFormats and OutputInis from ini section
  OutputFormats = GetSysIniSect("OutputTypes");
	if (!OutputFormats
	 || !SysIni->Section("OutputTypes"))
		return;

  long cnt = OutputFormats->GetCount();
  if (OutputInis)
    delete OutputInis;
  OutputInis = new wxArrayString();
  OutputInis->Alloc(cnt);
    
  for (long i = 0; i < cnt; i++)
    OutputInis->Add(*GetSysIni(OutputFormats->Item(i).c_str()));

	if (CurrProjectID
	 && strcmp(CurrProjectID->c_str(), "0"))
		CurrFormat = GetProjStr("OutputType", CurrProjectID->c_str());
	else
		CurrFormat = new wxString((OutputFormats->Item(0)).c_str());
}


// when project changed, get project info for listboxes
void D2gmain::GetProjectInfo(void)
{
	// get info for CurrProjectID, set CurrSourceDir, CurrSourceMap,
	// CurrOutputDir, CurrFormat, and CurrIni
  if (CurrProjectID
	 && strcmp(CurrProjectID->c_str(), "0")) {
    CurrSourceDir = FixEnvPath(GetProjStr("SourceDir", CurrProjectID->c_str()));
    CurrSourceMap = FixEnvPath(GetProjStr("SourceMap", CurrProjectID->c_str()));
		//UMXL = CurrSourceMap->Matches(_("*.mx?"));
    CurrOutputDir = FixEnvPath(GetProjStr("OutputDir", CurrProjectID->c_str()));
    CurrFormat = GetProjStr("OutputType", CurrProjectID->c_str());
		GetCurrIni(CurrFormat);
		GetProjectFileData(CurrProjectID->c_str(), CurrOutputDir);
  }
}

void D2gmain::GetDelProjectInfo(void)
{
	// get info for CurrProjectID, set CurrSourceDir, CurrSourceMap,
	// CurrOutputDir, CurrFormat, and CurrIni
  if (CurrDelProjectID
	 && strcmp(CurrDelProjectID->c_str(), "0")) {
    CurrDelSourceDir = GetProjStr("SourceDir", CurrDelProjectID->c_str());
    CurrDelSourceMap = GetProjStr("SourceMap", CurrDelProjectID->c_str());
    CurrDelOutputDir = GetProjStr("OutputDir", CurrDelProjectID->c_str());
    CurrDelFormat = GetProjStr("OutputType", CurrDelProjectID->c_str());
		GetCurrIni(CurrDelFormat);
		GetProjectFileData(CurrDelProjectID->c_str(), CurrDelOutputDir);
  }
}

void D2gmain::GetCurrIni(wxString *format)
{
	if (!format
	 || !SystemFileName
	 || !SysIni
	 || !SysIni->Section("OutputTypes"))
		return;

  CurrType = GetSysIni(format->c_str());
	CurrIni = new wxString();
	*CurrIni += UMXL ? _("_x2") : _("_d2");
	*CurrIni += *CurrType;
	*CurrIni += _(".ini");
}


void D2gmain::GetProjectFileData(const char *id, wxString *outdir)
{
	if (!outdir)
		return;

	//CurrProjDirUsed = GetProjBool("ProjDirUsed", id);
	CurrProjDirLocal = GetProjBool("LocalProjDir", id);
	CurrProjDir = FixEnvPath(GetProjStr("ProjDir", id));
	CurrProjFile = FixEnvPath(GetProjStr("ProjFile", id));
	//if (!CurrProjDirUsed) {
	//	CurrProjDirLoc = NULL;
	//	CurrProjDirD2G = NULL;
	//}
	//else 
	if (CurrProjDirLocal) {
		if (CurrProjDir)
			CurrProjDirLoc = new wxString(*CurrProjDir);
		CurrProjDirD2G = new wxString(*ProjDirD2GPar);
	}
	else {
		CurrProjDirLoc = MakeFullPath(outdir, ProjDirLocRel);
		if (CurrProjDir)
			CurrProjDirD2G = new wxString(*CurrProjDir);
	}

	CurrDitavalUsed = GetProjBool("DvalUsed", id);
	CurrDitaval = FixEnvPath(GetProjStr("DvalFile", id));
}


void D2gmain::GetCurrProjectID(void)
{
  if (CurrProject)
    CurrProjectID = GetProjStr("Active", (char *) CurrProject->c_str());
  SetProjStr("Projects", "LastActive", CurrProject->c_str());
	UpdateProjectsFile();
}


void D2gmain::GetCurrDelProjectID(void)
{
  if (CurrDelProject)
    CurrDelProjectID = GetProjStr("Deleted", (char *) CurrDelProject->c_str());
  SetProjStr("Projects", "LastDeleted", CurrDelProject->c_str());
	UpdateProjectsFile();
}


// see if project already exists under the current name
bool D2gmain::CurrProjectExists(void)
{
  if (!CurrProject)
    return false;

  if (GetProjInt("Active", (char *) CurrProject->c_str()) != 0)
    return true;
  if (ShowDelInCreate
	 && (GetProjInt("Deleted", (char *) CurrProject->c_str()) != 0))
    return true;

  return false;
}


void D2gmain::SelectCreateProject(wxString *project)
{
	wxString *lastproj = NULL;
	wxString *lastid = NULL;

  if (GetProjInt("Active", (char *) project->c_str()) != 0) {
		lastproj = CurrProject;
		lastid = CurrProjectID;
		CurrProject = project;
    GetCurrProjectID();
    GetProjectInfo();
		SetCreateProjectControls(true);
		CurrProjectID = lastid;
		CurrProject = lastproj;
  }
	else if (ShowDelInCreate) {  // get deleted project settings
		lastproj = CurrDelProject;
		lastid = CurrDelProjectID;
		CurrDelProject = project;
    GetCurrDelProjectID();
    GetDelProjectInfo();
		SetCreateProjectControls(false);
		CurrDelProjectID = lastid;
		CurrDelProject = lastproj;
	}
	m_editini->Disable();
	m_createcm->Disable();
	m_iniconfig->Disable();
	m_runeditini->Disable();
	m_runviewout->Disable();
	m_runcm->Disable();
}


wxString *D2gmain::GetCreateSourceMap(void)
{
	// given CreateSourceDir, try to find a bookmap, ditamap, or dita file
	// use long _findfirst(char *filespec, struct _finddata_t *fileinfo)
	// failure return is -1, else fileinfo->name has name found, max 256
	// fileinfo->attrib may have _A_SUBDIR (0x10) if named file is a subdir
	struct _finddata_t fileinfo;
	//UMXL = false;

	if (!wxSetWorkingDirectory(*CreateSourceDir))
		return CurrSourceMap;

	if ((_findfirst("*.bookmap", &fileinfo) != -1)
	 && ((fileinfo.attrib & _A_SUBDIR) != _A_SUBDIR)
	 && (fileinfo.name[0] != '\0'))
		return new wxString(fileinfo.name);

	if ((_findfirst("*.ditamap", &fileinfo) != -1)
	 && ((fileinfo.attrib & _A_SUBDIR) != _A_SUBDIR)
	 && (fileinfo.name[0] != '\0'))
		return new wxString(fileinfo.name);

	if ((_findfirst("*.dita", &fileinfo) != -1)
	 && ((fileinfo.attrib & _A_SUBDIR) != _A_SUBDIR)
	 && (fileinfo.name[0] != '\0'))
		return new wxString(fileinfo.name);

	if ((_findfirst("*.mxm", &fileinfo) != -1)
	 && ((fileinfo.attrib & _A_SUBDIR) != _A_SUBDIR)
	 && (fileinfo.name[0] != '\0')) {
		//UMXL = true;
		return new wxString(fileinfo.name);
	}

	if ((_findfirst("*.mxd", &fileinfo) != -1)
	 && ((fileinfo.attrib & _A_SUBDIR) != _A_SUBDIR)
	 && (fileinfo.name[0] != '\0')) {
		//UMXL = true;
		return new wxString(fileinfo.name);
	}

	return CurrSourceMap;
}


void D2gmain::SetCreateProjectControls(bool active)
{
	if (active) {
		if (CurrSourceDir)
			m_srcdirselect->SetPath(*CurrSourceDir);

		if (CurrSourceDir
		 && CurrSourceMap)
			m_srcmapselect->SetPath(*MakeFullPath(CurrSourceDir, CurrSourceMap));

		if (CurrFormat)
			m_formatselect->SetStringSelection(*CurrFormat);

		if (CurrOutputDir)
			m_outdirselect->SetPath(*CurrOutputDir);
	}
	else {
		if (CurrDelSourceDir)
			m_srcdirselect->SetPath(*CurrDelSourceDir);

		if (CurrDelSourceDir
		 && CurrDelSourceMap)
			m_srcmapselect->SetPath(*MakeFullPath(CurrDelSourceDir, CurrDelSourceMap));

		if (CurrDelFormat)
			m_formatselect->SetStringSelection(*CurrDelFormat);

		if (CurrDelOutputDir)
			m_outdirselect->SetPath(*CurrDelOutputDir);
	}

#if 0
	if (!CurrProjDirUsed)
		m_projinino->SetValue(true);
	else if (CurrProjDirLocal)
		m_projiniloc->SetValue(true);
	else
		m_projinid2g->SetValue(true);
#endif
}


void D2gmain::SetModifyProjectControls(void)
{
	m_mprojectselect->SetStringSelection(*CurrProject);

	if (CurrSourceDir)
		m_msrcdirselect->SetPath(*CurrSourceDir);

	if (CurrSourceDir
	 && CurrSourceMap)
		m_msrcmapselect->SetPath(*MakeFullPath(CurrSourceDir, CurrSourceMap));

	if (CurrFormat)
		m_mformatselect->SetStringSelection(*CurrFormat);

	if (CurrOutputDir)
		m_moutdirselect->SetPath(*CurrOutputDir);

#if 0
	if (!CurrProjDirUsed)
		m_mprojinino->SetValue(true);
	else if (CurrProjDirLocal)
		m_mprojiniloc->SetValue(true);
	else
		m_mprojinid2g->SetValue(true);
#endif
}


void D2gmain::SetDeleteProjectControls(void)
{
	m_dprojectselect->SetStringSelection(*CurrProject);

	if (CurrSourceDir) {
		m_dsrcdirtext->Clear();
		m_dsrcdirtext->AppendText(*CurrSourceDir);
	}

	if (CurrSourceDir
	 && CurrSourceMap) {
		m_dsrcmaptext->Clear();
		m_dsrcmaptext->AppendText(*MakeFullPath(CurrSourceDir, CurrSourceMap));
	}

	if (CurrFormat) {
		m_dformattext->Clear();
		m_dformattext->AppendText(*CurrFormat);
	}

	if (CurrOutputDir) {
		m_doutdirtext->Clear();
		m_doutdirtext->AppendText(*CurrOutputDir);
	}
}


void D2gmain::SetRestoreProjectControls(void)
{
	if (CurrDelSourceDir) {
		m_rsrcdirtext->Clear();
		m_rsrcdirtext->AppendText(*CurrDelSourceDir);
	}

	if (CurrDelSourceDir
	 && CurrDelSourceMap) {
		m_rsrcmaptext->Clear();
		m_rsrcmaptext->AppendText(*MakeFullPath(CurrDelSourceDir, CurrDelSourceMap));
	}

	if (CurrDelFormat) {
		m_rformattext->Clear();
		m_rformattext->AppendText(*CurrDelFormat);
	}

	if (CurrDelOutputDir) {
		m_routdirtext->Clear();
		m_routdirtext->AppendText(*CurrDelOutputDir);
	}
}


void D2gmain::SetRunProjectControls(void)
{
	bool hcomp = false;
	bool wcomp = false;
	bool fts = false;
	bool jar = false;

	m_runprojectsel->SetStringSelection(*CurrProject);

	if (CurrSourceDir) {
		m_runsrcdirtext->Clear();
		m_runsrcdirtext->AppendText(*CurrSourceDir);
	}

	if (CurrSourceDir
	 && CurrSourceMap)
		m_runsrcmapsel->SetPath(*MakeFullPath(CurrSourceDir, CurrSourceMap));

	//if (CurrSourceMap)
	//	UMXL = CurrSourceMap->Matches(_("*.mx?"));

	if (CurrOutputDir) {
		m_runoutdirtext->Clear();
		m_runoutdirtext->AppendText(*CurrOutputDir);
	}

	if (CurrFormat) {
		m_runformattext->Clear();
		m_runformattext->AppendText(*CurrFormat);

		GetCurrIni(CurrFormat);

		const char *cf = CurrFormat->c_str();
		if (!strcmp(cf, "HTML Help")) {
			m_runcomp->Enable();
			m_runcompyes->Enable();
			m_runcompno->Enable();
			m_runcwin->Enable();
			m_runcwinyes->Enable();
			m_runcwinno->Enable();
			hcomp = true;
		}
		else if (!strcmp(cf, "WinHelp")) {
			m_runcomp->Enable();
			m_runcompyes->Enable();
			m_runcompno->Enable();
			m_runcwin->Disable();
			m_runcwinyes->Disable();
			m_runcwinno->Disable();
			wcomp = true;
		}
		else {
			m_runcomp->Disable();
			m_runcompyes->Disable();
			m_runcompno->Disable();
			m_runcwin->Disable();
			m_runcwinyes->Disable();
			m_runcwinno->Disable();
		}

		if (!strcmp(cf, "Java Help") || !strcmp(cf, "Oracle Help")) {
			m_runfts->Enable();
			m_runftsyes->Enable();
			m_runftsno->Enable();
			fts = true;
		}
		else {
			m_runfts->Disable();
			m_runftsyes->Disable();
			m_runftsno->Disable();
		}

		if (!strcmp(cf, "Java Help")) {
			m_runjar->Enable();
			m_runjaryes->Enable();
			m_runjarno->Enable();
			jar = true;
		}
		else {
			if (!strcmp(cf, "Oracle Help"))
				SetRunAuto("MakeJar", false);
			m_runjaryes->SetValue(false);
			m_runjarno->SetValue(true);
			m_runjar->Disable();
			m_runjaryes->Disable();
			m_runjarno->Disable();
		}
	}

	bool set = GetRunAuto("WrapAndShip");
	WrapAndShip = set;
	m_runwasyes->SetValue(set);
	m_runwasno->SetValue(!set);

	if (hcomp) {
		set = GetRunAuto("CompileHelp");
		m_runcompyes->SetValue(set);
		m_runcompno->SetValue(!set);
		set = GetRunAuto("KeepCompileWindow", false);
		m_runcwinyes->SetValue(set);
		m_runcwinno->SetValue(!set);
	}

	if (wcomp) {
		set = GetRunAuto("CompileHelp");
		m_runcompyes->SetValue(set);
		m_runcompno->SetValue(!set);
	}

	if (fts) {
		set = GetRunAuto("MakeFTS");
		m_runftsyes->SetValue(set);
		m_runftsno->SetValue(!set);
	}

	if (jar) {
		set = GetRunAuto("MakeJar");
		m_runjaryes->SetValue(set);
		m_runjarno->SetValue(!set);
	}

	set = GetRunAuto("MakeArchive");
	m_runarchyes->SetValue(set);
	m_runarchno->SetValue(!set);

	set = GetRunAuto("MoveArchive");
	m_runshipyes->SetValue(set);
	m_runshipno->SetValue(!set);

	if (ShowDelInCreate)
		m_optshdelyes->SetValue(true);
	else
		m_optshdelno->SetValue(true);

	m_optshowlog->SetValue(ShowLog);
	m_optshowcon->SetValue(ShowConsole);

	UseDCLSource = GetRunAuto("UseDCLSource", false);
	UseDCLOutput = GetRunAuto("UseDCLOutput", false);

	if (UseDCLSource) {
		if (UseDCLOutput)
			m_rundclyes->SetValue(true);
		else
			m_rundclsrc->SetValue(true);
	}
	else if (UseDCLOutput)
		m_rundclout->SetValue(true);
	else
		m_rundclno->SetValue(true);

	set = GetRunAuto("OnlyAuto", false);
	m_runonlyyes->SetValue(set);
	m_runonlyno->SetValue(!set);

	if (CurrFormat) {
		SetNewIni();
		if (EditCommand)
			m_runeditini->Enable();
		if (CMCommand)
			m_runcm->Enable();
	}

	m_runviewout->Disable();
	SetViewOutputCommand();
}


bool D2gmain::GetRunAuto(char *key, bool set)
{
	char buf[128];
	buf[0] = '\0';

	if (!CurrIni)
		GetCurrIni(CurrFormat);

	if (!CurrOutputDir)
		return set;

	wxString *inidest = MakeFullPath(CurrOutputDir, CurrIni);
	GetPrivateProfileString("Automation", key, "",
		                         buf, 128, inidest->c_str());
	if ((*buf == '\0')
	 && CurrProjDir
	 && CurrProjFile) {
		wxString *docdest = MakeFullPath(CurrProjDir, CurrProjFile);
		GetPrivateProfileString("Automation", key, "",
															 buf, 128, docdest->c_str());
	}

	if (*buf) {
		if (!stricmp(buf, "no"))
			return false;
		if (!stricmp(buf, "yes"))
			return true;
	}

	// missing, set it to default
	SetRunAuto(key, set);
	return set;
}

void D2gmain::SetRunAuto(char *key, bool set)
{
	GetCurrIni(CurrFormat);
	wxString *inidest = MakeFullPath(CurrOutputDir, CurrIni);

	WritePrivateProfileString("Automation", key,
		                         set ? "Yes" : "No", inidest->c_str());
}


void D2gmain::SetNextProjectID(void)
{
	NewProjectID = GetProjStr("Projects", "NextID");
	SetProjInt("Projects", "NextID", ++NextProjectID);
	CurrProjectID = NewProjectID;
	UpdateProjectsFile();
}


void D2gmain::ModifyProjectData(void)
{
	SetProjStr("Active", CurrProject->c_str(), CurrProjectID->c_str());

	SetProjStr("SourceDir", CurrProjectID->c_str(), CurrSourceDir->c_str());
	SetProjStr("SourceMap", CurrProjectID->c_str(), CurrSourceMap->c_str());
	SetProjStr("OutputDir", CurrProjectID->c_str(), CurrOutputDir->c_str());
	SetProjStr("OutputType", CurrProjectID->c_str(), CurrFormat->c_str());
	ModifyProjectFileData(CurrProjectID->c_str());
	//UMXL = CurrSourceMap->Matches(_("*.mx?"));

	SetNewIni();  // make sure required ini is present
}


void D2gmain::ModifyDelProjectData(void)
{
	SetProjStr("Deleted", CurrDelProject->c_str(), CurrDelProjectID->c_str());

	SetProjStr("SourceDir", CurrDelProjectID->c_str(), CurrDelSourceDir->c_str());
	SetProjStr("SourceMap", CurrDelProjectID->c_str(), CurrDelSourceMap->c_str());
	SetProjStr("OutputDir", CurrDelProjectID->c_str(), CurrDelOutputDir->c_str());
	SetProjStr("OutputType", CurrDelProjectID->c_str(), CurrDelFormat->c_str());
	ModifyProjectFileData(CurrDelProjectID->c_str());
}


void D2gmain::ModifyProjectFileData(const char *id)
{
	//SetProjBool("ProjDirUsed", id, CurrProjDirUsed);
	SetProjBool("LocalProjDir", id, CurrProjDirLocal);
	if (CurrProjDir)
		SetProjStr("ProjDir", id, CurrProjDir->c_str());
	if (CurrProjFile)
		SetProjStr("ProjFile", id, CurrProjFile->c_str());

	SetProjBool("DvalUsed", id, CurrDitavalUsed);
	if (CurrDitaval)
		SetProjStr("DvalFile", id, CurrDitaval->c_str());

	UpdateProjectsFile();
}


void D2gmain::SetCurrProjDirLoc(wxString *outdir)
{
	// combine outdir and ProjDirLocRel to form CurrProjDirLoc
	CurrProjDirLoc = MakeFullPath(outdir, ProjDirLocRel);
}


void D2gmain::SetNewIni(void)
{
	// identify ini
	bool hasini = false;
	GetCurrIni(CurrFormat);

	if (!CurrOutputDir)
		return;

	wxString *inidest = MakeFullPath(CurrOutputDir, CurrIni);
	EditCommand = new wxString(*Editor);
	*EditCommand += " ";
	*EditCommand += *inidest;

	CMCommand = new wxString(*CMStart);
	*CMCommand += *inidest;

	if (SetNewIniFile(inidest->c_str(), CurrIni)) {
		hasini = true;
		DCirfile *ini = new DCirfile(inidest->c_str());
		if (ini) {
			if (ini->Section("Templates")
			 && ini->Find("Document")) {
				wxString *doc = new wxString(ini->PathRight());
				CurrProjDir = DirFromPath(doc);
				CurrProjFile = FileFromPath(doc);
			}
			ini->CloseFile();
			delete ini;
		}
	}

	if (CurrProjDir && CurrProjFile) {
		wxString *docdir = NULL;
		wxString *docdest = NULL;
		if (*(CurrProjDir->c_str()) == '.') {  // relative
			docdir = MakeFullPath(CurrProjDir, CurrProjFile);
			docdest = MakeFullPath(CurrOutputDir, docdir);
		}
		else {
			docdest = MakeFullPath(CurrProjDir, CurrProjFile);
		}
		EditDocCommand = new wxString(*Editor);
		*EditDocCommand += " ";
		*EditDocCommand += *docdest;
	}
	else
		EditDocCommand = NULL;

	FullLogName = MakeFullPath(CurrOutputDir, LogFileName);
	ShowLogCommand = new wxString(*Editor);
	*ShowLogCommand += " ";
	*ShowLogCommand += *FullLogName;
	FullHistoryName =  MakeFullPath(CurrOutputDir, HistoryFileName);

	if (hasini) {
		if (Creating) {
			m_editini->Enable();
			m_createcm->Enable();
			m_iniconfig->Enable();
		}
		if (Modifying) {
			m_meditini->Enable();
			m_modifycm->Enable();
			m_miniconfig->Enable();
		}
		if (Running) {
			m_runeditini->Disable();
			m_runcm->Disable();
		}
	}
}


bool D2gmain::SetNewIniFile(const char *iname, wxString *isrc, bool isdoc)
{
	DCfile *fp = new DCfile(iname);
	if (fp->FErr() == fno) {  // copy file to output dir
		delete fp;
		wxString *oldini = new wxString(CurrIni->c_str() + 1);
		wxString *inidest = MakeFullPath(CurrOutputDir, oldini);
		fp = new DCfile(inidest->c_str());
		if (fp->FErr() == fok) {  // rename old file to new name
			fp->CloseFile();
			delete fp;
			wxString *cmd = new wxString("ren ");
			*cmd += *oldini;
			*cmd += ' ';
			*cmd += *CurrIni;
			RunDOSCommand(cmd->c_str(), CurrOutputDir->c_str());
			return true;
		}
		wxString *inisrc = MakeFullPath(NewIniPath, isrc);
		DCrfile *rfp = new DCrfile(inisrc->c_str());
		if (rfp->FErr() == fno) {
			delete rfp;
#ifdef DITA
			inisrc = FixEnvPath(MakeFullPath(new wxString("%omsyshome%\\d2g\\system\\starts"), isrc));
#endif
#ifdef UDOC
			inisrc = FixEnvPath(MakeFullPath(new wxString("%omsyshome%\\x2g\\system\\starts"), isrc));
#endif
			rfp = new DCrfile(inisrc->c_str());
		}
		if (rfp->FErr() == fno) {
			delete rfp;
			if (!isdoc) {
				wxString msg(*NoIniStart);
				msg += *inisrc;
				msg += *NoIniMessage;
				wxMessageDialog err(this, msg, *NoIniTitle, wxOK);
				err.ShowModal();
			}
			return false;
		}
		DCblock *blk = new DCblock();
		blk->set(rfp->getSize());
		*blk = *rfp;
		DCwfile *wfp = new DCwfile(iname, ftxt);
		*wfp = *blk;
		rfp->CloseFile();
		delete rfp;
		wfp->CloseFile();
		delete wfp;
		delete blk;
	}
	else { // already there
		fp->CloseFile();
		delete fp;
	}
	return true;
}


void D2gmain::LaunchCM(void)
{
	if (!CMCommand)
		return;

	RunDOSCommand(CMCommand->c_str(), CurrOutputDir->c_str(), true);
}


void D2gmain::EditIniFile(void)
{
	if (!EditCommand)
		return;

	if (EditDocCommand) {  // doc ini exists, choose
		wxString *docdir = NULL;
		IniToEdit *i2e = new IniToEdit(NULL);
		i2e->m_i2estartname->SetLabel(*CurrIni);
		i2e->m_i2edocname->SetLabel(*CurrProjFile);
		i2e->ShowModal();
		int rval = i2e->GetReturnCode();
		i2e->Destroy();

		switch (rval) {
			case 1: // start ini
				break;

			case 2: // doc ini
				if (*(CurrProjDir->c_str()) == '.') // relative
					docdir = MakeFullPath(CurrOutputDir, CurrProjDir);
				else
					docdir = CurrProjDir;
				RunDOSCommand(EditDocCommand->c_str(), docdir->c_str(), true);
				return;

			default:  // cancel
				return;
		}
	}

	RunDOSCommand(EditCommand->c_str(), CurrOutputDir->c_str(), true);
}


void D2gmain::ShowLogFile(void)
{
	if (!ShowLogCommand)
		return;

	RunDOSCommand(ShowLogCommand->c_str(), CurrOutputDir->c_str(), true);
}


void D2gmain::ViewOutput(void)
{
	if (!ViewOutputCommand
	 || !CurrWrapDir)
		return;

	// ShellExecute output file
	RunDOSCommand(ViewOutputCommand->c_str(), CurrWrapDir->c_str(),
		 !ShowConsole, ShowConsole);
}


void D2gmain::RunConfigWizard(wxString *form)
{
	Configstart *cfs = new Configstart();
	wxString *docini = NULL;

	// get starting ini
	GetCurrIni(form);
	wxString *inidest = MakeFullPath(CurrOutputDir, CurrIni);
	IniFileName = NewName(inidest->c_str());
	DCpfile::psetup(IniFileName);


	// set vars needed by wizard
	bool isrtf = (!form->Cmp("Word RTF") || !form->Cmp("WinHelp")) ? true : false;
	bool iseclipse = (!form->Cmp("Eclipse Help")) ? true : false;
	bool isxml = (!form->Cmp("Flat XML") || !form->Cmp("DocBook") || !form->Cmp("DITA")) ? true : false;
	bool ishtm = (!form->Cmp("HTML 4.0") || !form->Cmp("XHTML 1.0")) ? true : false;

	cfs->m_configouttype = form;
	cfs->m_configsrcdir = CurrSourceDir;
	cfs->m_configstartini = new wxString(IniFileName);
	if (CurrSourceMap) {
		//UMXL = CurrSourceMap->Matches(_("*.mx?"));
		docini = new wxString(CurrSourceMap->BeforeLast('.'));
		cfs->m_configname = new wxString(*docini);
		docini->Append(".ini");
	}

	if (CurrProjDirLocal) {
		CurrProjDirLoc = MakeFullPath(CurrOutputDir, ProjDirLocRel);
		cfs->m_configdocini = docini ? MakeFullPath(CurrProjDirLoc, docini)
			                           : CurrProjDirLoc;
	}
	else
		cfs->m_configdocini = docini ? MakeFullPath(ProjDirD2GPar, docini)
			                           : ProjDirD2GPar;

	// read current values as defaults
	bool docset = true;
	bool docsetan = false;
	bool docsetav = false;
	bool docsetgd = false;
	bool docsetgf = false;
	bool docsetdv = false;
	bool dvwasset = false;
	bool docdvwasset = false;
	bool hasdoc = false;

	// see if doc file already set
	wxString *dpath = GetIniStr("Templates", "Document", IniFileName);
	if (dpath)
		cfs->m_configdocini = dpath;
	if (cfs->m_configdocini)
		DocFileName = NewName(cfs->m_configdocini->c_str());

	// read from existing start ini first

	// page 1a
	cfs->m_configdval = GetIniStr("ConditionOptions", "DitavalFile", IniFileName);
	if (cfs->m_configdval
	 && (cfs->m_configdval->Len() == 1)
	 && (cfs->m_configdval->Last() == ' '))
		cfs->m_configdval = NULL;
	if (cfs->m_configdval)
		dvwasset = true;
	cfs->m_configwasdef = GetIniBool("Automation", "WrapAndShip", IniFileName);

	// page 2
	cfs->m_configanamestr = GetIniStr("Automation", "ArchiveName", IniFileName);
	cfs->m_configaverstr = GetIniStr("Automation", "ArchiveVer", IniFileName);

	// page 2a
	cfs->m_configgrdirstr = GetIniStr("Automation", "CopyGraphicsFrom", IniFileName);
	cfs->m_configgrfilesstr = GetIniStr("Automation", "GraphCopyFiles", IniFileName);

	if (FileExists(DocFileName)) {
		hasdoc = true;
		DCpfile::psetup(DocFileName);
		if (!cfs->m_configwasdef)
			cfs->m_configwasdef = GetIniBool("Automation", "WrapAndShip", DocFileName);

		if (!cfs->m_configanamestr) {
			cfs->m_configanamestr = GetIniStr("Automation", "ArchiveName", DocFileName);
			if (cfs->m_configanamestr)
				docset = docsetan = true;
		}
		if (!cfs->m_configaverstr) {
			cfs->m_configaverstr = GetIniStr("Automation", "ArchiveVer", DocFileName);
			if (cfs->m_configaverstr)
				docset = docsetav = true;
		}
		if (!cfs->m_configgrdirstr) {
			cfs->m_configgrdirstr = GetIniStr("Automation", "CopyGraphicsFrom", DocFileName);
			if (cfs->m_configgrdirstr)
				docset = docsetgd = true;
		}
		if (!cfs->m_configgrfilesstr) {
			cfs->m_configgrfilesstr = GetIniStr("Automation", "GraphCopyFiles", DocFileName);
			if (cfs->m_configgrfilesstr)
				docset = docsetgf = true;
		}

		if (!cfs->m_configdval) {
			cfs->m_configdval = GetIniStr("ConditionOptions", "DitavalFile", DocFileName);
			if (cfs->m_configdval)
				docset = docsetdv = dvwasset = docdvwasset = true;
		}
		else {
			if (GetIniStr("ConditionOptions", "DitavalFile", DocFileName) != NULL)
				docdvwasset = true;
		}
	}

	// set defaults for any missing
	if (!cfs->m_configanamestr)
		cfs->m_configanamestr = cfs->m_configname;
	if (!cfs->m_configaverstr
	 && SysIni
	 && SysIni->Section("OutputTypes"))
		cfs->m_configaverstr = GetSysIni(CurrFormat->c_str());
	if (!cfs->m_configgrdirstr)
		cfs->m_configgrdirstr = CurrSourceDir;
	if (!cfs->m_configgrfilesstr) {
		if (!form->Cmp("Word RTF")
		 || !form->Cmp("WinHelp"))
			cfs->m_configgrfilesstr = new wxString("*.wmf *.bmp");
		else
			cfs->m_configgrfilesstr = new wxString("*.gif *.jpg");
	}
	if (!cfs->m_configdval) {
		cfs->m_configdval = CurrDitaval;
		cfs->m_configdvuse = CurrDitavalUsed;
	}
	else
		cfs->m_configdvuse = true;


	// run the wizard
	cfs->Create(this);
	SetWizardPageVars(form, cfs);

	cfs->m_wizardpage1a->m_configdvaldoc->SetValue(docsetdv);
	cfs->m_wizardpage1a->m_configdvalstart->SetValue(!docsetdv);
	cfs->m_wizardpage2->m_configanamedoc->SetValue(docsetan);
	cfs->m_wizardpage2->m_configanamestart->SetValue(!docsetan);
	cfs->m_wizardpage2->m_configaverdoc->SetValue(docsetav);
	cfs->m_wizardpage2->m_configaverstart->SetValue(!docsetav);
	cfs->m_wizardpage2a->m_configgrddoc->SetValue(docsetgd);
	cfs->m_wizardpage2a->m_configgrdstart->SetValue(!docsetgd);
	cfs->m_wizardpage2a->m_configgrfdoc->SetValue(docsetgf);
	cfs->m_wizardpage2a->m_configgrfstart->SetValue(!docsetgf);


	if (!cfs->Run()) { // cancelled
		delete cfs;
		return;
	}

	// collect info from wizard after finish
	docsetdv = cfs->m_wizardpage1a->m_configdvaldoc->GetValue();
	docsetan = cfs->m_wizardpage2->m_configanamedoc->GetValue();
	docsetav = cfs->m_wizardpage2->m_configaverdoc->GetValue();
	docsetgd = cfs->m_wizardpage2a->m_configgrddoc->GetValue();
	docsetgf = cfs->m_wizardpage2a->m_configgrfdoc->GetValue();

	//docset = (docsetdv || docsetan || docsetav || docsetgd || docsetgf) ? true : false;

	CurrProjDirUsed = true;
	char *dtxt = NewName(cfs->m_configdocini->c_str());
	if (DocFileName
	 && (!dtxt
		|| strcmp(dtxt, DocFileName))) {  // name changed
		DCpfile::pclose(DocFileName);
		hasdoc = false;
		DocFileName = dtxt;
	}
	else if (!DocFileName)
		DocFileName = dtxt;
	CurrProjDir = DirFromPath(cfs->m_configdocini);
	CurrProjFile = FileFromPath(cfs->m_configdocini);

	if (cfs->m_configdvuse) {
		CurrDitavalUsed = true;
		CurrDitaval = new wxString(*cfs->m_configdval);
	}
	else {
		CurrDitavalUsed = false;
		CurrDitaval = NULL;
	}

	ModifyProjectFileData(CurrProjectID->c_str());

	// update starting ini file and doc file with results

	if (DocFileName) {
		if (!hasdoc) {  // orig file name does not exist or was changed
			if (FileExists(DocFileName))  // changed in wiz to existing one
				docset = true;
			else if (!CreateDocFile())	// create doc file from scratch
				docset = false;

			if (docset)
				DCpfile::psetup(DocFileName);
		}
		if (docset) { // make any document file settings
			//SetIniBool("Automation", "WrapAndShip", cfs->m_configwasdef, DocFileName);
			if (docsetan
			 && cfs->m_configanamestr)
				SetIniStr("Automation", "ArchiveName",
									NewName(cfs->m_configanamestr->c_str()), DocFileName);
			if (docsetav
			 && cfs->m_configaverstr)
				SetIniStr("Automation", "ArchiveVer",
									NewName(cfs->m_configaverstr->c_str()), DocFileName);
			if (docsetgd
			 && cfs->m_configgrdirstr)
				SetIniStr("Automation", "CopyGraphicsFrom",
									NewName(cfs->m_configgrdirstr->c_str()), DocFileName);
			if (docsetgf
			 && cfs->m_configgrfilesstr)
				SetIniStr("Automation", "GraphCopyFiles",
									NewName(cfs->m_configgrfilesstr->c_str()), DocFileName);
			if (docsetdv) {
				if (cfs->m_configdvuse
				 && cfs->m_configdval)
					SetIniStr("ConditionOptions", "DitavalFile",
										NewName(cfs->m_configdval->c_str()), DocFileName);
				else if (dvwasset)
					SetIniStr("ConditionOptions", "DitavalFile",
										NULL, DocFileName);
			}
		}
		if (hasdoc || docset)
			DCpfile::pclose(DocFileName);

		// link doc file in ini file
		SetIniStr("Templates", "Document", DocFileName, IniFileName);
	}
	else  { // no DocFileName
		SetIniStr("Templates", "Document", NULL, IniFileName);
		docset = docsetdv = docsetan = docsetav = docsetgd = docsetgf = false;
	}

	// make remaining ini file settings
	SetIniBool("Automation", "WrapAndShip", cfs->m_configwasdef, IniFileName);
	// remove any already made in doc file
	if (cfs->m_configanamestr)
		SetIniStr("Automation", "ArchiveName", docsetan ? NULL :
							NewName(cfs->m_configanamestr->c_str()), IniFileName);
	if (cfs->m_configaverstr)
		SetIniStr("Automation", "ArchiveVer", docsetav ? NULL :
							NewName(cfs->m_configaverstr->c_str()), IniFileName);
	if (cfs->m_configgrdirstr)
		SetIniStr("Automation", "CopyGraphicsFrom", docsetgd ? NULL :
							NewName(cfs->m_configgrdirstr->c_str()), IniFileName);
	if (cfs->m_configgrfilesstr)
		SetIniStr("Automation", "GraphCopyFiles", docsetgf ? NULL :
							NewName(cfs->m_configgrfilesstr->c_str()), IniFileName);

	if (cfs->m_configdvuse
	 && cfs->m_configdval)
		SetIniStr("ConditionOptions", "DitavalFile", docsetdv ? NULL :
              NewName(cfs->m_configdval->c_str()), IniFileName);
	else if (docdvwasset)
		SetIniStr("ConditionOptions", "DitavalFile",
							" ", IniFileName);
	else if (dvwasset
	 && !docsetdv)
		SetIniStr("ConditionOptions", "DitavalFile",
							NULL, IniFileName);


	GetWizardPageVars(form, cfs);

	DCpfile::pclose(IniFileName);

	delete cfs;
}


bool D2gmain::CreateDocFile(void)
{
	wxString path = wxPathOnly(DocFileName);
	if (!wxDirExists(path)) {
		if (!wxMkdir(path))
			return false;  // or make one seg at a time ****
	}
	DCwfile *wfp = new DCwfile(DocFileName, ftxt);
	if (wfp->FErr() == fok) {
		wfp->putStr("; ");
		wfp->putStr(DocFileName);
		wfp->putStr(" is a document-specific file\n");
		wfp->putStr("; referenced in ");
		wfp->putStr(IniFileName);
		wfp->putStr("\n\n");
	}
	else
		return false;
	wfp->CloseFile();
	delete wfp;
	return true;
}


// called after pages exist, before run

void D2gmain::SetWizardPageVars(wxString *form, Configstart *cfs)
{
	// format-specific settings
	bool isrtf = (!form->Cmp("Word RTF") || !form->Cmp("WinHelp")) ? true : false;
	bool iseclipse = (!form->Cmp("Eclipse Help")) ? true : false;
	bool isxml = (!form->Cmp("Flat XML") || !form->Cmp("DocBook") || !form->Cmp("DITA")) ? true : false;
	bool ishtm = (!form->Cmp("HTML 4.0") || !form->Cmp("XHTML 1.0")) ? true : false;

	wxString *rstr = NULL;
	char *sect = NULL;
	char *pfile = NULL;

	if (!isrtf && !iseclipse && !isxml) {	// m_wizardpage3->
		sect = "HTMLOptions";
		// m_configtitletxt   Title
		rstr = GetIniStr(sect, "Title", IniFileName);
		if (!rstr)
			rstr = new wxString("DITA2Go Test");
		cfs->m_wizardpage3->m_configtitletxt->SetValue(*rstr);

		if (!ishtm) {
			if (!form->Cmp("OmniHelp")) {
				sect = "OmniHelpOptions";
				pfile = "ProjectName";
			}
			else if (!form->Cmp("HTML Help")) {
				sect = "MSHtmlHelpOptions";
				pfile = "HHPFileName";
			}
			else if (!form->Cmp("Java Help")) {
				sect = "JavaHelpOptions";
				pfile = "HSFileName";
			}
			else if (!form->Cmp("Oracle Help")) {
				sect = "OracleHelpOptions";
				pfile = "HSFileName";
			}
			// m_confighpftxt
			rstr = GetIniStr(sect, pfile, IniFileName);
			if (rstr)
				cfs->m_wizardpage3->m_confighpftxt->SetValue(*rstr);
			else
				cfs->m_wizardpage3->m_confighpftxt->SetValue(*cfs->m_configname);

			// m_confightitletxt  HelpFileTitle
			rstr = GetIniStr(sect, "HelpFileTitle", IniFileName);
			if (rstr)
				cfs->m_wizardpage3->m_confightitletxt->SetValue(*rstr); 
			else
				cfs->m_wizardpage3->m_confightitletxt->SetValue(*cfs->m_configname);

			// m_confightopictxt  DefaultTopicFile
			rstr = GetIniStr(sect, "DefaultTopicFile", IniFileName);
			if (rstr)
				cfs->m_wizardpage3->m_confightopictxt->SetValue(*rstr);

			if (!form->Cmp("OmniHelp")) {  // use empty key to read sect
				// m_configohtltxt OHTopLeftNav
				rstr = GetIniStr("OHTopLeftNav", "", IniFileName);
				if (rstr)
					cfs->m_wizardpage3->m_configohtltxt->SetValue(*rstr);
				else
					cfs->m_wizardpage3->m_configohtltxt->SetValue(*(new wxString("Your Company Logo<br />and Name Here\\")));

			}
		}
	}
	else if (iseclipse) {       // m_wizardpage4->
		sect = "EclipseHelpOptions";
		// m_configehtitletxt TocLabel
		rstr = GetIniStr(sect, "TocLabel", IniFileName);
		if (rstr)
			cfs->m_wizardpage4->m_configehtitletxt->SetValue(*rstr);
		else
			cfs->m_wizardpage4->m_configehtitletxt->SetValue(*cfs->m_configname);

		// m_configehpntxt    PluginName
		rstr = GetIniStr(sect, "PluginName", IniFileName);
		if (rstr)
			cfs->m_wizardpage4->m_configehpntxt->SetValue(*rstr);
		else
			cfs->m_wizardpage4->m_configehpntxt->SetValue(*cfs->m_configname);

		// m_configehpidtxt   PluginID
		rstr = GetIniStr(sect, "PluginID", IniFileName);
		if (rstr)
			cfs->m_wizardpage4->m_configehpidtxt->SetValue(*rstr);
		else
			cfs->m_wizardpage4->m_configehpidtxt->SetValue(*cfs->m_configname);

		// m_configehprovtxt  PluginProvider
		rstr = GetIniStr(sect, "PluginProvider", IniFileName);
		if (rstr)
			cfs->m_wizardpage4->m_configehprovtxt->SetValue(*rstr);

		// m_configehtopictxt  TocTopic
		rstr = GetIniStr(sect, "TocTopic", IniFileName);
		if (rstr)
			cfs->m_wizardpage4->m_configehtopictxt->SetValue(*rstr);
	}
	else if (isrtf) {           // m_wizardpage5->
		sect = "FileIDs";
		// m_configrtfidtxt
		rstr = GetIniStr(sect, cfs->m_configname->c_str(), IniFileName);
		if (rstr)
			cfs->m_wizardpage5->m_configrtfidtxt->SetValue(*rstr);
		else
			cfs->m_wizardpage5->m_configrtfidtxt->SetValue(*cfs->m_configname);

		if (!form->Cmp("Word RTF")) {
			sect = "WordOptions";
			// m_configwv97, 2k, xp, 2003
			// Word8 Word2000 Word2002 Word2003
			if (GetIniBool(sect, "Word8", IniFileName)) {
				cfs->m_wizardpage5->m_configwv97->SetValue(true);
				cfs->m_wizardpage5->m_configwv2k->SetValue(false);
				cfs->m_wizardpage5->m_configwvxp->SetValue(false);
				cfs->m_wizardpage5->m_configwv2003->SetValue(false);
			}
			if (GetIniBool(sect, "Word2000", IniFileName)) {
				cfs->m_wizardpage5->m_configwv97->SetValue(false);
				cfs->m_wizardpage5->m_configwv2k->SetValue(true);
				cfs->m_wizardpage5->m_configwvxp->SetValue(false);
				cfs->m_wizardpage5->m_configwv2003->SetValue(false);
			}
			if (GetIniBool(sect, "Word2002", IniFileName)) {
				cfs->m_wizardpage5->m_configwv97->SetValue(false);
				cfs->m_wizardpage5->m_configwv2k->SetValue(false);
				cfs->m_wizardpage5->m_configwvxp->SetValue(true);
				cfs->m_wizardpage5->m_configwv2003->SetValue(false);
			}
			if (GetIniBool(sect, "Word2003", IniFileName)) {
				cfs->m_wizardpage5->m_configwv97->SetValue(false);
				cfs->m_wizardpage5->m_configwv2k->SetValue(false);
				cfs->m_wizardpage5->m_configwvxp->SetValue(false);
				cfs->m_wizardpage5->m_configwv2003->SetValue(true);
			}
		}
		else {
			sect = "HelpOptions";
			// m_confighpjtxt  HPJFileName
			rstr = GetIniStr(sect, "HPJFileName", IniFileName);
			if (rstr)
				cfs->m_wizardpage5->m_confighpjtxt->SetValue(*rstr);
			else
				cfs->m_wizardpage5->m_confighpjtxt->SetValue(*cfs->m_configname);

			sect = "HelpContents";
			// m_configcnttitletxt  CntTitle
			rstr = GetIniStr(sect, "CntTitle", IniFileName);
			if (rstr)
				cfs->m_wizardpage5->m_configcnttitletxt->SetValue(*rstr);
			else
				cfs->m_wizardpage5->m_configcnttitletxt->SetValue(*cfs->m_configname);

			// m_configwhtopictxt  CntTopic
			rstr = GetIniStr(sect, "CntTopic", IniFileName);
			if (rstr)
				cfs->m_wizardpage5->m_configwhtopictxt->SetValue(*rstr);
		}
	}

}


void D2gmain::GetWizardPageVars(wxString *form, Configstart *cfs)
{
	// these all go in the format-specific ini
	bool isrtf = (!form->Cmp("Word RTF") || !form->Cmp("WinHelp")) ? true : false;
	bool iseclipse = (!form->Cmp("Eclipse Help")) ? true : false;
	bool isxml = (!form->Cmp("Flat XML") || !form->Cmp("DocBook") || !form->Cmp("DITA")) ? true : false;
	bool ishtm = (!form->Cmp("HTML 4.0") || !form->Cmp("XHTML 1.0")) ? true : false;

	wxString *rstr = NULL;
	char *sect = NULL;
	char *pfile = NULL;

	if (!isrtf && !iseclipse && !isxml) {	// m_wizardpage3
		sect = "HTMLOptions";
		// m_configtitletxt   Title
		wxString wxs = cfs->m_wizardpage3->m_configtitletxt->GetValue();
		rstr = new wxString(wxs);
		if (rstr && !rstr->IsEmpty())
			SetIniStr(sect, "Title", NewName(rstr->c_str()), IniFileName);

		if (!ishtm) {
			if (!form->Cmp("OmniHelp")) {
				sect = "OmniHelpOptions";
				pfile = "ProjectName";
			}
			else if (!form->Cmp("HTML Help")) {
				sect = "MSHtmlHelpOptions";
				pfile = "HHPFileName";
			}
			else if (!form->Cmp("Java Help")) {
				sect = "JavaHelpOptions";
				pfile = "HSFileName";
			}
			else if (!form->Cmp("Oracle Help")) {
				sect = "OracleHelpOptions";
				pfile = "HSFileName";
			}
			// m_confighpftxt
			wxString wxs2 = cfs->m_wizardpage3->m_confighpftxt->GetValue();
			rstr = new wxString(wxs2);
			if (rstr && !rstr->IsEmpty())
				SetIniStr(sect, pfile, NewName(rstr->c_str()), IniFileName);

			// m_confightitletxt  HelpFileTitle
			wxString wxs3 = cfs->m_wizardpage3->m_confightitletxt->GetValue();
			rstr = new wxString(wxs3);
			if (rstr && !rstr->IsEmpty())
				SetIniStr(sect, "HelpFileTitle", NewName(rstr->c_str()), IniFileName);

			// m_confightopictxt  DefaultTopicFile
			wxString wxs4 = cfs->m_wizardpage3->m_confightopictxt->GetValue();
			rstr = new wxString(wxs4);
			if (rstr && !rstr->IsEmpty())
				SetIniStr(sect, "DefaultTopicFile", NewName(rstr->c_str()), IniFileName);

			if (!form->Cmp("OmniHelp")) {
				// m_configohtltxt OHTopLeftNav
				wxString wxs5 = cfs->m_wizardpage3->m_configohtltxt->GetValue();
				rstr = new wxString(wxs5);
				if (rstr && !rstr->IsEmpty())  // empty key replaces current macro
					SetIniStr("OHTopLeftNav", "", NewName(rstr->c_str()), IniFileName);
			}
		}
	}
	else if (iseclipse) {       // m_wizardpage4
		sect = "EclipseHelpOptions";

		// m_configehtitletxt TocLabel
		wxString wxs6 = cfs->m_wizardpage4->m_configehtitletxt->GetValue();
		rstr = new wxString(wxs6);
		if (rstr && !rstr->IsEmpty())
			SetIniStr(sect, "TocLabel", NewName(rstr->c_str()), IniFileName);

		// m_configehpntxt    PluginName
		wxString wxs7 = cfs->m_wizardpage4->m_configehpntxt->GetValue();
		rstr = new wxString(wxs7);
		if (rstr && !rstr->IsEmpty())
			SetIniStr(sect, "PluginName", NewName(rstr->c_str()), IniFileName);

		// m_configehpidtxt   PluginID
		wxString wxs8 = cfs->m_wizardpage4->m_configehpidtxt->GetValue();
		rstr = new wxString(wxs8);
		if (rstr && !rstr->IsEmpty())
			SetIniStr(sect, "PluginID", NewName(rstr->c_str()), IniFileName);

		// m_configehprovtxt  PluginProvider
		wxString wxs9 = cfs->m_wizardpage4->m_configehprovtxt->GetValue();
		rstr = new wxString(wxs9);
		if (rstr && !rstr->IsEmpty())
			SetIniStr(sect, "PluginProvider", NewName(rstr->c_str()), IniFileName);

		// m_configehtopictxt  TocTopic
		wxString wxs10 = cfs->m_wizardpage4->m_configehtopictxt->GetValue();
		rstr = new wxString(wxs10);
		if (rstr && !rstr->IsEmpty())
			SetIniStr(sect, "TocTopic", NewName(rstr->c_str()), IniFileName);
	}
	else if (isrtf) {           // m_wizardpage5
		sect = "FileIDs";
		// m_configrtfidtxt
		wxString wxs11 = cfs->m_wizardpage5->m_configrtfidtxt->GetValue();
		rstr = new wxString(wxs11);
		if (rstr && !rstr->IsEmpty())
			SetIniStr(sect, cfs->m_configname->c_str(), NewName(rstr->c_str()), IniFileName);

		if (!form->Cmp("Word RTF")) {
			sect = "WordOptions";
			// m_configwv97, 2k, xp, 2003
			// Word8 Word2000 Word2002 Word2003
			SetIniBool(sect, "Word8", cfs->m_wizardpage5->m_configwv97->GetValue(), IniFileName);
			SetIniBool(sect, "Word2000", cfs->m_wizardpage5->m_configwv2k->GetValue(), IniFileName);
			SetIniBool(sect, "Word2002", cfs->m_wizardpage5->m_configwvxp->GetValue(), IniFileName);
			SetIniBool(sect, "Word2003", cfs->m_wizardpage5->m_configwv2003->GetValue(), IniFileName);
		}
		else {
			sect = "HelpOptions";
			// m_confighpjtxt  HPJFileName
			wxString wxs12 = cfs->m_wizardpage5->m_confighpjtxt->GetValue();
			rstr = new wxString(wxs12);
			if (rstr && !rstr->IsEmpty())
				SetIniStr(sect, "HPJFileName", NewName(rstr->c_str()), IniFileName);

			sect = "HelpContents";
			// m_configcnttitletxt  CntTitle
			wxString wxs13 = cfs->m_wizardpage5->m_configcnttitletxt->GetValue();
			rstr = new wxString(wxs13);
			if (rstr && !rstr->IsEmpty())
				SetIniStr(sect, "CntTitle", NewName(rstr->c_str()), IniFileName);

			// m_configwhtopictxt  CntTopic
			wxString wxs14 = cfs->m_wizardpage5->m_configwhtopictxt->GetValue();
			rstr = new wxString(wxs14);
			if (rstr && !rstr->IsEmpty())
				SetIniStr(sect, "CntTopic", NewName(rstr->c_str()), IniFileName);
		}
	}
}



// command processing


void D2gmain::CreateProject(void)
{
	Creating = true;
	GetCurrProjectID();
	SetNextProjectID();
	ModifyProjectData();

	if (ActiveProjects)
		delete ActiveProjects;
	ActiveProjects = GetProjSect("Active");

	m_projectselect->Clear();
	m_projectselect->Append(*ActiveProjects);
	if (DeletedProjects
	 && ShowDelInCreate)
		m_projectselect->Append(*DeletedProjects);
	m_projectselect->SetStringSelection(*CurrProject);

	m_mprojectselect->Clear();
	m_mprojectselect->Append(*ActiveProjects);
	m_mprojectselect->SetStringSelection(*CurrProject);
	SetModifyProjectControls();

	m_dprojectselect->Clear();
	m_dprojectselect->Append(*ActiveProjects);
	m_dprojectselect->SetStringSelection(*CurrProject);
	SetDeleteProjectControls();

	m_runprojectsel->Clear();
	m_runprojectsel->Append(*ActiveProjects);
	m_runprojectsel->SetStringSelection(*CurrProject);
	SetRunProjectControls();

	m_modifyproject->Enable();
	m_deleteproject->Enable();
	m_runstart->Enable();

	m_resulttext->Clear();
	m_resulttext->AppendText(*CreatedProjectText);
	m_resulttext->AppendText(*CurrProject);

	Creating = false;
}


void D2gmain::ModifyProject(void)
{
	Modifying = true;
	ModifyProjectData();
	m_mresulttext->Clear();
	m_mresulttext->AppendText(*ModifiedProjectText);
	m_mresulttext->AppendText(*CurrProject);

	Modifying = false;
}


void D2gmain::DeleteProject(void)
{
	SetProjStr("Deleted", CurrProject->c_str(), CurrProjectID->c_str());
	SetProjStr("Active", CurrProject->c_str(), NULL);

	CurrDelProject = CurrProject;
	GetCurrDelProjectID();

	// set ActiveProjects from ini section
	if (ActiveProjects)
		delete ActiveProjects;
	ActiveProjects = GetProjSect("Active");
	if (!ActiveProjects
	 || (ActiveProjects->GetCount() == 0)) {
		CurrProject = NULL;
		CurrProjectID = NULL;
		SetProjStr("Projects", "LastActive", "0");
		m_modifyproject->Disable();
		m_deleteproject->Disable();
		m_runstart->Disable();
	}
	else {
		CurrProject = new wxString(ActiveProjects->Item(0));
		GetCurrProjectID();
		m_modifyproject->Enable();
		m_deleteproject->Enable();
		m_runstart->Enable();
	}
	UpdateProjectsFile();

	// set DeletedProjects from ini section
	if (DeletedProjects)
		delete DeletedProjects;
	DeletedProjects = GetProjSect("Deleted");

	GetProjectInfo();
	GetDelProjectInfo();

	m_mprojectselect->Clear();
	if (ActiveProjects) {
		m_mprojectselect->Append(*ActiveProjects);
		m_mprojectselect->SetStringSelection(*CurrProject);
	}
	SetModifyProjectControls();

	m_dprojectselect->Clear();
	if (ActiveProjects) {
		m_dprojectselect->Append(*ActiveProjects);
		m_dprojectselect->SetStringSelection(*CurrProject);
	}
	SetDeleteProjectControls();

	m_rprojectselect->Clear();
	m_rprojectselect->Append(*DeletedProjects);
	m_rprojectselect->SetStringSelection(*CurrDelProject);
	SetRestoreProjectControls();

	m_runprojectsel->Clear();
	if (ActiveProjects) {
		m_runprojectsel->Append(*ActiveProjects);
		m_runprojectsel->SetStringSelection(*CurrProject);
	}
	SetRunProjectControls();

	m_dresulttext->Clear();
	m_dresulttext->AppendText(*DeletedProjectText);
	m_dresulttext->AppendText(*CurrDelProject);

	m_restoreproject->Enable();
	m_editini->Disable();
	m_createcm->Disable();
	m_iniconfig->Disable();
	m_meditini->Disable();
	m_modifycm->Disable();
	m_miniconfig->Disable();
	m_runeditini->Disable();
	m_runcm->Disable();
	m_runviewout->Disable();
}


void D2gmain::RestoreProject(void)
{
	SetProjStr("Active", CurrDelProject->c_str(), CurrDelProjectID->c_str());
	SetProjStr("Deleted", CurrDelProject->c_str(), NULL);

	CurrProject = CurrDelProject;
	GetCurrProjectID();

	// set DeletedProjects from ini section
	if (DeletedProjects)
		delete DeletedProjects;
	DeletedProjects = GetProjSect("Deleted");
	if (!DeletedProjects
	 || (DeletedProjects->GetCount() == 0)) {
		CurrDelProject = NULL;
		CurrDelProjectID = NULL;
		SetProjStr("Projects", "LastDeleted", "0");
		m_restoreproject->Disable();
	}
	else {
		CurrDelProject = new wxString(DeletedProjects->Item(0));
		GetCurrDelProjectID();
		m_restoreproject->Enable();
	}
	UpdateProjectsFile();

	// set ActiveProjects from ini section
	if (ActiveProjects)
		delete ActiveProjects;
	ActiveProjects = GetProjSect("Active");

	GetProjectInfo();
	GetDelProjectInfo();

	m_mprojectselect->Clear();
	m_mprojectselect->Append(*ActiveProjects);
	m_mprojectselect->SetStringSelection(*CurrProject);
	SetModifyProjectControls();

	m_dprojectselect->Clear();
	m_dprojectselect->Append(*ActiveProjects);
	m_dprojectselect->SetStringSelection(*CurrProject);
	SetDeleteProjectControls();

	m_rprojectselect->Clear();
	if (DeletedProjects) {
		m_rprojectselect->Append(*DeletedProjects);
		m_rprojectselect->SetStringSelection(*CurrDelProject);
	}
	SetRestoreProjectControls();

	m_runprojectsel->Clear();
	m_runprojectsel->Append(*ActiveProjects);
	m_runprojectsel->SetStringSelection(*CurrProject);
	SetRunProjectControls();

	m_rresulttext->Clear();
	m_rresulttext->AppendText(*RestoredProjectText);
	m_rresulttext->AppendText(*CurrProject);

	m_modifyproject->Enable();
	m_deleteproject->Enable();
	m_runstart->Enable();
	m_editini->Disable();
	m_createcm->Disable();
	m_iniconfig->Disable();
	m_meditini->Disable();
	m_modifycm->Disable();
	m_miniconfig->Disable();
	m_runeditini->Disable();
	m_runcm->Disable();
	m_runviewout->Disable();
}


void D2gmain::SetViewOutputCommand(bool run)
{
	bool isjh = false;
	bool isohj = false;
	bool isoh = false;
	bool iseh = false;
	bool ishh = false;
	bool ishtm = false;
	bool isword = false;
	bool isdita = false;
	bool isxml = false;
	char *suftype = NULL;
	char *suffix = NULL;
	char *suffix2 = NULL;
	char *prefix = NULL;
	char *nametype = NULL;
	char *nametype2 = NULL;
	char *projname = NULL;
	char *projver = NULL;
	char *inisec = NULL;
	bool makejar = false;

	IniFile = NULL;
	CurrWrapDir = NULL;
	OutputExt = NULL;
	ViewOutputCommand = NULL;
	m_runviewout->Disable();

	if (run
	 && !WrapAndShip)
		return;

	if (CurrFormat) {
		const char *cf = CurrFormat->c_str();
		if (!strcmp(cf, "HTML Help")) {
			inisec = "MSHtmlHelpOptions";
			nametype = "DefaultChmFile";
			nametype2 = "HHPFileName";
			ishh = true;
			suffix = "htm";
			suffix2 = "chm";
		}
		else if (!strcmp(cf, "WinHelp")) {
			inisec = "HelpOptions";
			nametype = "HPJFileName";
			suffix = "rtf";
			suffix2 = "hlp";
		}
		else if (!strcmp(cf, "Java Help")) {
			nametype = "HSFileName";
			inisec = "JavaHelpOptions";
			suffix = "htm";
			suffix2 = "hs";
			isjh = true;
		}
		else if (!strcmp(cf, "Oracle Help")) {
			nametype = "HSFileName";
			inisec = "OracleHelpOptions";
			suffix = "htm";
			suffix2 = "hs";
			isohj = true;
		}
		else if (!strcmp(cf, "Word RTF")) {
			inisec = "WordOptions";
			suftype = "WordSuffix";
			isword = true;
		}
		else if (!strcmp(cf, "OmniHelp")) {
			nametype = "ProjectName";
			inisec = "OmniHelpOptions";
			isoh = true;
			suftype = "HTMLSuffix";
		}
		else if (!strcmp(cf, "Eclipse Help")) {
			inisec = "EclipseHelpOptions";
			iseh = true;
		}
		else if (!strcmp(cf, "HTML 4.0")) {
			inisec = "HTMLOptions";
			suftype = "HTMLSuffix";
			ishtm = true;
		}
		else if (!strcmp(cf, "XHTML 1.0")) {
			inisec = "HTMLOptions";
			suftype = "HTMLSuffix";
			ishtm = true;
		}
		else if (!strcmp(cf, "DITA")) {
			inisec = "DITAOptions";
			suftype = "XMLSuffix";
			isdita = true;
		}
		else if (!strcmp(cf, "DocBook")) {
			inisec = "DocBookOptions";
			suftype = "XMLSuffix";
			isxml = true;
		}
		else {
			inisec = "HTMLOptions";
			suftype = "XMLSuffix";
			isxml = true;
		}
	}

	if (CurrIni && CurrOutputDir) {
		_chdir(CurrOutputDir->c_str());
		wxString *inidest = MakeFullPath(CurrOutputDir, CurrIni);
		DCirfile *inif = new DCirfile(inidest->c_str());
		if (inif
		 && (inif->FErr() == fok)
		 && (inif->Ferr == fok)) {
			inif = inif->SetConfigTemplate("Templates", "Configs");
			
			if (inif->Section("Setup")) {
				if (inif->Find("FileSuffix")) {
					suffix = inif->StrRight();
					if (*suffix == '.')
						suffix++;
				}
				else if (!suffix) {
					if (suftype
					 && inif->Find(suftype)) {
						suffix = inif->StrRight();
						if (*suffix == '.')
							suffix++;
					}
					else if (isword)
						suffix = "rtf";
					else if (isdita)
						suffix = "dita";  // or ditamap, or bookmap... ****
					else if (isxml)
						suffix = "xml";
					else
						suffix = "htm";
				}
				if (!suffix2)
					suffix2 = suffix;
				if (!isjh && !isohj)
					OutputExt = new wxString(suffix);
			}
			if (inif->Section("Automation")) {
				if (inif->Find("WrapPath"))
					WrapPath = inif->StrRight();
				else
					WrapPath = "_wrap";
				if (isjh) {
					if (inif->Find("MakeJar"))
						makejar = inif->BoolRight();
					if (makejar)
						suffix2 = "jar";
				}
				if (iseh) {
					if (inif->Find("ArchiveName"))
						projname = inif->StrRight();
					if (inif->Find("ArchiveVer"))
						projver = inif->StrRight();
					if (projname && projver) {
						char *nm = NewName(projname, strlen(projname) + strlen(projver));
						strcat(nm, projver);
						DeleteName(projname);
						projname = nm;
					}
					if (inif->Find("ArchiveExt"))
						suffix2 = inif->StrRight();
				}
			}

			// determine name of output file to run
			// replace map name with project name if any
			// combine any prefix (as in OH), name, suffix
			// for JH and OHJ, use HSFileName
			// for HH, use DefaultChmFile or HHPFileName
			// for OH, use ProjectName, OHProjFilePrefix, OHProjFileSuffix
			// for WH, use HPJFileName
			// for EH, use ArchiveName, ArchiveVer, ArchiveExt
			if (!projname) {
				if (inisec
				 && nametype
				 && inif->Section(inisec)
				 && (inif->Find(nametype)
				  || (nametype2
				   && inif->Find(nametype2)))) {
					char *per = NULL;
					projname = inif->StrRight();
					if ((isjh || isohj)
					 && ((per = strrchr(projname, '.')) != NULL)
					 && !strcmp(per, ".hs"))
						*per = '\0';
					else if (ishh
					 && ((per = strrchr(projname, '.')) != NULL)
					 && !strcmp(per, ".hhp"))
						*per = '\0';
				}
				else
					projname = NewName((CurrSourceMap->BeforeLast('.')).c_str());
			}

			if (isoh) {
				if (inif->Find("OHProjFilePrefix"))
					prefix = inif->StrRight();
				else
					prefix = "_";
				if (prefix) {
					char *nm = NewName(prefix, strlen(projname) + strlen(prefix));
					strcat(nm, projname);
					DeleteName(projname);
					projname = nm;
				}
				if (inif->Find("OHProjFileSuffix")) {
					char *suf = inif->StrRight();
					char *nm = NewName(projname, strlen(projname) + strlen(suf));
					strcat(nm, suf);
					DeleteName(projname);
					projname = nm;
				}
			}
			else if (ishtm) {
				if (inif->Section("Contents")
				 && (inif->Find("TOCFile")))
					projname = inif->StrRight();
			}

			if (projname && suffix2) {
				char *nm = NewName(projname, strlen(projname) + strlen(suffix2) + 1);
				if (*suffix2 != '.')
					strcat(nm, ".");
				strcat(nm, suffix2);
				DeleteName(projname);
				projname = nm;
			}

			// combine CurrOutputDir with WrapPath
			CurrWrapDir = MakeFullPath(CurrOutputDir, new wxString(WrapPath));

			// determine command needed to run it
			// for jh, ohj, eh, get from *HelpOptions
			// for rtf, get from WordOptions] if present
			// for HTML, XHTML, XML, OH, and WH just use name
			char *basecommand = NULL;
			if (inisec
			 && inif->Section(inisec)) {
				if (inif->Find("ViewOutputCommand"))
					basecommand = inif->StrRight();
				if (inif->Find("ViewOutputFile"))
					projname = inif->StrRight();
			}
			ViewOutputCommand = new wxString();
			if (basecommand) {
				*ViewOutputCommand += basecommand;
				*ViewOutputCommand += _(" ");
			}
			if (projname)
				*ViewOutputCommand += projname;
			inif->CloseFile();
			delete inif;
		}
	}
	if (CurrWrapDir && ViewOutputCommand)
		m_runviewout->Enable();
}


void D2gmain::StartRun(void)
{
	Running = true;
	m_runstart->Disable();
	m_runeditini->Disable();
	m_runcm->Disable();
	m_runshowlog->Disable();
	m_runviewout->Disable();
	m_runstop->Enable();
	SetNewIni();
	WrapAndShip = m_runwasyes->GetValue();

	SetViewOutputCommand(true);
	m_runeditini->Disable();
	m_runcm->Disable();
	m_runviewout->Disable();

	StopCommand = false;
	bool cmderr = false;

	SetUpLogging();
	LogEvent(loginfo, 1, "Project Manager started ",
		                    CurrProject->c_str(), NULL, NULL, NULL);

	wxString *cmd = new wxString();
	wxString *cmd2 = NULL;
#ifdef DITA
	*cmd += _("%omsyshome%\\common\\bin\\dcl -f ");
#endif
#ifdef UDOC
	*cmd += _("%omsyshome%\\common\\bin\\dcl -u -f ");
#endif
	*cmd += *CurrType;
	*cmd += _(" ");

	if (UseDCLSource
	 && UseDCLOutput) {
		*cmd += _("-t dcl ");
		*cmd += *MakeFullPath(CurrSourceDir, CurrSourceMap);
		cmd2 = new wxString();
		*cmd2 += _("%omsyshome%\\common\\bin\\dcl -f ");
		*cmd2 += *CurrType;
		*cmd2 += _(" ");
		if (OutputExt) {
			*cmd2 += _("-o .");
			*cmd2 += *OutputExt;
			*cmd2 += _(" ");
		}
		*cmd2 += CurrSourceMap->BeforeLast('.');
		*cmd2 += _(".dcl");
	}
	else if (UseDCLOutput) {
		*cmd += _("-t dcl ");
		*cmd += *MakeFullPath(CurrSourceDir, CurrSourceMap);
	}
	else if (UseDCLSource) {
		if (OutputExt) {
			*cmd += _("-o .");
			*cmd += *OutputExt;
			*cmd += _(" ");
		}
		*cmd += CurrSourceMap->BeforeLast('.');
		*cmd += _(".dcl");
	}
	else {
		if (OutputExt) {
			*cmd += _("-o .");
			*cmd += *OutputExt;
			*cmd += _(" ");
		}
		*cmd += *MakeFullPath(CurrSourceDir, CurrSourceMap);
	}

	m_runresulttext->Clear();
	m_runresulttext->AppendText(*RunningText);
	m_runresulttext->AppendText(*CurrProject);

	CloseLog();
	RunDOSCommand(cmd->c_str(), CurrOutputDir->c_str(),
	             !ShowConsole, ShowConsole, !ShowConsole);
	OpenLog();
	if (ExitType
	 || LocalExitCode) {
		LogResults((char *) cmd->c_str());
		cmderr = true;
	}

	if (cmd2) {
		CloseLog();
		RunDOSCommand(cmd2->c_str(), CurrOutputDir->c_str(),
								 !ShowConsole, ShowConsole, !ShowConsole);
		OpenLog();
		if (ExitType
		 || LocalExitCode) {
			LogResults((char *) cmd->c_str());
			cmderr = true;
		}
	}

	Running = false;
	if (ShowConsole) {  // async
		m_runresulttext->AppendText(*RunningAsychText);
		WaitForCurrProcess();
	}
	m_runstop->Disable();
	m_runstart->Enable();
	m_runeditini->Enable();
	m_runcm->Enable();
	m_runshowlog->Enable();
	m_runviewout->Enable();
	m_runresulttext->Clear();
	if (StopCommand)
		m_runresulttext->AppendText(*RunningStopText);
	else if (cmderr)
		m_runresulttext->AppendText(*RunningErrorText);
	else
		m_runresulttext->AppendText(*RunningDoneText);
	m_runresulttext->AppendText(*CurrProject);
	StopCommand = false;
	LogEvent(loginfo, 1, "Project Manager completed ",
		                     CurrProject->c_str(), NULL, NULL, NULL);
	CloseLog();
	if (ShowLog
	 || CheckLogErrors())
		ShowLogFile();
}


void D2gmain::StopRun(void)
{
	StopCommand = true;
}


// path handling

bool D2gmain::FileExists(char *fname)
{
	if (!fname)
		return false;

	DCfile *fp = new DCfile(fname);

	if (fp->FErr() == fno) {
		delete fp;
		return false;
	}

	fp->CloseFile();
	delete fp;
	return true;
}


wxString *D2gmain::MakeFullPath(wxString *dir, wxString *fn)
{
	wxString *str = new wxString(*dir);
	wxString *ofn = fn ? new wxString(*fn) : NULL;
	if (str->Last() == '\\')
		(*str) = str->BeforeLast('\\');
	if (ofn) {
		wxString *nfn = new wxString();
		if (ofn->StartsWith(".\\", nfn))
			*ofn = *nfn;
		while (ofn->StartsWith("..\\", nfn)) {
			*ofn = *nfn;
			(*str) = str->BeforeLast('\\');
		}
		if (ofn->StartsWith(".\\", nfn))
			*ofn = *nfn;
		if (str->Last() != '\\')
			(*str) += _("\\");
		(*str) += *ofn;
	}
	return str;
}


wxString *D2gmain::FileFromPath(wxString *path)
{
	wxString *str = new wxString(path->AfterLast('\\'));
	return str;
}


wxString *D2gmain::DirFromPath(wxString *path)
{
	wxString *str = new wxString(path->BeforeLast('\\'));
	return str;
}


wxString *D2gmain::FixEnvPath(wxString *str)
{
	// get NewIniPath and ProjDirD2GPar
	// and resolve any environment vars in them.
	int first = 0;
	int next = 0;
	const char *cstr = NULL;
	char *estr = NULL;
	char *ostr = NULL;
	char *nstr = NULL;

	if (!str)
		return str;

	while (str->Find('%') != wxNOT_FOUND) {
		cstr = str->c_str();
		for (int i = 0; cstr[i] != '\0'; i++) {
			if (cstr[i] == '%') {
				if (first > 0) {
					estr = NewName(cstr + first, i - first);
					nstr = getenv(estr);
					if (nstr && *nstr) {
						ostr = NewName((cstr + first) - 1, (i - first) + 2);
						str->Replace(ostr, nstr);
						DeleteName(ostr);
						DeleteName(estr);
						break;
					}
					DeleteName(estr);
					first = 0;
				}
				else
					first = i + 1;
			}
		}
		if (first == 0)
			break;
	}
	return str;	
}




// end of d2gdcl.cpp

