#include <TFile.h>
#include <TVector3.h>
#include <TH1D.h>
#include <TStopwatch.h>
#include <TGeoTube.h>

#include "Tools/Flux/GCylindTH1Flux.h"
#include "Tools/Geometry/ROOTGeomAnalyzer.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/EventGen/GMCJDriver.h"
#include "Framework/Ntuple/NtpWriter.h"
#include "Framework/Ntuple/NtpMCFormat.h"
#include "Framework/Utils/XSecSplineList.h"
#include "Framework/Utils/AppInit.h"
#include "Framework/Utils/UnitUtils.h"

#include <string>
#include <iostream>

//void generate()
int main(int argc, char* argv[])
{

	////////////////////////////////////
	// VERBOSITY
	////////////////////////////////////
	
	//const std::string fconfname = "/wd/sw/GENIE/R-3_00_04/binary/config/Messenger.xml";
	//const std::string fconfname = "/wd/sw/GENIE/R-3_00_04/binary/config/Messenger_laconic.xml";
  const std::string fconfname = "Messenger_laconic.xml";
  //const std::string fconfname = "Messenger.xml";

	genie::utils::app_init::MesgThresholds(fconfname);
 
	////////////////////////////////////
	// X-SEC
	////////////////////////////////////

	const std::string fxsecname = "/wd/sw/GENIE/R-3_00_04/genie_xsec/v3_00_04/NULL/G1801a00000-k250-e1000/data/gxspl-FNALbig.xml";
  //const std::string fxsecname = "mu_e_geometry_v12.xml";

	genie::utils::app_init::XSecTable(fxsecname, false);
 
  genie::XSecSplineList* xpl = genie::XSecSplineList::Instance();
  xpl->SetCurrentTune("G18_01a_00_000");
  std::cout << xpl->NSplines() << " splines: " << xpl->CurrentTune() << std::endl;
  xpl->Print(std::cout);

	////////////////////////////////////
	// GEOMETRY
	////////////////////////////////////

	const std::string fgeoname = "/wd/dune-it/enurec/geo/geometry_v12.gdml";
	//const std::string topVol = "KLOETrackingRegion_volume";
	const std::string lunits = "cm";
	const std::string dunits = "g_cm3";

	// create geometry driver
	genie::geometry::ROOTGeomAnalyzer* geo_driver = new genie::geometry::ROOTGeomAnalyzer(fgeoname);

	TGeoManager* gm = geo_driver->GetGeometry();
	TGeoNavigator* nav = gm->GetCurrentNavigator();
	nav->cd("volWorld/volDetEnclosure_0/volKLOEFULLECALSENSITIVE_EXTTRK_NEWGAP_0/KLOETrackingRegion_volume_0");
	TGeoTube* tb = (TGeoTube*) nav->GetCurrentVolume();

	double p_loc[] = {0,0,0};
	double p_mst[] = {0,0,0};

	nav->LocalToMaster(p_loc,p_mst);

	// set volume where events will be generated
	geo_driver->SetTopVolName(tb->GetName());

	// set unities
	geo_driver->SetLengthUnits(genie::utils::units::UnitFromString(lunits));
	geo_driver->SetDensityUnits(genie::utils::units::UnitFromString(lunits));

	const double cmtom = 0.01;

	////////////////////////////////////
	// FLUX
	////////////////////////////////////
	
	const double beam_y_angle = -6./180. * TMath::Pi();
	//const double beam_y_angle = 0.;
	//const double beam_radius = 300.;
	const double beam_radius = 3.;
	//const double dist_from_kloe_center = 300.;
	//const double dist_from_kloe_center = 200.;
	const double dist_from_kloe_center = 2.;
	const std::string ffluxname = "/wd/dune-it/enurec/flux/histos_g4lbne_v3r5p4_QGSP_BERT_OptimizedEngineeredNov2017_neutrino_LBNEND_fastmc.root";

	TFile fflux(ffluxname.c_str());

	TH1D* h_numu_flux = (TH1D*) fflux.Get("numu_flux");
	TH1D* h_numubar_flux = (TH1D*) fflux.Get("numubar_flux");
	TH1D* h_nue_flux = (TH1D*) fflux.Get("nue_flux");
	TH1D* h_nuebar_flux = (TH1D*) fflux.Get("nuebar_flux");
	TH1D* h_nutau_flux = (TH1D*) fflux.Get("nutau_flux");
	TH1D* h_nutaubar_flux = (TH1D*) fflux.Get("nutaubar_flux");

	// create flux driver
	genie::flux::GCylindTH1Flux* flux_driver = new genie::flux::GCylindTH1Flux();

	// nu beam direction
	TVector3 nudir(1,0,0);
	nudir.RotateZ(beam_y_angle);
	flux_driver->SetNuDirection(nudir);

	// beam radius
	flux_driver->SetTransverseRadius(beam_radius);

	// beam spot
	//TVector3 beamspot(-dist_from_kloe_center,4.33096000,7.39616250);
	TVector3 beamspot(-dist_from_kloe_center,0.,0.);
	beamspot.RotateZ(beam_y_angle);
	beamspot.SetX(beamspot.X() + p_mst[0]*cmtom);
	beamspot.SetY(beamspot.Y() + p_mst[1]*cmtom);
	beamspot.SetZ(beamspot.Z() + p_mst[2]*cmtom);
	flux_driver->SetBeamSpot(beamspot);

	std::cout << "beam directorion: " << nudir.X() << " " << nudir.Y() << " " << nudir.Z() << std::endl;
	std::cout << "beam center     : " << beamspot.X() << " " << beamspot.Y() << " " << beamspot.Z() << std::endl;

	// beam spectra
	//flux_driver->AddEnergySpectrum (12, h_nue_flux);
	//flux_driver->AddEnergySpectrum (-12, h_nuebar_flux);
	flux_driver->AddEnergySpectrum (14, h_numu_flux);
	//flux_driver->AddEnergySpectrum (-14, h_numubar_flux);
	//flux_driver->AddEnergySpectrum (16, h_nutau_flux);
	//flux_driver->AddEnergySpectrum (-16, h_nutaubar_flux);

	////////////////////////////////////
	// MC JOB DRIVER
	////////////////////////////////////

	//const std::string fpathname = "/home/mt/Documents/wd/genie/max_path_length/max_path_length_volWorld_1000x1000.xml";

	genie::GMCJDriver* mcjob_driver = new genie::GMCJDriver();

	mcjob_driver->UseFluxDriver(flux_driver);
	mcjob_driver->UseGeomAnalyzer(geo_driver);

	// pre-caclucated max path length
	//mcjob_driver->UseMaxPathLengths(fpathname);

	// ???
	mcjob_driver->UseSplines();        
	//mcjob_driver->ForceSingleProbScale(); 
	//mcjob_driver->PreSelectEvents(false);


	// Configure
 
  std::cout << xpl->NSplines() << " splines: " << xpl->CurrentTune() << std::endl;
  std::cout << "before" << std::endl;
	mcjob_driver->Configure();
  std::cout << "after" << std::endl;

	////////////////////////////////////
	// CONFIG OUTPUT
	////////////////////////////////////

	const std::string foutname = "data/ghep/numu_geoV12_1000.root";
	genie::NtpMCFormat_t kDefOptNtpFormat = genie::kNFGHEP;
	const int run_num = 0;

	genie::NtpWriter ntpw(kDefOptNtpFormat, run_num);
	ntpw.CustomizeFilenamePrefix(foutname);
	ntpw.Initialize();

	////////////////////////////////////
	// GENERATE
	////////////////////////////////////
	TStopwatch sw;
	sw.Start();

	const int n_event = 1000;

	genie::EventRecord * event = 0;
	for(int ievent = 0; ievent < n_event; ievent++)
	{
		event = mcjob_driver->GenerateEvent();
		ntpw.AddEventRecord(ievent, event);
		delete event;
	}

	sw.Stop();
	sw.Print();

	////////////////////////////////////
	// SAVE OUTPUT
	////////////////////////////////////

	ntpw.Save();

	////////////////////////////////////
	// CLEAN-UP
	////////////////////////////////////

	delete geo_driver;
	delete flux_driver;
	delete mcjob_driver;

	std::cout << "**** done ****" << std::endl;

	exit(0);
}
