/**************************************************************************
 * Copyright(c) 1998-2010, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */

/////////////////////////////////////////////////////////////
//
// Class for cuts on AOD reconstructed B->D+pi
//
// 
/////////////////////////////////////////////////////////////

#include <TDatabasePDG.h>
#include <Riostream.h>

#include "AliRDHFCutsBtoDpluspi.h"
#include "AliAODRecoDecayHF3Prong.h"
#include "AliAODRecoDecayHF2Prong.h"
#include "AliAODTrack.h"
#include "AliAODPidHF.h"
#include "AliESDtrack.h"
#include "AliAODVertex.h"


using std::cout;
using std::endl;

/// \cond CLASSIMP
ClassImp(AliRDHFCutsBtoDpluspi);
/// \endcond


//--------------------------------------------------------------------------
AliRDHFCutsBtoDpluspi::AliRDHFCutsBtoDpluspi(const char* name) : 
  AliRDHFCuts(name),
  fUseStrongPid(0),
  fMaxPtStrongPid(0.),
  fMaxPStrongPidK(0.),
  fMaxPStrongPidpi(0.),
  //fUseImpParProdCorrCut(kFALSE),
  //fUsed0MeasMinusExpCut(kFALSE),
  //fMaxd0MeasMinusExp(0x0),
  // fUsed0Cut(kFALSE),
  //fMaxd0(0x0),
  fScaleNormDLxyBypOverPt(kTRUE)
{
  //
  // Default Constructor
  //
  Int_t nvars=20;
  SetNVars(nvars);
  TString varNames[20]={"D inv. mass [GeV]",
			"D pTK [GeV/c]",
			"D pTPi [GeV/c]",
			"D d0K [cm]   lower limit!",
			"D d0Pi [cm]  lower limit!",
			"D sigmavert (cm)",
			"D dist prim-sec (cm)",
			"D cosThetaPoint",
			"D dca cut (cm)",
			"D norm dec len XY ",
			"D cosThetaPointXY",
			"D pT [GeV/c]", 
			"pi pT [GeV/c]",
			"prod d0",
			"B cosThetaPoint",
			"B dec len (cm)",
			"B norm dec len XY",
			"Topomatic D",
			"Topomatic pi",
			"cos opening angle D-pi",



  };
  Bool_t isUpperCut[20]={kTRUE,
			 kFALSE,
			 kFALSE,
			 kFALSE,
			 kFALSE,
			 kTRUE,
			 kFALSE,
			 kFALSE,
			 kFALSE,
			 kFALSE,
			 kFALSE,
			 kFALSE,
			 kFALSE,
			 kTRUE,
			 kFALSE,
			 kFALSE,
			 kFALSE,
			 kTRUE,
			 kTRUE,
			 kFALSE
  };
  SetVarNames(nvars,varNames,isUpperCut);
  /* Bool_t forOpt[20]={kFALSE,
		     kFALSE,
		     kFALSE,
		     kFALSE,
		     kFALSE,
		     kFALSE,
		     kTRUE,
		     kTRUE,
		     kTRUE,
		     kTRUE,
		     kTRUE,
		     kFALSE,
		     kTRUE,
		     kTRUE};
		     SetVarsForOpt(7,forOpt);*/
  Float_t limits[2]={0,999999999.};
  SetPtBins(2,limits);
  if(fPidHF)delete fPidHF;
  fPidHF=new AliAODPidHF();
  Double_t plim[2]={0.6,0.8};
  Double_t nsigma[5]={2.,1.,2.,3.,0.};
  
  fPidHF->SetPLimit(plim,2);
  fPidHF->SetAsym(kTRUE);
  fPidHF->SetSigma(nsigma);
  fPidHF->SetMatch(1);
  fPidHF->SetTPC(1);
  fPidHF->SetTOF(1);
  fPidHF->SetITS(0);
  fPidHF->SetTRD(0);
  fPidHF->SetCompat(kTRUE);

  
}








