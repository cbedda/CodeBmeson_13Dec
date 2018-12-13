class AliAnalysisGrid;
class AliAnalysisAlien;
//class AliAnalysisTaskSEB0toDminuspi_wBcuts;
//class AliRDHFCutsBtoDpluspi;

void RunAnalysisAODB0tryDpi_root5()
{
  //
  // Test macro for AliAnalysisTaskSE's for heavy-flavour candidates
  // It has the structure of a Analysis Train:
  // - in this macro, change things related to running mode
  //   and input preparation 
  // - add your task using a AddTaskXXX macro 
  //
  // A.Dainese, andrea.dainese@lnl.infn.it
  // "grid" mode added by R.Bala, bala@to.infn.it
  //
  // gSystem->SetIncludePath("-I. -I$ROOTSYS/include -I$ALICE_PHYSICS/include -g");

 gSystem->SetIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWG/FLOW/Base -I$ALICE_PHYSICS/PWG/FLOW/Tasks -I$ALICE_PHYSICS/PWG -g"); 
  //
  TString trainName = "D2H";
  TString analysisMode = "grid"; // "local", "grid", or "proof"
  TString inputMode    = "list"; // "list", "xml", or "dataset"
  Long64_t nentries=123567890,firstentry=0;
  Bool_t useParFiles=kFALSE;
  Bool_t useAlienPlugin=kTRUE;
  TString pluginmode="terminate"; //test, full, terminate
  TString testfileslistWithPlugin="";
  Bool_t saveProofToAlien=kFALSE;
  TString proofOutdir = "";
  TString loadMacroPath="$ALICE_PHYSICS/PWGHF/vertexingHF/macros/";
  Bool_t isMC=kTRUE;
  //TString loadMacroPath="./"; // this is normally needed for CAF
  //

  if(analysisMode=="grid") {
    // Connect to AliEn
    TGrid::Connect("alien://");
  } else if(analysisMode=="proof") {
    // Connect to the PROOF cluster
    if(inputMode!="dataset") {printf("Input mode must be dataset, for proof analysis\n"); return;}
    gEnv->SetValue("XSec.GSI.DelegProxy","2");
    TProof::Open("alicecaf");
    //TProof::Reset("alicecaf");
    if(saveProofToAlien) {
      TGrid::Connect("alien://");
      if(gGrid) {
	TString homedir = gGrid->GetHomeDirectory();
	TString workdir = homedir + trainName;
	if(!gGrid->Cd(workdir)) {
	  gGrid->Cd(homedir);
	  if(gGrid->Mkdir(workdir)) {
	    gGrid->Cd(trainName);
	    ::Info("VertexingTrain::Connect()", "Directory %s created", gGrid->Pwd());
	  }
	}	   
	gGrid->Mkdir("proof_output");
	gGrid->Cd("proof_output");
	proofOutdir = Form("alien://%s", gGrid->Pwd());
      } 
    }
  }


  // AliRoot libraries
  if(analysisMode=="local" || analysisMode=="grid") {
    TString loadLibraries="LoadLibraries.C"; loadLibraries.Prepend(loadMacroPath.Data());
    gROOT->LoadMacro(loadLibraries.Data());
    LoadLibraries(useParFiles);
    gSystem->Load("libGui");
    gSystem->Load("libRAWDatabase");
    gSystem->Load("libCDB");
    gSystem->Load("libSTEER");
    gSystem->Load("libTRDbase");
    gSystem->Load("libPWGTRD");
  } else if (analysisMode=="proof") {
    gSystem->Load("libTree");
    gSystem->Load("libGeom");
    gSystem->Load("libPhysics");
    gSystem->Load("libVMC");
    gSystem->Load("libMinuit");
    gSystem->Load("libGui");
    gSystem->Load("libRAWDatabase");
    gSystem->Load("libCDB");
    gSystem->Load("libSTEER");
    gSystem->Load("libTRDbase");
    gSystem->Load("libPWGTRD");
    // Enable the needed packages
    //gProof->ClearPackages();
    TString parDir="/afs/cern.ch/user/d/dainesea/code/";
    TString parFile;
    if(!useParFiles) {
      gProof->UploadPackage("AF-v4-17");
      gProof->EnablePackage("AF-v4-17");
      // --- Enable the PWGHFvertexingHF Package
      parFile="PWGHFvertexingHF.par"; parFile.Prepend(parDir.Data());
      gProof->UploadPackage(parFile.Data());
      gProof->EnablePackage("PWGHFvertexingHF");
    } else {
      // --- Enable the STEERBase Package
      parFile="STEERBase.par"; parFile.Prepend(parDir.Data());
      gProof->UploadPackage(parFile.Data());
      gProof->EnablePackage("STEERBase");
      // --- Enable the ESD Package
      parFile="ESD.par"; parFile.Prepend(parDir.Data());
      gProof->UploadPackage(parFile.Data());
      gProof->EnablePackage("ESD");
      // --- Enable the AOD Package
      parFile="AOD.par"; parFile.Prepend(parDir.Data());
      gProof->UploadPackage(parFile.Data());
      gProof->EnablePackage("AOD");
      // --- Enable the ANALYSIS Package
      parFile="ANALYSIS.par"; parFile.Prepend(parDir.Data());
      gProof->UploadPackage(parFile.Data());
      gProof->EnablePackage("ANALYSIS");
      // --- Enable the ANALYSISalice Package
      parFile="ANALYSISalice.par"; parFile.Prepend(parDir.Data());
      gProof->UploadPackage(parFile.Data());
      gProof->EnablePackage("ANALYSISalice");
      // --- Enable the CORRFW Package
      parFile="CORRFW.par"; parFile.Prepend(parDir.Data());
      gProof->UploadPackage(parFile.Data());
      gProof->EnablePackage("CORRFW");
      // --- Enable the PWGHFbase Package
      parFile="PWGHFbase.par"; parFile.Prepend(parDir.Data());
      gProof->UploadPackage(parFile.Data());
      gProof->EnablePackage("PWGHFbase");
      // --- Enable the PWGHFvertexingHF Package
      parFile="PWGHFvertexingHF.par"; parFile.Prepend(parDir.Data());
      gProof->UploadPackage(parFile.Data());
      gProof->EnablePackage("PWGHFvertexingHF");
    }
    gProof->ShowEnabledPackages(); // show a list of enabled packages
  }


  // Create Alien plugin, if requested
  if(useAlienPlugin) {  
    //    if(analysisMode!="grid") {printf("Analysis mode must be grid, to use alien plugin\n"); return;}
    AliAnalysisGrid *alienHandler = CreateAlienHandler(pluginmode,useParFiles,testfileslistWithPlugin,isMC);  
    if(!alienHandler) return;
  }


  //-------------------------------------------------------------------
  // Prepare input
  TChain *chainAOD = 0;
  TString dataset; // for proof

  if(!useAlienPlugin) {
    TString makeAODInputChain="../MakeAODInputChain.C"; makeAODInputChain.Prepend(loadMacroPath.Data());
    if(inputMode=="list") {
      // Local files
      gROOT->LoadMacro(makeAODInputChain.Data());
      chainAOD = MakeAODInputChain();// with this it reads ./AliAOD.root and ./AliAOD.VertexingHF.root
      //chainAOD = MakeAODInputChain("alien:///alice/cern.ch/user/r/rbala/newtrain/out_lhc08x/180100/",1,1);
      printf("ENTRIES %d\n",chainAOD->GetEntries());
    } else if(inputMode=="xml") {
      // xml
      gROOT->LoadMacro(makeAODInputChain.Data());
      chainAOD = MakeAODInputChain("collection_aod.xml","collection_aodHF.xml");
    } else if(inputMode=="dataset") {
      // CAF dataset
      //gProof->ShowDataSets();
      dataset="/ITS/dainesea/AODVertexingHF_LHC08x_180100";
    }
  }

  // Create the analysis manager
  AliAnalysisManager *mgr  = new AliAnalysisManager("My Manager","My Manager");
  mgr->SetDebugLevel(10);
  // Connect plug-in to the analysis manager
  if(useAlienPlugin) mgr->SetGridHandler(alienHandler);

  // Input
  AliAODInputHandler *inputHandler = new AliAODInputHandler("handler","handler for D2H");
  if(analysisMode=="proof" ) {
    inputHandler->AddFriend("./AliAOD.VertexingHF.root");
    //inputHandler->AddFriend("deltas/AliAOD.VertexingHF.root");
    if(saveProofToAlien) mgr->SetSpecialOutputLocation(proofOutdir);
  }
  mgr->SetInputEventHandler(inputHandler);
  //-------------------------------------------------------------------

  
  //-------------------------------------------------------------------
  // Analysis tasks (wagons of the train)   
  //




  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
  
  AddTaskPhysicsSelection(isMC,kTRUE);// second argument to enable pileupcut
  // else AddTaskPhysicsSelection(kTRUE,kTRUE);



  
// in your macro:
//gROOT->ProcessLine(".L $ALICE_PHYSICS/OADB/macros/AddTaskPhysicsSelection.C");
//AliPhysicsSelectionTask*physSelTask=AddTaskPhysicsSelection(isMC,enablePileupCuts); // in your UserExec:
//UInt_t fSelectMask= fInputHandler->IsEventSelected();
//Bool_t isINT7selected = fSelectMask & AliVEvent::kINT7;
//Bool_t isHMV0selected = fSelectMask & AliVEvent::kHighMultV0
  

   
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
  //  AddTaskPhysicsSelection(kTRUE,kTRUE);
  AddTaskMultSelection(kTRUE);
  //  __R_ADDTASK__->SetSelectedTriggerClass((AliVEvent::kINT7 | AliVEvent::kHighMultV0));



  
  // First add the task for the PID response setting
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
  AliAnalysisTaskSE *setupTask;
  if(!isMC)  setupTask = AddTaskPIDResponse(kFALSE, kTRUE));//kFALSE,kTRUE);//first option to be change for MC!!!!!!!!!! kTRUE=MC
 else setupTask = AddTaskPIDResponse(kTRUE,kTRUE);//first option to be change for MC!!!!!!!!!! kTRUE=MC

  /*
    
    // attach a private task (not committed)
    // (the files MyTask.h MyTask.cxx AddMyTask.C have to be declared in plugin
    // configuration, see below)
*/

  /*
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskDplus.C"); 

//AddTaskDStarSpectra.C");
  //AddTaskDplus.C");

  //AliAnalysisTaskSEDStarSpectra *d0massTask01 = AddTaskDStarSpectra(1,0,10,
    //								    " DStar180-kINt7-central-6-7.root",
								    //"DStar080-kMB-central-6-7.root",
								    //						    "alien:///alice/cern.ch/user/s/sjaelani/jay/DStar010-kINT7-central-6-7-tight1.root",
								    //								    DStar010-kINT7-central.root",
    //								    "vv01",kFALSE,kFALSE);

  //   d0massTask01->SetDebugLevel(10);
  
  AliAnalysisTaskSEDplus *Dplus=AddTaskDplus(
					     //1,0,10,0,0,kFALSE,kFALSE,"_010_Topod0Cut_onlyTPCPid_Raa",
  1,30,50,0,kTRUE,kFALSE,kFALSE,"_3050_NDLxyLoose_d0Cut_Raa",
  //"alien:///alice/cern.ch/user/f/fgrosa//DplustoKpipiCuts_3050_ndlxyloose_d0cut_Raa_kINT7.root",
  //		     "alien:///alice/cern.ch/user/f/fgrosa//DplustoKpipiCuts_010_central_topod0cut_onlyTPCPID_Raa_kINT7.root",

  					         "DplustoKpipiCuts_3050__Raa_kINT7_Fab.root",

					     //					     "DplustoKpipiCuts_PbPb_CINT7.root",
  "AnalysisCuts",0,kFALSE,-1,-1
  				     );

  */
  gROOT->LoadMacro("AliRDHFCutsBtoDpluspi.cxx++g");
   
   gROOT->LoadMacro("AliAnalysisTaskSEB0toDminuspi_wBcuts.cxx++g");
  gROOT->LoadMacro("AddTaskB0Dminuspi_wBcuts.C");

