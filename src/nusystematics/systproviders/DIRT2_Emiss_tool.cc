#include "nusystematics/systproviders/DIRT2_Emiss_tool.hh"

#include "nusystematics/utility/exceptions.hh"

#include "systematicstools/utility/FHiCLSystParamHeaderUtility.hh"

#include "Framework/GHEP/GHepParticle.h"

#include "TLorentzVector.h"

using namespace systtools;
using namespace nusyst;
using namespace fhicl;

DIRT2_Emiss::DIRT2_Emiss(ParameterSet const &params)
    : IGENIESystProvider_tool(params),
      pidx_Emiss_CorrTail_Ar_p(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_CorrTail_Ar_n(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_Linear_Ar_p(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_Linear_Ar_n(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_ShiftPeak_Ar_p(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_ShiftPeak_Ar_n(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_CorrTail_C_p(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_CorrTail_C_n(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_Linear_C_p(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_Linear_C_n(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_ShiftPeak_C_p(systtools::kParamUnhandled<size_t>),
      pidx_Emiss_ShiftPeak_C_n(systtools::kParamUnhandled<size_t>),
      valid_file(nullptr), valid_tree(nullptr) {}

SystMetaData DIRT2_Emiss::BuildSystMetaData(ParameterSet const &cfg,
                                                     paramId_t firstId) {

  SystMetaData smd;

  for (std::string const &pname :
       {"Emiss_CorrTail_Ar_p", "Emiss_CorrTail_Ar_n", "Emiss_Linear_Ar_p", "Emiss_Linear_Ar_n",  "Emiss_ShiftPeak_Ar_p", "Emiss_ShiftPeak_Ar_n",
       "Emiss_CorrTail_C_p", "Emiss_CorrTail_C_n", "Emiss_Linear_C_p", "Emiss_Linear_C_n",  "Emiss_ShiftPeak_C_p", "Emiss_ShiftPeak_C_n"}) {
    systtools::SystParamHeader phdr;
    if (ParseFhiclToolConfigurationParameter(cfg, pname, phdr, firstId)) {
      phdr.systParamId = firstId++;
      smd.push_back(phdr);
    }
  }

  // OPTION_IN_CONF_FILE can be defined in the configuration file
  // then it is copied to tool_option when running "GenerateSystProviderConfig" to generation paramHeader

  std::string OPT_STRING = cfg.get<std::string>("OPT_STRING", ""); // second argument is the default when OPT_STRING does not exist
  tool_options.put("OPT_STRING", OPT_STRING);

  bool OPT_BOOL = cfg.get<bool>("OPT_BOOL", false);
  tool_options.put("OPT_BOOL", OPT_BOOL);

  fill_valid_tree = cfg.get<bool>("fill_valid_tree", false);
  tool_options.put("fill_valid_tree", fill_valid_tree);

  return smd;
}

bool DIRT2_Emiss::SetupResponseCalculator(
    fhicl::ParameterSet const &tool_options) {

  std::cout << "[DIRT2_Emiss::SetupResponseCalculator] OPT_STRING = " << tool_options.get<std::string>("OPT_STRING") << std::endl;
  std::cout << "[DIRT2_Emiss::SetupResponseCalculator] OPT_BOOL = " << tool_options.get<bool>("OPT_BOOL") << std::endl;

  systtools::SystMetaData const &md = GetSystMetaData();

  if (HasParam(md, "Emiss_CorrTail_Ar_p")) {
    pidx_Emiss_CorrTail_Ar_p = GetParamIndex(md, "Emiss_CorrTail_Ar_p");
  }

  if (HasParam(md, "Emiss_CorrTail_Ar_n")) {
    pidx_Emiss_CorrTail_Ar_n = GetParamIndex(md, "Emiss_CorrTail_Ar_n");
  }

  if (HasParam(md, "Emiss_Linear_Ar_p")) {
    pidx_Emiss_Linear_Ar_p = GetParamIndex(md, "Emiss_Linear_Ar_p");
  }

  if (HasParam(md, "Emiss_Linear_Ar_n")) {
    pidx_Emiss_Linear_Ar_n = GetParamIndex(md, "Emiss_Linear_Ar_n");
  }

  if (HasParam(md, "Emiss_ShiftPeak_Ar_p")) {
    pidx_Emiss_ShiftPeak_Ar_p = GetParamIndex(md, "Emiss_ShiftPeak_Ar_p");
  }

  if (HasParam(md, "Emiss_ShiftPeak_Ar_n")) {
    pidx_Emiss_ShiftPeak_Ar_n = GetParamIndex(md, "Emiss_ShiftPeak_Ar_n");
  }

  if (HasParam(md, "Emiss_CorrTail_C_p")) {
    pidx_Emiss_CorrTail_C_p = GetParamIndex(md, "Emiss_CorrTail_C_p");
  }

  if (HasParam(md, "Emiss_CorrTail_C_n")) {
    pidx_Emiss_CorrTail_C_n = GetParamIndex(md, "Emiss_CorrTail_C_n");
  }

  if (HasParam(md, "Emiss_Linear_C_p")) {
    pidx_Emiss_Linear_C_p = GetParamIndex(md, "Emiss_Linear_C_p");
  }

  if (HasParam(md, "Emiss_Linear_C_n")) {
    pidx_Emiss_Linear_C_n = GetParamIndex(md, "Emiss_Linear_C_n");
  }

  if (HasParam(md, "Emiss_ShiftPeak_C_p")) {
    pidx_Emiss_ShiftPeak_C_p = GetParamIndex(md, "Emiss_ShiftPeak_C_p");
  }

  if (HasParam(md, "Emiss_ShiftPeak_C_n")) {
    pidx_Emiss_ShiftPeak_C_n = GetParamIndex(md, "Emiss_ShiftPeak_C_n");
  }

  fill_valid_tree = tool_options.get<bool>("fill_valid_tree", false);
  if (fill_valid_tree) {
    InitValidTree();
  }

  this->SetupNormalization();

  return true;
}

event_unit_response_t
DIRT2_Emiss::GetEventResponse(genie::EventRecord const &ev) {

  genie::GHepParticle *FSLep = ev.FinalStatePrimaryLepton();
  genie::GHepParticle *ISLep = ev.Probe();

  TLorentzVector FSLepP4 = *FSLep->P4();
  TLorentzVector ISLepP4 = *ISLep->P4();
  TLorentzVector emTransfer = (ISLepP4 - FSLepP4);

  // TH: get Ermv directly from GENIE event record
  double Emiss_preFSI;
  int nucleon_PDG, target_PDG;
  genie::GHepParticle *nucleon = ev.HitNucleon();
  if (nucleon == NULL){
    // TH: some events don't have an initial nucleon (e.g. coherent scattering)
    //     want to skip these events and not re-weight
    Emiss_preFSI = -999;
    nucleon_PDG = -999;
  }
  else {
    // Emiss_preFSI = nucleon->RemovalEnergy();
    Emiss_preFSI = nucleon->Mass() - nucleon->Energy();
    nucleon_PDG = nucleon->Pdg();
  } 

  // GHepRecord::TargetNucleus() is designed to return nullptr for a free nucleon target (e.g., hydrogen)
  // If TargetNucleus() is available, use ev.TargetNucleus()->Pdg()
  // if not, force it to hydrogen
  target_PDG = ev.TargetNucleus() ? ev.TargetNucleus()->Pdg() : 1000010010;

  // now make the output
  systtools::event_unit_response_t resp;
  systtools::SystMetaData const &md = GetSystMetaData();

  if (pidx_Emiss_CorrTail_Ar_p != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_CorrTail_Ar_p].systParamId, {}} );
    if (target_PDG == 1000180400 && nucleon_PDG == 2212){
      for (double var : md[pidx_Emiss_CorrTail_Ar_p].paramVariations) {
        resp.back().responses.push_back( GetEmissCorrTailRW( Emiss_preFSI, var) );
      } 
    }
    else{
      for (unsigned int i = 0; i < md[pidx_Emiss_CorrTail_Ar_p].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_CorrTail_Ar_n != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_CorrTail_Ar_n].systParamId, {}} );
    if (target_PDG == 1000180400 && nucleon_PDG == 2112){
      for (double var : md[pidx_Emiss_CorrTail_Ar_n].paramVariations) {
        resp.back().responses.push_back( GetEmissCorrTailRW( Emiss_preFSI, var) );
      } 
    }
    else{
      for (unsigned int i = 0; i < md[pidx_Emiss_CorrTail_Ar_n].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_Linear_Ar_p != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_Linear_Ar_p].systParamId, {}} );
    if (target_PDG == 1000180400 && nucleon_PDG == 2212){
      for (double var : md[pidx_Emiss_Linear_Ar_p].paramVariations) {
        resp.back().responses.push_back( GetEmissLinearRW( Emiss_preFSI, var) );
      }
    }
    else{
      for (unsigned int i = 0; i < md[pidx_Emiss_Linear_Ar_p].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_Linear_Ar_n != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_Linear_Ar_n].systParamId, {}} );
    if (target_PDG == 1000180400 && nucleon_PDG == 2112){
      for (double var : md[pidx_Emiss_Linear_Ar_n].paramVariations) {
        resp.back().responses.push_back( GetEmissLinearRW( Emiss_preFSI, var) );
      }
    }
    else{
      for (unsigned int i = 0; i < md[pidx_Emiss_Linear_Ar_n].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_ShiftPeak_Ar_p != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_ShiftPeak_Ar_p].systParamId, {}} );
    if (target_PDG == 1000180400 && nucleon_PDG == 2212){
      for (double var : md[pidx_Emiss_ShiftPeak_Ar_p].paramVariations) {
        resp.back().responses.push_back( GetEmissShiftPeakRW( Emiss_preFSI, var) );
      }
    }
    else {
      for (unsigned int i = 0; i < md[pidx_Emiss_ShiftPeak_Ar_p].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_ShiftPeak_Ar_n != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_ShiftPeak_Ar_n].systParamId, {}} );
    if (target_PDG == 1000180400 && nucleon_PDG == 2112){
      for (double var : md[pidx_Emiss_ShiftPeak_Ar_n].paramVariations) {
        resp.back().responses.push_back( GetEmissShiftPeakRW( Emiss_preFSI, var) );
      }
    }
    else {
      for (unsigned int i = 0; i < md[pidx_Emiss_ShiftPeak_Ar_n].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_CorrTail_C_p != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_CorrTail_C_p].systParamId, {}} );
    if (target_PDG == 1000060120 && nucleon_PDG == 2212){
      for (double var : md[pidx_Emiss_CorrTail_C_p].paramVariations) {
        resp.back().responses.push_back( GetEmissCorrTailRW( Emiss_preFSI, var) );
      } 
    }
    else{
      for (unsigned int i = 0; i < md[pidx_Emiss_CorrTail_C_p].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_CorrTail_C_n != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_CorrTail_C_n].systParamId, {}} );
    if (target_PDG == 1000060120 && nucleon_PDG == 2112){
      for (double var : md[pidx_Emiss_CorrTail_C_n].paramVariations) {
        resp.back().responses.push_back( GetEmissCorrTailRW( Emiss_preFSI, var) );
      } 
    }
    else{
      for (unsigned int i = 0; i < md[pidx_Emiss_CorrTail_C_n].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_Linear_C_p != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_Linear_C_p].systParamId, {}} );
    if (target_PDG == 1000060120 && nucleon_PDG == 2212){
      for (double var : md[pidx_Emiss_Linear_C_p].paramVariations) {
        resp.back().responses.push_back( GetEmissLinearRW( Emiss_preFSI, var) );
      }
    }
    else{
      for (unsigned int i = 0; i < md[pidx_Emiss_Linear_C_p].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_Linear_C_n != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_Linear_C_n].systParamId, {}} );
    if (target_PDG == 1000060120 && nucleon_PDG == 2112){
      for (double var : md[pidx_Emiss_Linear_C_n].paramVariations) {
        resp.back().responses.push_back( GetEmissLinearRW( Emiss_preFSI, var) );
      }
    }
    else{
      for (unsigned int i = 0; i < md[pidx_Emiss_Linear_C_n].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_ShiftPeak_C_p != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_ShiftPeak_C_p].systParamId, {}} );
    if (target_PDG == 1000060120 && nucleon_PDG == 2212){
      for (double var : md[pidx_Emiss_ShiftPeak_C_p].paramVariations) {
        resp.back().responses.push_back( GetEmissShiftPeakRW( Emiss_preFSI, var) );
      }
    }
    else {
      for (unsigned int i = 0; i < md[pidx_Emiss_ShiftPeak_C_p].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }

  if (pidx_Emiss_ShiftPeak_C_n != systtools::kParamUnhandled<size_t>) {
    resp.push_back( {md[pidx_Emiss_ShiftPeak_C_n].systParamId, {}} );
    if (target_PDG == 1000060120 && nucleon_PDG == 2112){
      for (double var : md[pidx_Emiss_ShiftPeak_C_n].paramVariations) {
        resp.back().responses.push_back( GetEmissShiftPeakRW( Emiss_preFSI, var) );
      }
    }
    else {
      for (unsigned int i = 0; i < md[pidx_Emiss_ShiftPeak_C_n].paramVariations.size(); i++) {
        resp.back().responses.push_back(1);
      }
    }
  }


  if (fill_valid_tree) {

    pdgfslep = ev.FinalStatePrimaryLepton()->Pdg();
    momfslep = FSLepP4.Vect().Mag();
    cthetafslep = FSLepP4.Vect().CosTheta();

    Pdgnu = ISLep->Pdg();
    NEUTMode = 0;
    NEUTMode = genie::utils::ghep::NeutReactionCode(&ev);

    QELikeTarget_t qel_targ = GetQELikeTarget(ev);
    QELTarget = e2i(qel_targ);

    Enu = ISLepP4.E();
    Q2 = -emTransfer.Mag2();
    W = ev.Summary()->Kine().W(true);
    q0 = emTransfer.E();
    q3 = emTransfer.Vect().Mag();

    valid_tree->Fill();
  }

  return resp;
}

