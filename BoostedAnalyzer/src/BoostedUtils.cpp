#include "BoostedTTH/BoostedAnalyzer/interface/BoostedUtils.hpp"

using namespace std;


std::string BoostedUtils::GetAnalyzerPath(){
  char* cpath;
  cpath = getenv("CMSSW_BASE");
  std::string path = cpath;
  
  if (path.size() == 0){
    cerr << "could not find path of exe" << endl;
    return path;
  }
  else
    return path+"/src/BoostedTTH/BoostedAnalyzer";
}


TLorentzVector BoostedUtils::GetTLorentzVector(const math::XYZTLorentzVector& vec){
  
  TLorentzVector result(vec.Px(),vec.Py(),vec.Pz(),vec.E());
  
  return result;
  
}

vector<TLorentzVector> BoostedUtils::GetTLorentzVectors(const std::vector<math::XYZTLorentzVector>& vecs){
  vector<TLorentzVector> results;
  for(auto vec=vecs.begin();vec!=vecs.end();vec++){
    TLorentzVector result(vec->Px(),vec->Py(),vec->Pz(),vec->E());
    results.push_back(result);
  }
  return results;
  
}

math::XYZTLorentzVector BoostedUtils::GetXYZTLorentzVector(const TLorentzVector& vec){
  
  math::XYZTLorentzVector result(vec.Px(),vec.Py(),vec.Pz(),vec.E());
  
  return result;  
}

bool BoostedUtils::FirstIsHarder(math::XYZTLorentzVector vec1,math::XYZTLorentzVector vec2){
  return vec1.Pt()>vec2.Pt();
}

bool BoostedUtils::FirstJetIsHarder(pat::Jet jet1, pat::Jet jet2){
  return jet1.pt()>jet2.pt();
}

bool BoostedUtils::FirstHasHigherCSV(pat::Jet jet1,pat::Jet jet2){
  return jet1.bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags") > jet2.bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags");
}


bool BoostedUtils::FirstHasHigherCSVold(pat::Jet jet1,pat::Jet jet2){
  return jet1.bDiscriminator("combinedSecondaryVertexBJetTags") > jet2.bDiscriminator("combinedSecondaryVertexBJetTags");
}


float BoostedUtils::DeltaEta(const math::XYZTLorentzVector& vec1,const math::XYZTLorentzVector& vec2){
  if(vec1.Pt()<0.001||vec2.Pt()<0.001) return -2;
  
  float deta = fabs(vec1.Eta()-vec2.Eta());
  
  return deta;
}


float BoostedUtils::DeltaEta(const pat::Jet& jet1,const pat::Jet& jet2){
  if(jet1.pt()<0.001||jet2.pt()<0.001) return -2;
  
  math::XYZTLorentzVector vec1 = jet1.p4();
  math::XYZTLorentzVector vec2 = jet2.p4();
  
  float deta = fabs(vec1.Eta()-vec2.Eta());
  
  return deta;
}


float BoostedUtils::DeltaPhi(const math::XYZTLorentzVector& vec1,const math::XYZTLorentzVector& vec2){
  if(vec1.Pt()<0.001||vec2.Pt()<0.001) return -2;
  
  float dphi = ROOT::Math::VectorUtil::DeltaPhi(vec1,vec2);
  
  return fabs(dphi);
}


float BoostedUtils::DeltaPhi(const pat::Jet& jet1,const pat::Jet& jet2){
  if(jet1.pt()<0.001||jet2.pt()<0.001) return -2;
  
  math::XYZTLorentzVector vec1 = jet1.p4();
  math::XYZTLorentzVector vec2 = jet2.p4();
  
  float dphi = ROOT::Math::VectorUtil::DeltaPhi(vec1,vec2);
  
  return fabs(dphi);
}


float BoostedUtils::DeltaR(const math::XYZTLorentzVector& vec1,const math::XYZTLorentzVector& vec2){
  if(vec1.Pt()<0.001||vec2.Pt()<0.001){
    //throw std::invalid_argument( "BoostedUtils::DeltaR: vector with zero pt" );
    std::cerr << "BoostedUtils::DeltaR: vector with zero pt" << std::endl;
    return -2;
  }
  
  float dr = ROOT::Math::VectorUtil::DeltaR(vec1,vec2);
  
  return dr;
}


float BoostedUtils::DeltaR(const pat::Jet& jet1,const pat::Jet& jet2){
  
  math::XYZTLorentzVector vec1 = jet1.p4();
  math::XYZTLorentzVector vec2 = jet2.p4();
  
  float dr = BoostedUtils::DeltaR(vec1,vec2);
  
  return dr;
}


float BoostedUtils::DeltaKt(const math::XYZTLorentzVector& vec1,const math::XYZTLorentzVector& vec2){
  
  float dr = BoostedUtils::DeltaR(vec1,vec2);
  float ptmin=min(vec1.Pt(),vec2.Pt());
  
  return sqrt(dr*dr*ptmin*ptmin);
}


float BoostedUtils::DeltaKt(const pat::Jet& jet1,const pat::Jet& jet2){
  if(jet1.pt()<0.001||jet2.pt()<0.001) return -2;
  
  math::XYZTLorentzVector vec1 = jet1.p4();
  math::XYZTLorentzVector vec2 = jet2.p4();
  
  return BoostedUtils::DeltaKt(vec1,vec2);
}


float BoostedUtils::CosThetaStar(const math::XYZTLorentzVector& vec1, const math::XYZTLorentzVector& vec2){
  if(vec1.Pt()<0.001||vec2.Pt()<0.001) return -2;
  
  TLorentzVector sumVec = BoostedUtils::GetTLorentzVector(vec1+vec2);
  TVector3 cmboost = -sumVec.BoostVector();
  
  TLorentzVector boostedvec1 = GetTLorentzVector(vec1);
  boostedvec1.Boost(cmboost);
  
  return cos( sumVec.Angle(boostedvec1.Vect()) );
}


float BoostedUtils::CosThetaCM(const math::XYZTLorentzVector& vec,const math::XYZTLorentzVector& boostVec){
  if(vec.Pt()<0.001||boostVec.Pt()<0.001) return -2;
  
  TLorentzVector vec_ = GetTLorentzVector(vec);
  TLorentzVector boostVec_ = GetTLorentzVector(boostVec);
  
  TVector3 zBoost = TVector3(0,0,-boostVec_.BoostVector().Pz());
  TLorentzVector boostedVec = vec_;
  boostedVec.Boost(zBoost);
  float theta = boostedVec.Theta();
  return TMath::Cos(theta);
}