//--------------------------------------------------------------------------
AliRDHFCutsBtoDpluspi::AliRDHFCutsBtoDpluspi(const AliRDHFCutsBtoDpluspi &source) :
  AliRDHFCuts(source),
  fUseStrongPid(source.fUseStrongPid),
  fMaxPtStrongPid(source.fMaxPtStrongPid),
  fMaxPStrongPidK(source.fMaxPStrongPidK),
  fMaxPStrongPidpi(source.fMaxPStrongPidpi),
  //fUseImpParProdCorrCut(source.fUseImpParProdCorrCut),
  //fUsed0MeasMinusExpCut(source.fUsed0MeasMinusExpCut),
  //fMaxd0MeasMinusExp(0x0),
  //fUsed0Cut(source.fUsed0Cut),
  //fMaxd0(0x0),
  fScaleNormDLxyBypOverPt(source.fScaleNormDLxyBypOverPt)
{
  //
  // Copy constructor
  //
  //if(source.fMaxd0MeasMinusExp) Setd0MeasMinusExpCut(source.fnPtBins,source.fMaxd0MeasMinusExp);
  //if(source.fMaxd0) Setd0Cut(source.fnPtBins,source.fMaxd0);
}
//--------------------------------------------------------------------------
AliRDHFCutsBtoDpluspi &AliRDHFCutsBtoDpluspi::operator=(const AliRDHFCutsBtoDpluspi &source)
{
  //
  // assignment operator
  //
  if(&source == this) return *this;

  AliRDHFCuts::operator=(source);

  fUseStrongPid=source.fUseStrongPid;
  fMaxPtStrongPid=source.fMaxPtStrongPid;
  fMaxPStrongPidK=source.fMaxPStrongPidK;
  fMaxPStrongPidpi=source.fMaxPStrongPidpi;
  //fUseImpParProdCorrCut=source.fUseImpParProdCorrCut;
  //fUsed0MeasMinusExpCut=source.fUsed0MeasMinusExpCut;
  //fUsed0Cut=source.fUsed0Cut;
  //if(source.fMaxd0MeasMinusExp) Setd0MeasMinusExpCut(source.fnPtBins,source.fMaxd0MeasMinusExp);
  //if(source.fMaxd0) Setd0Cut(source.fnPtBins,source.fMaxd0);
  fScaleNormDLxyBypOverPt=source.fScaleNormDLxyBypOverPt;

  return *this;
}
/*
//---------------------------------------------------------------------------
void AliRDHFCutsBtoDpluspi::Setd0MeasMinusExpCut(Int_t nPtBins, Float_t *cutval) {
  //
  // store the cuts
  //
  if(nPtBins!=fnPtBins) {
    printf("Wrong number of pt bins: it has to be %d\n",fnPtBins);
    AliFatal("exiting");
  } 
  if(!fMaxd0MeasMinusExp)  fMaxd0MeasMinusExp = new Float_t[fnPtBins];
  for(Int_t ib=0; ib<fnPtBins; ib++) fMaxd0MeasMinusExp[ib] = cutval[ib];
  fUsed0MeasMinusExpCut=kTRUE;
  return;
}
//---------------------------------------------------------------------------
void AliRDHFCutsBtoDpluspi::Setd0MeasMinusExpDDaughterCut(Int_t nPtBins, Float_t *cutval) {
  //
  // store the cuts
  //
  if(nPtBins!=fnPtBins) {
    printf("Wrong number of pt bins: it has to be %d\n",fnPtBins);
    AliFatal("exiting");
  } 
  if(!fMind0MeasMinusExpDDaughter)  fMind0MeasMinusExpDDaughter = new Float_t[fnPtBins];
  for(Int_t ib=0; ib<fnPtBins; ib++) fMind0MeasMinusExpDDaughter[ib] = cutval[ib];
  fMind0MeasMinusExpDDaughterCut=kTRUE;
  return;
}

//---------------------------------------------------------------------------
void AliRDHFCutsBtoDpluspi::Setd0Cut(Int_t nPtBins, Float_t *cutval) {
  //
  // store the cuts
  //
  if(nPtBins!=fnPtBins) {
    printf("Wrong number of pt bins: it has to be %d\n",fnPtBins);
    AliFatal("exiting");
  }
  if(!fMaxd0)  fMaxd0 = new Float_t[fnPtBins];
  for(Int_t ib=0; ib<fnPtBins; ib++) fMaxd0[ib] = cutval[ib];
  fUsed0Cut=kTRUE;
  return;
}
*/

//---------------------------------------------------------------------------
//void AliRDHFCutsBtoDpluspi::GetCutVarsForOpt(AliAODRecoDecayHF *d1,AliAODRecoDecayHF *d2,Float_t *vars,Int_t nvars,Int_t *pdgdaughtersB,Int_t *pdgdaughtersD,AliAODEvent *aod) {}ELIMINATO

//---------------------------------------------------------------------------
Bool_t AliRDHFCutsBtoDpluspi::IsInFiducialAcceptance(Double_t pt, Double_t y) const
{
  //
  // Checking if B is in fiducial acceptance region 
  //

  if(fMaxRapidityCand>-998.){
    if(TMath::Abs(y) > fMaxRapidityCand) return kFALSE;
    else return kTRUE;
  }

  if(pt > 5.) {
    // applying cut for pt > 5 GeV as D (to be modified???)
    AliDebug(2,Form("pt of B = %f (> 5), cutting at |y| < 0.8",pt)); 
    if (TMath::Abs(y) > 0.8) return kFALSE;
    
  } else {
    // appliying smooth cut for pt < 5 GeV
    Double_t maxFiducialY = -0.2/15*pt*pt+1.9/15*pt+0.5; 
    Double_t minFiducialY = 0.2/15*pt*pt-1.9/15*pt-0.5;		
    AliDebug(2,Form("pt of B = %f (< 5), cutting  according to the fiducial zone [%f, %f]\n",pt,minFiducialY,maxFiducialY)); 
    if (y < minFiducialY || y > maxFiducialY) return kFALSE;    
  }

  return kTRUE;
}

