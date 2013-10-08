// File: tree.cc
// fwd_central study


// Header file to access Pythia 8 program elements.
#include "Pythia.h"

// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"


//for atoi
#include <cstdlib>
#include <string>

using namespace Pythia8;

int main(int argc, char* argv[]) {

  //usage: n_events path_to_the_output_file seed
  
  // Select new PDF set; LHAPDF file name conventions.
  //string pdfSet = "cteq5l.LHgrid";
  //string pdfSet = "cteq61.LHpdf";
  //string pdfSet = "cteq61.LHgrid";
  //string pdfSet = "MRST2004nlo.LHgrid";
  //string pdfSet = "MRST2001lo.LHgrid";

  string pdfSet = argv[4];

  Pythia pythia;
  pythia.readString("HardQCD:all = on"); 
  pythia.readString("SoftQCD:all = off"); 
  pythia.readString("PhaseSpace:pTHatMin = 10.");
  pythia.readString("Beams:eCM = 7000.");
 
  pythia.readString("PartonLevel:all = on");
 
  pythia.readString("PartonLevel:FSR = off");
  pythia.readString("PartonLevel:ISR = off");
  pythia.readString("PartonLevel:MPI = off");

  pythia.readString("Random:setSeed = on"); 
  //pythia.readString("Random:seed = 123456"); 
  pythia.readString(argv[3]); 

  //pythia.readString("PDF:useLHAPDF = on");
  //pythia.readString("PDF:LHAPDFset = " + pdfSet);

  //pythia.readString(argv[4]);

  pythia.readString("PDF:pSet=5");
  pythia.readString("PDF:useHard = on");
  pythia.readString(argv[4]);

  //pythia.readString("PDF:pSet = 5"); //mstw2008lo.00.dat
  //pythia.readString("PDF:pSet = 66"); //mstw2008lo_integratedgluonBFKL_.00.dat
  //pythia.readString("PDF:pSet = 67"); //mstw2008lo_integratedgluonBK_.00.dat

  pythia.init();

  // Set up the ROOT TFile and TTree.
  //TFile *file = TFile::Open("pytree_no_FSR.root","recreate");

  TFile *file = TFile::Open(argv[2], "recreate");

  int n_jets;
 
  std::vector<float>*cen_pt = new std::vector<float>(); 
  std::vector<float>*cen_y = new std::vector<float>();
  std::vector<float>*cen_phi = new std::vector<float>();

  std::vector<float>*fwd_pt = new std::vector<float>();
  std::vector<float>*fwd_y = new std::vector<float>();
  std::vector<float>*fwd_phi = new std::vector<float>();

  TTree *T = new TTree("T","ev1 Tree");

  T->Branch("cen_pt","vector<float>",&cen_pt);
  T->Branch("cen_y","std::vector<float>",&cen_y);
  T->Branch("cen_phi","std::vector<float>",&cen_phi);
 
  T->Branch("fwd_pt","std::vector<float>",&fwd_pt);
  T->Branch("fwd_y","std::vector<float>",&fwd_y);
  T->Branch("fwd_phi","std::vector<float>",&fwd_phi);

  SlowJet slowJet( -1,0.5,35.,4.7,2,1);


  float jet_pt, jet_y, jet_phi;


 // Begin event loop. Generate event; skip if generation aborted.


  for (int iEvent = 0; iEvent < atoi(argv[1]); ++iEvent) {
    if (!pythia.next()) continue;
    slowJet. analyze( pythia.event );

    cen_pt->clear();
    cen_y->clear();
    cen_phi->clear();
      
    fwd_pt->clear();
    fwd_y->clear();
    fwd_phi->clear();
 
    
    for (int i = 0; i < slowJet.sizeJet(); ++i) {
      
      jet_pt = slowJet.pT(i);
      jet_y =  slowJet.y(i);
      jet_phi = slowJet.phi(i);

      if (fabs(jet_y)<=2.8){
	cen_pt->push_back(jet_pt);
	cen_y->push_back(jet_y);
	cen_phi->push_back(jet_phi);
     }

      if (fabs(jet_y)>=3.2 && fabs(jet_y)<=4.7){
	fwd_pt->push_back(jet_pt);
	fwd_y->push_back(jet_y);
	fwd_phi->push_back(jet_phi);
      }
    }

 
    T->Fill();

  // End event loop.
  }

  // Statistics on event generation.
  pythia.stat();

  //  Write tree.
  T->Print();
  T->Write();
  delete file;
  pythia.settings.listAll();

  
  // Done.
  return 0;
}