std::vector<math::XYZTLorentzVector> BoostedUtils::GetLepVecs(const std::vector<pat::Electron>& selectedElectrons, const std::vector<pat::Muon> selectedMuons){
  
  std::vector<math::XYZTLorentzVector> leptonVecs;
  
  for(std::vector<pat::Electron>::const_iterator itEle = selectedElectrons.begin(); itEle != selectedElectrons.end(); ++itEle){
    leptonVecs.push_back(itEle->p4());
  }
  for(std::vector<pat::Muon>::const_iterator itMu = selectedMuons.begin(); itMu != selectedMuons.end(); ++itMu){
    leptonVecs.push_back(itMu->p4());
  }
  
  std::sort(leptonVecs.begin(), leptonVecs.end(),BoostedUtils::FirstIsHarder);
  
  return leptonVecs;
  
} 


math::XYZTLorentzVector BoostedUtils::GetPrimLepVec(const std::vector<pat::Electron>& selectedElectrons,const std::vector<pat::Muon> selectedMuons){
  
  std::vector<math::XYZTLorentzVector> leptonVecs = GetLepVecs(selectedElectrons,selectedMuons);
  
  if(leptonVecs.size()==0){
    std::cerr<< "No PrimLep Found!" << std::endl;
    return math::XYZTLorentzVector();
  }
  
  return leptonVecs[0];
}


void BoostedUtils::GetNuVecs(const math::XYZTLorentzVector& lepvec, const TVector2& metvec, math::XYZTLorentzVector& nu1, math::XYZTLorentzVector& nu2){
  
  double mu = (80.4*80.4)/2 + metvec.Px()*lepvec.Px() + metvec.Py()*lepvec.Py();
  double a = (mu*lepvec.Pz())/(lepvec.E()*lepvec.E() - lepvec.Pz()*lepvec.Pz());
  double a2 = TMath::Power(a, 2);
  double b = (TMath::Power(lepvec.E(), 2.)*metvec.Mod() - TMath::Power(mu, 2.)) / (TMath::Power(lepvec.E(), 2)- TMath::Power(lepvec.Pz(), 2));
  float pz1,pz2;
  
  if (a2-b < 0) { 
    pz1 = a;
    pz2 = a;
  } else {
    double root = sqrt(a2-b);
    pz1 = a + root;
    pz2 = a - root;
  }
  
  nu1.SetPxPyPzE(metvec.Px(),metvec.Py(),pz1,sqrt(metvec.Mod2()+pz1*pz1));
  nu2.SetPxPyPzE(metvec.Px(),metvec.Py(),pz2,sqrt(metvec.Mod2()+pz2*pz2));
}


vector<math::XYZTLorentzVector> BoostedUtils::GetJetVecs(const std::vector<pat::Jet>& jets){
  std::vector<math::XYZTLorentzVector> jetVecs;
  
  for(std::vector<pat::Jet>::const_iterator itJet=jets.begin();itJet!=jets.end();++itJet){
    jetVecs.push_back(itJet->p4());
  }
  
  return jetVecs;
}


boosted::SubFilterJetCollection BoostedUtils::GetSortedByPt(boosted::SubFilterJetCollection const &subfilterjets){
  boosted::SubFilterJetCollection result = subfilterjets;
  std::sort(result.begin(), result.end(),BoostedUtils::FirstFatJetIsHarder<boosted::SubFilterJet>);
  return result;
}


boosted::HEPTopJetCollection BoostedUtils::GetSortedByPt(boosted::HEPTopJetCollection const &heptopjets){
  boosted::HEPTopJetCollection result = heptopjets;
  std::sort(result.begin(), result.end(),BoostedUtils::FirstFatJetIsHarder<boosted::HEPTopJet>);
  return result;
}


bool BoostedUtils::PassesCSV(const pat::Jet& jet, const char workingPoint){
  
  float CSVLv2wp = 0.423;
  float CSVMv2wp = 0.814;
  float CSVTv2wp = 0.941;
  
  float csvValue = jet.bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags");
  
  switch(workingPoint){
    case 'L': if(csvValue > CSVLv2wp){ return true; } break;
    case 'M': if(csvValue > CSVMv2wp){ return true; } break;
    case 'T': if(csvValue > CSVTv2wp){ return true; } break;
    case '-': return true; break;
  }
  
  return false;
}


float BoostedUtils::GetClosestJetIDs(int& idJet1, int& idJet2, const std::vector<pat::Jet>& jets){
  
  float minDr = 9.;
  for(std::vector<pat::Jet>::const_iterator itJet1 = jets.begin();itJet1 != jets.end();++itJet1){
    for(std::vector<pat::Jet>::const_iterator itJet2 = itJet1+1;itJet2 != jets.end();++itJet2){
      math::XYZTLorentzVector jetVec1 = itJet1->p4();
      math::XYZTLorentzVector jetVec2 = itJet2->p4();
      
      if(BoostedUtils::DeltaR(jetVec1,jetVec2)<minDr){
	      idJet1 = itJet1 - jets.begin();
	      idJet2 = itJet2 - jets.begin();
	      minDr = BoostedUtils::DeltaR(jetVec1,jetVec2);
      }
    }
  }
  
  return minDr;
}


float BoostedUtils::GetClosestLepJetID(int& idJet, const math::XYZTLorentzVector& lepVec, const std::vector<pat::Jet>& jets){
  
  float minDr = 9.;
  for(std::vector<pat::Jet>::const_iterator itJet = jets.begin(); itJet != jets.end(); ++itJet){
    math::XYZTLorentzVector jetVec = itJet->p4();
    if(DeltaR(lepVec,jetVec)<minDr){
	    idJet = itJet - jets.begin();
	    minDr = DeltaR(lepVec,jetVec);
    }
  }

  return minDr;
}


float BoostedUtils::GetJetAverageJetEtaMax(const std::vector<pat::Jet>& jets1, const std::vector<pat::Jet>& jets2){
  int count=0;
  float avgval=0.;
  for(std::vector<pat::Jet>::const_iterator itJet1 = jets1.begin(); itJet1 != jets1.end(); ++itJet1){
    for(std::vector<pat::Jet>::const_iterator itJet2 = itJet1+1; itJet2 != jets1.end(); ++itJet2){
      avgval += itJet1->eta()-itJet2->eta();
      count++;
    }
  }
  avgval /= count;
  
  float imax = 0.;
  float etamax=0.;
  for(std::vector<pat::Jet>::const_iterator itJet = jets2.begin(); itJet != jets2.end(); ++itJet){
    imax = abs(itJet->eta()-avgval);
    if(imax>etamax) {
      etamax = imax;
    }
  }
  return etamax;
}