//---------------------------------------------------------------------------
 Int_t AliRDHFCutsBtoDpluspi::GetPIDBitMask(AliAODRecoDecayHF *rdB,AliAODRecoDecayHF *rdD)
{
  if(!fUsePID || !rdB) return -1;
  //if(fUsePID)printf("i am inside the pid \n");
  Int_t mask=0;
  Int_t sign=rdD->GetCharge(); 
  for(Int_t daught=0;daught<3;daught++){
    AliAODTrack *track=(AliAODTrack*)rdD->GetDaughter(daught);

    if(sign==track->Charge()){//pions
      Int_t isPion=fPidHF->MakeRawPid(track,AliPID::kPion);
      if(isPion==0)mask+=1;
      else if(isPion>0)mask+=3;
      mask=mask<<2;
    }
    else{//kaons
      Int_t isKaon=fPidHF->MakeRawPid(track,AliPID::kKaon);
      if(isKaon==0)mask+=1;
      else if(isKaon>0)mask+=3;
      mask=mask<<2;
    }
  }

  AliAODTrack *trackpi=(AliAODTrack*)rdB->GetDaughter(1);
  if(sign!= trackpi->Charge()){
    Int_t isPion=fPidHF->MakeRawPid(trackpi,AliPID::kPion);
    if(isPion==0)mask+=1;
    else if(isPion>0)mask+=3;
    mask=mask<<2;
  }

      
  mask=mask>>2;
  return mask;   
}
//---------------------------------------------------------------------------
 Int_t AliRDHFCutsBtoDpluspi::IsSelectedPIDD(AliAODRecoDecayHF *rd)
{
  //
  // PID selection, returns 3 if accepted, 0 if not accepted
  // 
  if(!fUsePID || !rd) return 3;
  //if(fUsePID)printf("i am inside the pid \n");
  Int_t nkaons=0;
  Int_t nNotKaons=0;
  Int_t sign= rd->GetCharge(); 
  for(Int_t daught=0;daught<3;daught++){
    AliAODTrack *track=(AliAODTrack*)rd->GetDaughter(daught);
    Int_t isPion=fPidHF->MakeRawPid(track,AliPID::kPion);
    Int_t isKaon=fPidHF->MakeRawPid(track,AliPID::kKaon);
    Int_t isProton=fPidHF->MakeRawPid(track,AliPID::kProton);
    
    if(isProton>0 &&  isKaon<0  && isPion<0) return 0;
    if(isKaon>0 && isPion<0) nkaons++;
    if(isKaon<0) nNotKaons++;  
    if(sign==track->Charge()){//pions
      if(isPion<0)return 0;
      if(rd->Pt()<fMaxPtStrongPid && isPion<=0 && fUseStrongPid&2 && track->P()<fMaxPStrongPidpi)return 0;
    }
    else{//kaons
      if(isKaon<0)return 0;
	if(rd->Pt()<fMaxPtStrongPid && isKaon<=0 && fUseStrongPid&1&& track->P()<fMaxPStrongPidK)return 0;
    }
  }

    
  if(nkaons>1)return 0;
  if(nNotKaons==3)return 0;
  
  return 3;   
  }

//---------------------------------------------------------------------------
Int_t AliRDHFCutsBtoDpluspi::IsSelectedPIDBpi(AliAODTrack *trackpi)
{
  //
  // PID selection, returns 3 if accepted, 0 if not accepted
  // 
  if(!fUsePID || !trackpi) return 3;
  //if(fUsePID)printf("i am inside the pid \n");

    Int_t isPion=fPidHF->MakeRawPid(trackpi,AliPID::kPion);
    if(isPion<0) return 0;
    
 
  return 3;   
  }


//---------------------------------------------------------------------------
void AliRDHFCutsBtoDpluspi::DefineFirstDCuts(Float_t &maxInvMass, Float_t &minptK, Float_t &minptpi,Float_t &mind0K,Float_t &mind0pi,Float_t &maxDsigmavert,Float_t &midDist,Float_t &minCos,Float_t &minDCA,Float_t &minNDl,Float_t &minCosXY, Float_t &minDpt  )
const {
   Float_t cut0[fnPtBins];
   Float_t cut1[fnPtBins];
   Float_t cut2[fnPtBins];
   Float_t cut3[fnPtBins];
   Float_t cut4[fnPtBins];
   Float_t cut5[fnPtBins];
   Float_t cut6[fnPtBins];
   Float_t cut7[fnPtBins];
   Float_t cut8[fnPtBins];
   Float_t cut9[fnPtBins];
   Float_t cut10[fnPtBins];
   Float_t cut11[fnPtBins];

    
   for (int i=0;i<fnPtBins;i++){

     cut0[i]=fCutsRD[GetGlobalIndex(0,i)];
     cut1[i]=fCutsRD[GetGlobalIndex(1,i)];
     cut2[i]=fCutsRD[GetGlobalIndex(2,i)];
     cut3[i]=fCutsRD[GetGlobalIndex(3,i)];
     cut4[i]=fCutsRD[GetGlobalIndex(4,i)];
     cut5[i]=fCutsRD[GetGlobalIndex(5,i)];
     cut6[i]=fCutsRD[GetGlobalIndex(6,i)];
     cut7[i]=fCutsRD[GetGlobalIndex(7,i)];
     cut8[i]=fCutsRD[GetGlobalIndex(8,i)];
     cut9[i]=fCutsRD[GetGlobalIndex(9,i)];
     cut10[i]=fCutsRD[GetGlobalIndex(10,i)];
     cut11[i]=fCutsRD[GetGlobalIndex(11,i)];

    }

   maxInvMass= *std::max_element(cut0, cut0+fnPtBins );
   minptK= *std::min_element(cut1, cut1+fnPtBins ); 
   minptK=*std::min_element(cut2, cut2+fnPtBins );
   mind0K = *std::min_element(cut3, cut3+fnPtBins );
   mind0pi= *std::min_element(cut4, cut4+fnPtBins );
   maxDsigmavert=*std::max_element(cut5, cut1+fnPtBins );
   midDist= *std::min_element(cut6, cut6+fnPtBins );
   minCos=*std::min_element(cut7, cut7+fnPtBins );
   minDCA=*std::min_element(cut8, cut8+fnPtBins );
   minNDl=*std::min_element(cut9, cut9+fnPtBins );
   minCosXY=*std::min_element(cut10, cut10+fnPtBins );
   minDpt=*std::min_element(cut11, cut11+fnPtBins );  

 
 }

 //---------------------------------------------------------------------------


void AliRDHFCutsBtoDpluspi::DefineFirstPiCuts(Float_t &minBpipt)const {

     Float_t cut12[fnPtBins];

    
   for (int i=0;i<fnPtBins;i++){

     cut12[i]=fCutsRD[GetGlobalIndex(12,i)];
   }

   
   minBpipt= *std::min_element(cut12, cut12+fnPtBins );
  


 }


