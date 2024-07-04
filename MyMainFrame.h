// Standard
#include <iostream>
#include <queue>
#include <thread>
#include <tuple>
#include <time.h>

// ROOT GUI
#include <TGButton.h>
#include <TGClient.h>
#include <TGComboBox.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TTimer.h>
#include <TGSpeedo.h>

// ROOT
#include "TApplication.h"
#include "TDatime.h"
#include "TFile.h"
#include "TH2F.h"
#include "TString.h"
#include "TParameter.h"

// QSI Camera
#include "qsiapi.h"


class MyMainFrame : public TGMainFrame {
private:
    TGCompositeFrame *fCframe, *fCframeExposure, *fCframeShutter, *fCframeReadOut, *fCframeGain;
    TGTextButton *fConnect, *fOpen, *fPhoto, *fExit;
    TGSpeedo* fSpeedo;
    TTimer* fTimer;

    // Settings
    TGTextEntry *fSetExpTime;
    TGLabel *fLExpTime, *fLExpTimeLimits, *fLShutterPriority, *fLReadOut, *fLGain;
    TGComboBox *fCBShutter, *fCBReadOut, *fCBGain;

    // Properties
    Bool_t m_isConnected;
    Bool_t m_isOpened;
    Float_t m_minExpTime, m_maxExpTime;
    QSICamera cam;
    double m_exposureTime, m_minExposureTime, m_maxExposureTime;
    static int m_count;

    std::thread supportWorker;
    std::queue<std::tuple<unsigned short*, int, int>> jobs;
    std::mutex queue_mutex;
    std::condition_variable launchCondition;
    void QueueJob(unsigned short* image, int cols, int rows);
    void ThreadLoop();

public:
    MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MyMainFrame();

    // camera
    bool ConnectCamera();
    bool DisconnectCamera();
    bool CameraChangeShutterMode(bool isOpen = false);
    bool CameraSetExposureTime(double value);
    bool CameraMakePhoto();
    bool SaveImage(unsigned short* image, int cols, int rows);

    // slots
    void ChangeConnectMode();
    void ChangeShutterMode();
    void ChangeFanMode();
    void ChangeExposureTime();
    void ChangeShutterPriority();
    void ChangeReadoutSpeed();
    void ChangeGain();
    void MakePhoto();
    void Update();
    ClassDef(MyMainFrame, 0)
};
