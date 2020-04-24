// Copyright 2019 Matteo Tenti - INFN-BO
//
// generate muon neutrino events on KLOE detector
//
// inputs:
//   - geometry: geometry_v12.gdml
//   - flux: histos_g4lbne_v3r5p4_QGSP_BERT_
//            OptimizedEngineeredNov2017_
//            antineutrino_LBNEND_fastmc.root
//   - top volume: volWorld/volDetEnclosure_0/
//                   volKLOEFULLECALSENSITIVE_EXTTRK_NEWGAP_0/
//                   KLOETrackingRegion_volume_0/
//                   volKLOESTTFULLNEWCONFNEWGAPLAR_0

#include <string>
#include <iostream>

#include <TFile.h>
#include <TVector3.h>
#include <TH1D.h>
#include <TStopwatch.h>
#include <TGeoTube.h>
#include <TGeoManager.h>

#include "Tools/Flux/GCylindTH1Flux.h"
#include "Tools/Geometry/ROOTGeomAnalyzer.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/EventGen/GMCJDriver.h"
#include "Framework/Ntuple/NtpWriter.h"
#include "Framework/Ntuple/NtpMCFormat.h"
#include "Framework/Utils/XSecSplineList.h"
#include "Framework/Utils/AppInit.h"
#include "Framework/Utils/UnitUtils.h"
#include "Framework/Utils/CmdLnArgParser.h"
#include "Framework/Utils/PrintUtils.h"
#include "Framework/Utils/RunOpt.h"

#include "Framework/Algorithm/AlgConfigPool.h"

void GetCommandLineArgs (int argc, char ** argv);
void PrintSyntax        (void);

Long_t run_num = 0;                     // run number