//---------------------------------------------------------------------------
Int_t AliRDHFCutsBtoDpluspi::IsSelectedBpi(TObject* objpi,Int_t selectionLevel, AliAODEvent* aod) {

  fIsSelectedPID=0;
  Int_t  returnvaluePID=0;

  AliAODTrack* pitrack=(AliAODTrack*) objpi; 
  
  Float_t minBPipt;  
    DefineFirstPiCuts( minBPipt  );
    
    if(pitrack->Pt()*pitrack->Pt() < minBPipt*minBPipt) return 0; 

  
  if(selectionLevel==AliRDHFCuts::kAll || 
     selectionLevel==AliRDHFCuts::kCandidate ||     
     selectionLevel==AliRDHFCuts::kPID) {
    returnvaluePID = IsSelectedPIDBpi(pitrack);
    fIsSelectedPID=returnvaluePID;
  }

  if(returnvaluePID==0)return 0;

  
 return 3;

}

//---------------------------------------------------------------------------
Int_t AliRDHFCutsBtoDpluspi::IsSelectedD(TObject* objd,Int_t selectionLevel, AliAODEvent* aod) {
  //
  // Apply selection, returns 3 if accepted, 0 if not accepted
  //


  fIsSelectedCuts=0;
  fIsSelectedPID=0;

  if(!fCutsRD){
    cout<<"Cut matrix not inizialized. Exit..."<<endl;
    return 0;
  }
  //PrintAll();
  AliAODRecoDecayHF3Prong* d=(AliAODRecoDecayHF3Prong*)objd;

  
  if(!d){
    cout<<"AliAODRecoDecayHF3Prong null"<<endl;
    return 0;
  }

  if(fKeepSignalMC) if( IsSignalMC(d,aod,411)) return 3;//changed from IsSignalMC(d,aod,411) 

  // PID selection
  Int_t returnvaluePID=3;
  Int_t returnvalueCuts=3;

  Double_t pt=d->Pt();
  //if(pt<fMinPtCand) return 0;
  // if(pt>fMaxPtCand) return 0;

  if(fUseTrackSelectionWithFilterBits && d->HasBadDaughters()) return 0;//can not be checked for B because D is not a track
  
  // selection on candidate
  if(selectionLevel==AliRDHFCuts::kAll || 
     selectionLevel==AliRDHFCuts::kCandidate) {


    AliAODVertex *origownvtx=0x0;

    //is it usefull?? or should I replace the d with b??  non included for now nell'analisi della D sono rimosse le daughters e il vertice ricalcolato
    //recalculate vertex w/o daughters
    if(fRemoveDaughtersFromPrimary && !fUseMCVertex) {
      if(d->GetOwnPrimaryVtx()) origownvtx=new AliAODVertex(*d->GetOwnPrimaryVtx());
      if(!RecalcOwnPrimaryVtx(d,aod)) {
	CleanOwnPrimaryVtx(d,aod,origownvtx);
	return 0;
      }
    }

    if(fUseMCVertex) {
      if(d->GetOwnPrimaryVtx()) origownvtx=new AliAODVertex(*d->GetOwnPrimaryVtx());
      if(!SetMCPrimaryVtx(d,aod)) {
	CleanOwnPrimaryVtx(d,aod,origownvtx);
	return 0;
      }
    }

    

    Int_t ptbin=PtBin(pt);
    if (ptbin==-1) {
      CleanOwnPrimaryVtx(d,aod,origownvtx);
      return 0;
    }

    Float_t maxInvMass, minptK, minptpi, mind0K, mind0pi, maxDsigmavert, midDist,minCos, minDCA, minNDl,minCosXY, minDpt;  
    DefineFirstDCuts(maxInvMass, minptK, minptpi, mind0K, mind0pi, maxDsigmavert, midDist,minCos, minDCA, minNDl,minCosXY, minDpt  );
    
  

	


    
    Double_t mDplusPDG = TDatabasePDG::Instance()->GetParticle(411)->Mass();
    Double_t mDplus=d->InvMassDplus();
    
    if(TMath::Abs(mDplus-mDplusPDG)>maxInvMass) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}

    //2track cuts
    //   if(d->GetDist12toPrim()<fCutsRD[GetGlobalIndex(5,ptbin)]|| d->GetDist23toPrim()<fCutsRD[GetGlobalIndex(5,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}

 

    //DCA
    //  

    if(d->Pt2Prong(1) < minptK*minptK || TMath::Abs(d->Getd0Prong(1))<mind0K) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}//Kaon

    if(d->Pt2Prong(0) < minptpi*minptpi || TMath::Abs(d->Getd0Prong(0))<mind0pi) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}//Pion1

    if(d->Pt2Prong(2) < minptpi*minptpi || TMath::Abs(d->Getd0Prong(2))<mind0pi) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}//Pion2

       //sec vert
    Double_t sigmavert=d->GetSigmaVert(aod);
    if(sigmavert>maxDsigmavert) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
 
  if(d->DecayLength()*d->DecayLength()<midDist*midDist) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}

    if(d->CosPointingAngle()< minCos) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    
     for(Int_t i=0;i<3;i++) if(d->GetDCA(i)>minDCA) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}



    if(fScaleNormDLxyBypOverPt){
      if(d->NormalizedDecayLengthXY()*d->P()/pt < minNDl){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    }else{
      if(d->NormalizedDecayLengthXY()<minNDl){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    }
    if(d->CosPointingAngleXY()<minCosXY){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}

    if(d->Pt2() < minDpt*minDpt){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0; }
   
    // unset recalculated primary vertex when not needed any more
    CleanOwnPrimaryVtx(d,aod,origownvtx);
  
    fIsSelectedCuts=returnvalueCuts;

    if(!returnvalueCuts) return 0; // returnvalueCuts cannot be 0 here
  }
  
  if(selectionLevel==AliRDHFCuts::kAll || 
     selectionLevel==AliRDHFCuts::kCandidate ||     
     selectionLevel==AliRDHFCuts::kPID) {
    returnvaluePID = IsSelectedPIDD(d);
    fIsSelectedPID=returnvaluePID;
  }
  if(returnvaluePID==0)return 0;

  // selection on daughter tracks 
  if(selectionLevel==AliRDHFCuts::kAll || 
     selectionLevel==AliRDHFCuts::kTracks) {
    if(!AreDaughtersSelected(d,aod)) return 0;
  }
  
 


  return 3;
}