std::string DIRT2_Emiss::AsString() { return ""; }

void DIRT2_Emiss::SetupNormalization() {
  //TODO: Make sure it is inizialized with the Ar23 config
  genie::AlgConfigPool * conf_pool = genie::AlgConfigPool::Instance();
  genie::Registry * gpl = conf_pool->GlobalParameterList();
  genie::LocalFGM LFG;
  LFG.Configure(*gpl);
  int A = 40;
  int target_pdgc = 1000180400;
  int nucleon_pdgc = 2112; //neutron

  double neutronMass = genie::constants::kNeutronMass;

  genie::Target tgt(target_pdgc);

  float R0 = 1.4;
  float R = R0*std::pow(A, 1./3);
  float rmax = 3*R;
  float dr = R/400.;
  float fPMax = 0.4;
  int npbins = (int) (1000*fPMax);
  double dp = fPMax / (npbins-1);
  double Q = 0.01;

  int nbins_r = (int)(rmax/dr) + 1;

  TH2D *prob = new TH2D("prob", "prob", npbins, 0, fPMax, nbins_r, 0, rmax);
  TH2D *prob_e = new TH2D("prob_e", "prob_e", npbins, 0, fPMax, 200, 0, 0.05);
  
  for(int j = 0; j < nbins_r; j++) {
    float r = j * dr;
    float nucl_density = genie::utils::nuclear::Density(r,A);
    double KF = LFG.LocalFermiMomentum( tgt, nucleon_pdgc, r );
    
    double fSRC_Fraction = 0.12;
    double fPCutOff = 0.7;

    double integral_positive_e = 0;
    double integral_negative_e = 0;

    for(int i = 0; i < npbins; i++) {
      double p  = i * dp;
      double p2 = TMath::Power(p,2);

      // calculate |phi(p)|^2
      double phi2 = 0;
          if (p <= KF){
              phi2 = (1./(4*genie::constants::kPi)) * (3/TMath::Power(KF,3.)) * ( 1 - fSRC_Fraction );
          }else if( p > KF && p < fPCutOff ){
              phi2 = (1./(4*genie::constants::kPi)) * ( fSRC_Fraction / (1./KF - 1./fPCutOff) ) / TMath::Power(p,4.);
          }

      // calculate probability density : dProbability/dp
      double dP_dp = 4*genie::constants::kPi * p2 * phi2*r*r*nucl_density;
      double Tf = sqrt(std::pow(KF, 2) + std::pow(neutronMass, 2)) - neutronMass;
      double Tnucl = sqrt(p2 + std::pow(neutronMass, 2)) - neutronMass;
      double Emiss = Q + Tf - Tnucl;

      if (Emiss >= 0){
        integral_positive_e += dP_dp;
        prob->Fill(p, r, dP_dp);
      } else {
        integral_negative_e += dP_dp;
      }
    }
    integral_positive_e = std::max(integral_positive_e, 1e-10);
    //Rescale all the bins
    for(int i = 0; i < npbins; i++) {
      prob->SetBinContent(i + 1, j +1, prob->GetBinContent(i + 1, j + 1)*(integral_positive_e + integral_negative_e)/integral_positive_e);
    }
  }

  for(uint i = 1; i <= prob->GetNbinsX(); i++) {
    for(uint j = 1; j <= prob->GetNbinsY(); j++) {
      double p = prob->GetXaxis()->GetBinCenter(i);
      double r = prob->GetYaxis()->GetBinCenter(j);
      double KF = LFG.LocalFermiMomentum( tgt, nucleon_pdgc, r );
      double Tf = sqrt(std::pow(KF, 2) + genie::constants::kNucleonMass2) - genie::constants::kNucleonMass;
      double Tnucl = sqrt(p*p + genie::constants::kNucleonMass2) - genie::constants::kNucleonMass;
      double Emiss = Q + Tf - Tnucl;

      prob_e->Fill(p, Emiss, prob->GetBinContent(i, j));
    }
  }

  // for(uint i = 1; i <= prob_e->GetNbinsX(); i++) {
  //   double density_negative_e = prob_e->GetBinContent(i, 0);
  //   double total_density = prob_e->Integral(i, i, 0, prob_e->GetNbinsY());
  //   double rescaling = 0;
  //   for(uint j = 1; j <= prob_e->GetNbinsY(); j++) {
  //     prob_e->SetBinContent(i, j, prob_e->GetBinContent(i, j) - density_negative_e);
  //   }
  // }

  valid_file->cd();
  prob->Write();
  TH1D *prob_p = prob->ProjectionX();
  prob_p->Write();
  TH1D *prob_r = prob->ProjectionY();
  prob_r->Write();
  prob_e->Write();

  TH1D *prob_ex = prob_e->ProjectionX();
  prob_ex->Write();
  TH1D *prob_ey = prob_e->ProjectionY();
  prob_ey->Write();
}

void DIRT2_Emiss::InitValidTree() {

  valid_file = new TFile("DIRT2_EmissWeights_validTree.root", "RECREATE");
  valid_tree = new TTree("valid_tree", "");

  valid_tree->Branch("NEUTMode", &NEUTMode);
  valid_tree->Branch("QELTarget", &QELTarget);
  valid_tree->Branch("Enu", &Enu);
  valid_tree->Branch("Pdg_nu", &Pdgnu);
  valid_tree->Branch("Pdg_FSLep", &pdgfslep);
  valid_tree->Branch("P_FSLep", &momfslep);
  valid_tree->Branch("CosTheta_FSLep", &cthetafslep);
  valid_tree->Branch("Q2", &Q2);
  valid_tree->Branch("W", &W);
  valid_tree->Branch("q0", &q0);
  valid_tree->Branch("q3", &q3);
  valid_tree->Branch("pmiss", &pmiss);
  valid_tree->Branch("Emiss", &Emiss);
}

DIRT2_Emiss::~DIRT2_Emiss() {
  if (valid_file) {
    valid_tree->SetDirectory(valid_file);
    valid_file->Write();
    valid_file->Close();
    delete valid_file;
  }
} 