bool BoostedUtils::GetTopTag(const boosted::HEPTopJet& topJet,const double& fW, const double& mTopMin, const bool& altConf){
  std::vector<pat::Jet> subjets;
  subjets.push_back(topJet.nonW);
  subjets.push_back(topJet.W1);
  subjets.push_back(topJet.W2);
  
  if(subjets.size()!=3) return false;
  
  if(!altConf)  std::sort(subjets.begin(), subjets.end(),FirstJetIsHarder);
  else          TopSubjetCSVDef(subjets);
  
  std::vector<math::XYZTLorentzVector> subjetVecs = GetJetVecs(subjets);
    
  float m12=-999;
  float m23=-999;
  float m13=-999;
  float m123=-999;
  
  m12=(subjetVecs[0]+subjetVecs[1]).M();
  m13=(subjetVecs[0]+subjetVecs[2]).M();
  m23=(subjetVecs[1]+subjetVecs[2]).M();
  m123=(subjetVecs[0]+subjetVecs[1]+subjetVecs[2]).M();

  if(m123<mTopMin) return false;
  
  float mT = 172.3;
  float mW = 80.4;
  
  float Rmin = (1-fW)*mW/mT;
  float Rmax = (1+fW)*mW/mT;
  
  if(!altConf){
    float y1 = 1+pow(m13/m12,2);
    float y2 = 1+pow(m12/m13,2);
    float x = 1-pow(m23/m123,2);  
  
    if(((0.2<atan(m23/m123)) && (atan(m23/m123)<1.3)) && ((Rmin<m23/m123) && (m23/m123<Rmax))) return true;
    if(((x>pow(Rmin,2)*y1) && (x<pow(Rmax,2)*y1)) && (m23/m123>0.35)) return true;
    if(((x>pow(Rmin,2)*y2) && (x<pow(Rmax,2)*y2)) && (m23/m123>0.35)) return true;
  }
  else{
    if(Rmin<m23/m123 && m23/m123<Rmax && 0.2<m12/m13 && 0.2<m13/m12 ) return true;
  }
  
  return false;
}


void BoostedUtils::TopSubjetCSVDef(std::vector<pat::Jet> &subjets){
  std::sort(subjets.begin(), subjets.end(),BoostedUtils::FirstHasHigherCSV);
  
  pat::Jet Bsubjet = subjets[0];
  pat::Jet W1subjet, W2subjet;
  
  if(subjets[1].pt()>subjets[2].pt()){
    W1subjet = subjets[1];
    W2subjet = subjets[2];
  }
  else{
    W1subjet = subjets[2];
    W2subjet = subjets[1];
  }
  
  subjets.clear();
  subjets.push_back(Bsubjet);
  subjets.push_back(W1subjet);
  subjets.push_back(W2subjet);
}


std::vector<pat::Jet> BoostedUtils::GetHiggsFilterJets(const boosted::SubFilterJet& higgsJet, const int& nCSVJets){

  std::vector<pat::Jet> subJets = higgsJet.filterjets;
  
  return GetHiggsFilterJets(subJets,nCSVJets);
}


std::vector<pat::Jet> BoostedUtils::GetHiggsFilterJets(const std::vector<pat::Jet>& higgsDecayJets, const int& nCSVJets){

  std::vector<pat::Jet> subJets = higgsDecayJets;
  
  std::sort(subJets.begin(), subJets.end(),BoostedUtils::FirstHasHigherCSV);
  
  std::vector<pat::Jet> HighCSVSubJets;
  
  for(std::vector<pat::Jet>::iterator itSubJet = subJets.begin(); itSubJet != subJets.end(); ){
    
    bool csvDistance = true;
    for(std::vector<pat::Jet>::iterator itCSVJet = HighCSVSubJets.begin(); itCSVJet != HighCSVSubJets.end(); ++itCSVJet){
      if(BoostedUtils::DeltaR(*itCSVJet,*itSubJet)<.4){
        csvDistance = false;
        break;
      }
    }
    
    if(csvDistance){
      HighCSVSubJets.push_back(*itSubJet);
      subJets.erase(itSubJet);
    }
    else{
      ++itSubJet;
    }
    
    if((int) HighCSVSubJets.size()>=nCSVJets) break;
  }
  
  
  if(subJets.size()==0) return HighCSVSubJets;
  else{
    std::sort(subJets.begin(), subJets.end(),BoostedUtils::FirstJetIsHarder);

    subJets.insert(subJets.begin(),HighCSVSubJets.begin(),HighCSVSubJets.end());
  }
  
  return subJets;
}


float BoostedUtils::GetHiggsMass(const boosted::SubFilterJet& higgsJet, const int& nFilterJets, const int& nBTags, const float& csvWP){
  
  if(nFilterJets<2 || nBTags>nFilterJets) return -1.;
  if(((int)higgsJet.filterjets.size())<nFilterJets || ((int)higgsJet.filterjets.size())<nBTags) return -1.;
  
  std::vector<pat::Jet> filterJets = GetHiggsFilterJets(higgsJet,nBTags);
  
  if(nBTags>0){
    if(filterJets[nBTags-1].bDiscriminator("combinedSecondaryVertexBJetTags")<csvWP) return -1.;
  }
  
  std::vector<math::XYZTLorentzVector> filterJetVecs = GetJetVecs(filterJets);
  math::XYZTLorentzVector sumVec = filterJetVecs[0];
  
  for(std::vector<math::XYZTLorentzVector>::const_iterator itFiltJet = filterJetVecs.begin()+1; itFiltJet-filterJetVecs.begin() < nFilterJets; ++itFiltJet){
    sumVec += *itFiltJet;
  }
  
  return sumVec.M();
}