//---------------------------------------------------------------------------
Int_t AliRDHFCutsBtoDpluspi::IsSelectedB(TObject* objb,Int_t selectionLevel, AliAODEvent* aod) {
  //
  // Apply selection, returns 3 if accepted, 0 if not accepted
  //


  fIsSelectedCuts=0;
  fIsSelectedPID=0;

  if(!fCutsRD){
    cout<<"Cut matrix not inizialized. Exit..."<<endl;
    return 0;
  }
  //PrintAll();
   AliAODRecoDecayHF2Prong* b=(AliAODRecoDecayHF2Prong*)objb; 
   AliAODRecoDecayHF3Prong* d=(AliAODRecoDecayHF3Prong*)b->GetSecondaryVtx()->GetDaughter(0);	
 // AliAODTrack* aodTrackPi = (AliAODTrack*)Bpart->GetSecondaryVtx()->GetDaughter(1);
   
  if(!d ||!b ){
    cout<<"AliAODRecoDecayHF3Prong null"<<endl;
    return 0;
  }

  if(fKeepSignalMC) if( IsSignalMC(b,aod,511)) return 3;//changed from IsSignalMC(d,aod,411) 

  // PID selection
  Int_t returnvaluePID=3;
  Int_t returnvalueCuts=3;

  Double_t pt=b->Pt();
  if(pt<fMinPtCand) return 0;
  if(pt>fMaxPtCand) return 0;

  if(fUseTrackSelectionWithFilterBits && d->HasBadDaughters()) return 0;//can not be checked for B because D is not a track
  
  // selection on candidate
  if(selectionLevel==AliRDHFCuts::kAll || 
     selectionLevel==AliRDHFCuts::kCandidate) {


    AliAODVertex *origownvtx=0x0;

    /* is it usefull?? or should I replace the d with b??  non included for now nell'analisi della D sono rimosse le daughters e il vertice ricalcolato
    //recalculate vertex w/o daughters
    AliAODVertex *origownvtx=0x0;
    if(fRemoveDaughtersFromPrimary && !fUseMCVertex) {
      if(d->GetOwnPrimaryVtx()) origownvtx=new AliAODVertex(*d->GetOwnPrimaryVtx());
      if(!RecalcOwnPrimaryVtx(d,aod)) {
	CleanOwnPrimaryVtx(d,aod,origownvtx);
	return 0;
      }
    }

    if(fUseMCVertex) {
      if(d->GetOwnPrimaryVtx()) origownvtx=new AliAODVertex(*d->GetOwnPrimaryVtx());
      if(!SetMCPrimaryVtx(d,aod)) {
	CleanOwnPrimaryVtx(d,aod,origownvtx);
	return 0;
      }
    }

    */ 

    Int_t ptbin=PtBin(pt);
    if (ptbin==-1) {
      CleanOwnPrimaryVtx(d,aod,origownvtx);
      return 0;
    }

    
    
    Double_t mDplusPDG = TDatabasePDG::Instance()->GetParticle(411)->Mass();
    Double_t mB0PDG = TDatabasePDG::Instance()->GetParticle(511)->Mass();
    Double_t mDplus=d->InvMassDplus();
    Double_t mB0=b->InvMass2Prongs(0,1,411,211);
    
    if(TMath::Abs(mDplus-mDplusPDG)>fCutsRD[GetGlobalIndex(0,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}

    //2track cuts
    //   if(d->GetDist12toPrim()<fCutsRD[GetGlobalIndex(5,ptbin)]|| d->GetDist23toPrim()<fCutsRD[GetGlobalIndex(5,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}

 

    //DCA
    //  

    if(d->Pt2Prong(1) < fCutsRD[GetGlobalIndex(1,ptbin)]*fCutsRD[GetGlobalIndex(1,ptbin)] || TMath::Abs(d->Getd0Prong(1))<fCutsRD[GetGlobalIndex(3,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}//Kaon

    if(d->Pt2Prong(0) < fCutsRD[GetGlobalIndex(2,ptbin)]*fCutsRD[GetGlobalIndex(2,ptbin)] || TMath::Abs(d->Getd0Prong(0))<fCutsRD[GetGlobalIndex(4,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}//Pion1

    if(d->Pt2Prong(2) < fCutsRD[GetGlobalIndex(2,ptbin)]*fCutsRD[GetGlobalIndex(2,ptbin)] || TMath::Abs(d->Getd0Prong(2))<fCutsRD[GetGlobalIndex(4,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}//Pion2

       //sec vert
    Double_t sigmavert=d->GetSigmaVert(aod);
    if(sigmavert>fCutsRD[GetGlobalIndex(5,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
 
  if(d->DecayLength()*d->DecayLength()<fCutsRD[GetGlobalIndex(6,ptbin)]*fCutsRD[GetGlobalIndex(6,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}

    if(d->CosPointingAngle()< fCutsRD[GetGlobalIndex(7,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    
     for(Int_t i=0;i<3;i++) if(d->GetDCA(i)>fCutsRD[GetGlobalIndex(8,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}



    if(fScaleNormDLxyBypOverPt){
      if(d->NormalizedDecayLengthXY()*d->P()/pt<fCutsRD[GetGlobalIndex(9,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    }else{
      if(d->NormalizedDecayLengthXY()<fCutsRD[GetGlobalIndex(9,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    }
    if(d->CosPointingAngleXY()<fCutsRD[GetGlobalIndex(10,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}

    if(b->Pt2Prong(0) < fCutsRD[GetGlobalIndex(11,ptbin)]*fCutsRD[GetGlobalIndex(11,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0; }
    if(b->Pt2Prong(1) < fCutsRD[GetGlobalIndex(12,ptbin)]*fCutsRD[GetGlobalIndex(12,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0; }
     
    
    if(b->Getd0Prong(0)*b->Getd0Prong(1) > fCutsRD[GetGlobalIndex(13,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    if(b->CosPointingAngle()<fCutsRD[GetGlobalIndex(14,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    if(b->DecayLength()*b->DecayLength()<fCutsRD[GetGlobalIndex(15,ptbin)]*fCutsRD[GetGlobalIndex(15,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    if(b->NormalizedDecayLengthXY()<fCutsRD[GetGlobalIndex(16,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
  


    //    if(fUseImpParProdCorrCut){
    ///if(d->Getd0Prong(0)*d->Getd0Prong(1)<0. && d->Getd0Prong(2)*d->Getd0Prong(1)<0.) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    //}

    
    
  //   if(d->Pt2Prong(0)<fCutsRD[GetGlobalIndex(8,ptbin)]*fCutsRD[GetGlobalIndex(8,ptbin)] && d->Pt2Prong(1)<fCutsRD[GetGlobalIndex(8,ptbin)]*fCutsRD[GetGlobalIndex(8,ptbin)] && d->Pt2Prong(2)<fCutsRD[GetGlobalIndex(8,ptbin)]*fCutsRD[GetGlobalIndex(8,ptbin)]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}


  Double_t diffIPMCD, diffIPMCpi,errdiffIPMCD,errdiffIPMCpi;
  b->Getd0MeasMinusExpProng(0,aod->GetMagneticField(),diffIPMCD,errdiffIPMCD);
  b->Getd0MeasMinusExpProng(1,aod->GetMagneticField(),diffIPMCD,errdiffIPMCD);
  Double_t normD=  TMath::Abs(diffIPMCD/errdiffIPMCD);
  Double_t normpi=  TMath::Abs(diffIPMCpi/errdiffIPMCpi);
  if(normD>fCutsRD[GetGlobalIndex(17,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
  
  if(normpi>fCutsRD[GetGlobalIndex(18,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}

  Double_t cosoa=(b->PxProng(0)*b->PxProng(1)+b->PyProng(0)*b->PyProng(1)+b->PzProng(0)*b->PzProng(1))/(TMath::Sqrt(b->P2Prong(0))*TMath::Sqrt(b->P2Prong(1)));
  if(cosoa<fCutsRD[GetGlobalIndex(19,ptbin)]){CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
 

  //Quuuuuuuuuuiiii

    // d0meas-exp
  /*  if(fUsed0MeasMinusExpCut){
     Double_t dd0max=0;
      for(Int_t ipr=0; ipr<3; ipr++) {
	Double_t diffIP, errdiffIP;
	d->Getd0MeasMinusExpProng(ipr,aod->GetMagneticField(),diffIP,errdiffIP);
	Double_t normdd0=0.;
	if(errdiffIP>0) normdd0=diffIP/errdiffIP;
	if(ipr==0) dd0max=normdd0;
	else if(TMath::Abs(normdd0)>TMath::Abs(dd0max)) dd0max=normdd0;
      }
      if(TMath::Abs(dd0max)>]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
      }*/

    // d0
    // if(fUsed0Cut){
    // Double_t d0=d->ImpParXY()*10000.;
    //  if(TMath::Abs(d0)>fMaxd0[ptbin]) {CleanOwnPrimaryVtx(d,aod,origownvtx); return 0;}
    //}

    // unset recalculated primary vertex when not needed any more
    CleanOwnPrimaryVtx(d,aod,origownvtx);
  
    fIsSelectedCuts=returnvalueCuts;

    if(!returnvalueCuts) return 0; // returnvalueCuts cannot be 0 here
  }

  /*
  if(selectionLevel==AliRDHFCuts::kAll || 
     selectionLevel==AliRDHFCuts::kCandidate ||     
     selectionLevel==AliRDHFCuts::kPID) {
    returnvaluePID = IsSelectedBPID(b,d);
    fIsSelectedPID=returnvaluePID;
  }
  if(returnvaluePID==0)return 0;

  // selection on daughter tracks 
  if(selectionLevel==AliRDHFCuts::kAll || 
     selectionLevel==AliRDHFCuts::kTracks) {
    if(!AreDaughtersSelected(d,aod)) return 0;
  }
  
  */


  return 3;
}




//---------------------------------------------------------------------------

/*
void AliRDHFCutsBtoDpluspi::SetStandardCutsPP2010() {
  //
  //STANDARD CUTS USED FOR 2010 pp analysis 
  //                                            
  
  SetName("BtoDpluspiCutsStandard");
  SetTitle("Standard Cuts for D+ analysis");
  
  // PILE UP REJECTION
  SetOptPileup(AliRDHFCuts::kRejectPileupEvent);

  // EVENT CUTS
  SetMinVtxContr(1);

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
  
  AddTrackCuts(esdTrackCuts);
  
 
  const Int_t nptbins =15;
  const Int_t nvars=14;
  Float_t ptbins[nptbins+1];
  ptbins[0]=0.;
  ptbins[1]=1;	
  ptbins[2]=2.;
  ptbins[3]=3.;
  ptbins[4]=4.;
  ptbins[5]=5.;
  ptbins[6]=6.;
  ptbins[7]=7.;
  ptbins[8]=8.;
  ptbins[9]=9.;
  ptbins[10]=10.;
  ptbins[11]=12.;
  ptbins[12]=14.;
  ptbins[13]=16.;
  ptbins[14]=24.;
  ptbins[15]=99999.;
      
    
  Float_t** anacutsval;
  anacutsval=new Float_t*[nvars];
  
  for(Int_t ic=0;ic<nvars;ic++){anacutsval[ic]=new Float_t[nptbins];}

  //Double_t cutsDplus[12]={0.2,0.4,0.4,0.,0.,0.01,0.06,0.02,0.,0.85,0.,10000000000.};
  for(Int_t ipt=0;ipt<nptbins;ipt++){
    anacutsval[0][ipt]=0.2;
    anacutsval[3][ipt]=0.;
    anacutsval[4][ipt]=0.;
    anacutsval[5][ipt]=0.01;
    anacutsval[11][ipt]=10000000000.;
    }

  anacutsval[1][0]=0.3;
  anacutsval[1][1]=0.4;
  anacutsval[1][2]=0.4; 
  anacutsval[2][0]=0.3;
  anacutsval[2][1]=0.3;
  anacutsval[2][2]=0.4;  
  for(Int_t ipt=3;ipt<nptbins;ipt++){
    anacutsval[1][ipt]=0.4;
    anacutsval[2][ipt]=0.4;
  }
  
  anacutsval[6][0]=0.022100;
  anacutsval[6][1]=0.022100;
  anacutsval[6][2]=0.034;
  anacutsval[6][3]=0.020667;
  anacutsval[6][4]=0.020667;
  anacutsval[6][5]=0.023333;
    
  
  anacutsval[7][0]=0.08;
  anacutsval[7][1]=0.08;
  anacutsval[7][2]=0.09;  
  anacutsval[7][3]=0.095;
  anacutsval[7][4]=0.095;
   
  anacutsval[8][0]=0.5;
  anacutsval[8][1]=0.5;
  anacutsval[8][2]=1.0;
  anacutsval[8][3]=0.5;
  anacutsval[8][4]=0.5;
     
    
  anacutsval[9][0]=0.97;
  anacutsval[9][1]=0.936;
  anacutsval[9][2]=0.95; 
  anacutsval[9][3]=0.95; 
  anacutsval[9][4]= 0.95;
  anacutsval[9][5]=0.92;
  anacutsval[9][6]=0.92;
  anacutsval[9][7]=0.92;
  anacutsval[9][8]=0.92;
  anacutsval[9][9]=0.90;
 for(Int_t ipt=10;ipt<nptbins;ipt++){
   anacutsval[9][ipt]=0.90; 
 }
  
  
  anacutsval[10][0]=0.0055;
  anacutsval[10][1]=0.0055;
  anacutsval[10][2]= 0.0028;
  anacutsval[10][3]=0.000883;
  anacutsval[10][4]=0.000883;

  
  for(Int_t ipt=5;ipt<nptbins;ipt++){
    anacutsval[6][ipt]=0.02333;
    anacutsval[7][ipt]=0.115;
    anacutsval[8][ipt]=0.5;
    anacutsval[10][ipt]=0.000883;
    }   

  anacutsval[12][0]=8;
  anacutsval[12][1]=8;
  
  anacutsval[13][0]=0.98;
  anacutsval[13][1]=0.98;
  for(Int_t ipt=2;ipt<nptbins;ipt++){
    anacutsval[12][ipt]=0.;
    anacutsval[13][ipt]=0.;
 }
  
  
  
  SetGlobalIndex(nvars,nptbins);
  SetPtBins(nptbins+1,ptbins);
  SetCuts(nvars,nptbins,anacutsval);
  SetUsePID(kTRUE);
  fPidHF->SetOldPid(kTRUE);
  SetRemoveDaughtersFromPrim(kTRUE);
  
  PrintAll();

  for(Int_t iic=0;iic<nvars;iic++){delete [] anacutsval[iic];}
  delete [] anacutsval;
  anacutsval=NULL;

  delete esdTrackCuts;
  esdTrackCuts=NULL;

  return;
}
*/


 
//--------------------------------------------------------------------------

UInt_t AliRDHFCutsBtoDpluspi::GetPIDTrackTPCTOFBitMap(AliAODTrack *track) const{

  UInt_t bitmap=0;

  Double_t sigmaTPCPionHyp=-999.;
  Double_t sigmaTPCKaonHyp=-999.;
  Double_t sigmaTPCProtonHyp=-999.;
  Double_t sigmaTOFPionHyp=-999.;
  Double_t sigmaTOFKaonHyp=-999.;
  Double_t sigmaTOFProtonHyp=-999.;
  
  Int_t oksigmaTPCPionHyp=fPidHF->GetnSigmaTPC(track,2,sigmaTPCPionHyp);
  Int_t oksigmaTPCKaonHyp=fPidHF->GetnSigmaTPC(track,3,sigmaTPCKaonHyp);
  Int_t oksigmaTPCProtonHyp=fPidHF->GetnSigmaTPC(track,4,sigmaTPCProtonHyp);
  Int_t oksigmaTOFPionHyp=fPidHF->GetnSigmaTOF(track,2,sigmaTOFPionHyp);
  Int_t oksigmaTOFKaonHyp=fPidHF->GetnSigmaTOF(track,3,sigmaTOFKaonHyp);
  Int_t oksigmaTOFProtonHyp=fPidHF->GetnSigmaTOF(track,4,sigmaTOFProtonHyp);

  sigmaTPCPionHyp=TMath::Abs(sigmaTPCPionHyp);
  sigmaTPCKaonHyp=TMath::Abs(sigmaTPCKaonHyp);
  sigmaTPCProtonHyp=TMath::Abs(sigmaTPCProtonHyp);
  sigmaTOFPionHyp=TMath::Abs(sigmaTOFPionHyp);
  sigmaTOFKaonHyp=TMath::Abs(sigmaTOFKaonHyp);
  sigmaTOFProtonHyp=TMath::Abs(sigmaTOFProtonHyp);

  if (oksigmaTPCPionHyp && sigmaTPCPionHyp>0.){
    if (sigmaTPCPionHyp<1.) bitmap+=1<<kTPCPionLess1;
    else{
      if (sigmaTPCPionHyp<2.) bitmap+=1<<kTPCPionMore1Less2;
      else { 
        if (sigmaTPCPionHyp<3.) bitmap+=1<<kTPCPionMore2Less3; 
        else bitmap+=1<<kTPCPionMore3;
      }
    }
  }
  
  if (oksigmaTPCKaonHyp && sigmaTPCKaonHyp>0.){
    if (sigmaTPCKaonHyp<1.) bitmap+=1<<kTPCKaonLess1;
    else{
      if (sigmaTPCKaonHyp<2.) bitmap+=1<<kTPCKaonMore1Less2;
      else { 
        if (sigmaTPCKaonHyp<3.) bitmap+=1<<kTPCKaonMore2Less3; 
        else bitmap+=1<<kTPCKaonMore3;
      }
    }
  }
  
  if (oksigmaTPCProtonHyp && sigmaTPCProtonHyp>0.){
    if (sigmaTPCProtonHyp<1.) bitmap+=1<<kTPCProtonLess1;
    else{
      if (sigmaTPCProtonHyp<2.) bitmap+=1<<kTPCProtonMore1Less2;
      else { 
        if (sigmaTPCProtonHyp<3.) bitmap+=1<<kTPCProtonMore2Less3; 
        else bitmap+=1<<kTPCProtonMore3;
      }
    }
  }
  
  if (oksigmaTOFPionHyp && sigmaTOFPionHyp>0.){
    if (sigmaTOFPionHyp<1.) bitmap+=1<<kTOFPionLess1;
    else{
      if (sigmaTOFPionHyp<2.) bitmap+=1<<kTOFPionMore1Less2;
      else { 
        if (sigmaTOFPionHyp<3.) bitmap+=1<<kTOFPionMore2Less3; 
        else bitmap+=1<<kTOFPionMore3;
      }
    }
  }
  
  if (oksigmaTOFKaonHyp && sigmaTOFKaonHyp>0.){
    if (sigmaTOFKaonHyp<1.) bitmap+=1<<kTOFKaonLess1;
    else{
      if (sigmaTOFKaonHyp<2.) bitmap+=1<<kTOFKaonMore1Less2;
      else { 
        if (sigmaTOFKaonHyp<3.) bitmap+=1<<kTOFKaonMore2Less3; 
        else bitmap+=1<<kTOFKaonMore3;
      }
    }
  }
  
  if (oksigmaTOFProtonHyp && sigmaTOFProtonHyp>0.){
    if (sigmaTOFProtonHyp<1.) bitmap+=1<<kTOFProtonLess1;
    else{
      if (sigmaTOFProtonHyp<2.) bitmap+=1<<kTOFProtonMore1Less2;
      else { 
        if (sigmaTOFProtonHyp<3.) bitmap+=1<<kTOFProtonMore2Less3; 
        else bitmap+=1<<kTOFProtonMore3;
      }
    }
  }
  
  
  
  return bitmap;

}
//---------------------------------------------------------------------------
void AliRDHFCutsBtoDpluspi::PrintAll() const {
  //
  // print all cuts values
  // 
  AliRDHFCuts::PrintAll();
  /*if(fUsed0MeasMinusExpCut){
    printf("Cuts on d0meas-d0exp:\n");
    for(Int_t ib=0;ib<fnPtBins;ib++){
      printf("%f   ",fMaxd0MeasMinusExp[ib]);
    }
    printf("\n");
  }else{
    printf("No cut on d0meas-d0exp:\n");
  }
  if(fUsed0Cut){
    printf("Cuts on d0:\n");
    for(Int_t ib=0;ib<fnPtBins;ib++){
      printf("%f   ",fMaxd0[ib]);
    }
    printf("\n");
  }else{
    printf("No cut on d0\n");
  }
  if(fScaleNormDLxyBypOverPt){
    printf("NormDLxy scaled by p/pt\n");
  }else{
    printf("NormDLxy NOT scaled by p/pt\n");
  }
  if(fUseImpParProdCorrCut){
    printf("d0K*d0pi1 vs. d0K*d0pi2 cut enabled\n");
  }else{
    printf("d0K*d0pi1 vs. d0K*d0pi2 cut disabled\n");
  }

  */
}
