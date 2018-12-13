#ifndef ALIRDHFCUTSBTODPLUSPI_H
#define ALIRDHFCUTSBTODPLUSPI_H
/* Copyright(c) 1998-2010, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */ 


/////////////////////////////////////////////////////////////
///
/// \class AliRDHFCutsBtoDpluspi
/// \brief Class for cuts on AOD reconstructed D+->Kpipi
///
/// \author Author: R. Bala bala@to.infn.it
/// \author        G. Ortona ortona@to.infn.it
/////////////////////////////////////////////////////////////

 
#include "AliRDHFCuts.h"
class AliRDHFCuts;
class AliAODEvent;
class AliAODTrack;
class AliAODRecoDecayHF;

class AliRDHFCutsBtoDpluspi : public AliRDHFCuts 
{
 public:

  enum TrackPIDBit{kTPCPionLess1,kTPCPionMore1Less2,kTPCPionMore2Less3,kTPCPionMore3,
                   kTPCKaonLess1,kTPCKaonMore1Less2,kTPCKaonMore2Less3,kTPCKaonMore3,
                   kTPCProtonLess1,kTPCProtonMore1Less2,kTPCProtonMore2Less3,kTPCProtonMore3,
                   kTOFPionLess1,kTOFPionMore1Less2,kTOFPionMore2Less3,kTOFPionMore3,
                   kTOFKaonLess1,kTOFKaonMore1Less2,kTOFKaonMore2Less3,kTOFKaonMore3,
                   kTOFProtonLess1,kTOFProtonMore1Less2,kTOFProtonMore2Less3,kTOFProtonMore3};
  
  AliRDHFCutsBtoDpluspi(const char* name="CutsBtoDpluspi");
  
  virtual ~AliRDHFCutsBtoDpluspi(){};
  AliRDHFCutsBtoDpluspi(const AliRDHFCutsBtoDpluspi& source);
  AliRDHFCutsBtoDpluspi& operator=(const AliRDHFCutsBtoDpluspi& source); 

   using AliRDHFCuts::GetCutVarsForOpt;
   virtual void GetCutVarsForOpt(AliAODRecoDecayHF *d,Float_t *vars,Int_t nvars,Int_t *pdgdaughters){ return ;};
// virtual void GetCutVarsForOpt(AliAODRecoDecayHF *d,Float_t *vars,Int_t nvars,Int_t *pdgdaughters,AliAODEvent *aod);
 using AliRDHFCuts::IsSelected;
 //virtual Int_t IsSelected(TObject* obj, Int_t selectionLevel, AliAODEvent* aod, Bool_t* bCutArray);
  virtual Int_t IsSelected(TObject* obj, Int_t selectionLevel) {::Error("AliAnalysisTaskB0toDStarPi", "Old selection function"); return 0;}

 
  //using AliRDHFCuts::IsSelected;
  
   Int_t IsSelectedD(TObject* objd,Int_t selectionLevel,AliAODEvent* aod);
   Int_t IsSelectedB(TObject* objb,Int_t selectionLevel,AliAODEvent* aod);
   Int_t IsSelectedBpi(TObject* objpi,Int_t selectionLevel,AliAODEvent* aod);
   Int_t IsSelectedPIDD(AliAODRecoDecayHF *rd);
   Int_t IsSelectedPIDBpi(AliAODTrack *trackpi);

  void DefineFirstDCuts(Float_t &maxInvMass, Float_t &minptK, Float_t &minptpi,Float_t &mind0K,Float_t &mind0pi,Float_t &maxDsigmavert,Float_t &midDist,Float_t &minCos,Float_t &minDCA,Float_t &minNDl,Float_t &minCosXY, Float_t &minDpt  )const;
  void DefineFirstPiCuts(Float_t &minBPipt)const;



  virtual Bool_t IsInFiducialAcceptance(Double_t pt,Double_t y) const;
  //virtual void SetStandardCutsPP2010();

  Int_t GetPIDBitMask(AliAODRecoDecayHF *rdB,AliAODRecoDecayHF *rd);
  UInt_t GetPIDTrackTPCTOFBitMap(AliAODTrack *track) const;
  Float_t GetMassCut(Int_t iPtBin=0)  { return (GetCuts() ? fCutsRD[GetGlobalIndex(0,iPtBin)] : 1.e6);}
  Float_t GetDCACut(Int_t iPtBin=0)  { return (GetCuts() ? fCutsRD[GetGlobalIndex(1,iPtBin)] : 1.e6);}
  void SetUseStrongPid(Int_t spid){fUseStrongPid=spid;}
  void SetMaxPtStrongPid(Float_t spid){fMaxPtStrongPid=spid;}
  void SetMaxPStrongPidK(Float_t spid){fMaxPStrongPidK=spid;}
  void SetMaxPStrongPidpi(Float_t spid){fMaxPStrongPidpi=spid;}
  Int_t GetStrongPid() {return fUseStrongPid;}
  Float_t GetMaxPtStrongPid() {return fMaxPtStrongPid;}
  Float_t GetMaxPtStrongPidK() {return fMaxPStrongPidK;}
  Float_t GetMaxPtStrongPidpi()  {return fMaxPStrongPidpi;}
  /*  void SetUseImpParProdCorrCut(Bool_t use){
    fUseImpParProdCorrCut=use;
  }
  Bool_t GetUseImpParProdCorrCut() const {
    return fUseImpParProdCorrCut;
    }*/
  void SetScaleNormDLxyBypOverPt(Bool_t opt){
    fScaleNormDLxyBypOverPt=opt;
  }

  //  void Setd0MeasMinusExpCut(Int_t nPtBins, Float_t *cutval);
  //void Setd0Cut(Int_t nPtBins, Float_t *cutval);
   void PrintAll()const;

  

 protected:

  //private:
  Int_t fUseStrongPid; /// use strong pid 0 no,1 only for K,2 pi 3 both
  Float_t fMaxPtStrongPid;/// Maximum pt of candidate to apply strong Pid
  Float_t fMaxPStrongPidK;/// Maximum P of track to apply strong Pid on K
  Float_t fMaxPStrongPidpi;/// Maximum P of track to apply strong Pid on pi
  // Bool_t fUseImpParProdCorrCut; /// switch for d0K*d0pi1 vs. d0K*d0pi2 cut
  //Bool_t fUsed0MeasMinusExpCut; /// switch for cut on d0meas-d0exp
  //Float_t* fMaxd0MeasMinusExp;  //[fnPtBins] cut values on d0meas-d0exp
  //Bool_t fUsed0Cut; /// switch for cut on d0
  //Float_t* fMaxd0;  //[fnPtBins] cut values on d0
  Bool_t fScaleNormDLxyBypOverPt; /// switch for normDLxy variable

  /// \cond CLASSIMP    
  ClassDef(AliRDHFCutsBtoDpluspi,10);  /// class for cuts on AOD reconstructed
                                   /// D+->Kpipi
  /// \endcond
};

#endif