<<<<<<< HEAD
double BoostedUtils::GetBestHiggsMassOhio(math::XYZTLorentzVector lepton, math::XYZTLorentzVector met, std::vector<math::XYZTLorentzVector> jets, std::vector<double> btag, double &minChi, double &dRbb, math::XYZTLorentzVector &bjet1, math::XYZTLorentzVector &bjet2, std::vector<math::XYZTLorentzVector> loose_jets, std::vector<double> loose_btag){
  if(jets.size()<6 && loose_jets.size()>0){
    jets.push_back( loose_jets[0] );
    btag.push_back( loose_btag[0] );
  }
  int nJets = int(jets.size());
  double chi_top_lep=10000;
  double chi_top_had=10000;
  //double chi_W_lep=10000; //isn't really used
  double chi_W_had=10000;
  minChi = 1000000;
  dRbb = 1000000;
  double btagCut = 0.814;
  double W_mass = 80.0;
  double top_mass = 172.5;
  //double H_mass=120.0;
  // updated 8/22/2012 from J. Timcheck
  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  double sigma_hadW = 12.77;
  double sigma_hadTop = 18.9;
  double sigma_lepTop = 32.91;
  // //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  // double sigma_hadW = 12.59;
  // double sigma_hadTop = 19.9;
  // double sigma_lepTop = 39.05;
  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttJets
  //double sigma_hadW = 12.72,
  //sigma_hadTop = 18.12,
  //sigma_lepTop = 38.72;
  
  double metPz[2];
  double chi=999999;
  //stuff to find:
  double higgs_mass_high_energy=0;
  int nBtags = 0;
  for(int i=0;i<nJets;i++){
    if(btag[i]>btagCut) nBtags++;
  }
  int nUntags = nJets-nBtags;
  double lowest_btag = 99.;
  double second_lowest_btag = 999.;
  int ind_lowest_btag = 999;
  int ind_second_lowest_btag = 999;
  if( nJets>=6 && nBtags>=4 ){
    if( nUntags<2 ){
      for(int i=0;i<nJets;i++){
	      if( btag[i]<lowest_btag ){
	        second_lowest_btag = lowest_btag;
	        ind_second_lowest_btag = ind_lowest_btag;
	        lowest_btag = btag[i];
	        ind_lowest_btag = i;
	      }
	      else if( btag[i]<second_lowest_btag ){
	        second_lowest_btag = btag[i];
	        ind_second_lowest_btag = i;
	      }
      }
    }
  }
  //Handle 6j3t.
  int ind_promoted_btag = 999;
  if( nJets>=6 && nBtags==3 ){
    for(int i=0;i<nJets;i++){
      int rank = 0;
      for(int j=0;j<nJets;j++){
	      if( btag[j] > btag[i] ){
	        rank++;
	      }
      }
      if( rank == 3 ) ind_promoted_btag = i;
    }
  }
  // First get the neutrino z
  double energyLep = lepton.E();
  double a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
  double radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
  radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
  if (radical < 0.0) radical = 0.0;
  metPz[0] = (lepton.Pz()*a/energyLep) + 0.5*sqrt(radical);
  metPz[0] = metPz[0] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  metPz[1] = (lepton.Pz()*a/energyLep) - 0.5*sqrt(radical);
  metPz[1] = metPz[1] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  // Loop over all jets, both Pz, calcaulte chi-square
  math::XYZTLorentzVector metNew;
  for( int ipznu=0; ipznu<2; ipznu++ ){
    metNew.SetXYZT(met.Px(),met.Py(),metPz[ipznu],0.0); //neutrino has mass 0
    
    //with b-tag info
    if( (nJets>=6 && nBtags>=4) || (nJets>=6 && nBtags==3) ){
      std::vector<math::XYZTLorentzVector> not_b_tagged,b_tagged;
      
      //fill not_b_tagged and b_tagged
      for( int i=0;i<nJets;i++ ){
	      if((btag[i]>btagCut && i!=ind_second_lowest_btag && i!=ind_lowest_btag) || (i==ind_promoted_btag)) b_tagged.push_back(jets[i]);
	      else not_b_tagged.push_back(jets[i]);
      }
      
      //first make possible t_lep's with b-tagged jets (includes making W_lep)
      for( int i=0; i<int(b_tagged.size()); i++ ){
	      //math::XYZTLorentzVector W_lep = metNew+lepton; //used for histogram drawing only
	      math::XYZTLorentzVector top_lep = metNew+lepton+b_tagged.at(i);
	      chi_top_lep = pow((top_lep.M()-top_mass)/sigma_lepTop,2);
        
	      //next make possible W_had's with not b-tagged jets
	      for( int j=0; j<int(not_b_tagged.size()); j++ ){
	        for( int k=0; k<int(not_b_tagged.size()); k++ ){
	          if( j!=k ){
	            math::XYZTLorentzVector W_had=not_b_tagged.at(j)+not_b_tagged.at(k);
	            chi_W_had=pow((W_had.M()-W_mass)/sigma_hadW,2);
              
	            //now make possible top_had's (using the W_had + some b-tagged jet)
	            for( int l=0; l<int(b_tagged.size()); l++ ){
		            if( l!=i ){
		              math::XYZTLorentzVector top_had = W_had+b_tagged.at(l);
		              chi_top_had = pow((top_had.M()-top_mass)/sigma_hadTop,2);
		              chi = chi_top_lep+chi_W_had+chi_top_had;

		              //accept the lowest chi
		              if( chi<minChi ){
		                minChi = chi;

		                //pick the other two b's that have the highest et (energy in transverse plane) as higgs mass constituents
		                math::XYZTLorentzVector H2;
		                int numH2Constituents=0;
		                math::XYZTLorentzVector bBest[2];
		                for( int m=0; m<int(b_tagged.size()); m++ ){
		                  if( m!=i && m!=l && numH2Constituents<2 ){
			                  bBest[numH2Constituents] = b_tagged.at(m);
			                  numH2Constituents++;
			                  H2 += b_tagged.at(m);
		                  }
		                }
		                dRbb = DeltaR(bBest[0],bBest[1]);
		                higgs_mass_high_energy = H2.M();
		                bjet1 = bBest[0];
		                bjet2 = bBest[1];
		              }
		            }
	            }
	          }
	        }
	      }
      }
    }
  }
  return higgs_mass_high_energy;
}


