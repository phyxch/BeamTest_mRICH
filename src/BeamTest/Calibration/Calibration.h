#ifndef Calibration_h
#define Calibration_h

#include <string>
#include <vector>

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TVector2.h>

#include "../PixelMap/mRICH.h"

class PixelMap;

class Calibration : public TObject
{
  public:
    Calibration();
    ~Calibration();

    // is processing with PMT? true for PMT | false for MPPC
    void is_PMT(bool b)
    {
      is_pmt = b;
    }

    void set_det(std::string detector)
    {
      mDet = detector;
    }

    int Init();
    int initChain();
    int initTdcCut();
    int initTimeDurationCut();

    int Make();
    int Finish();

    // ring finder
    int initRingFinder();
    int writeRingFinder();
    int clearRingFinder(); 
    int HoughTransform(int numOfPhotons, TH2D *h_PhotonDist, std::vector<int> xPixel, std::vector<int> yPixel);
    bool findRing(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit, double &x_Cherenkov, double &y_Cherenkov, double &r_Cherenkov);
    bool isSamePosition(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);
    bool isCollinear(TVector2 firstHit, TVector2 secondHit, TVector2 thirdHit);
    bool isOnRing(TVector2 photonHit, double x_HoughTransform, double y_HoughTransform, double r_HoughTransform);
    float findPixelCoord(int pixel); // return correspoding pixel coordinate

    // beam finder
    int initBeamFinder();
    int writeBeamFinder();
    int clearBeamFinder(); 

  private:
    PixelMap *pixel_map;
    std::string mDet, mHome;

    bool is_pmt;

    std::string mOutPutFile;
    TFile *mFile_OutPut;

    // static int const mNumOfPixels = 33;
    float mTdc_Start;
    float mTdc_Stop;
    float mTime_Low;
    float mTime_High;
    float mTimeDuration[mRICH::mNumOfPixels][mRICH::mNumOfPixels][2];
    TH1D *h_mTDC[mRICH::mNumOfPixels][mRICH::mNumOfPixels]; // 0 for x-pixel | 1 for y-pixel
    TH2D *h_mRingImage;
    TH2D *h_mRingImage_DisPlay;
    TH2D *h_mRingImage_DisPlay_beam;
    TH1D *h_mNumOfPhotons;
    TProfile *p_mNumOfPhotons;

    static const int MAXEDGE = 100000;
    const int MAROCPOLARITY = 1; // 1 falling, 0 rising

    TChain *mChainInPut; // branches list below

    unsigned int tTrigNum;
    double tTrigTime;//long unsigned int tTimestamp;
    unsigned int tNedge;

    unsigned int tPolarity[MAXEDGE];
    unsigned int tChannel[MAXEDGE];
    unsigned int tTime[MAXEDGE];
    unsigned int tSlot[MAXEDGE];
    unsigned int tFiber[MAXEDGE];

    void ResetEventData();

    // ring finder
    std::vector<int> mXPixelMap; // corresponding binX number for each photon hit
    std::vector<int> mYPixelMap; // corresponding binY number for each photon hit

    TH2D *h_mRingFinder; // x: photon out_x | y: photon out_y with detector effect
    TH3D *h_mHoughTransform; // x | y | R
    TH3D *h_mQA_HT; // QA for hough transform
    TH2D *h_mRingFinder_Display; // QA for ring finder
    TH2D *h_mRingFinder_SingleEvent; // QA for ring finder

    TH3D *h_mCherenkovRing; // x | y | R
    TH3D *h_mNumOfCherenkovPhotons; // number of photons | number of photons on ring | ring radius

    // beam finder
    std::vector<int> mXBeamMap; // corresponding binX number for each photon hit
    std::vector<int> mYBeamMap; // corresponding binY number for each photon hit

    TH2D *h_mRingFinder; // x: photon out_x | y: photon out_y with detector effect
    TH2D *h_mRingFinder_SingleEvent; // QA for ring finder

    TH3D *h_mCherenkovRing; // x | y | R
    TH3D *h_mNumOfCherenkovPhotons; // number of photons | number of photons on ring | ring radius

    ClassDef(Calibration,1)
};

#endif