//________________________________________________________________________________________
int main(int argc, char *argv[]) {
  TStopwatch sw;
  sw.Start();
  
  ////////////////////////////////////
  // PARSE COMMAND LINE ARGUMENTS
  ////////////////////////////////////
  
  
  GetCommandLineArgs(argc,argv);


  genie::utils::app_init::MesgThresholds(genie::RunOpt::Instance()->MesgThresholdFiles());


  ////////////////////////////////////
  // SET TUNE AND EVENT GENERATOR LIST
  ////////////////////////////////////

//  genie::RunOpt::Instance()->SetEventGeneratorList("Default");

  if (!genie::RunOpt::Instance()->Tune()) {
    std::cout << " No TuneId in RunOption" << std::endl;
    exit(-1);
  }
  genie::RunOpt::Instance()->BuildTune();



  ////////////////////////////////////
  // VERBOSITY
  ////////////////////////////////////

  const std::string fconfname_lac = "Messenger_laconic.xml";
  const std::string fconfname_whi = "Messenger_whisper.xml";
  const std::string fconfname_std = "Messenger.xml";

  genie::utils::app_init::MesgThresholds(fconfname_whi);

  ////////////////////////////////////
  // X-SEC
  ////////////////////////////////////

  std::string tune_name = genie::RunOpt::Instance()->Tune()->Name();
  std::size_t found;
  while(1){
    found = tune_name.find("_");
    if(found==std::string::npos)break;
    tune_name.erase(found,1);
  }
  
  std::string fxsecdir = "/data/genie_xsec/v3_00_06/NULL/";
  fxsecdir += tune_name;
  fxsecdir += "-k250-e1000/data/";
  
  std::string fxsecname = "gxspl-FNALbigger.xml";
  fxsecname.insert(0,fxsecdir);

  genie::utils::app_init::XSecTable(fxsecname, true);

  std::cout << "***********************************" << std::endl;
  std::cout << " finish reading cross sections " << std::endl;
  std::cout << "***********************************" << std::endl;
  sw.Print();
  sw.Start();
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************" << std::endl;


  ////////////////////////////////////
  //  SET RUN NUMBER AND RANDOM SEED
  ////////////////////////////////////

  const long int random_seed = 12345678 + run_num * 1234;
  genie::utils::app_init::RandGen(random_seed);

  ////////////////////////////////////
  // GEOMETRY
  ////////////////////////////////////

  const std::string fgeoname = "/data/geometries/nd_hall_kloe_sttonly.gdml";
  const std::string lunits = "cm";
  const std::string dunits = "g_cm3";

  // create geometry driver
  genie::geometry::ROOTGeomAnalyzer *geo_driver =
      new genie::geometry::ROOTGeomAnalyzer(fgeoname);

  TGeoManager *gm = geo_driver->GetGeometry();
  TGeoNavigator *nav = gm->GetCurrentNavigator();
  nav->cd("volWorld/rockBox_lv_0/volDetEnclosure_0/volKLOE_0/volSTTFULL_0");
  TGeoVolume *tb = reinterpret_cast<TGeoVolume *>(nav->GetCurrentVolume());

  double p_loc[] = {0, 0, 0};
  double p_mst[] = {0, 0, 0};

  nav->LocalToMaster(p_loc, p_mst);

  // set volume where events will be generated
  geo_driver->SetTopVolName(tb->GetName());

  std::cout << "Setting top volume to: " << tb->GetName()
            << " center (x,y,z): " << p_mst[0] << "; " << p_mst[1] << "; "
            << p_mst[2] << std::endl;

  // set unities
  geo_driver->SetLengthUnits(genie::utils::units::UnitFromString(lunits));
  geo_driver->SetDensityUnits(genie::utils::units::UnitFromString(dunits));

  const double mtocm = 100.;
  const double cmtom = 1. / mtocm;

  std::cout << "***********************************" << std::endl;
  std::cout << " finish reading geometry " << std::endl;
  std::cout << "***********************************" << std::endl;
  sw.Print();
  sw.Start();
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************" << std::endl;

  ////////////////////////////////////
  // FLUX
  ////////////////////////////////////

  const double beam_y_angle = 6. / 180. * TMath::Pi();
  const double beam_radius = 3.;
  const double dist_from_kloe_center = 3.;
  const std::string ffluxname_nu =
      "/data/flux/histos_g4lbne_v3r5p4_QGSP_BERT_OptimizedEngineeredNov2017_neutrino_LBNEND_fastmc.root";
  const std::string ffluxname_anu =
      "/data/flux/"
      "histos_g4lbne_v3r5p4_QGSP_BERT_"
      "OptimizedEngineeredNov2017_antineutrino_"
      "LBNEND_fastmc.root";

  TFile fflux(ffluxname_nu.c_str());

  TH1D *h_numu_flux = reinterpret_cast<TH1D *>(fflux.Get("numu_flux"));
  TH1D *h_numubar_flux = reinterpret_cast<TH1D *>(fflux.Get("numubar_flux"));
  TH1D *h_nue_flux = reinterpret_cast<TH1D *>(fflux.Get("nue_flux"));
  TH1D *h_nuebar_flux = reinterpret_cast<TH1D *>(fflux.Get("nuebar_flux"));
  TH1D *h_nutau_flux = reinterpret_cast<TH1D *>(fflux.Get("nutau_flux"));
  TH1D *h_nutaubar_flux = reinterpret_cast<TH1D *>(fflux.Get("nutaubar_flux"));

  // create flux driver
  genie::flux::GCylindTH1Flux *flux_driver = new genie::flux::GCylindTH1Flux();

  // nu beam direction
  TVector3 nudir(0, 0, 1);
  nudir.RotateX(beam_y_angle);
  flux_driver->SetNuDirection(nudir);

  // beam radius
  flux_driver->SetTransverseRadius(beam_radius);

  // beam spot
  TVector3 beamspot(0., 0., -dist_from_kloe_center);
  beamspot.RotateX(beam_y_angle);
  beamspot.SetX(beamspot.X() + p_mst[0] * cmtom);
  beamspot.SetY(beamspot.Y() + p_mst[1] * cmtom);
  beamspot.SetZ(beamspot.Z() + p_mst[2] * cmtom);
  flux_driver->SetBeamSpot(beamspot);

  std::cout << "beam directorion: " << nudir.X() << " " << nudir.Y() << " "
            << nudir.Z() << std::endl;
  std::cout << "beam center     : " << beamspot.X() << " " << beamspot.Y()
            << " " << beamspot.Z() << std::endl;

  // beam spectra
  flux_driver->AddEnergySpectrum (12, h_nue_flux);
  flux_driver->AddEnergySpectrum (-12, h_nuebar_flux);
  flux_driver->AddEnergySpectrum(14, h_numu_flux);
  flux_driver->AddEnergySpectrum (-14, h_numubar_flux);
  // flux_driver->AddEnergySpectrum (16, h_nutau_flux);
  // flux_driver->AddEnergySpectrum (-16, h_nutaubar_flux);

  // flux integral for normalization
  double flux_integral=0.;
  flux_integral+=h_nue_flux->Integral();
  flux_integral+=h_nuebar_flux->Integral();
  flux_integral+=h_numu_flux->Integral();
  flux_integral+=h_numubar_flux->Integral();
  // flux_integral+=h_nutau_flux->Integral();
  // flux_integral+=h_nutaubar_flux->Integral();



  std::cout << "***********************************" << std::endl;
  std::cout << " finish reading fluxes " << std::endl;
  std::cout << "***********************************" << std::endl;
  sw.Print();
  sw.Start();
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************" << std::endl;

  ////////////////////////////////////
  // MC JOB DRIVER
  ////////////////////////////////////

  genie::GMCJDriver *mcjob_driver = new genie::GMCJDriver();
  mcjob_driver->SetEventGeneratorList(
      genie::RunOpt::Instance()->EventGeneratorList());
  mcjob_driver->UseFluxDriver(flux_driver);
  mcjob_driver->UseGeomAnalyzer(geo_driver);

  // If you supply the maximum path lengths for all materials in your geometry
  // (eg for ROOT/GEANT geometries they can be computed running GENIE's gmxpl
  // application, see $GENIE/src/stdapp/gMaxPathLengths.cxx ) you can speed up
  // the driver init phase by quite a bit (especially for complex geometries).
  // mcjob_driver->UseMaxPathLengths(fpathname);

  // Instructs the driver to use cross section splines rather than computing
  // cross sections by integrating the differential cross section model which
  // can be very time-consuming.
  // **Note**
  // -- If you called GEVGDriver::CreateSplines() already the driver would
  //    a) assume that you want to use them and b) would be assured that it
  //    has all the splines it needs, so you do not need to call this method.
  // -- If you populated the XSecSplineList in another way without this driver
  //    knowing about it, eg from an external XML file, do call this method
  //    to let the driver know that you want to use the splines. However, note
  //    that the driver would **explicitly check** that you have loaded all the
  //    splines it needs. If not, then its fiery personality will take over and
  //    it will refuse your request, reverting back to not using splines.
  mcjob_driver->UseSplines();

  // Use a single probability scale. That generates unweighted events.
  mcjob_driver->ForceSingleProbScale();

  // Set whether to pre-select events based on a max-path lengths file. This
  // should be turned off if using pre-generated interaction probabilities
  // calculated from a given flux file
  // mcjob_driver->PreSelectEvents(false);

  // Configure
  mcjob_driver->Configure();

  std::cout << "***********************************" << std::endl;
  std::cout << " finish configuring MC job driver  " << std::endl;
  std::cout << "***********************************" << std::endl;
  sw.Print();
  sw.Start();
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************" << std::endl;

  ////////////////////////////////////
  // CONFIG OUTPUT
  ////////////////////////////////////

  const std::string foutname = "numu_internal_10k";
  
  
  genie::NtpMCFormat_t kDefOptNtpFormat = genie::kNFGHEP;
  // const int run_num = 0;

  genie::NtpWriter ntpw(kDefOptNtpFormat, run_num);
  ntpw.CustomizeFilenamePrefix(foutname);
  ntpw.Initialize();

  std::cout << "***********************************" << std::endl;
  std::cout << " finish configuring output " << std::endl;
  std::cout << "***********************************" << std::endl;
  sw.Print();
  sw.Start();
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************" << std::endl;

  ////////////////////////////////////
  // GENERATE
  ////////////////////////////////////

  const int n_event = 10000;
  int ievent = 0;

  genie::EventRecord *event = 0;
  do {
    std::cout << ievent << " of " << n_event << std::endl;
    event = mcjob_driver->GenerateEvent();
    ntpw.AddEventRecord(ievent, event);
    ievent++;
    delete event;
  } while (ievent < n_event);

  std::cout << "***********************************" << std::endl;
  std::cout << " finish generating events " << std::endl;
  std::cout << "***********************************" << std::endl;
  sw.Print();
  sw.Start();
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************" << std::endl;

  ////////////////////////////////////
  // SAVE OUTPUT
  ////////////////////////////////////

  ntpw.Save();

  std::cout << "***********************************" << std::endl;
  std::cout << " finish saving output " << std::endl;
  std::cout << "***********************************" << std::endl;
  sw.Print();
  sw.Start();
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************" << std::endl;

  ////////////////////////////////////
  // CALC EXPOSURE
  ////////////////////////////////////
  
  double n_tot = mcjob_driver->NFluxNeutrinos();
  double p_scale = mcjob_driver->GlobProbScale();
  double beam_area = TMath::Pi()*TMath::Power(beam_radius,2.);
  double n_pot = n_tot/p_scale/flux_integral/beam_area;

  std::cout << "***********************************" << std::endl;
  std::cout << " number of simulated NPOTs " << std::endl;
  std::cout << "***********************************" << std::endl;
  std::cout << "n_run= "<< run_num <<std::endl;
  std::cout << "n_tot= " << n_tot << std::endl;
  std::cout << "p_scale= " << p_scale << std::endl;
  std::cout << "flux_integral= " << flux_integral << std::endl;
  std::cout << "beam_area= " << beam_area << std::endl; 
  std::cout << "n_pot= " << n_pot << std::endl;
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************" << std::endl;



  ////////////////////////////////////
  // CLEAN-UP
  ////////////////////////////////////

  delete geo_driver;
  delete flux_driver;
  delete mcjob_driver;

  std::cout << "***********************************" << std::endl;
  std::cout << " finish cleaning " << std::endl;
  std::cout << "***********************************" << std::endl;
  sw.Print();
  sw.Start();
  std::cout << "***********************************" << std::endl;
  std::cout << "***********************************" << std::endl;

  std::cout << "***********************************" << std::endl;
  std::cout << " Done " << std::endl;
  std::cout << "***********************************" << std::endl;
  sw.Stop();
}

//________________________________________________________________________________________
void GetCommandLineArgs(int argc, char ** argv)
{
// Get the command line arguments


  genie::RunOpt::Instance()->ReadFromCommandLine(argc, argv);

  genie::CmdLnArgParser parser(argc,argv);

  // help?
  bool help = parser.OptionExists('h');
  if(help) {
      PrintSyntax();
      exit(0);
  }

  //
  // *** run number
  //
  if( parser.OptionExists('r') ) {
    run_num = parser.ArgAsLong('r');
    std::cout << "Reading MC run number: "<<run_num<<std::endl;  
  }


  return;
}
//________________________________________________________________________________________

void PrintSyntax(void)
{

  
  std::cout << "\n **Syntax** ";
  std::cout << "\n generate  [-h]";
  std::cout << "\n           [-r run#]";
  std::cout << "\n           [--tune tune]";
  std::cout << "\n";
  std::cout << "\n";
   
}
//________________________________________________________________________________________