float BoostedUtils::GetBestHiggsMassOhio2(math::XYZTLorentzVector lepton, math::XYZTLorentzVector &met, std::vector<math::XYZTLorentzVector> jets, std::vector<double> btag, double &minChi, double &dRbb, math::XYZTLorentzVector &bjet1, math::XYZTLorentzVector &bjet2, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, math::XYZTLorentzVector &toplep, math::XYZTLorentzVector &tophad){
  int nJets = int(jets.size());
  double pfmet_px=met.Px(), pfmet_py=met.Py();
  double chi_top_lep=10000;
  double chi_top_had=10000;
  //double chi_W_lep=10000; //isn't really used
  double chi_W_had=10000;
  minChi = 1000000;
  dRbb = 1000000;
  double btagCut = 0.814;
  double W_mass = 80.0;
  double top_mass = 172.5;
  //double H_mass=120.0;
  // updated 8/22/2012 from J. Timcheck
  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  double sigma_hadW = 12.77;
  double sigma_hadTop = 18.9;
  //double sigma_lepTop = 32.91;
  double sigma_lepTop = 18.9;
  // //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttH
  // double sigma_hadW = 12.59;
  // double sigma_hadTop = 19.9;
  // double sigma_lepTop = 39.05;
  //sigma's from >=6j >=4t, muon, no imaginary neutrino pz ttJets
  //double sigma_hadW = 12.72,
  //sigma_hadTop = 18.12,
  //sigma_lepTop = 38.72;
  
  /// more initializitions
  bjet1.SetPxPyPzE(1.,1.,1.,2.);
  bjet2.SetPxPyPzE(1.,1.,1.,2.);
  // chi2lepW = 0.;
  // chi2leptop = 0.;
  // chi2hadtop = 0.;
  mass_lepW = 0.;
  mass_leptop = 0.;
  mass_hadW = 0.;
  mass_hadtop = 0.;
  toplep.SetPxPyPzE(1.,1.,1.,2.);
  tophad.SetPxPyPzE(1.,1.,1.,2.);
  double metPz[2];
  double chi=999999;
  //stuff to find:
  double higgs_mass_high_energy=0;
  int nBtags = 0;
  for(int i=0;i<nJets;i++){
    if(btag[i]>btagCut) nBtags++;
  }
  int nUntags = nJets-nBtags;
  double lowest_btag = 99.;
  double second_lowest_btag = 999.;
  int ind_lowest_btag = 999;
  int ind_second_lowest_btag = 999;
  std::vector<double> btag_sorted = btag;
  if( nJets>=6 && nBtags>=4 ){
    if( nUntags<2 ){
      for(int i=0;i<nJets;i++){
	      if( btag[i]<lowest_btag ){
	        second_lowest_btag = lowest_btag;
	        ind_second_lowest_btag = ind_lowest_btag;
	        lowest_btag = btag[i];
	        ind_lowest_btag = i;
	      }
	      else if( btag[i]<second_lowest_btag ){
	        second_lowest_btag = btag[i];
	        ind_second_lowest_btag = i;
	      }
      }
    }
      // if( nBtags==3 )
//       {
//       sort(btag_sorted.begin(),btag_sorted.end());
//       double fourth_highest_csv = btag_sorted[nJets-4];
//       for (int f=0; f<nJets; f++)
//       {
//       if (btag[f]==fourth_highest_csv) ind_fourth_highest = f;
//       }
//       }
  }
  //Handle 6j3t.
  int ind_promoted_btag = 999;
  if( nJets>=6 && nBtags==3 ){
    for(int i=0;i<nJets;i++){
      int rank = 0;
      for(int j=0;j<nJets;j++){
	      if( btag[j] > btag[i] ){
	        rank++;
	      }
      }
      if( rank == 3 ) ind_promoted_btag = i;
    }
  }
  // First get the neutrino z
  double energyLep = lepton.E();
  double a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
  double radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
  radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
  bool imaginary = false;
  if(radical < 0.0) imaginary = true;
  if(imaginary){
    radical = -1.0;
    double value = .001;
    while(true){
	    met.SetPxPyPzE(pfmet_px,pfmet_py,0.0,sqrt(pow(pfmet_px,2)+pow(pfmet_py,2))); //neutrino mass 0, pt = sqrt(px^2+py^2)
	    //	energyLep = lepton.E();
	    a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
	    radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
	    radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
	    if(radical>=0) break;
	    pfmet_px -= pfmet_px*value;
	    pfmet_py -= pfmet_py*value;
	  }
  }
  metPz[0] = (lepton.Pz()*a/energyLep) + 0.5*sqrt(radical);
  metPz[0] = metPz[0] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  metPz[1] = (lepton.Pz()*a/energyLep) - 0.5*sqrt(radical);
  metPz[1] = metPz[1] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  
  // Loop over all jets, both Pz, calcaulte chi-square
  math::XYZTLorentzVector metNew;
  for( int ipznu=0; ipznu<2; ipznu++ ){
    metNew.SetPxPyPzE(met.Px(),met.Py(),metPz[ipznu],0.0); //neutrino has mass 0
    metNew.SetE(metNew.P());
    
    //with b-tag info
    if(( nJets>=6 && nBtags>=4 )||( nJets>=6 && nBtags==3 )){
      std::vector<math::XYZTLorentzVector> not_b_tagged,b_tagged;
      
      //fill not_b_tagged and b_tagged
      for( int i=0;i<nJets;i++ ){
	      //if (nBtags>=4)
	      //{
	      if( (btag[i]>btagCut && i!=ind_second_lowest_btag && i!=ind_lowest_btag) || (i==ind_promoted_btag) ) b_tagged.push_back(jets[i]);
	      else not_b_tagged.push_back(jets[i]);
	      //}

        // 	  if (nBtags==3)
        // 	  {
        // 	  if( btag[i]>btagCut || i==ind_fourth_highest) b_tagged.push_back(jets[i]);
        // 	  else not_b_tagged.push_back(jets[i]);
	      //}
      }
      
      //first make possible t_lep's with b-tagged jets (includes making W_lep)
      for( int i=0; i<int(b_tagged.size()); i++ ){
	      math::XYZTLorentzVector top_lep = metNew+lepton+b_tagged.at(i);
	      chi_top_lep = pow((top_lep.M()-top_mass)/sigma_lepTop,2);
	      
        //next make possible W_had's with not b-tagged jets
	      for( int j=0; j<int(not_b_tagged.size()); j++ ){
	        for( int k=0; k<int(not_b_tagged.size()); k++ ){
	          if( j!=k ){
	            math::XYZTLorentzVector W_had = not_b_tagged.at(j)+not_b_tagged.at(k);
	            chi_W_had = pow((W_had.M()-W_mass)/sigma_hadW,2);
	            
              //now make possible top_had's (using the W_had + some b-tagged jet)
	            for( int l=0; l<int(b_tagged.size()); l++ ){
		            if( l!=i ){
		              math::XYZTLorentzVector top_had=W_had+b_tagged.at(l);
		              chi_top_had = pow((top_had.M()-top_mass)/sigma_hadTop,2);
		              chi = chi_top_lep+chi_W_had+chi_top_had;
		              
                  //accept the lowest chi
		              if( chi<minChi ){
		                minChi = chi;
		                //pick the other two b's that have the highest et (energy in transverse plane) as higgs mass constituents
		                math::XYZTLorentzVector H2;
		                int numH2Constituents=0;
		                math::XYZTLorentzVector bBest[2];
		                for( int m=0; m<int(b_tagged.size()); m++ ){
		                  if( m!=i && m!=l && numH2Constituents<2 ){
			                  bBest[numH2Constituents] = b_tagged.at(m);
			                  numH2Constituents++;
			                  H2 += b_tagged.at(m);
		                  }
		                }
		                dRbb = ROOT::Math::VectorUtil::DeltaR(bBest[0],bBest[1]);
		                higgs_mass_high_energy = H2.M();
		                bjet1 = bBest[0];
		                bjet2 = bBest[1];
		                mass_lepW = W_mass;
		                mass_leptop = top_lep.M();
		                mass_hadW = W_had.M();
		                mass_hadtop = top_had.M();
		                toplep = top_lep;
		                tophad = top_had;
		              }
		            }
	            }
	          }
	        }
	      }
      }
    }
  }
  chi2lepW = 0.;
  chi2leptop = chi_top_lep;
  chi2hadtop = chi_top_had;
  chi2hadW = chi_W_had;
  return higgs_mass_high_energy;
  
}