if(!isMC)AliAnalysisTaskSEB0toDminuspi_wBcuts *myTask  = AddTaskB0Dminuspi_wBcuts(0,0,100,"DplustoKpipiCuts_PbPb_Tight.root","BtoDpluspi_cut1",kFALSE);
 else AliAnalysisTaskSEB0toDminuspi_wBcuts *myTask  = AddTaskB0Dminuspi_wBcuts(0,0,100,"DplustoKpipiCuts_PbPb_Tight.root","BtoDpluspi_cut1",kTRUE);



  
      
//gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/vertexingHF/macros/AddTaskCleanupVertexingHF.C");
// AddTaskCleanupVertexingHF();
  
/*    if(analysisMode.Data()=="proof") {
    gProof->LoadMacro("AliDStarJets.cxx++g");
    } else {
    gROOT->LoadMacro("AliDStarJets.cxx++g");
    }
    gROOT->LoadMacro("AddTaskDStarJets.C");
    AliDStarJets *myTask = AddTaskDStarJets();*/
  
  //-------------------------------------------------------------------
  
  //
  // Run the analysis
  //    
  if(chainAOD) printf("CHAIN HAS %d ENTRIES\n",(Int_t)chainAOD->GetEntries());
  
  if(!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  if(analysisMode=="grid" && !useAlienPlugin) analysisMode="local";
  if(analysisMode!="proof") {
    mgr->StartAnalysis(analysisMode.Data(),chainAOD,nentries,firstentry);
  } else {
    // proof
    mgr->StartAnalysis(analysisMode.Data(),dataset.Data(),nentries,firstentry);
  }
  
  return;
}
//_____________________________________________________________________________
//
AliAnalysisGrid* CreateAlienHandler(TString pluginmode="test",Bool_t useParFiles=kFALSE, TString testfileslistWithPlugin="", Bool_t MC)
{
  // Check if user has a valid token, otherwise make one. This has limitations.
  // One can always follow the standard procedure of calling alien-token-init then
  //   source /tmp/gclient_env_$UID in the current shell.
   AliAnalysisAlien *plugin = new AliAnalysisAlien();
   // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
   plugin->SetRunMode(pluginmode.Data());
   plugin->SetUser("cbedda");
   // Set versions of used packages
   //plugin->SetAPIVersion("V1.1x");
   //plugin->SetROOTVersion("v5-34-30-alice-1");
   //   plugin->SetAliROOTVersion("vAN-20170111");
   plugin->SetAliPhysicsVersion("vAN-20181128-1");
   plugin->SetNtestFiles(1);
   //   gROOT->LoadMacro("$ALICE_PHYSICS/PWGHF/vertexingHF/AddGoodRuns.C");
   gROOT->LoadMacro("../../AddGoodRuns.C");

   Int_t year=2015;
   TString nAOD="";//"202"(17d2a);
   TString lhcper="LHC15a2a";//"LHC18b_pass1";//"LHC16k_pass2";//"LHC17d2a";//"LHC18f4a";
   TString mcprod="LHC15a2a";//"LHC17d2a_cent_new";//"LHC18f4a";
   TString extrafolder="pass1";//"pass2";//extrafolder for data, es pass1_pidfix
   TString datafolder="LHC18b";//"LHC16k";//folder for data

   TString outputfoldername="output_Bmeson_pp7MC_11Dec";//"output_Bmeson_pp13MC_HMV0_18b";//"output_Bmeson_pp7MC_11Dec";//"output_Bmeson_pp13_16k";//"output_Bmeson_pp13MC_SigVer_bo";//"output_Bmeson_pp13_16kl";//"output_Bmeson_pp13_29Nov_HM16k";//folder for data

   // Declare input data to be processed.
   //************************************************
   // Set data file list to test on local mode
   //************************************************  
   plugin->SetFileForTestMode(testfileslistWithPlugin.Data());

   //************************************************
   // Set data search pattern for DATA
   //************************************************  
   //Method 1: To create automatically xml through plugin  
/*   plugin->SetGridDataDir("/alice/data/2010/LHC10d"); // specify LHC period
   plugin->SetDataPattern("pass2/AOD018/*AliAOD.root"); // specify reco pass and AOD set
   plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
   // OR plugin->SetFriendChainName("deltas/AliAOD.VertexingHF.root");
   // Adds only the good runs from the Monalisa Run Condition Table
   // More than one period can be added but the period name has to be removed from GridDataDir (to be tested)
   Int_t totruns=0;
   //totruns += AddGoodRuns(plugin,"LHC10b"); // specify LHC period
   //totruns += AddGoodRuns(plugin,"LHC10c"); // specify LHC period
   totruns += AddGoodRuns(plugin,"LHC10d"); // specify LHC period
   plugin->SetNrunsPerMaster(totruns);
*/
   // Method 2: Declare existing data files (e.g xml collections)

   //plugin->AddDataFile("/alice/cern.ch/user/r/rbala/000168068_000170593.xml");
   //  plugin->SetDataPattern("*AliAOD.root");
   //  plugin->SetFriendChainName("./AliAOD.VertexingHF.root"); 

   //************************************************
   // Set data search pattern for MONTECARLO
   //************************************************
   
   if(!MC){
      plugin->SetGridDataDir(Form("/alice/data/%d/%s",year,datafolder.Data()));///alice/sim/2015/LHC15a2a"); // specify MC sample per pp
     plugin->SetDataPattern(Form("*/%s/AOD%s/*AliAOD.root",extrafolder.Data(),nAOD.Data())); // specify AOD set per pp
     //plugin->SetGridDataDir("/alice/data/2016/LHC16l");///alice/sim/2015/LHC15a2a"); // specify MC sample per pp
     //plugin->SetDataPattern("*/pass2/AOD/*AliAOD.root"); // specify AOD set per pp
   }

   //    plugin->SetRunPrefix("000");
   //   plugin->SetGridDataDir("/alice/data/2013/LHC13b"); // specify MC sample per pPb
    //plugin->SetDataPattern("ESDs/pass3/AOD/*/AliAOD.root"); // specify AOD set per pPb
   // plugin->SetGridDataDir("/alice/data/2016/LHC16q"); // specify LHC period
   // plugin->SetDataPattern("/pass1_CENT_woSDD/AOD/*AliAOD.root"); //per 16q// specify reco pass and AOD set
   //  plugin->SetDataPattern("/pass1_FAST/AOD/*AliAOD.root"); //per 16q// specify reco pass and AOD set
   else {
     //plugin->SetGridDataDir("/alice/sim/2016/LHC16i2a");
     // plugin->SetDataPattern("*/AOD198/*/AliAOD.root");
     plugin->SetGridDataDir(Form("/alice/sim/%d/%s",year,lhcper.Data()));
     plugin->SetDataPattern(Form("*/AOD%s/*/AliAOD.root",nAOD.Data()));
     //   	plugin->SetGridDataDir("/alice/sim/2018/LHC18f4a");
     //    plugin->SetDataPattern("*/AOD/*/AliAOD.root");
   }
   
   //plugin->SetGridDataDir("/alice/sim/2017/LHC17d2a_cent_new");
  // plugin->SetDataPattern("*/AOD/*/AliAOD.root");
   plugin->SetFriendChainName("./AliAOD.VertexingHF.root");
    
   // OR plugin->SetFriendChainName("deltas/AliAOD.VertexingHF.root");
   // Adds only the good runs from the Monalisa Run Condition Table 
   // More than one period can be added!
   Int_t totruns=0;
     if(!MC)  totruns += AddGoodRuns(plugin,Form("%s",lhcper.Data()));
     // if(!MC)  totruns += AddGoodRuns(plugin,"LHC16l");
     //else  totruns += AddGoodRuns(plugin,"LHC16i2a","LHC16i2a_pass1_pidfix");
     //else totruns += AddGoodRuns(plugin,"LHC18f4a","LHC18f4a");
   else totruns += AddGoodRuns(plugin,Form("%s",lhcper.Data()),Form("%s",mcprod.Data()));
   //LHC17d2a_cent_new // totruns += AddGoodRuns(plugin,"LHC16q"); // specify LHC period for anchor runs; and the name of the MC production pp
   //totruns += AddGoodRuns(plugin,"LHC10c","LHC10f7"); // specify LHC period for anchor runs;  and the name of the MC production
   //totruns += AddGoodRuns(plugin,"LHC10d","LHC10f7"); // specify LHC period for anchor runs;  and the name of the MC production


   //Henrique settings
    plugin->SetNrunsPerMaster(50);
   plugin->SetTTL(2.5*60*60);
   plugin->SetSplitMaxInputFileNumber(50);
   plugin->SetMasterResubmitThreshold(10);

   //
   // Define alien work directory where all files will be copied. Relative to alien $HOME.
   plugin->SetGridWorkingDir("myHFanalysisPbPb");
   // Name of executable
				   plugin->SetExecutable("myHFanalysisPro.sh");
   // Declare alien output directory. Relative to working directory.
   //  plugin->SetGridOutputDir("output_Bmeson_pPb_16qwSDD_commit_Cri_new"); // In this case will be $HOME/work/output
   //    plugin->SetGridOutputDir("output_Bmeson_pPbMC_22Jun_count_check");
				   plugin->SetGridOutputDir(Form("%s",outputfoldername.Data()));
    // plugin->SetGridOutputDir("output_Bmeson_pPbMC_d3plus_RightSign");

//SignalCommit_topo"); // In this case will be $HOME/work/output                      
// Declare the analysis source files names separated by blancs. To be compiled runtime
   // using ACLiC on the worker nodes.
    plugin->SetAnalysisSource("AliRDHFCutsBtoDpluspi.cxx AliAnalysisTaskSEB0toDminuspi_wBcuts.cxx ");
   // Declare all libraries (other than the default ones for the framework. These will be
   // loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.

    //libPWGflowBase.so libPWGflowTasks.so libPWGHFbase.so libPWGHFvertexingHF.so libGui.so libRAWDatabase.so libCDB.so libSTEER.so libTRDbase.so libPWGTRD.so
    
     plugin->SetAdditionalLibs("AliRDHFCutsBtoDpluspi.h AliRDHFCutsBtoDpluspi.cxx AliAnalysisTaskSEB0toDminuspi_wBcuts.h AliAnalysisTaskSEB0toDminuspi_wBcuts.cxx "); //AliAnalysisTaskSEDStarSpectra2.cxx"); //AliAnalysisTaskSEDStarSpectra2.cxx AliAnalysisTaskSEDStarSpectra2.h");
 
     //plugin->SetAdditionalLibs("AliAnalysisTaskSEB0toDminuspi_2.h AliAnalysisTaskSEB0toDminuspi_2.cxx AddTaskB0Dminuspi_2.C");
     //plugin->SetAdditionalLibs("AddTaskB0Dminuspi_2.C");
 
   // use par files
   if(useParFiles) {
     plugin->EnablePackage("STEERBase.par");
     plugin->EnablePackage("ESD.par");
     plugin->EnablePackage("AOD.par");
     plugin->EnablePackage("ANALYSIS.par");
     plugin->EnablePackage("OADB.par");
     plugin->EnablePackage("ANALYSISalice.par");
     plugin->EnablePackage("CORRFW.par");
     plugin->EnablePackage("PWGHFbase.par");
     plugin->EnablePackage("PWGHFvertexingHF.par");
   }
   plugin->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS -I$ALICE_PHYSICS/include -I$ALICE_ROOT/ITS -I$ALICE_ROOT/TPC -I$ALICE_ROOT/CONTAINERS -I$ALICE_ROOT/STEER/STEER -I$ALICE_ROOT/STEER/STEERBase -I$ALICE_ROOT/STEER/ESD -I$ALICE_ROOT/STEER/AOD -I$ALICE_ROOT/TRD -I$ALICE_ROOT/macros -I$ALICE_ROOT/ANALYSIS  -I$ALICE_PHYSICS/OADB -I$ALICE_PHYSICS/PWGHF -I$ALICE_PHYSICS/PWGHF/base -I$ALICE_PHYSICS/PWGHF/vertexingHF -I$ALICE_PHYSICS/PWG/FLOW/Base -I$ALICE_PHYSICS/PWG/FLOW/Tasks -I$ALICE_PHYSICS/PWG -g"); 

   plugin->SetDefaultOutputs(kTRUE);
   // merging via jdl
   plugin->SetMergeViaJDL(kTRUE);
   plugin->SetOneStageMerging(kFALSE);
   plugin->SetMaxMergeStages(2);

   // Optionally set a name for the generated analysis macro (default MyAnalysis.C)
   plugin->SetAnalysisMacro("AnalysisHF.C");
   // Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
   // Optionally modify the name of the generated JDL (default analysis.jdl)
   plugin->SetJDLName("TaskHF.jdl");

   return plugin;
}
