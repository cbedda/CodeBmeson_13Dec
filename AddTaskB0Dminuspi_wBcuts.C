//if like define a different number of signal for TPC PID

//by default the task is anyway computing 1, 2 and 3 sigmas

const Bool_t theRareOn = kTRUE;

const Bool_t anaType   = 1;//0 HD; 1 UU;

const Bool_t doImp   = kFALSE;// imp par studies

//----------------------------------------------------



AliAnalysisTaskSEB0toDminuspi_wBcuts *AddTaskB0Dminuspi_wBcuts(Int_t system=0/*0=pp,1=PbPb*/,

						   Float_t minC=0,
Float_t maxC=100, TString cutsfile="", TString usercomment = "username", Bool_t theMCon=kFALSE)//, Double_t TopomaticDdaughter=0.,Double_t pTD=0.,Double_t pTpi=0.,Double_t prodd0=999., Double_t cosB=-999.,Double_t cosBXY=-999.,Double_t dlB=0.,Double_t NdlBXY=0., Double_t TopomaticD=999.,Double_t Topomaticpi=999., Double_t cosoaDpi=-999.)

{



  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  if (!mgr) {

    ::Error("", "No analysis manager to connect to.");

    return NULL;

  }
  //from makeTFile
     AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(kFALSE);
    //default
    esdTrackCuts->SetRequireTPCRefit(kTRUE);
    esdTrackCuts->SetRequireITSRefit(kTRUE);
    //esdTrackCuts->SetMinNClustersITS(4); // default is 5
    esdTrackCuts->SetMinNClustersTPC(70);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,
					   AliESDtrackCuts::kAny); 
    // default is kBoth, otherwise kAny
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    esdTrackCuts->SetPtRange(0.3,1.e10);
    
    
    
    const Int_t nptbins=5;
    Float_t* ptbins;
    ptbins=new Float_t[nptbins+1];
    ptbins[0]=0.;
    ptbins[1]=3.;
    ptbins[2]=6.;
    ptbins[3]=10.;
    ptbins[4]=18.;
    ptbins[5]=50.;
    //    ptbins[6]=6.;
    //ptbins[7]=8.;
    //ptbins[8]=10.;
    //ptbins[9]=18.;
    //ptbins[10]=30.;
    //ptbins[11]=50.;
 
    const Int_t nvars=20;
    
     Float_t** anacutsval;
    anacutsval=new Float_t*[nvars];
    
    for(Int_t ic=0;ic<nvars;ic++){anacutsval[ic]=new Float_t[nptbins];}

    
    Int_t ic=0; //    0 "D inv. mass [GeV]",

    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.2;
    }
    ic=1;   // “D pTK [GeV/c]",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.3;
    }
  
    ic=2;   //D pTPi [GeV/c]"
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.3;
    }
    
  
    
    ic=3; //D d0K [cm]   lower limit!",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.;
    }
    
    ic=4; //D d0Pi [cm]  lower limit!",

    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.;
    }
    
    ic=5;//“D sigmavert (cm)",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.04;
    }
    
    
    ic=6; //D dist prim-sec (cm)",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.05;
    }
    
    
    
    ic=7; //D cosThetaPoint",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0;
      //if(ipt>3){
	//       for(Int_t ipt=4;ipt<nptbins;ipt++){
      //	anacutsval[ic][ipt]=0.05;
      // }    
    }   
    
    ic=8; //D dca cut (cm)",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.5;
    }
     
    
    ic=9; //D norm dec len XY ",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.6;
      // if(ipt>3){
    //for(Int_t ipt=4;ipt<nptbins;ipt++){
      //	anacutsval[ic][ipt]=0.5;
      // } 
    }
    
    
  

    ic=10; //“D cosThetaPointXY",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.7;
    }
 

    ic=11;  //“D pT [GeV/c]", 
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=2.;
    }

    ic=12; // ”pi pT [GeV/c]",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=1.;
      // if(ipt>3){
	// for(Int_t ipt=4;ipt<nptbins;ipt++){
      //	anacutsval[ic][ipt]=3.;
      // }
    }
    

    ic=13;  //“prod d0",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0;
    }

    ic=14;  //”B cosThetaPoint",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.85;
    }
    ic=15;  // ”B dec len (cm)",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.;
    } 
    ic=16;   //“B norm dec len XY",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0;
    }
    ic=17;  // “Topomatic D",
     for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=1.5;
    }
     ic=18;   //“Topomatic pi",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=1.5;
    }
    ic=19;   // “cos opening angle D-pi",
    for(Int_t ipt=0;ipt<nptbins;ipt++){
      anacutsval[ic][ipt]=0.;
    }
   
          
    
      AliRDHFCutsBtoDpluspi* analysiscuts=new AliRDHFCutsBtoDpluspi("AnalysisCuts");
    analysiscuts->SetName("AnalysisCuts");
    analysiscuts->SetTitle("Cuts for Dplus Analysis and CF");
    analysiscuts->SetPtBins(nptbins+1,ptbins);
    analysiscuts->SetCuts(nvars,nptbins,anacutsval);
    analysiscuts->AddTrackCuts(esdTrackCuts);
    analysiscuts->SetUsePID(kTRUE);
    //analysiscuts->SetUseImpParProdCorrCut(kFALSE);

    analysiscuts->SetTriggerClass(""); //HMV0
    //	analysiscuts->SetTriggerMask(AliVEvent::kFastOnly);
    if(theMCon) analysiscuts->SetTriggerMask(AliVEvent::kINT7);//kINT7);//kHighMult
    else analysiscuts->SetTriggerMask(AliVEvent::kHighMultV0);//kINT7);//kHighMult//set trigger for data
   
 analysiscuts->SetUseCentrality(kFALSE);
  analysiscuts->SetOptPileup(AliRDHFCuts::kRejectMVPileupEvent); //DEFAULT
 //   analysiscuts->SetOptPileup(AliRDHFCuts::kRejectPileupEvent);
  //  analysiscuts->ConfigurePileupCuts(5,0.8);
    cout<<"This is the odject I'm going to save:"<<nptbins<<endl;
    
    analysiscuts->PrintAll();

    

    /*

  // cuts are stored in a TFile generated by makeTFile4CutsB0toDminuspi.C in ./macros/

  // set there the cuts!!!!!

 Bool_t stdcuts=kFALSE;
 // Bool_t stdcuts=kTRUE;
  
  TFile* filecuts;
  if( cutsfile.EqualTo("") ) {
    stdcuts=kTRUE; 
  } else {
      filecuts=TFile::Open(cutsfile.Data());
      if(!filecuts ||(filecuts&& !filecuts->IsOpen())){
	//AliFatal("Input file not found : check your cut object");
	::Error("here","Input file not found : check your cut object");
      }
  }
  






  

  AliRDHFCutsDplustoKpipi* RDHFB0toDminuspi=new AliRDHFCutsDplustoKpipi();

 RDHFB0toDminuspi = (AliRDHFCutsDplustoKpipi*)filecuts->Get("AnalysisCuts");

*/
 
  //RDHFB0toDminuspi->SetStandardCutsPbPb2010();
  // if(stdcuts) {
   /*
    if(system==0){ 
      // RDHFB0toDminuspi->SetStandardCutsPP2010();
      //      RDHFB0toDminuspi->SetUseCentrality(kFALSE);


    }
    else if(system==1) {

      // RDHFB0toDminuspi->SetStandardCutsPbPb2010();
     
      //RDHFB0toDminuspi->SetMinCentrality(minC);
      
      //RDHFB0toDminuspi->SetMinCentrality(maxC);

      // RDHFB0toDminuspi->SetUseAOD049(kTRUE);

       RDHFB0toDminuspi->SetUseCentrality(kTRUE);

    }

    //  }

    //  else RDHFB0toDminuspi = (AliRDHFCutsDplustoKpipi*)filecuts->Get("AnalysisCuts");

  //  RDHFB0toDminuspi->SetName("AnalysisCuts");

  //RDHFB0toDminuspi->SetUsePhysicsSelection(kTRUE);
  //if(theMCon==kFALSE){

    //RDHFB0toDminuspi->SetOptPileup(AliRDHFCuts::kRejectPileupEvent);
    RDHFB0toDminuspi->SetTriggerMask(AliVEvent::kINT7);
    RDHFB0toDminuspi->SetTriggerClass("");
    //}
//

*/
  //  RDHFB0toDminuspi->SetUseCentrality(kFALSE);


  // mm let's see if everything is ok

  if(!analysiscuts){

    cout<<"Specific AliRDHFCuts not found"<<endl;

    return 0;

  }

  

  //CREATE THE TASK

  printf("CREATE TASK\n");

  // create the task

  AliAnalysisTaskSEB0toDminuspi_wBcuts *task = new AliAnalysisTaskSEB0toDminuspi_wBcuts("AliAnalysisTaskSEB0toDminuspi_wBCuts",analysiscuts);

 
  task->SetReadMC(theMCon);
  task->SetDebugLevel(2);
 
  // task->SetDplusMassLimit(1.845,1.895);
  //  task->SetBMassLimits(4.5,6.5);


  //SetCut on inverted topomatic cut for D mesons  
  //task->SetTopomaticCutOnDDaughters(TopomaticDdaughter);
  /*
  //Set B 2prong cuts
  task->SetDpTCut(pTD); 
  task->SetpipTCut(pTpi); 
  task->Setprodd0Cut(prodd0); 
  task->SetCosBCut(cosB); 
  task->SetCosXYBCut(cosBXY); 
  task->SetdlBCut(dlB); 
  task->SetNdlXYBCut(NdlBXY); 
  task->SetTopomaticD(TopomaticD); 
  task->SetTopomaticpi(Topomaticpi); 
  task->SetcosoaDpi(cosoaDpi);
  */
  mgr->AddTask(task);



  // Create and connect containers for input/output

  usercomment = "_" + usercomment;  

  TString outputfile = AliAnalysisManager::GetCommonFileName();

  outputfile += ":PWG3_D2H_B0piDplus";
  outputfile += usercomment;
  

  // ------ input data ------
  TString input = "cinputB0";
  //input += usercomment;
  TString output1 = "Integrated";
  output1 += usercomment;
  TString output2 = "pTbin";
  output2 += usercomment;
  TString output3 = "coutputB0Norm";
  output3 += usercomment;
  TString output4 = "cuts";
  output4 += usercomment;
  //AliAnalysisDataContainer *cinput0  = mgr->GetCommonInputContainer();

  AliAnalysisDataContainer *cinput0  =  mgr->CreateContainer(input,TChain::Class(), 

							     AliAnalysisManager::kInputContainer);

 // ----- output data -----

  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer(output1,TList::Class(),AliAnalysisManager::kOutputContainer,outputfile.Data());

  AliAnalysisDataContainer *coutputBin = mgr->CreateContainer(output2,TList::Class(),AliAnalysisManager::kOutputContainer,outputfile.Data());

  AliAnalysisDataContainer *coutputB0Norm = mgr->CreateContainer(output3,AliNormalizationCounter::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data());

  AliAnalysisDataContainer *coutputCuts = mgr->CreateContainer(output4,AliRDHFCutsBtoDpluspi::Class(),AliAnalysisManager::kOutputContainer, outputfile.Data()); //cuts



  mgr->ConnectInput(task,0,mgr->GetCommonInputContainer());

  mgr->ConnectOutput(task,1,coutput1);

  mgr->ConnectOutput(task,2,coutputBin);

  mgr->ConnectOutput(task,3,coutputB0Norm);

  mgr->ConnectOutput(task,4,coutputCuts);
  

  return task;

}