double BoostedUtils::StudyTopsBBSystOhio(math::XYZTLorentzVector &metv, std::vector<double> lep, std::vector< std::vector<double> > jets, std::vector<double> csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb, double &testquant1, double &testquant2, double &testquant3, double &testquant4, double &testquant5, double &testquant6, double &testquant7, math::XYZTLorentzVector &b1, math::XYZTLorentzVector &b2){
  double pi = TMath::Pi();
  math::XYZTLorentzVector lepton(lep[0],lep[1],lep[2],lep[3]);
  std::vector<math::XYZTLorentzVector> jet_TLVs;
  math::XYZTLorentzVector jet;
  int nJets = jets.size();
  for(int i=0;i<nJets;i++){
    jet.SetPxPyPzE(jets[i][0],jets[i][1],jets[i][2],jets[i][3]);
    jet_TLVs.push_back(jet);
  } 
  //double minChi;
  //double dRbb;
  math::XYZTLorentzVector bjet1;
  math::XYZTLorentzVector bjet2;
  math::XYZTLorentzVector leptop;
  math::XYZTLorentzVector hadtop;
  double bhm = GetBestHiggsMassOhio2(lepton, metv, jet_TLVs, csv, minChi, dRbb, bjet1, bjet2, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, leptop, hadtop); // Jon T. version 2
  b1 = bjet1;
  b2 = bjet2;
  math::XYZTLorentzVector bsyst = bjet1+bjet2;
  math::XYZTLorentzVector topsyst = leptop+hadtop;
  //testquant1 = bsyst.Angle(leptop.Vect());
  //testquant2 = bsyst.Angle(hadtop.Vect());
  //testquant1 = bsyst.DeltaPhi(leptop);
  //testquant2 = bsyst.DeltaPhi(hadtop);
  //testquant1 = bsyst.Eta() - leptop.Eta();
  //testquant2 = bsyst.Eta() - hadtop.Eta();
  //testquant1 = bsyst.Eta() - hadtop.Eta();
  //testquant2 = bsyst.DeltaPhi(hadtop);
  //testquant2 = bsyst.Eta();
  //testquant1 = topsyst.Phi();
  //testquant2 = bsyst.Phi();
  //testquant2 = bsyst.Angle(hadtop.Vect());
  //dphi, deta stuff
  testquant1 = bsyst.Eta() - leptop.Eta();
  testquant2 = bsyst.Eta() - hadtop.Eta();
  double dphihad = ROOT::Math::VectorUtil::DeltaPhi(bsyst,hadtop);
  double dphilep = ROOT::Math::VectorUtil::DeltaPhi(bsyst,leptop);
  testquant3 = fabs((dphilep - pi)*(dphilep + pi)) + pow(dphihad,2);
  testquant3 = sqrt(testquant3 / (2.0*pow(pi,2)));
  testquant4 = bsyst.Eta();
  testquant5 = (hadtop.Eta() + leptop.Eta())/2;
  testquant6 = sqrt(abs((bsyst.Eta() - leptop.Eta())*(bsyst.Eta() - hadtop.Eta())));
  testquant7 = ROOT::Math::VectorUtil::Angle(bsyst,topsyst.Vect());
  return bhm;
}


void BoostedUtils::TTHRecoVarsOhio(const std::vector<pat::Jet>& selectedJets,const std::vector<pat::Jet>& selectedJetsLoose, const pat::MET& pfMET, const math::XYZTLorentzVector& lepton, float& higgsMass_, float& dRbb_,float& dEtaHadTopbb_,float& dEtaLeptopbb_, float& dEtaFN_)
{
  math::XYZTLorentzVector met = pfMET.p4();
  std::vector< std::vector<double> > fjets;
  std::vector<math::XYZTLorentzVector> jets = BoostedUtils::GetJetVecs(selectedJets);
  std::vector<double> btag;
  for(std::vector<pat::Jet>::const_iterator itJet = selectedJets.begin(); itJet != selectedJets.end(); ++itJet){
    std::vector<double> ajet;
    ajet.push_back(itJet->px());
    ajet.push_back(itJet->py());
    ajet.push_back(itJet->pz());
    ajet.push_back(itJet->energy());
    fjets.push_back(ajet);
    btag.push_back(itJet->bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags"));
  }
  std::vector<math::XYZTLorentzVector> jetsLoose = GetJetVecs(selectedJetsLoose);
  std::vector<double> btagLoose;
  for(std::vector<pat::Jet>::const_iterator itJetLoose = selectedJetsLoose.begin(); itJetLoose != selectedJetsLoose.end(); ++itJetLoose){
    btagLoose.push_back(itJetLoose->bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags"));
  }  
  std::vector<double> lep;
  lep.push_back(lepton.Px());
  lep.push_back(lepton.Py());
  lep.push_back(lepton.Pz());
  lep.push_back(lepton.E()); 

  double minChi;
  double dRbb;
  math::XYZTLorentzVector bjet1;
  math::XYZTLorentzVector bjet2;

  double higgsmass1 = GetBestHiggsMassOhio(lepton, met, jets, btag, minChi, dRbb, bjet1, bjet2, jetsLoose, btagLoose);

  double minChi2; 
  double chi2lepW;
  double chi2leptop;
  double chi2hadW;
  double chi2hadtop;
  double mass_lepW;
  double mass_leptop;
  double mass_hadW;
  double mass_hadtop;
  double dRbb2;
  double testquant1; 
  double testquant2; 
  double testquant3; 
  double testquant4; 
  double testquant5; 
  double testquant6;
  double testquant7;
  math::XYZTLorentzVector b1; 
  math::XYZTLorentzVector b2;
  
  //double higgsmass2 = BoostedUtils::StudyTopsBBSystOhio(MET, METphi, met, lep, fjets, btag, minChi2, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, dRbb2, testquant1, testquant2, testquant3,testquant4, testquant5, testquant6, testquant7, b1, b2);
  BoostedUtils::StudyTopsBBSystOhio(met, lep, fjets, btag, minChi2, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, dRbb2, testquant1, testquant2, testquant3,testquant4, testquant5, testquant6, testquant7, b1, b2);

  higgsMass_ = higgsmass1;
  dRbb_ = dRbb2;
  dEtaHadTopbb_ = abs(testquant2);
  dEtaLeptopbb_ = abs(testquant1);
  dEtaFN_ = testquant6;
  
}


std::vector<pat::Jet> BoostedUtils::GetCSVJets(const boosted::HEPTopJetCollection& heptopjets, const boosted::SubFilterJetCollection& subfilterjets, const std::vector<pat::Jet>& selectedJets, const float& ptcut, const float& etacut){
  std::vector<pat::Jet> subJets;

  for(size_t j=0; j< subfilterjets.size(); j++){
    for(size_t k=0; k< subfilterjets[j].filterjets.size(); k++){
      if(subfilterjets[j].filterjets[k].pt()>=ptcut && std::fabs(subfilterjets[j].filterjets[k].eta())<etacut) subJets.push_back(subfilterjets[j].filterjets[k]);
    }
  }

  for(size_t j=0; j< heptopjets.size(); j++){
    if(heptopjets[j].nonW.pt()>=ptcut && std::fabs(heptopjets[j].nonW.eta())<etacut) subJets.push_back(heptopjets[j].nonW);
    if(heptopjets[j].W1.pt()>=ptcut && std::fabs(heptopjets[j].W1.eta())<etacut) subJets.push_back(heptopjets[j].W1);
    if(heptopjets[j].W2.pt()>=ptcut && std::fabs(heptopjets[j].W2.eta())<etacut) subJets.push_back(heptopjets[j].W2);
  }

  float DeltaR;
  float drmin;
  std::vector<pat::Jet>::iterator itMJet;

  for(std::vector<pat::Jet>::const_iterator itJet = selectedJets.begin(); itJet != selectedJets.end(); ++itJet){
    drmin = 0.3;
    itMJet = subJets.end();
    for(std::vector<pat::Jet>::iterator itSJet = subJets.begin(); itSJet != subJets.end(); ++itSJet){
      DeltaR = BoostedUtils::DeltaR(itJet->p4(), itSJet->p4());
      if(DeltaR<drmin){
        drmin = DeltaR;
        itMJet = itSJet;
      }
    }
    if(itMJet != subJets.end()) subJets.erase(itMJet);
  }

  return subJets;
}


// vector<TLorentzVector> BEANUtils::GetCA12MatchDiff(const BNsubfilterjetCollection& subfilterJets, const BNjetCollection& selectedJets, vector<float>& DCSV_Matchedfilterjets, vector<float>& Dr_Matchedfilterjets, const float& ptcut, const float& etacut){
//   vector<TLorentzVector> matchjetdiff;
//   BNjet filterjet;
//   BNjet matchedjet;
//   float DeltaR;
//   float drmin;
//   TLorentzVector mjet;
//   for(size_t i=0; i< selectedJets.size(); i++){
//     drmin = 0.3;
//     for(size_t j=0; j< subfilterJets.size(); j++){
//       for(size_t k=0; k< subfilterJets[j].filterjets.size(); k++){
//         filterjet = subfilterJets[j].filterjets[k];
//         DeltaR = BEANUtils::DeltaR(selectedJets[i], filterjet);
//         if(DeltaR<drmin && filterjet.pt>=ptcut && filterjet.eta<etacut){
//           drmin = DeltaR;
//           matchedjet = filterjet;
//         }
//       }
//     }
//     if(drmin < 0.3){
//       mjet.SetPtEtaPhiE(matchedjet.pt/selectedJets[i].pt, selectedJets[i].eta-matchedjet.eta, selectedJets[i].phi-matchedjet.phi, matchedjet.energy/selectedJets[i].energy);
//       matchjetdiff.push_back(mjet);
//       DCSV_Matchedfilterjets.push_back(selectedJets[i].btagCombinedSecVertex-matchedjet.btagCombinedSecVertex);
//       Dr_Matchedfilterjets.push_back(drmin);
//     }
//   }
//
//   return matchjetdiff;
// }


// vector<TLorentzVector> BEANUtils::GetCA15MatchDiff(const BNtoptagjetCollection& toptagjets, const BNjetCollection& selectedJets, vector<float>& DCSV_Matchedfilterjets, vector<float>& Dr_Matchedfilterjets, const float& ptcut, const float& etacut){
//   vector<TLorentzVector> matchjetdiff;
//   BNjet filterjet;
//   BNjet matchedjet;
//   float DeltaR;
//   float drmin;
//   TLorentzVector mjet;
//   for(size_t i=0; i< selectedJets.size(); i++){
//   drmin = 0.3;
//     for(size_t j=0; j< toptagjets.size(); j++){
//       filterjet = toptagjets[j].nonW;
//       DeltaR = BEANUtils::DeltaR(selectedJets[i], filterjet);
//       if(DeltaR<drmin && filterjet.pt>=ptcut && filterjet.eta<etacut){
//         drmin = DeltaR;
//         matchedjet = filterjet;
//       }
//       filterjet = toptagjets[j].W1;
//       DeltaR = BEANUtils::DeltaR(selectedJets[i], filterjet);
//       if(DeltaR<drmin && filterjet.pt>=ptcut && filterjet.eta<etacut){
//         drmin = DeltaR;
//         matchedjet = filterjet;
//       }
//       filterjet = toptagjets[j].W2;
//       DeltaR = BEANUtils::DeltaR(selectedJets[i], filterjet);
//       if(DeltaR<drmin && filterjet.pt>=ptcut && filterjet.eta<etacut){
//         drmin = DeltaR;
//         matchedjet = filterjet;
//       }
//     }
//     if(drmin < 0.3){
//       mjet.SetPtEtaPhiE(matchedjet.pt/selectedJets[i].pt, selectedJets[i].eta-matchedjet.eta, selectedJets[i].phi-matchedjet.phi, matchedjet.energy/selectedJets[i].energy);
//       matchjetdiff.push_back(mjet);
//       DCSV_Matchedfilterjets.push_back(selectedJets[i].btagCombinedSecVertex-matchedjet.btagCombinedSecVertex);
//       Dr_Matchedfilterjets.push_back(drmin);
//     }
//   }
//   return matchjetdiff;
// }




/*










vector<BNjet> BEANUtils::GetJetsByDr(TLorentzVector vec,const std::vector<BNjet>& jets){
  closerToVec closer;
  closer.vec=vec;
  std::vector<BNjet> sorted_jets = jets;
  std::sort(sorted_jets.begin(),sorted_jets.end(),closer);
  return sorted_jets;
}




void BEANUtils::GetHiggsJetCSVVarsDr(const BNsubfilterjet& jet, BNjet& csvec1_fjet_,BNjet& csvec2_fjet_){
  float csvec1 = -.1;
  float csvec2 = -.1;
  
  int fjindex1 = -1;
  int fjindex2 = -1;
  
  for(size_t i=0;i<jet.filterjets.size();i++){
    if(jet.filterjets[i].pt>20&&jet.filterjets[i].btagCombinedSecVertex>csvec1){
      csvec1 = fmax(jet.filterjets[i].btagCombinedSecVertex,-.1);
      fjindex1 = i;
    }
  }
  for(size_t i=0;i<jet.filterjets.size();i++){
    if(jet.filterjets[i].pt>20&&jet.filterjets[i].btagCombinedSecVertex>csvec2&&i!=fjindex1&&DeltaR(jet.filterjets[i],jet.filterjets[fjindex1])>0.4){
      csvec2 = fmax(jet.filterjets[i].btagCombinedSecVertex,-.1);
      fjindex2 = i;
    }
  }
  
  if(fjindex1>=0) csvec1_fjet_ = jet.filterjets[fjindex1];
  if(fjindex2>=0) csvec2_fjet_ = jet.filterjets[fjindex2];
}


void BEANUtils::GetHiggsJetCSVVarsDr(const BNjetCollection& filterjets,BNjet& b1_fjet_,BNjet& b2_fjet_,BNjet& g_fjet_){
  float csvec1 = -.1;
  float csvec2 = -.1;
 
  int fjindex1 = -1;
  int fjindex2 = -1;
  int fjindex3 = -1;
 
  for(size_t i=0;i<filterjets.size();i++){
    if(filterjets[i].pt<20) continue;
    if(filterjets[i].btagCombinedSecVertex>csvec1){
      csvec1 = fmax(filterjets[i].btagCombinedSecVertex,-.1);
      fjindex1 = i;
    }
  }
  for(size_t i=0;i<filterjets.size();i++){
    if(filterjets[i].pt<20) continue;
    if(i==fjindex1) continue;
    if(DeltaR(filterjets[i],filterjets[fjindex1])<0.4) continue;
    if(filterjets[i].btagCombinedSecVertex>csvec2){
      csvec2 = fmax(filterjets[i].btagCombinedSecVertex,-.1);
      fjindex2 = i;
    }
  }
  for(size_t i=0;i<filterjets.size();i++){
    if(filterjets[i].pt<20) continue;
    if(i==fjindex1) continue;
    if(i==fjindex2) continue;
   
    fjindex3 = i;
  }
 
  if(fjindex1>=0) b1_fjet_ = filterjets[fjindex1];
  if(fjindex2>=0) b2_fjet_ = filterjets[fjindex2];
  if(fjindex3>=0) g_fjet_ = filterjets[fjindex3];
}




float BEANUtils::GetHiggsMass(const BNsubfilterjet& higgs_jet, const int n_filterjets, const bool hardest_jets, const int n_btags, const float csv_wp){
  
  TLorentzVector vec;
  
  int n_filterjets_found = 0;
  int n_btags_found = 0;
  
  for(size_t i=0;i<higgs_jet.filterjets.size() && n_filterjets_found<n_filterjets;i++){
  	if(higgs_jet.filterjets[i].btagCombinedSecVertex<csv_wp)
	  if(n_filterjets_found-n_btags_found < n_filterjets-n_btags) continue;
	else
	  n_btags_found ++;
	
    vec = vec +BEANUtils::GetJetVec(higgs_jet.filterjets[i]);
	n_filterjets_found++;
  }
  
  if(hardest_jets && n_filterjets_found!=n_filterjets) return -1;
  return vec.M();
}



BNjet BEANUtils::GetTopLepBjet(const BNsubfilterjet& sfjet,const BNtoptagjet& topjet, const std::vector<BNjet>& ak5jets){
  std::vector<BNjet> sortedJets=ak5jets;
  std::sort(sortedJets.begin(), sortedJets.end(),BEANUtils::FirstHasHigherCSV);  
  bool clean = false;
  BNjet bjet;
  for(size_t i=0; i<sortedJets.size()&&!clean;i++){    
    clean=true;
    if(DeltaR(sortedJets[i],topjet.W1)<0.2) clean=false;
    if(DeltaR(sortedJets[i],topjet.W2)<0.2) clean=false;
    if(DeltaR(sortedJets[i],topjet.nonW)<0.2) clean=false;
    for(size_t j=0; j<sfjet.filterjets.size()&&j<3;j++){    
      if(DeltaR(sortedJets[i],sfjet.filterjets[j])<0.2) clean=false;
    }
    if(clean) bjet=sortedJets[i];
  }
  return bjet;
}
  


bool BEANUtils::GetSelected_HiggsJets(const BNsubfilterjetCollection& subfilterjets, BNsubfilterjetCollection& selected_higgsjets, BNtoptagjet& topHadCand, BNsubfilterjet& higgsCand, bool check_subjet_overlap=true){
  
  bool foundHiggsJet=false;
  for(size_t i=0; i<subfilterjets.size();i++){
    BNjet csvec1_fjet;
    BNjet csvec2_fjet;
    BEANUtils::GetHiggsJetCSVVarsDr(subfilterjets[i],csvec1_fjet,csvec2_fjet);    
    if(csvec2_fjet.btagCombinedSecVertex>0.814&&subfilterjets[i].fatjet.pt>200 and BEANUtils::DeltaR(topHadCand.fatjet,subfilterjets[i].fatjet)>1.5 and fabs(subfilterjets[i].fatjet.eta)<=2.0) {
      //std::cout<<"test"<<std::endl;
           
      if(check_subjet_overlap){
        bool clean = true;
        for(size_t j=0; j<subfilterjets[i].filterjets.size();j++){
          if(BEANUtils::DeltaR(topHadCand.W1,subfilterjets[i].filterjets[j])<0.2) clean=false;
          if(BEANUtils::DeltaR(topHadCand.W2,subfilterjets[i].filterjets[j])<0.2) clean=false;
          if(BEANUtils::DeltaR(topHadCand.nonW,subfilterjets[i].filterjets[j])<0.2) clean=false;
          }
         if(!clean) continue;
        }//0.814 or 0.244
        
      selected_higgsjets.push_back(subfilterjets[i]);
      foundHiggsJet=true;
    }
  }
  
  if(foundHiggsJet){
    size_t maxHiggsTag=-1.0;
    for(size_t i=0; i<selected_higgsjets.size();i++){
      BNjet csvec1_fjet;
      BNjet csvec2_fjet;
      BEANUtils::GetHiggsJetCSVVarsDr(selected_higgsjets[i],csvec1_fjet,csvec2_fjet);
      if(csvec2_fjet.btagCombinedSecVertex>maxHiggsTag){
        maxHiggsTag=csvec2_fjet.btagCombinedSecVertex;
        higgsCand=selected_higgsjets[i];
      }
      
    }
  }
  
  return foundHiggsJet;
}

bool BEANUtils::MCContainsTTbar(const BNmcparticleCollection& mcparticlesStatus3){
  bool foundT=false;
  bool foundTbar=false;
  for(size_t i=0; i<mcparticlesStatus3.size();i++){
    if(mcparticlesStatus3[i].id==6) foundT=true;
    if(mcparticlesStatus3[i].id==-6) foundTbar=true;
  }
  return foundT&&foundTbar;
}
bool BEANUtils::MCContainsHiggs(const BNmcparticleCollection& mcparticlesStatus3){
  for(size_t i=0; i<mcparticlesStatus3.size();i++){
    if(mcparticlesStatus3[i].id==25) return true;
  }
  return false;
}





// parses input file names
vector<string> BEANUtils:: ParseFileNames(string fnames){
    std::vector<string> filenames;
    while(fnames.size() > 0 && *fnames.rbegin()==' '){
        fnames.erase(fnames.begin() + (fnames.size() - 1));
    }
    
    while(true){
        //erase spaces at the beginning:
        while(fnames.size() > 0 && fnames[0]==' ') fnames.erase(0, 1);
        if(fnames.size()==0) break;
        size_t spacepos = fnames.find(' ');
        if(spacepos==string::npos){
            //take the whole rest and go out of here:
            filenames.push_back(fnames);
            break;
        }
        else{
            //take all up to (excluding) the space:
            filenames.push_back(fnames.substr(0, spacepos));
            fnames.erase(0, spacepos);
        }
    }
    return filenames;
}
*/
=======
>>>>>>> 98f6d978ea3f5dc60afab538e0815910dbb02ba6
